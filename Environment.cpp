#include "Environment.h"

#include "CelestialObject.h"
#include "Star.h"
#include "Planet.h"
//#include "Asteroid.h"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>

Environment::Environment() {}

void Environment::addStar() {
    const int maxAttempts = 100;  // maximum number of attempts to find a valid position

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        Star* newStar = new Star();
        
        bool isFarEnough = true;
        for (const auto& celestialObject : celestialObjects) {
            double distance = std::sqrt((newStar->x - celestialObject->x) * (newStar->x - celestialObject->x) +
                                        (newStar->y - celestialObject->y) * (newStar->y - celestialObject->y));
            if (distance < MIN_STAR_DISTANCE) {
                isFarEnough = false;
                break;
            }
        }

        if (isFarEnough) {
            celestialObjects.push_back(newStar);  // Add to the list of celestial objects
            break;  // exit loop as the star has been successfully added
        }
    }

}

void Environment::addPlanet() {
    const int maxAttempts = 100;  // maximum number of attempts to find a valid position

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        //std::cout << "Attempt " << attempt + 1 << "/" << maxAttempts << std::endl;

        Planet* newPlanet = new Planet();
        //std::cout << "New planet created at (" << newPlanet->x << ", " << newPlanet->y << ")" << std::endl;

        bool isFarEnough = true;
        for (const auto& celestialObject : celestialObjects) {
            if (!celestialObject) {
                //std::cerr << "Warning: Null celestial object encountered!" << std::endl;
                continue;
            }

            double distance = (newPlanet->x - celestialObject->x) * (newPlanet->x - celestialObject->x) +
                              (newPlanet->y - celestialObject->y) * (newPlanet->y - celestialObject->y);
            //std::cout << "Distance to object at (" << celestialObject->x << ", " << celestialObject->y << "): " << distance << std::endl;

            if (isStar(celestialObject) && distance < MIN_STAR_DISTANCE * MIN_STAR_DISTANCE) {
                //std::cout << "Too close to a star, retrying..." << std::endl;
                isFarEnough = false;
                break;
            }
            
            if (isPlanet(celestialObject) && distance < MIN_PLANET_DISTANCE * MIN_PLANET_DISTANCE) {
                //std::cout << "Too close to a planet, retrying..." << std::endl;
                isFarEnough = false;
                break;
            }
        }

        if (isFarEnough) {
            celestialObjects.push_back(newPlanet); 
            //std::cout << "Planet added at (" << newPlanet->x << ", " << newPlanet->y << ")" << std::endl;
            break;  // exit loop as the planet has been successfully added
        } else {
            delete newPlanet;
            //std::cout << "New planet deleted, position was invalid." << std::endl;
        }
    }
}

void Environment::addDebris() {

	Debris* newDebris = new Debris();
	
	bool isFarEnough = true;
	
    for (const auto& celestialObject : celestialObjects) {
        if (!celestialObject) {
            continue;
        }

        double distance = (newDebris->x - celestialObject->x) * (newDebris->x - celestialObject->x) +
                          (newDebris->y - celestialObject->y) * (newDebris->y - celestialObject->y);

        if (distance < (celestialObject->radius + newDebris->radius) *
        			   (celestialObject->radius + newDebris->radius)) {
            isFarEnough = false;
            break;
        }
    }
    if (isFarEnough) {
        celestialObjects.push_back(newDebris); 
    } else {
        delete newDebris;
    }
}

void Environment::createDebris() {
	//celestialObjects.insert(celestialObjects.end(), objectsToCreate.begin(), objectsToCreate.end());
	//objectsToCreate.clear();
	if (objectsToCreate.size() > 0) {
		celestialObjects.push_back(objectsToCreate[0]);
		objectsToCreate.erase(objectsToCreate.begin());
	}
}


