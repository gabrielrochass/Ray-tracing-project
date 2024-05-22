#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"

#include <bits/stdc++.h>

using namespace std;

struct hit_record {
    vetor<double> p;
    vetor<double> normal;
    double t;
};

class triangulo{
    public:
        triangulo() {}
        triangulo(vetor<double> v1, vetor<double> v2, vetor<double> v3) : vetor1(v1), vetor2(v2), vetor3(v3) {};

        bool hit(
            const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;
        
    public:
        vetor<double> vetor1;
        vetor<double> vetor2;
        vetor<double> vetor3;
        vetor<double> vertice1;
        vetor<double> vertice2;
        vetor<double> vertice3;
};

bool sphere::hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const{
    double denominador = produtoEscalar(normal, raioDeInterseccao.direcao); // usado pra determinar se o raio é paralelo ao plano
    // Verifica se o denominador é zero para evitar divisão por zero
    if (fabs(denominador) < 1e-6) { // 0
        return false; // Raio é paralelo ao plano -> não intersecta
    }
    // Calcula a distância para o ponto de interseção ao longo do raio
    double t = -produtoEscalar(subtracao(raioDeInterseccao.origem, pontoNoPlano), normal) / denominador;
    return t >= 0; // Se t fo
}

#endif