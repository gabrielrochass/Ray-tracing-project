#include <iostream>
#include <cmath>

#ifndef VECTOR_H
#define VECTOR_H

// Criando estrutura Vetor
struct Vetor {
    double x, y, z;
    Vetor(double xRecebido, double yRecebido, double zRecebido) : x(xRecebido), y(yRecebido), z(zRecebido) {} // Construtor do Vetor
    Vetor() : x(0), y(0), z(0) {} // Construtor padrão (inicializa com zero)
};

// Função soma
Vetor soma(const Vetor& v1, const Vetor& v2) {
    return Vetor(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
};

// Função subtração
Vetor subtracao(const Vetor& v1, const Vetor& v2) {
    return Vetor(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
};

// Função Vetor unitário
Vetor unitario(const Vetor& v) {
    double norm = norma(v);
    return  Vetor(v.x / norm, v.y / norm, v.z / norm);
};

// Função Produto Vetorial
Vetor produtoVetorial(const Vetor& v1, const Vetor& v2) {
    return Vetor(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

// Função multiplicação por escalar
Vetor multiplicacaoPorEscalar(const Vetor& v, double escalar) {
    return Vetor(v.x * escalar, v.y * escalar, v.z * escalar);
}

// Função norma
double norma(const Vetor& vector) {
    return sqrt(pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2));
};

// Função do Produto Escalar
double produtoEscalar(const Vetor& v1, const Vetor& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}; 

#endif // VECTOR_H
