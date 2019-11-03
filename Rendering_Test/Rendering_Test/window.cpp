#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "World.h"
#include "Character.h"
#ifdef _WIN32
#include <windows.h>
#endif

//using namespace std;

#define FPS 60
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 736
#define MOVE_SPEED 8
#define TILE_SIZE 32
#define NUM_TILES_X 40
#define NUM_TILES_Y 23
#define IS_SOLID 1
#define IS_NOT_SOLID 0
#define JOYAXIS_DEADZONE 8000

static bool keep_playing = true;

static const char background_image[] = "C:/Users/Dan/source/repos/Rendering_Test/backgrounds/dungeon_map.png";
static const char char_sprite[] = "C:/Users/Dan/source/repos/Rendering_Test/sprites/main_char.png";
static const char enemy_sprite[] = "C:/Users/Dan/source/repos/Rendering_Test/sprites/enemy_char.png"; 

int CheckCollision(V2 player_delta_pos, int* tile_map)
{
	int player_delta_tile_x, player_delta_tile_y;
	player_delta_tile_x = ((int)player_delta_pos.x / TILE_SIZE);
	player_delta_tile_y = ((int)player_delta_pos.y / TILE_SIZE);
	if (tile_map[player_delta_tile_y*NUM_TILES_X+player_delta_tile_x] == 1)
	{
		return IS_SOLID;
	}
	else
	{
		return IS_NOT_SOLID;
	}
	
}

int InitAndLoadWindow(SDL_Window** main_window, SDL_Surface** main_surface, SDL_Rect *window_rect)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	*main_window = SDL_CreateWindow("Render Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (main_window == NULL)
	{
		printf("Failed to create window\n");
		SDL_Quit();
		return 0;
	}

	*main_surface = SDL_GetWindowSurface(*main_window);
	if (main_surface == NULL)
	{
		printf("Failed to create window surface\n");
		SDL_Quit();
		return 0;
	}

	*window_rect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

	return 1;
}

int InitController(SDL_Joystick** controller)
{
	if( SDL_NumJoysticks() < 1 )
	{
		printf( "Warning: No joysticks connected!\n" );
	}
	else
	{
		*controller = SDL_JoystickOpen( 0 );
		if( controller == NULL )
		{
			printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			return -1;
		}
	}
	return 1;
}
int LoadImages(SDL_Surface** sprite_surface, SDL_Surface** background_surface, 
				SDL_Surface** enemy_surface)
{
	int img_flags = (int)IMG_INIT_PNG;
	if (!(IMG_Init(img_flags)))
	{
		printf("Failed to initialise SDL_image.lib\n");
		SDL_Quit();
		return 0;
	}

	*sprite_surface = IMG_Load(char_sprite);
	if (sprite_surface == NULL)
	{
		printf("Failed to create sprite surface\n");
		SDL_Quit();
		return 0;
	}
	*background_surface = IMG_Load(background_image);
	if (background_surface == NULL)
	{
		printf("Failed to create background surface\n");
		SDL_Quit();
		return 0;
	}
	*enemy_surface = IMG_Load(enemy_sprite);
	if (enemy_surface == NULL)
	{
		printf("Failed to create enemy surface\n");
		SDL_Quit();
		return 0;
	}
	return 1;
}

int InitRenderer(SDL_Renderer** renderer, SDL_Window* main_window, 
				SDL_Texture** char_texture, SDL_Texture** background_texture, 
				SDL_Texture** enemy_texture,SDL_Surface* sprite_surface, 
				SDL_Surface* background_surface, SDL_Surface* enemy_surface)
{
	*renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
		printf("Failed to create renderer\n");
		SDL_Quit();
		return 0;
	}

	*char_texture = SDL_CreateTextureFromSurface(*renderer, sprite_surface);
	if (char_texture == NULL)
	{
		printf("Failed to create char texture\n");
		SDL_Quit();
		return 0;
	}
	*background_texture = SDL_CreateTextureFromSurface(*renderer, background_surface);
	if (char_texture == NULL)
	{
		printf("Failed to create background texture\n");
		SDL_Quit();
		return 0;
	}
	*enemy_texture = SDL_CreateTextureFromSurface(*renderer, enemy_surface);
	if (enemy_texture == NULL)
	{
		printf("Failed to create enemy texture\n");
		SDL_Quit();
		return 0;
	}
	return 1;
}

