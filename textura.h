#ifndef TEXTURA_H
#define TEXTURA_H

#include <vector>
#include <string>
#include "vector.h"

using namespace std;

struct Textura {
    vector<vector<vetor<double>>> textura;
    int largura, altura;

    Textura();
    Textura(const string& arquivo);
    vetor<double> corTextura(double u, double v) const;
};

#endif
