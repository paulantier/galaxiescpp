#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "CelestialObject.h"
#include "Star.h"
#include "Planet.h"
#include "Debris.h"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>


class Environment {

public:

    Environment();
    
    void addStar();
    void addPlanet();
    void addDebris();
    void createDebris();
    
    void applyRelations();  // Update all interactions and movement
    void updatePositions();
    
    void checkCollisions();
    int handleCollision(CelestialObject* obj1, CelestialObject* obj2, double distance, double radius1, double radius2, int loop_id1, int loop_id2);
    
    std::vector<CelestialObject*> celestialObjects;
    std::vector<CelestialObject*> objectsToCreate;
   
private :
	void applyGravitationalForce(CelestialObject* Object1, CelestialObject* Object2);
	
	bool isStar(CelestialObject* obj);
	
	bool isPlanet(CelestialObject* obj);
	
	bool isDebris(CelestialObject* obj);
};

#endif // ENVIRONMENT_H
