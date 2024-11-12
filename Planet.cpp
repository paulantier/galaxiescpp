#include "Planet.h"
#include "Aleatoire.h"
#include "CelestialObject.h"  // Include CelestialObject base class

#include <cmath>
#include <vector>

//Constructor
Planet::Planet()
    : CelestialObject(),  // Initialize base class
      isTelluric(true)     // Default to telluric planet (can be random later)
{
    // Set position and velocity using Aleatoire (random)
    x = Aleatoire::Uniforme(-2*SCENE_WIDTH / kDisplaySizeRatio, 3*SCENE_WIDTH / kDisplaySizeRatio);
    y = Aleatoire::Uniforme(-2*SCENE_WIDTH / kDisplaySizeRatio, 3*SCENE_HEIGHT / kDisplaySizeRatio);
    vx = TimeRatio * Aleatoire::Uniforme(PLANET_MIN_INITIAL_SPEED, PLANET_MAX_INITIAL_SPEED);
    vy = TimeRatio * Aleatoire::Uniforme(PLANET_MIN_INITIAL_SPEED, PLANET_MAX_INITIAL_SPEED);  
    ax = 0; ay = 0;  // Default acceleration

    // Determine if the planet is telluric or gaseous
    isTelluric = Aleatoire::PileOuFace(0.6);  // 60% chance for a telluric planet

    if (isTelluric) { // Telluric planet
        radius = Aleatoire::Normale(PLANET_T_MIN_RADIUS, PLANET_T_MAX_RADIUS);
        Density = Aleatoire::Normale(PLANET_T_MIN_DENSITY, PLANET_T_MAX_DENSITY);

        // Randomly assign a type of telluric planet
        int PlanetType = rand() % 100;

        if (PlanetType < 5) {  // Earth-like
            Color = {int(Aleatoire::Uniforme(0, 50)),  // R
                   
// Co  int(Aleatoire::Uniforme(100, 200)),  // G
                     int(Aleatoire::Uniforme(150, 255))};  // B
        } else if (PlanetType < 30) {  // Mars-like
            Color = {int(Aleatoire::Uniforme(150, 255)),  // R
                     int(Aleatoire::Uniforme(50, 80)),    // G
                     int(Aleatoire::Uniforme(30, 50))};   // B
        } else if (PlanetType < 70) {  // Venus-like
            Color = {int(Aleatoire::Uniforme(200, 230)),  // R
                     int(Aleatoire::Uniforme(160, 190)),  // G
                     int(Aleatoire::Uniforme(0, 30))};    // B
        } else {  // Mercury-like
            Color = {int(Aleatoire::Uniforme(100, 150)),  // R
                     int(Aleatoire::Uniforme(100, 150)),  // G
                     int(Aleatoire::Uniforme(100, 150))}; // B
        }
    } else {  // Gas planet
        radius = Aleatoire::Normale(PLANET_G_MIN_RADIUS, PLANET_G_MAX_RADIUS);
        Density = Aleatoire::Normale(PLANET_G_MIN_DENSITY, PLANET_G_MAX_DENSITY);

        // Set color for gas planets
        Color = {int(Aleatoire::Uniforme(50, 100)),   // R
                 int(Aleatoire::Uniforme(150, 255)),  // G
                 int(Aleatoire::Uniforme(200, 255))}; // B
    }

    // Calculate the planet's mass based on its radius and density
    Mass = (4.0 / 3.0) * M_PI * std::pow(radius, 3) * Density;

    // Calculate the border color based on the planet's color
    ColorBorder = {std::max(0, Color[0] + PlanetBorderColorOffset),
                   std::max(0, Color[1] + PlanetBorderColorOffset),
                   std::max(0, Color[2] + PlanetBorderColorOffset)};
}

