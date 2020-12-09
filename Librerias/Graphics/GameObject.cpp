#include "GameObject.h"
#include "TextureManager.h"
#include "Moves.h"
#include <iostream>



GameObject::GameObject(const char *texturesheet, int x, int y) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}
void GameObject::Update() {
    srcRect.h = 100;
    srcRect.w = 100;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
    if(Game::event.type==SDL_MOUSEBUTTONDOWN) {
        xpos=position_x();
        ypos=position_y();
    }

}

void GameObject::Render(int a, int b) {
    /*int posx=position_x();
    int posy=position_y();
    if(Game::event.type==SDL_MOUSEBUTTONDOWN){
        if(posx==a){
            if(Game::event.type==SDL_MOUSEBUTTONUP){
                xpos=posx;
            }
            //std::cout <<"postion of piece x: "<<a<<std::endl;
            //std::cout<<"x: "<<posx<<std::endl;
        }else{
            std::cout <<"postion of piece x: "<<a<<std::endl;
            std::cout<<"x: "<<posx<<std::endl;
        }
        if(posy==b){
            if(Game::event.type== SDL_MOUSEBUTTONUP){
                ypos=posy;
            }
            //std::cout <<"postion of piece y: "<<b<<std::endl;
            //std::cout << destRect.y<<std::endl;
            //std::cout<<"y: "<<posy<<std::endl;

        }else{
            std::cout <<"postion of piece y: "<<a<<std::endl;
            std::cout<<"y: "<<posy<<std::endl;
        }
    }


    /*while(Game::event.type== SDL_MOUSEBUTTONDOWN) {
        if(piece_xpos()==position_x()){
            xpos = position_x();
        }
        if(piece_ypos()==position_y()){
            ypos = position_y();
        }
    }
     */
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

void GameObject::RenderBackground() {
    SDL_RenderCopy(Game::renderer, objTexture, NULL, NULL);
}

int GameObject::piece_xpos(){
    return xpos;
}

int GameObject::piece_ypos(){
    return ypos;
}

