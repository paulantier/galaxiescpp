#ifndef ALEATOIRE_H
#define ALEATOIRE_H

#include <random>

class Aleatoire {
public:
    // Génère un nombre suivant une loi normale centrée entre a et b
    static double Normale(double a, double b);
    
    // Génère un nombre suivant une loi log normale centrée entre a et b
    static double LogNormale(double a, double b);

    // Génère un nombre suivant une loi uniforme entre a et b
    static double Uniforme(double a, double b);
    
    static bool PileOuFace(double probabilityTrue);

private:
    // Générateur de nombres aléatoires
    static std::mt19937& getGenerator();
};

#endif // ALEATOIRE_H
