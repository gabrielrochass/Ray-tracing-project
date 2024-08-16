#ifndef TEXTURA_H
#define TEXTURA_H

#include <vector>
#include <string>
#include "vector.h"

using namespace std;

// define a estrutura da textura
struct Textura {
    vector<vector<vetor<double>>> textura; // matriz de cores da textura
    int largura, altura;

    Textura();
    Textura(const string& arquivo); // carrega a textura de um arquivo
    vetor<double> corTextura(double u, double v) const; // retorna a cor da textura em um ponto (u, v)
};

#endif
