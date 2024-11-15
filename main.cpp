#include <SDL2/SDL.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <cmath>
#include <condition_variable>
#include "Environment.h"
#include "Camera.h"

// Mutex pour synchroniser l'accès à l'environnement
std::mutex envMutex;
bool quit = false;

void drawFilledCircle(SDL_Renderer* renderer, int x, int y, int r) {
    for (int dy = -r; dy <= r; dy++) {
        int dx = static_cast<int>(sqrt(r * r - dy * dy));
        SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
    }
}

void drawCircleBorder(SDL_Renderer* renderer, int x, int y, int radius, double thickness) {
    int rmin = radius - int(thickness / 2);
    int rmax = radius + int(thickness / 2);

    for (int r = rmin; r <= rmax; ++r) {
        for (double angle = 0; angle < 2 * M_PI; angle += 0.01) {
            int dx = static_cast<int>(r * cos(angle));
            int dy = static_cast<int>(r * sin(angle));
            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        }
    }
}

void updateEnvironmentThread(Environment& env) {
    while (!quit) {
        {
            std::lock_guard<std::mutex> lock(envMutex);
            env.applyRelations();
            env.updatePositions();
            env.checkCollisions();
            //env.createDebris();
        }
        // Attendre un peu pour éviter de monopoliser le CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Galaxies!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCENE_WIDTH, SCENE_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Environment env;
    Camera camera(SCENE_WIDTH, SCENE_HEIGHT);
    
    int mouseX=0;
    int mouseY=0;
    int holdLeftClick=false;
    int prevMouseX=0;
    int prevMouseY=0;

    // Ajouter des objets dans l'environnement
    for (int i = 0; i < 2; ++i) { env.addStar(); }
    for (int i = 0; i < 50; ++i) { env.addPlanet(); }
    for (int i = 0; i < 20; ++i) { env.addDebris(); }

    // Thread pour la mise à jour de l'environnement
    std::thread updater(updateEnvironmentThread, std::ref(env));

    // Rendu dans le thread principal
    SDL_Event event;
    while (!quit) {
        // Gestion des événements
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

        // Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        {
            std::lock_guard<std::mutex> lock(envMutex);
            
            //std::cout << " number of objects : " << env.celestialObjects.size() << std::endl;
            
            if (env.objectsToCreate.size() > 0) {
            	std::cout << " object to remove x : " << env.objectsToCreate[0]->x << std::endl;
            	std::cout << " object to remove y : " << env.objectsToCreate[0]->y << std::endl;
            	std::cout << " object to remove radius : " << env.objectsToCreate[0]->radius << std::endl;
            }
            
            std::cout << " object x : " << env.celestialObjects[0]->x << std::endl;
            std::cout << " object y : " << env.celestialObjects[0]->y << std::endl;
            std::cout << " object radius : " << env.celestialObjects[0]->radius << std::endl;
            for (auto& object : env.celestialObjects) {
            	
            	
                if (object == nullptr) {
                std::cout << "Warning : nullptr object in display loop!" << std::endl;
                continue;
                }

                int adjustedX = int(object->x * kDisplaySizeRatio) - camera.getX();
                int adjustedY = int(object->y * kDisplaySizeRatio) - camera.getY();
                camera.applyZoom(adjustedX, adjustedY);

                std::vector<int> MainColor;
				std::vector<int> BorderColor;
				int MinDisplaySize;
					
            	// Check for Star
            	if (Star* star = dynamic_cast<Star*>(object)) {
                	MainColor = star->Color;
                	BorderColor = star->ColorBorder;
                	MinDisplaySize = 3;
            	}
            	else if (Planet* planet = dynamic_cast<Planet*>(object)) {
                	MainColor = planet->Color;
                	BorderColor = planet->ColorBorder;
                	MinDisplaySize = 1;
            	}
            	else if (Debris* debris = dynamic_cast<Debris*>(object)) {
            		MainColor = debris->Color;
                	BorderColor = {false};
                	MinDisplaySize = 1;
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
        					 	std::max(MinDisplaySize,int(object->radius * kDisplaySizeRatio * camera.getZoomFactor())));
        		
        		if(BorderColor[0]) {
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
        	}
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Limiter à ~60 FPS
    }

    // Arrêter le thread secondaire
    quit = true;
    updater.join();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
