#include "Debris.h"
#include "Aleatoire.h"
#include "CelestialObject.h"  // Include CelestialObject base class

#include <cmath>
#include <vector>

//Constructor
Debris::Debris()
    : CelestialObject()  // Initialize base class
{
    // Set position and velocity using Aleatoire (random)
    x = Aleatoire::Uniforme(-4*SCENE_WIDTH / kDisplaySizeRatio, 5*SCENE_WIDTH / kDisplaySizeRatio);
    y = Aleatoire::Uniforme(-4*SCENE_WIDTH / kDisplaySizeRatio, 5*SCENE_HEIGHT / kDisplaySizeRatio);
    vx = TimeRatio * Aleatoire::Uniforme(DEBRIS_MIN_INITIAL_SPEED, DEBRIS_MAX_INITIAL_SPEED);
    vy = TimeRatio * Aleatoire::Uniforme(DEBRIS_MIN_INITIAL_SPEED, DEBRIS_MAX_INITIAL_SPEED);  
    ax = 0; ay = 0;  // Default acceleration

    radius = Aleatoire::Normale(DEBRIS_MIN_RADIUS, DEBRIS_MAX_RADIUS);
    Density = Aleatoire::Normale(DEBRIS_MIN_DENSITY, DEBRIS_MAX_DENSITY);
    
    Color = {int(Aleatoire::Uniforme(60, 120)),  // R
             int(Aleatoire::Uniforme(60, 120)),  // G
             int(Aleatoire::Uniforme(60, 120))}; // B


	// Calculate the planet's mass based on its radius and density
	Mass = (4.0 / 3.0) * M_PI * std::pow(radius, 3) * Density;

}

