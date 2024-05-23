#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "sphere_list.h"
#include "sphere.h"
#include <limits>


struct plano {
    vetor<double> pontoNoPlano;
    vetor<double> normal;

    plano() {}
    plano(vetor<double> ponto, vetor<double> normal) : pontoNoPlano(ponto), normal(normal) {}
    bool hitPlano(const plano& plano, const raio<double>& raioDeInterseccao);
};

// define se um raio intersecta ou não um plano -> retorna true se intersecta, false caso contrário
bool plano::hitPlano(const plano& plano, const raio<double>& raioDeInterseccao) {
    double denominador = produtoEscalar(plano.normal, raioDeInterseccao.direcao); // usado pra determinar se o raio é paralelo ao plano
    // Verifica se o denominador é zero para evitar divisão por zero
    if (fabs(denominador) < 1e-6) { // 0
        return false; // Raio é paralelo ao plano -> não intersecta
    }
    // Calcula a distância para o ponto de interseção ao longo do raio
    double t = -produtoEscalar(subtracao(raioDeInterseccao.origem, plano.pontoNoPlano), plano.normal) / denominador;
    return t >= 0; // Se t for maior ou igual a zero, o raio intersecta o plano
} //**************
