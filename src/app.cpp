#include <math.h>
#include <iostream>
#include <SDL.h>
#define PI 3.14259265

void handleEvent(bool *appIsRunning, bool *upArrowDown, bool *downArrowDown, bool *leftArrowDown, bool *rightArrowDown) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Handle each specific event
        if (event.type == SDL_QUIT) {
            *appIsRunning = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                *upArrowDown = true;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                *leftArrowDown = true;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                *downArrowDown = true;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                *rightArrowDown = true;
            }
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                *upArrowDown = false;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                *leftArrowDown = false;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                *downArrowDown = false;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                *rightArrowDown = false;
            }
        }
    }
}


float angle =0, pdx =10, pdy =10;

void moveRectangle (SDL_Rect *playerRect, int resW, int resH, bool upArrowDown, bool downArrowDown, bool leftArrowDown, bool rightArrowDown) {

    // Movement Logic    
    if (upArrowDown) {
        playerRect->x += pdx;
        playerRect->y += pdy;
    }
    if (downArrowDown) {
        playerRect->x -= pdx;
        playerRect->y -= pdy;
    }
    if (leftArrowDown) {
        angle -= PI/32;
        if(angle<0){ angle += 2*PI; };
        pdx = cos(angle)*5;
        pdy = sin(angle)*5;
    }
    if (rightArrowDown) {
        angle += PI/32;
        if(angle>2*PI){ angle -= 2*PI; };
        pdx = cos(angle)*5;
        pdy = sin(angle)*5;
    }

    // Bound checking
    if (playerRect->x < 0) {
        playerRect->x = 0;
    }
    else if (playerRect->x + playerRect->w - 1 >= resW) {
        playerRect->x = resW - playerRect->w;
    }
    if (playerRect->y < 0) {
        playerRect->y = 0;
    }
    else if (playerRect->y + playerRect->h - 1 >= resH) {
        playerRect->y = resH - playerRect->h;
    }

}

int map[] = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

};
/*
int map[] = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

};
*/
// X=32
int mapX = 16, mapY = 16;

void drawMap2D(SDL_Renderer *renderer) {
    int x0,y0;
    for(int x =0; x< mapX; x++) {
        for(int y=0; y< mapY; y++) { 
                SDL_Rect wall;
                wall.x = x*(512/mapX)+1; //1440
                wall.y = y*(512/mapY)+1;
                wall.w = (512/mapX)-1; //1440
                wall.h = (512/mapY)-1;
            if(map[y*mapX + x]==1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &wall);
            } else {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }
}


void drawRays3D(SDL_Rect *playerRect, SDL_Renderer *renderer){

    int mx, my, mp, dof;
    float rayX, rayY, rayAngle = angle, xo, yo;     
    for (int r=0; r<1; r++){
        // Check Horizontal lines
        dof=0;
        float aTan = -1/tan(rayAngle);
        // Looking up
        if(rayAngle > PI) {
            rayY = (((int)(playerRect->y)>>5)<<5) - 0.0001; 
            rayX = ((playerRect->y) - rayY) * aTan + (playerRect->x);
            std::cout << rayY << std::endl;
            //std::cout << rayX << std::endl;
            std::cout << playerRect->y << std::endl;
            yo = -32;
            xo = -yo*aTan;
        }
        // Looking Down
        if(rayAngle < PI) {
            // This takes the pointer and bit shifts it 6 places right
            // then 6 places left giving an interval of 64
            rayY = (((int)(playerRect->y)>>5)<<5) + 32; 
            rayX = ((playerRect->y) - rayY) * aTan + (playerRect->x);
            std::cout << rayY << std::endl;
            std::cout << playerRect->y << std::endl;
            //std::cout << rayX << std::endl;
            yo = 32;
            xo = -yo*aTan;
        }
        if(rayAngle==0 || rayAngle == PI) {
            rayX = playerRect->x; 
            rayY = playerRect->y; 
            dof = 8;
        }

        while(dof<8){
            mx=(int)(rayX)>>5; 
            my=(int)(rayY)>>5; 
            mp=my*mapX+mx;
          //  std::cout << mp <<std::endl;
            if(mp<mapX*mapY && map[mp]==1) { //Hit a wall
                dof=8; 
            }
            else{ 
                rayX += xo;
                rayY += yo;
                dof +=1;
            }
        }
    
    SDL_RenderDrawLine(renderer, playerRect->x, playerRect->y, rayX, rayY);

    }


}

int main(int argc, char const *argv[])
{
    int resW = 512, resH = 512; //1440
    SDL_Rect playerRect;
    playerRect.w = resW/64;
    playerRect.h = resH/64;
    playerRect.x = resW/2 - playerRect.w/2;
    playerRect.y = resH/2 - playerRect.h/2;
    
    bool upArrowDown = false, leftArrowDown = false, downArrowDown = false, rightArrowDown = false;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool appIsRunning = true;
    
    //Uint64 lastDrawTime = SDL_GetTicks64();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized: " << SDL_GetError();
    }
    else {
        std::cout << "SDL video system is ready to go\n";
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "3D RayCast",        // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        resW,                     // width, in pixels
        resH,                     // height, in pixels
        SDL_WINDOW_SHOWN         // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        // In the case that the renderer could not be made...
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Runs the main game frames
    while (appIsRunning) {

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        drawMap2D(renderer);
        // Handle new events
        handleEvent( &appIsRunning, &upArrowDown, &downArrowDown, &leftArrowDown, &rightArrowDown);

        // Handle movement of character
        moveRectangle(&playerRect, resW, resH, upArrowDown, downArrowDown, leftArrowDown, rightArrowDown);

        SDL_SetRenderDrawColor(renderer, 0, 105, 180, SDL_ALPHA_OPAQUE);
        drawRays3D(&playerRect, renderer);       

        SDL_SetRenderDrawColor(renderer, 255, 105, 180, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &playerRect);
        SDL_RenderDrawLine(renderer, playerRect.x, playerRect.y, playerRect.x + 10*pdx, playerRect.y+10*pdy);
        SDL_RenderPresent(renderer);
        
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    std::cout << "exiting..." << std::endl;
    SDL_Quit();
    return 0;
}
