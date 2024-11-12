#ifndef STAR_H
#define STAR_H

#include "CelestialObject.h"
#include "Aleatoire.h"

#include <vector>
#include <cmath>

class Star : public CelestialObject {
public:
    double Density;
    double Mass;
    double Temperature;
    std::vector<int> Color; // RGB, 8-bit
    std::vector<int> ColorBorder; // RGB, 8-bit

    // Default constructor (no parameters)
    Star();

    // Update position (inherited from CelestialObject)
    void updatePosition() override {
        CelestialObject::updatePosition(); // USELESS BUT MIGHT BECOME USEFUL
    }
    
    double getMass() const override {return Mass;}
    double getVolume() const override {return 4.0 * M_PI * radius * radius * radius / 3.0;}
    void setMass(double newMass) override {Mass = newMass;}
    void setDensity(double newDensity) override {Density = newDensity;}
    void updateTemperature() override {Temperature = kT_M_R * std::exp(0.875 * std::log(Mass/kStarDensityCorrectionFactor) - 0.5 * std::log(radius * 1E-4));}
    void updateStarColorFromTemperature() override  {updateStarColor();}

private:
	void updateStarColor() { 
    	// Valeurs RGB pour différentes températures stellaires (en K)
    	const int temp_ranges[] = {2000, 3500, 5000, 6000, 7500, 10000};
    	const int colors[][3] = {
        	{255, 60, 60},   // Rouge (2000 - 3500 K)
        	{255, 130, 60},  // Orange (3500 - 5000 K)
        	{255, 240, 180}, // Jaune (5000 - 6000 K)
        	{240, 240, 240}, // Blanc (6000 - 7500 K)
        	{180, 240, 255}, // Bleu-blanc (7500 - 10000 K)
        	{150, 200, 255}  // Bleu (10000 K et plus)
    	};
	
    	// Cas où la température est inférieure à 2000 K
    	if (Temperature < temp_ranges[0]) {
        	Color = {colors[0][0], colors[0][1], colors[0][2]};
        	ColorBorder = {colors[0][0], colors[0][1], colors[0][2]};
        	return;
    	}
	
    	// Cas où la température est supérieure à 10 000 K
    	if (Temperature >= temp_ranges[5]) {
        	Color = {colors[5][0], colors[5][1], colors[5][2]};
        	ColorBorder = {colors[5][0], colors[5][1], colors[5][2]};
        	return;
    	}
	
    	// Interpolation linéaire pour les températures intermédiaires
    	for (int i = 0; i < 5; ++i) {
        	if (Temperature >= temp_ranges[i] && Temperature < temp_ranges[i + 1]) {
            	double t_ratio = (double)(Temperature - temp_ranges[i]) / (temp_ranges[i + 1] - temp_ranges[i]);
            	
            	int r = colors[i][0] + t_ratio * (colors[i + 1][0] - colors[i][0]);
            	int g = colors[i][1] + t_ratio * (colors[i + 1][1] - colors[i][1]);
            	int b = colors[i][2] + t_ratio * (colors[i + 1][2] - colors[i][2]);
	
            	Color = {r, g, b};
            	ColorBorder = {std::min(r + StarBorderColorOffset,255),
            				   std::min(g + StarBorderColorOffset,255), 
            				   std::min(b + StarBorderColorOffset,255)};
            	return;
        	}
    	}
	}

};

#endif // STAR_H
