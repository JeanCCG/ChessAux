#include "GameObject.h"
#include "TextureManager.h"



GameObject::GameObject(const char *texturesheet, int x, int y) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}
void GameObject::Update() {
    srcRect.h = 50;
    srcRect.w = 50;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w * 2;
    destRect.h = srcRect.h * 2;
}

void GameObject::Render() {
    if(Game::event.type==SDL_MOUSEBUTTONDOWN){
        if(Game::event.motion.x>=300 && Game::event.motion.x<400){
            xpos=300;
        }if(Game::event.motion.y>=200 && Game::event.motion.y<=300){
            ypos = 200;
        }
    }
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

void GameObject::RenderBackground() {
    SDL_RenderCopy(Game::renderer, objTexture, NULL, NULL);
}