void UpdatePlayer(Character *player, SDL_Rect* sprite_rect, int *tilemap, V2 velocity, int frame, int direction)
{
	if (velocity.x != 0 || velocity.y != 0)
	{
		V2 player_delta_pos = player->GetPosition();
		int err = 0;
		V2 player_offset_x = {32, 0};
		if (velocity.x != 0.0f && velocity.y != 0.0f)
		{
			velocity *= (2 - ((float)sqrt(2)));
		}

		player_delta_pos += (velocity * MOVE_SPEED);

		if (!(CheckCollision(player_delta_pos + player_offset_x, tilemap) &&
			CheckCollision(player_delta_pos, tilemap)))
		{
			player->position = player_delta_pos;
		}
		*sprite_rect = { (int)frame * 32,direction*32,32,32 };
	}
}

int ReadInput(SDL_Event* event, V2 *velocity, int *direction)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_W)
		{
			*direction = 3;
			velocity->y = -1;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_S)
		{
			*direction = 0;
			velocity->y = 1;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_D)
		{
			*direction = 2;
			velocity->x = 1;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_A)
		{
			*direction = 1;
			velocity->x = -1;
		}
		if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			keep_playing = false;
		}
	}
	if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.scancode == SDL_SCANCODE_W)
		{
			velocity->y = 0;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_S)
		{
			velocity->y = 0;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_D)
		{
			velocity->x = 0;
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_A)
		{
			velocity->x = 0;
		}
	}
	if (event->type == SDL_JOYBUTTONDOWN)
	{
		if (event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
		{
			*direction = 3;
			velocity->y = -1;
		}
		else if (event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		{
			*direction = 0;
			velocity->y = 1;
		}
		else if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		{
			*direction = 2;
			velocity->x = 1;
		}
		else if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		{
			*direction = 1;
			velocity->x = -1;
		}
	}
	if (event->type == SDL_JOYBUTTONUP)
	{
		if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_UP)
		{
			velocity->y = 0;
		}
		else if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		{
			velocity->y = 0;
		}
		else if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		{
			velocity->x = 0;
		}
		else if (event->key.keysym.scancode == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		{
			velocity->x = 0;
		}
	}
	// if (event->type == SDL_JOYAXISMOTION)
	// {
	// 	// Check if it is first player controller
	// 	// Can handle second controller later
	// 	if (event->jaxis.which == 0)
	// 	{
	// 		if( event->jaxis.axis == 1 )
	// 		{
	// 			// Check for deadzone
	// 			if( (event->jaxis.value < JOYAXIS_DEADZONE) &&  (event->jaxis.value > -JOYAXIS_DEADZONE))
	// 			{
	// 				velocity->y = 0;
	// 			}
	// 			else if( event->jaxis.value > JOYAXIS_DEADZONE )
	// 			{
	// 				*direction = 0;
	// 				velocity->y =  1;
	// 			}
	// 			else
	// 			{
	// 				*direction = 3;
	// 				velocity->y = -1;
	// 			}
	// 		}
	// 		else if ( event->jaxis.axis == 0 )
	// 		{
	// 			// Check for deadzone
	// 			if( (event->jaxis.value < JOYAXIS_DEADZONE) &&  (event->jaxis.value > -JOYAXIS_DEADZONE))
	// 			{
	// 				velocity->x = 0;
	// 			}
	// 			else if( event->jaxis.value > JOYAXIS_DEADZONE )
	// 			{
	// 				*direction = 2;
	// 				velocity->x =  1;
	// 			}
	// 			else
	// 			{
	// 				*direction = 1;
	// 				velocity->x = -1;
	// 			}
	// 		}
	// 	}
	// }

	return 1;
}


int main(int argc, char *args[])
{
	SDL_Event event;
	SDL_Window *main_window = NULL;
	SDL_Surface *main_surface = NULL;
	SDL_Surface *sprite_surface = NULL, *background_surface = NULL, *enemy_surface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *char_texture = NULL, *background_texture = NULL, *enemy_texture =  NULL;
	SDL_Rect sprite_rect = {32,64,32,32 }, player_rect = {0,376,64,64 }; 
	SDL_Rect enemy_rect = {32,32,32,32 }, enemy_dest_rect = {624,376,64,64 }, window_rect = {0, 0, 0, 0};
	SDL_Joystick *controller = NULL;
	Character *player = NULL;

	int frame = 0;
	int error = 0;
	int direction = 0;
	uint8_t sides = 0;
	uint32_t start_time, end_time, delta_time;
	V2 velocity = {0,0};
	
	Tilemap tilemaps[4];
	int tilemap0[NUM_TILES_Y][NUM_TILES_X];

	//Continue here
	//Need to make tilemap creation dynamic (somewhat anyway)
	tilemaps[0].tiles = *tilemap0;
	CreateTilemap(&tilemaps[0].tiles, NUM_TILES_X, NUM_TILES_Y, (sides |= 1 << 0));
	CreateTilemap(&tilemaps[1].tiles, NUM_TILES_X, NUM_TILES_Y, (sides |= 1 << 1));
	// CreateTilemap(&tilemap[2].tiles, NUM_TILES_X, NUM_TILES_Y, (sides |= 1 << 3));
	// CreateTilemap(&tilemap[3].tiles, NUM_TILES_X, NUM_TILES_Y, (sides |= 1 << 4));
	World world;

	if (!InitAndLoadWindow(&main_window, &main_surface, &window_rect))
	{
		printf("Failed to init window\n");
		return -1;
	}
	if (!InitController(&controller))
	{
		printf("Failed to init controller\n");
		return -1;
	}
	if (!LoadImages(&sprite_surface, &background_surface, &enemy_surface))
	{
		printf("Failed to load images\n");
		return -1;
	}
	if (!InitRenderer(&renderer, main_window, &char_texture, 
					  &background_texture, &enemy_texture, 
					  sprite_surface, background_surface, enemy_surface))
	{
		printf("Failed to init renderer\n");
		return -1;
	}
	
	CreateWorld(&world, NUM_TILES_X, NUM_TILES_Y, TILE_SIZE, tilemaps);
	
	player = new Character({64, 376}, (64/TILE_SIZE), (376/TILE_SIZE));
	SDL_UpdateWindowSurface(main_window);
	SDL_RenderClear(renderer);
	Uint32 frame_delay = 1000/FPS;
	
	
	do {
		start_time = SDL_GetTicks();
		if (frame > 2)
			frame = 0;
		SDL_PollEvent(&event);
		ReadInput(&event, &velocity, &direction);
		UpdatePlayer(player, &sprite_rect, world.tilemap[0].tiles, velocity, frame, direction);
		player_rect.x = (int)player->position.x;
		player_rect.y = (int)player->position.y;
		SDL_RenderClear(renderer);
		if (SDL_RenderCopy(renderer, background_texture, NULL, &window_rect) != 0)
		{
			printf("Failed to render background\n");
			return -1;
		}
		if (SDL_RenderCopy(renderer, char_texture, &sprite_rect, &player_rect) != 0)
		{
			printf("Failed to render char\n");
			return -1;
		}
		if (SDL_RenderCopy(renderer, enemy_texture, &enemy_rect, &enemy_dest_rect) != 0)
		{
			printf("Failed to render enemy\n");
			return -1;
		}
		SDL_RenderPresent(renderer);
		frame++;
		end_time = SDL_GetTicks();
		delta_time = end_time - start_time;
		if (frame_delay > delta_time)
			SDL_Delay(frame_delay - delta_time);
		//std::cout << "MS/F: " << (SDL_GetTicks() - start_time) << std::endl;
	}while (keep_playing);

	//TODO Dan:
	//Cleanup function needed
	SDL_DestroyTexture(char_texture);
	SDL_FreeSurface(sprite_surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(background_texture);
	SDL_FreeSurface(background_surface);
	SDL_JoystickClose(controller);
	SDL_DestroyWindow(main_window);

	SDL_Quit();

	return 0;
}