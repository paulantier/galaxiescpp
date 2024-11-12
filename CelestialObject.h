// CelestialObject.h
#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>
#include "consts.h"

class CelestialObject {
public:
    double x, y;       // Position (in meters)
    double vx, vy;     // Velocity (in m/s)
    double ax, ay;     // Acceleration (in m/s^2)
    double radius;

    CelestialObject(double xPos = 0, double yPos = 0, double velocityX = 0, double velocityY = 0, double accelerationX = 0, double accelerationY = 0, double Radius = 0)
        : x(xPos), y(yPos), vx(velocityX), vy(velocityY), ax(accelerationX), ay(accelerationY), radius(Radius) {}

    virtual ~CelestialObject() = default; // Destructor because polymorphism

    virtual void updatePosition() {
        x += vx * TimeRatio / FrameRate;
        y += vy * TimeRatio / FrameRate;
    }

    virtual void updateVelocity() {
        vx += ax * TimeRatio / FrameRate;
        vy += ay * TimeRatio / FrameRate;
    }
    
    virtual void resetAcceleration() {
        ax = 0;
        ay = 0;
    }
    
    virtual void setPosition(double newX, double newY){
    	x = newX;
    	y = newY;
    }
    
    virtual void setVelocity(double newVx, double newVy){
    	vx = newVx;
    	vy = newVy;
    }
    
    virtual double getRadius() {
    	return radius;
    }
    
    virtual void setRadius(double newRadius) {
    	radius = newRadius;
    }
    
    
    virtual double getMass() const = 0; 
    virtual double getVolume() const = 0;  
    
    virtual void setMass(double newMass) = 0;
    virtual void setDensity(double newDensity) = 0;
    
    virtual void updateTemperature() = 0;
    virtual void updateStarColorFromTemperature() = 0;
    


};

#endif // CELESTIALOBJECT_H
