#include <iostream>
#include "vector.h"
#include "ponto.h"

using namespace std;

struct Raio {
    Vetor origem;
    Vetor direcao;

    Raio(Vetor comeco, Vetor direc) {
        this->origem = comeco;
        this->direcao = unitario(direc);
    }
};

