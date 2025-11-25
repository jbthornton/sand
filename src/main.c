#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Game{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	bool is_running;
};

bool game_init_sdl(struct Game *g);
bool game_init(struct Game **game);
void game_free(struct Game **game);
void game_run(struct Game *g);
void game_draw(struct Game *g);

bool game_init_sdl(struct Game *g){
	if(!SDL_Init(SDL_INIT_VIDEO)){
		fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
		return false;
	}

	g->window = SDL_CreateWindow("Title", 1080, 720, 0);
	if(g->window == NULL){
		fprintf(stderr, "Error creating SDL3 window: %s\n", SDL_GetError());
		return false;
	}

	g->renderer = SDL_CreateRenderer(g->window,NULL);
	if(g->renderer == NULL){
		fprintf(stderr, "Error creating SDL3 renderer: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool game_init(struct Game **game){
	*game = calloc(1,sizeof(struct Game));
	if(*game == NULL){
		fprintf(stderr, "Could not calloc game\n");
		return false;
	}
	struct Game *g = *game;

	if(!game_init_sdl(g)){
		return false;
	}

	g->is_running = true;
	return true;
}

void game_free(struct Game **game){
	if(*game){
		struct Game *g = *game;
		if(g->window){
			SDL_DestroyWindow(g->window);
			g->window = NULL;
		}

		if(g->renderer){
			SDL_DestroyRenderer(g->renderer);
			g->renderer = NULL;
		}

		SDL_Quit();
	
		free(g);
		g = NULL;
		*game = NULL;
	}
}

void game_events(struct Game *g){
	while(SDL_PollEvent(&g->event)){
		switch(g->event.type){
		case SDL_EVENT_QUIT:
			g->is_running = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			switch(g->event.key.scancode){
			case SDL_SCANCODE_ESCAPE:
				g->is_running = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

	}
}

void game_draw(struct Game *g){
	SDL_SetRenderDrawColor(g->renderer, 128, 0, 255, 255);
	SDL_RenderClear(g->renderer);
	SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g){
	while(g->is_running){
		game_events(g);
		game_draw(g);
		SDL_Delay(16);//run ~60 times per second
	}
}

int main(){
	int exit_status = EXIT_FAILURE;
	struct Game *game = NULL;
	if(game_init(&game)){
		exit_status = EXIT_SUCCESS;
	}
	game_run(game);

	game_free(&game);
	return exit_status;
}
