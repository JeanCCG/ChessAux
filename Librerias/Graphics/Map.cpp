#include "Map.h"
#include "TextureManager.h"

int Base[8][8] ={
        {1,5,7,11,9,7,5,1},
        {3,3,3,3,3,3,3,3},
        {12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12},
        {2,2,2,2,2,2,2,2},
        {100,4,6,8,10,6,4,100},
};

Map::Map(){
    peon_B = TextureManager::LoadTexture("assets/peon_blanco.png");
    peon_N = TextureManager::LoadTexture("assets/peon_negro.png");
    torre_B = TextureManager::LoadTexture("assets/torre_blanco.png");
    torre_N = TextureManager::LoadTexture("assets/torre_negro.png");
    caballo_B = TextureManager::LoadTexture("assets/caballo_blanco.png");
    caballo_N = TextureManager::LoadTexture("assets/caballo_negro.png");
    alfil_B = TextureManager::LoadTexture("assets/alfil_blanco.png");
    alfil_N = TextureManager::LoadTexture("assets/alfil_negro.png");
    reina_B = TextureManager::LoadTexture("assets/reyna_blanco.png");
    reina_N = TextureManager::LoadTexture("assets/reyna_negro.png");
    rey_B = TextureManager::LoadTexture("assets/rey_blanco.png");
    rey_N = TextureManager::LoadTexture("assets/rey_negro.png");

    LoadMap(Base);
    src.x = src.y = 0;
    src.w = dest.w = 100;
    src.h = dest.h = 100;
    dest.x = dest.y = 0;

}

void Map::LoadMap(int arr[8][8]){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            map[i][j] = arr[i][j];
        }
    }
}

void Map::DrawMap() {
    int Tipo = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Tipo = map[i][j];
            dest.x=j*100;
            dest.y=i*100;
            switch (Tipo) {
                case 100:
                    TextureManager::Draw(torre_B, src, dest);
                    break;
                case 1:
                    TextureManager::Draw(torre_N, src, dest);
                    break;
                case 2:
                    TextureManager::Draw(peon_B, src, dest);
                    break;
                case 3:
                    TextureManager::Draw(peon_N, src, dest);
                    break;
                case 4:
                    TextureManager::Draw(caballo_B, src, dest);
                    break;
                case 5:
                    TextureManager::Draw(caballo_N, src, dest);
                    break;
                case 6:
                    TextureManager::Draw(alfil_B, src, dest);
                    break;
                case 7:
                    TextureManager::Draw(alfil_N, src, dest);
                    break;
                case 8:
                    TextureManager::Draw(reina_B, src, dest);
                    break;
                case 9:
                    TextureManager::Draw(reina_N, src, dest);
                    break;
                case 10:
                    TextureManager::Draw(rey_B, src, dest);
                    break;
                case 11:
                    TextureManager::Draw(rey_N, src, dest);
                    break;
                default:
                    break;
            }
        }
    }

}


