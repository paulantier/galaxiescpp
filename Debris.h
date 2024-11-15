#ifndef DEBRIS_H
#define DEBRIS_H

#include "CelestialObject.h"  // Include CelestialObject base class

#include <cmath>
#include <vector>

class Debris : public CelestialObject {  // Inherit from CelestialObject
public:
    double Density;   // in kg/m3
    double Mass;
    std::vector<int> Color; // RGB, 8-bit
    
    Debris();  // Constructor
    
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

#endif // DEBRIS_H