void Environment::applyGravitationalForce(CelestialObject* Object1, CelestialObject* Object2) {
    // Get the positions of the two objects
    double x1 = Object1->x, y1 = Object1->y;
    double x2 = Object2->x, y2 = Object2->y;

    // Calculate the distance between the two objects
    double dx = x2 - x1;
    double dy = y2 - y1;
    double r = std::sqrt(dx * dx + dy * dy);

    if (r == 0) {
        return;
    }

    // Cast to the appropriate class to access Mass
    double mass1 = 0, mass2 = 0;
	
	mass1 = Object1->getMass();
	mass2 = Object2->getMass();

    // Calculate the gravitational force magnitude
    double force = GravitationalConstant * (mass1 * mass2) / (r * r);

    // Calculate the acceleration for each object
    double ax1 = force * dx / (mass1 * r);  // Acceleration of Object1 in x direction
    double ay1 = force * dy / (mass1 * r);  // Acceleration of Object1 in y direction
    double ax2 = -force * dx / (mass2 * r); // Acceleration of Object2 in x direction
    double ay2 = -force * dy / (mass2 * r); // Acceleration of Object2 in y direction

    // Apply the accelerations to the objects
    Object1->ax += ax1;
    Object1->ay += ay1;
    Object2->ax += ax2;
    Object2->ay += ay2;
}

void Environment::applyRelations() {
    // Loop over each unique pair without repetition
    int loop_size = celestialObjects.size();
    
    for (int i = 0; i < loop_size; ++i) {
        for (int j = i + 1; j < loop_size; ++j) {
            applyGravitationalForce(celestialObjects[i], celestialObjects[j]);
        }
    }
}

void Environment::updatePositions() {
	int loop_size = celestialObjects.size();

	for (int i = 0; i < loop_size; ++i) {
		
		celestialObjects[i]->updateVelocity();
		celestialObjects[i]->updatePosition();
		celestialObjects[i]->resetAcceleration();
	}
}

bool Environment::isStar(CelestialObject* obj) {
    return dynamic_cast<Star*>(obj) != nullptr;  // Checks if obj is a Star
}

bool Environment::isPlanet(CelestialObject* obj) {
    return dynamic_cast<Planet*>(obj) != nullptr;  // Checks if obj is a Planet
}

bool Environment::isDebris(CelestialObject* obj) {
    return dynamic_cast<Debris*>(obj) != nullptr;  // Checks if obj is a Debris
}



void Environment::checkCollisions() {
	int initialNumberOfObjects = celestialObjects.size();
	std::vector<int> objectsToRemove = {};
	int idToRemove = -1;
	
    for (int i = 0; i < initialNumberOfObjects; ++i) {
        for (int j = i + 1; j < initialNumberOfObjects; ++j) {
        
            double dx = celestialObjects[j]->x - celestialObjects[i]->x;
            double dy = celestialObjects[j]->y - celestialObjects[i]->y;
            double distance = std::sqrt(dx * dx + dy * dy);
			
            if (distance < celestialObjects[i]->radius+celestialObjects[j]->radius) {
                // Collision detected between celestialObjects[i] and celestialObjects[j]
                //std::cout << "Collision to handle" << std::endl;
                idToRemove = handleCollision(celestialObjects[i], celestialObjects[j], distance, celestialObjects[i]->getRadius(), celestialObjects[j]->getRadius(), i, j);
                //std::cout << "Collision handled" << std::endl;
            }
            
            if (idToRemove != -1){
            	objectsToRemove.push_back(idToRemove);
            }
        }
    }
    
	// Sort the list of objects to remove in reverse order
    std::sort(objectsToRemove.rbegin(), objectsToRemove.rend());
    //Removing duplicates
    objectsToRemove.erase(std::unique(objectsToRemove.begin(), objectsToRemove.end()), objectsToRemove.end());


    // Now remove objects in reverse order to prevent shifting issues
    for (int id_iter : objectsToRemove) {
        if (id_iter >= 0 && static_cast<std::size_t>(id_iter) < celestialObjects.size()) { // type casting to avoid warning
            //std::cout << "Removing object with id: " << id_iter << std::endl;
            std::cout << "Removing object !" << std::endl;
            celestialObjects.erase(celestialObjects.begin() + id_iter);
        }
    }
    
}

