#include "Aleatoire.h"
#include <random>
#include <cmath>

// Initialisation du générateur aléatoire
std::mt19937& Aleatoire::getGenerator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

double Aleatoire::Normale(double a, double b) {
    // Centre de la distribution normale
    double mean = (a + b) / 2.0;
    // Écart-type : pour couvrir l'intervalle de manière raisonnable
    double stddev = std::abs(b - a) / 6.0;

    std::normal_distribution<double> dist(mean, stddev);
    return std::min(std::max(a,dist(getGenerator())),b);
}

double Aleatoire::LogNormale(double a, double b) {
    // Calcul de la médiane géométrique pour centrer la distribution
    double median = std::sqrt(a * b);

    // Calcul de l'écart-type logarithmique pour couvrir [a, b]
    double stddevLog = std::log(b / a) / 6.0;

    // Transformation pour la distribution log-normale
    double meanLog = std::log(median);

    std::lognormal_distribution<double> dist(meanLog, stddevLog);
    return std::min(std::max(a,dist(getGenerator())),b);
}

double Aleatoire::Uniforme(double a, double b) {
    std::uniform_real_distribution<double> dist(a, b);
    return std::min(std::max(a,dist(getGenerator())),b);
}

bool Aleatoire::PileOuFace(double probabilityTrue) {
    // Génère un nombre entre 0 et 1, retourne True si inférieur à la probabilité donnée
    return Uniforme(0.0, 1.0) < probabilityTrue;
}
