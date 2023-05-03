#include <math.h>
#include <iostream>
#include <SDL.h>
#define PI 3.14259265
#define Rad PI/1440 //This is actually an eigth radian


int resW = 512 + 512, resH = 512;
int mapX = 16, mapY = 16;
bool upArrowDown = false, leftArrowDown = false, downArrowDown = false, rightArrowDown = false;
bool appIsRunning = true;
float angle =0, pdx =10, pdy =10;

int map[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,
    1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

};



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


void drawMap2D(SDL_Renderer *renderer) {
    int x0,y0;
    for(int x =0; x< mapX; x++) {
        for(int y=0; y< mapY; y++) { 

            SDL_Rect wall;
            wall.x = x*(resW/mapX/2)+1; //1440
            wall.y = y*(resH/mapY)+1;
            wall.w = (resW/mapX/2)-1; //1440
            wall.h = (resH/mapY)-1;

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


float rayDist(float ax, float ay, float bx, float by, float ang) {
    return ( sqrt(pow(bx-ax,2) + pow(by-ay,2)) );
}


void drawRays3D(SDL_Rect *playerRect, SDL_Renderer *renderer){

    int mx, my, mp, dof, bitshift =5;
    float rayX, rayY, rayAngle = angle, xo, yo, finalDist;     
    rayAngle = angle - (Rad)*256;
    if(rayAngle < 0) rayAngle += 2*PI;
    if(rayAngle > 2*PI) rayAngle -= 2*PI;
    
    for (int r=0; r<512; r++){ 
        // Check Horizontal lines for ray collisions
        dof=0;
        float distH = 10000000, hx=playerRect->x, hy=playerRect->y;
        float aTan = -1/tan(rayAngle);
        // Character is Looking up
        if(rayAngle > PI) {
            // This takes the pointer and bit shifts it 6 places right
            // then "bitshift" places left giving an interval of 2^bitshift
            // This effectively increments rayY by 2^bitshift each vertical box
            rayY = (((int)(playerRect->y)>>bitshift)<<bitshift) - 0.0001; 
            rayX = ((playerRect->y) - rayY) * aTan + (playerRect->x);
            yo = -pow(2,bitshift);
            xo = -yo*aTan;
        }
        // Character is Looking Down
        if(rayAngle < PI) {
            rayY = (((int)(playerRect->y)>>bitshift)<<bitshift) + pow(2,bitshift); 
            rayX = ((playerRect->y) - rayY) * aTan + (playerRect->x);
            yo = pow(2,bitshift);
            xo = -yo*aTan;
        }
        if(rayAngle==0 || rayAngle == PI) {
            rayX = playerRect->x; 
            rayY = playerRect->y; 
            dof = 8;
        }

        // Cast rays at a length of 'dof' 
        while(dof<16){
            mx=(int)(rayX)>>bitshift; 
            my=(int)(rayY)>>bitshift; 
            mp=my*mapX+mx;
            if(0<mp && mp<mapX*mapY && map[mp]==1) { //Hit a wall
                hx = rayX;
                hy = rayY;
                distH = rayDist(playerRect->x,playerRect->y,hx,hy, rayAngle);
                dof=16; 
            }
            else{ 
                rayX += xo;
                rayY += yo;
                dof +=1;
            }
        } 
    
        // Check Vertical lines
        dof=0;
        float distV = 10000000, vx=playerRect->x, vy=playerRect->y;
        float Tan = -1 * tan(rayAngle);
        // Character is Looking left
        if(rayAngle > PI/2 && rayAngle < 3*PI /2) {
            rayX = (((int)(playerRect->x)>>bitshift)<<bitshift) - 0.0001; 
            rayY = ((playerRect->x) - rayX) * Tan + (playerRect->y);
            xo = -pow(2,bitshift);
            yo = -xo*Tan;
        }
        // Character is Looking right
        if(rayAngle < PI/2 || rayAngle > 3*PI/2 ) {
            rayX = (((int)(playerRect->x)>>bitshift)<<bitshift) + pow(2,bitshift); 
            rayY = ((playerRect->x) - rayX) * Tan + (playerRect->y);
            xo = pow(2,bitshift);
            yo = -xo*Tan;
        }
        if(rayAngle==0 || rayAngle == PI) {
            rayX = playerRect->x; 
            rayY = playerRect->y; 
            dof = 8;
        }

        while(dof<16){
            mx=(int)(rayX)>>bitshift; 
            my=(int)(rayY)>>bitshift; 
            mp=my*mapX+mx;
          //  std::cout << mp <<std::endl;
            if( mp>0 && mp<mapX*mapY && map[mp]==1) { //Hit a wall
                vx = rayX;
                vy = rayY;
                distV = rayDist(playerRect->x,playerRect->y,vx,vy, rayAngle);
                dof=16; 
            }
            else{ 
                rayX += xo;
                rayY += yo;
                dof +=1;
            }
        }

    if(distV>distH) { 
        rayX = hx;
        rayY = hy;   
        finalDist = distH;
        SDL_SetRenderDrawColor(renderer, 80, 105, 180, SDL_ALPHA_OPAQUE);

    } 
    else {
        rayX = vx;
        rayY = vy;
        finalDist = distV;
        SDL_SetRenderDrawColor(renderer, 0, 105, 180, SDL_ALPHA_OPAQUE);

    }
    SDL_RenderDrawLine(renderer, playerRect->x, playerRect->y, rayX, rayY);
   
    // Draw the 3D Walls
    
    float diffAngle = rayAngle-angle;
    if(diffAngle<0) {
        diffAngle += 2*PI;
    }
    if(diffAngle> 2*PI) {
        diffAngle -= 2*PI;
    }
    finalDist = finalDist * cos(diffAngle);
    float lineHeight = (32*512)/finalDist;
    float lineOffset = 256 - lineHeight/2;
    SDL_RenderDrawLine(renderer, r*1 + 513, lineOffset, r*1 + 513, lineHeight+lineOffset);

    if(lineHeight>512) {
        lineHeight = 512;
    }

    // Increment the loop by 1 degree and end loop
    rayAngle += Rad;
    if(rayAngle < 0) rayAngle += 2*PI;
    if(rayAngle > 2*PI) rayAngle -= 2*PI;

    }

}


int main(int argc, char const *argv[]) {

    SDL_Rect playerRect;
    playerRect.w = 512/64;
    playerRect.h = 512/64;
    playerRect.x = resW/4 - playerRect.w/2;
    playerRect.y = resH/2 - playerRect.h/2;
    
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    // Initialize SDL
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
    
    // Create the renderer
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

        // Function to draw the 2d Map
        drawMap2D(renderer);

        // Handle new events
        handleEvent( &appIsRunning, &upArrowDown, &downArrowDown, &leftArrowDown, &rightArrowDown);

        // Handle movement of character
        moveRectangle(&playerRect, resW, resH, upArrowDown, downArrowDown, leftArrowDown, rightArrowDown);
        
        // Draw the 3D raycast
        drawRays3D(&playerRect, renderer);       

        SDL_SetRenderDrawColor(renderer, 255, 105, 180, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &playerRect);
        SDL_RenderDrawLine(renderer, playerRect.x, playerRect.y, playerRect.x + 10*pdx, playerRect.y+10*pdy);
        SDL_RenderPresent(renderer);
        
    }


    // Code to exit and cleanup the program
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    std::cout << "exiting..." << std::endl;
    SDL_Quit();
    return 0;
}