int Environment::handleCollision(CelestialObject* obj1, CelestialObject* obj2, double distance, double radius1, double radius2, int loop_id1, int loop_id2) {
    
    int idToRemove=-1;
    
    if (isStar(obj1) && isStar(obj2) && distance < 0.5 * (radius1 + radius2) ) {
        
        std::cout << "Star-Star Collision !" << std::endl;
        
        CelestialObject* smallestStar;
        CelestialObject* largestStar;
        
        if (obj1->getMass() < obj2->getMass()) {
            smallestStar = obj1;
            largestStar = obj2;
            idToRemove = loop_id1;
        } else {
            smallestStar = obj2;
            largestStar = obj1;
            idToRemove = loop_id2;
        }
        	
		double totalMass = smallestStar->getMass() + largestStar->getMass();
        // Calculate new position for the larger star at the center of mass
        double newX = (smallestStar->x * smallestStar->getMass() + largestStar->x * largestStar->getMass())/totalMass;
        double newY = (smallestStar->y * smallestStar->getMass() + largestStar->y * largestStar->getMass())/totalMass;

        // Calculate new mass and volume based on density averaging
        double newMass = smallestStar->getMass() + largestStar->getMass();
        double rSmall3 = std::pow(smallestStar->getRadius(),3);
		double rLarge3 = std::pow(largestStar->getRadius(),3);
		double newRadius = std::cbrt(rSmall3 + rLarge3);

        double newDensity = newMass / (4.0*M_PI/3.0 * std::pow(newRadius,3));

        // Update the larger star's properties
        largestStar->setMass(newMass);
        largestStar->setRadius(newRadius);
        largestStar->setDensity(newDensity);
        largestStar->setPosition(newX, newY);

        // Calculate weighted average of their velocities
        double newVx = (smallestStar->vx * smallestStar->getMass() + largestStar->vx * largestStar->getMass())/totalMass;
        double newVy = (smallestStar->vy * smallestStar->getMass() + largestStar->vy * largestStar->getMass())/totalMass;
        largestStar->setVelocity(newVx,newVy);
        
        largestStar->updateTemperature();
        
        largestStar->updateStarColorFromTemperature();
        
    } else if (isPlanet(obj1) && isPlanet(obj2)) {
    	std::cout << "Planet-Planet Collision !" << std::endl;
    	CelestialObject* smallestPlanet;
        CelestialObject* largestPlanet;
        
        if (obj1->getMass() < obj2->getMass()) {
            smallestPlanet = obj1;
            largestPlanet = obj2;
            idToRemove = loop_id1;
        } else {
            smallestPlanet = obj2;
            largestPlanet = obj1;
            idToRemove = loop_id2;
        }
        
        double newMass = largestPlanet->getMass() - smallestPlanet->getMass();
        double totalMass = largestPlanet->getMass() + smallestPlanet->getMass();
		
		double newRadius = std::cbrt( (3 * largestPlanet->getVolume()) / (4 * M_PI) );
		
		// Calculate weighted average of their velocities
        double newVx = (smallestPlanet->vx * smallestPlanet->getMass() + largestPlanet->vx * largestPlanet->getMass())/totalMass;
        double newVy = (smallestPlanet->vy * smallestPlanet->getMass() + largestPlanet->vy * largestPlanet->getMass())/totalMass;
		
        // Update the larger star's properties
        largestPlanet->setMass(newMass);
        largestPlanet->setRadius(newRadius);
        largestPlanet->setVelocity(newVx,newVy);
        
        
        double totalEnergy = largestPlanet->getMass()*(largestPlanet->vx * largestPlanet->vx + largestPlanet->vy * largestPlanet->vx) + smallestPlanet->getMass()*(smallestPlanet->vx * smallestPlanet->vx + smallestPlanet->vy * smallestPlanet->vx);
        			  
        double largestPlanetNewEnergy = newMass * (newVx*newVx + newVy*newVy);
        
        double remainingEnergy = totalEnergy - largestPlanetNewEnergy;
        double remainingMass = totalMass - newMass;
        
        double unitX = (largestPlanet->x - smallestPlanet->x) / distance;
       	double unitY = (largestPlanet->y - smallestPlanet->y) / distance;
       	
       	double debrisVelocity = std::sqrt(remainingEnergy / remainingMass);
        
       	while (remainingMass > 0) {
       		
       		Debris* newDebris = new Debris();
       		
       		if (newDebris->getMass() <= remainingMass) {
       			remainingMass -= newDebris->getMass();
       			remainingEnergy-= remainingMass * debrisVelocity * debrisVelocity;
       		}
       		else {
       			
       			debrisVelocity = std::sqrt(remainingEnergy / newDebris->getMass());
       			remainingMass = 0.0;
       		
       		}
       			
       		double randomAnglePosition = Aleatoire::Uniforme(-M_PI/4.0, M_PI/4.0);
       		double randomAngleDirection = Aleatoire::Uniforme(-M_PI/2.0, M_PI/2.0);
       		
       		//std::cout << "unitX : " << unitX << std::endl;
       		//std::cout << "unitY : " << unitY << std::endl;
       		//std::cout << "cos(randomAngle) : " << std::cos(randomAnglePosition) << std::endl;
       		//std::cout << "sin(randomAngle) : " << std::sin(randomAnglePosition) << std::endl;
       		//std::cout << "newRadius : " << newRadius << std::endl;
       		
       		newDebris->x = largestPlanet->x + (newRadius + newDebris->radius + 1) * 
       						(unitX * std::cos(randomAnglePosition) - unitY * std::sin(randomAnglePosition));
       						
       		newDebris->y = largestPlanet->y + (newRadius + newDebris->radius + 1) * 
       						(unitX * std::sin(randomAnglePosition) + unitY * std::cos(randomAnglePosition));
       		
       		newDebris->vx = debrisVelocity * (unitX * std::cos(randomAngleDirection) - unitY * std::sin(randomAngleDirection));
       		newDebris->vy = debrisVelocity * (unitX * std::sin(randomAngleDirection) + unitY * std::cos(randomAngleDirection));
       		
       		//std::cout << "x : " << newDebris->x << std::endl;
       		//std::cout << "y : " << newDebris->y << std::endl;
       		
       		objectsToCreate.push_back(newDebris);  			
       		
       	}
                 
    } else if ((isStar(obj1) && isPlanet(obj2)) || (isStar(obj2) && isPlanet(obj1))) {
        // Star gains mass from the planet, and planet is destroyed
        std::cout << "Planet-Star Collision !" << std::endl;
        
        CelestialObject* planet;
        CelestialObject* star;
        int starRadius;
        
        if (isPlanet(obj1)){
        	planet = obj1;
        	star = obj2;
        	idToRemove = loop_id1;
        	starRadius = radius2;
        } else {
        	planet = obj2;
        	star = obj1;
        	idToRemove = loop_id2;
        	starRadius = radius1;
        }
        
        if (distance < starRadius) {

        	// Add planet's mass to the star
        	star->setMass(star->getMass() + planet->getMass());
	
        	star->setDensity(star->getMass() / star->getVolume());
        	
        	star->updateTemperature();
        	
        	star->updateStarColorFromTemperature();
        	
        } else {
        	idToRemove = -1;
        } 
        
    } else if ( (isDebris(obj1) && !isDebris(obj2)) || (isDebris(obj2) && !isDebris(obj2)) ) {
    	// Object gains mass from the debris, and debris is destroyed
        std::cout << "Debris Absorption!" << std::endl;
        
        CelestialObject* debris;
        CelestialObject* obj;
        
        if (isDebris(obj1)){
        
        	debris = obj1;
        	obj = obj2;
        	
        	idToRemove = loop_id1;
        	
        } else {
        
        	debris = obj2;        	
        	obj = obj1;
        	
        	idToRemove = loop_id2;
        }
        
        // Add debris' mass to the object
        obj->setMass(obj->getMass() + debris->getMass());

        obj->setDensity(obj->getMass() / obj->getVolume());
        
        obj->updateTemperature();
        
        obj->updateStarColorFromTemperature();
        
    }
    
    return idToRemove;
}
