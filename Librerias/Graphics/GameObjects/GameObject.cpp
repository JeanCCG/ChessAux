#include "../GameObjects/GameObject.h"
#include "../TextureManager/TextureManager.h"
#include <iostream>


GameObject::GameObject(const char *texturesheet, int x, int y) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void GameObject::Update() {
    srcRect.h = 90;
    srcRect.w = 90;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
}

void GameObject::Render() {
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

void GameObject::RenderBackground() {
    SDL_RenderCopy(Game::renderer, objTexture, NULL, NULL);
}

int GameObject::piece_xpos() {
    return xpos;
}

int GameObject::piece_ypos() {
    return ypos;
}

