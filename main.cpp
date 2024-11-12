#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Environment.h"
#include "Camera.h"
#include <iostream>  // For printing debug information


void drawFilledCircle(SDL_Renderer* renderer, int x, int y, int r) {

    for (int dy = -r; dy <= r; dy++) {
        int dx = static_cast<int>(sqrt(r * r - dy * dy));

        // Dessine la ligne horizontale pour cette rangée
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
    }
}

void drawCircleBorder(SDL_Renderer* renderer, int x, int y, int radius, double thickness) {

    int rmin = radius - int(thickness/2);
    int rmax = radius + int(thickness/2);

    for (int r = rmin; r <= rmax; ++r) {
        for (double angle = 0; angle < 2 * M_PI; angle += 0.01) { 
            int dx = static_cast<int>(r * cos(angle));
            int dy = static_cast<int>(r * sin(angle));

            // Dessine le point sur le périmètre pour ce rayon et cet angle
            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    	std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    	return -1;
	}

    SDL_Window* window = SDL_CreateWindow("Galaxies!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCENE_WIDTH, SCENE_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create an environment and add stars/planets
    Environment env;
    Camera camera(SCENE_WIDTH, SCENE_HEIGHT);
    
    bool quit = false;
    
    bool holdLeftClick = false;
    int prevMouseX = 0, prevMouseY = 0;
     
    std::cout << "Adding stars..." << std::endl;
    for (int i = 0; i < 2; ++i) {  
        env.addStar();
    }
    
    std::cout << "Adding planets..." << std::endl;
    for (int i = 0; i < 200; ++i) {  
        env.addPlanet();
    }

    // Main rendering loop
    SDL_Event event;
    
    int mouseX=0;
    int mouseY=0;
    
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            
            // Vérifie si le bouton gauche de la souris est enfoncé ou relâché
    		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        		holdLeftClick = true;  // clic gauche enfoncé
        		prevMouseX = event.motion.x;
        		prevMouseY = event.motion.y;
    		}
    		else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        		holdLeftClick = false;  // clic gauche relâché
    		} 
            
            SDL_GetMouseState(&mouseX, &mouseY);
            
            if (event.type == SDL_MOUSEWHEEL) {
            
            	if (event.wheel.y > 0) {
                	camera.zoomIn();  // Zoom avant
            	} else if (event.wheel.y < 0) {
                	camera.zoomOut();  // Zoom arrière
            	}
        	}
            
        	int deltaX = 0;
        	int deltaY = 0;
	
        	if (holdLeftClick) {
        		deltaX = prevMouseX - event.motion.x;
        		deltaY = prevMouseY - event.motion.y;
        	}
	
        	camera.move(deltaX, deltaY);
        	
        	prevMouseX = event.motion.x;
        	prevMouseY = event.motion.y;
        }
        
        env.applyRelations();  // Update all interactions and movement
    	env.updatePositions();
    	env.checkCollisions();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //std::cout << "Rendering celestial objects..." << std::endl;
        for (auto& object : env.celestialObjects) {
        
            if (object == nullptr) {
                std::cout << "nullptr during rendering." << std::endl;
                continue;
            }
            
            //Adjust display from world coordinates to camera coordinates   	
        	int adjustedX = int(object->x * kDisplaySizeRatio) - camera.getX();
        	int adjustedY = int(object->y * kDisplaySizeRatio) - camera.getY();
			
			camera.applyZoom(adjustedX, adjustedY);

			std::vector<int> MainColor;
			std::vector<int> BorderColor;
				
            // Check for Star
            if (Star* star = dynamic_cast<Star*>(object)) {
                MainColor = star->Color;
                BorderColor = star->ColorBorder;
            }
            else if (Planet* planet = dynamic_cast<Planet*>(object)) {
                MainColor = planet->Color;
                BorderColor = planet->ColorBorder;
            }
            else {
                std::cout << "Unknown object type during rendering." << std::endl;
            }
            
            SDL_SetRenderDrawColor(renderer,
            					   MainColor[0],
            					   MainColor[1],
            					   MainColor[2], 255);
            
        	drawFilledCircle(renderer,
        					 adjustedX,
        					 adjustedY,
        					 int(object->radius * kDisplaySizeRatio * camera.getZoomFactor()));
        	
            // Draw borders to identify Planets from Stars
        	SDL_SetRenderDrawColor(renderer,
        						   BorderColor[0],
        						   BorderColor[1],
        						   BorderColor[2], 255);
        									 
        	drawCircleBorder(renderer,
        					 adjustedX,
        					 adjustedY,
        					 int(object->radius * kDisplaySizeRatio * camera.getZoomFactor()),
        					 int(BORDER_SIZE * camera.getZoomFactor())); 
            
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(int(1000.0/FrameRate));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
