#include <iostream>
#include <cmath>
#include "ponto.h"
#include "vector.h"


#ifndef RAIO_H
#define RAIO_H

using namespace std;

template<typename T>
struct raio { 
    vetor<T> origem;
    vetor<T> direcao;

    raio(vetor<T> comeco, vetor<T> direc) {
        this->origem = comeco;
        this->direcao = vetorUni(direc);
    }
};

template<typename T>
vetor<T> raioAt(raio<T> r, T t) { // calcula o ponto no espaço que o raio atinge 
    
    return soma(r.origem, mult(t, r.direcao));
}


#endif 