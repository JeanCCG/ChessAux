#include "Game.h"
#include "../TextureManager/TextureManager.h"
#include "../GameObjects/GameObject.h"
#include "../Map/Map.h"
using namespace std;

GameObject* tablero;
GameObject* peon_10;
GameObject* peon_11;
GameObject* peon_12;
GameObject* peon_13;
GameObject* peon_14;
GameObject* peon_15;
GameObject* peon_16;
GameObject* peon_17;

GameObject* peon_60;
GameObject* peon_61;
GameObject* peon_62;
GameObject* peon_63;
GameObject* peon_64;
GameObject* peon_65;
GameObject* peon_66;
GameObject* peon_67;

GameObject* torre_00;
GameObject* torre_07;
GameObject* torre_70;
GameObject* torre_77;

GameObject* caballo_01;
GameObject* caballo_06;
GameObject* caballo_71;
GameObject* caballo_76;

GameObject* alfil_02;
GameObject* alfil_05;
GameObject* alfil_72;
GameObject* alfil_75;

GameObject* reina_03;
GameObject* reina_73;

GameObject* rey_04;
GameObject* rey_74;

GameObject* caballo;
//Map* piesas;

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
    peon_10 = new GameObject("assets/peon_negro.png",0,100);
    peon_11 = new GameObject("assets/peon_negro.png",100,100);
    peon_12 = new GameObject("assets/peon_negro.png",200,100);
    peon_13 = new GameObject("assets/peon_negro.png",300,100);
    peon_14 = new GameObject("assets/peon_negro.png",400,100);
    peon_15 = new GameObject("assets/peon_negro.png",500,100);
    peon_16 = new GameObject("assets/peon_negro.png",600,100);
    peon_17 = new GameObject("assets/peon_negro.png",700,100);

    peon_60 = new GameObject("assets/peon_blanco.png",0,600);
    peon_61 = new GameObject("assets/peon_blanco.png",100,600);
    peon_62 = new GameObject("assets/peon_blanco.png",200,600);
    peon_63 = new GameObject("assets/peon_blanco.png",300,600);
    peon_64 = new GameObject("assets/peon_blanco.png",400,600);
    peon_65 = new GameObject("assets/peon_blanco.png",500,600);
    peon_66 = new GameObject("assets/peon_blanco.png",600,600);
    peon_67 = new GameObject("assets/peon_blanco.png",700,600);

    torre_00 = new GameObject("assets/torre_negro.png",0,0);
    torre_07 = new GameObject("assets/torre_negro.png",700,0);
    torre_70 = new GameObject("assets/torre_blanco.png",0,700);
    torre_77 = new GameObject("assets/torre_blanco.png",700,700);

    caballo_01 = new GameObject("assets/caballo_negro.png", 100,0);
    caballo_06 = new GameObject("assets/caballo_negro.png", 600,0);
    caballo_71 = new GameObject("assets/caballo_blanco.png",100,700);
    caballo_76 = new GameObject("assets/caballo_blanco.png",600,700);

    alfil_02 = new GameObject("assets/alfil_negro.png",200,0);
    alfil_05 = new GameObject("assets/alfil_negro.png",500,0);
    alfil_72 = new GameObject("assets/alfil_blanco.png", 200, 700);
    alfil_75 = new GameObject("assets/alfil_blanco.png",500,700);

    reina_03 = new GameObject("assets/reyna_negro.png", 300,0);
    reina_73 = new GameObject("assets/reyna_blanco.png", 300,700);

    rey_04 = new GameObject("assets/rey_negro.png", 400,0);
    rey_74 = new GameObject("assets/rey_blanco.png", 400,700);






    //piesas = new Map();

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

void Game::update() {
    caballo->Update();
    peon_10->Update();
    peon_11->Update();
    peon_12->Update();
    peon_13->Update();
    peon_14->Update();
    peon_15->Update();
    peon_16->Update();
    peon_17->Update();
    peon_60->Update();
    peon_61->Update();
    peon_62->Update();
    peon_63->Update();
    peon_64->Update();
    peon_65->Update();
    peon_66->Update();
    peon_67->Update();

    torre_00->Update();
    torre_07->Update();
    torre_70->Update();
    torre_77->Update();

    caballo_01->Update();
    caballo_06->Update();
    caballo_71->Update();
    caballo_76->Update();

    alfil_02->Update();
    alfil_05->Update();
    alfil_72->Update();
    alfil_75->Update();

    reina_03->Update();
    reina_73->Update();

    rey_04->Update();
    rey_74->Update();
}

void Game::render() {
    SDL_RenderClear(renderer);
    tablero->RenderBackground();
    caballo->Render();
    peon_10->Render();
    peon_11->Render();
    peon_12->Render();
    peon_13->Render();
    peon_14->Render();
    peon_15->Render();
    peon_16->Render();
    peon_17->Render();
    peon_60->Render();
    peon_61->Render();
    peon_62->Render();
    peon_63->Render();
    peon_64->Render();
    peon_65->Render();
    peon_66->Render();
    peon_67->Render();

    torre_00->Render();
    torre_07->Render();
    torre_70->Render();
    torre_77->Render();

    caballo_01->Render();
    caballo_06->Render();
    caballo_71->Render();
    caballo_76->Render();

    alfil_02->Render();
    alfil_05->Render();
    alfil_72->Render();
    alfil_75->Render();

    reina_03->Render();
    reina_73->Render();

    rey_04->Render();
    rey_74->Render();
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
