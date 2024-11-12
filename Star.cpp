#include "Star.h"

#include "CelestialObject.h"
#include "Aleatoire.h"

#include <vector>
#include <cmath>

Star::Star()
    : CelestialObject(),  // Initialize base class (CelestialObject)
      Density(0), Mass(0), Temperature(0), Color({0, 0, 0}), ColorBorder({0, 0, 0}) {

    // Random initialization of position
    x = Aleatoire::Uniforme(0, SCENE_WIDTH / kDisplaySizeRatio);
    y = Aleatoire::Uniforme(0, SCENE_HEIGHT / kDisplaySizeRatio);
    
    // Random initialization of the radius and density
    radius = Aleatoire::Normale(STAR_MIN_RADIUS, STAR_MAX_RADIUS);
    Density = kStarDensityCorrectionFactor * Aleatoire::LogNormale(STAR_MIN_DENSITY, STAR_MAX_DENSITY);
    
    // Calculate mass based on radius and density
    Mass = 4.0 / 3.0 * M_PI * std::pow(radius, 3.0) * Density;
    
    // Estimate the temperature using a relation between mass and radius (Mass-Luminosity relation)
    updateTemperature();
    
    updateStarColor();
}


