#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Game{
	SDL_Window* window;
	SDL_Renderer* renderer;
};

bool game_init_sdl(struct Game *g);
void game_free(struct Game *g);
void game_run(struct Game *g);

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

void game_free(struct Game *g){
	if(g->window){
		SDL_DestroyWindow(g->window);
		g->window = NULL;
	}

	if(g->renderer){
		SDL_DestroyRenderer(g->renderer);
		g->renderer = NULL;
	}

	SDL_Quit();
}

void game_run(struct Game *g){
	SDL_RenderClear(g->renderer);
	SDL_RenderPresent(g->renderer);
	SDL_Delay(5000);
}

int main(){
	int exit_status = EXIT_FAILURE;
	struct Game game = {0};
	if(game_init_sdl(&game)){
		exit_status = EXIT_SUCCESS;
	}
	game_run(&game);

	game_free(&game);
	return exit_status;
}
