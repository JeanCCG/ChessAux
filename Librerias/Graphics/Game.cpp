#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
using namespace std;

GameObject* tablero;
GameObject* caballo;
Map* piesas;

SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game(){

}
Game::~Game(){

}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags=0;
    if(fullscreen){
        flags=SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING)==0){
        cout << "SDL initialization"<<endl;
        window=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
        if(window){
            cout<<"window created"<<endl;
        }
        renderer=SDL_CreateRenderer(window,-1,0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            cout<<"renderer created"<<endl;
        }
        isRunning=true;
    }else{
        isRunning=false;
    }
    tablero = new GameObject("assets/chess_board.png", -1,0);
    caballo = new GameObject("assets/caballo.png",100,500);
    piesas = new Map();

}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            isRunning=false;
            break;
        default:
            break;
    }
}

void Game::update(){
    caballo->Update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    tablero->RenderBackground();
    piesas->DrawMap();
    caballo->Render();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout<<"Game cleaned"<<endl;
}

bool Game::running(){
    return isRunning;
}
