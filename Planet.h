#ifndef PLANET_H
#define PLANET_H

#include "Aleatoire.h"
#include "CelestialObject.h"  // Include CelestialObject base class

#include <cmath>
#include <vector>

class Planet : public CelestialObject {  // Inherit from CelestialObject
public:
    bool isTelluric;  // Flag to distinguish between telluric or gas planets
    double Density;   // in kg/m3
    double Mass;
    std::vector<int> Color; // RGB, 8-bit
    std::vector<int> ColorBorder; // RGB, 8-bit
    
    Planet();  // Constructor
    
    // Inherited directly from CelestialObject put here for safety
    void updatePosition() override {
        CelestialObject::updatePosition(); // USELESS BUT MIGHT BECOME USEFUL
    }
    void updateTemperature() override{}
    void updateStarColorFromTemperature() override {}
    
    double getMass() const override {return Mass;}
    double getVolume() const override {return 4.0 * M_PI * radius * radius * radius / 3.0;}
    void setMass(double newMass) override {Mass = newMass;}
    void setDensity(double newDensity) override {Density = newDensity;}
};

#endif // PLANET_H
