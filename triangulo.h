#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "vector.h"
#include "hit_record.h"
#include "raio.h"
#include <cmath>

using namespace std;

class triangulo {
public:
    triangulo() {}

    triangulo(const vetor<double>& v1, const vetor<double>& v2, const vetor<double>& v3) 
        : vertice1(v1), vertice2(v2), vertice3(v3) {
        vetor1 = subtracao(vertice2, vertice1);
        vetor2 = subtracao(vertice3, vertice1);
    }

    bool hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
        vetor<double> e1 = subtracao(vertice2, vertice1);
        vetor<double> e2 = subtracao(vertice3, vertice1);
        vetor<double> p = produtoVetorial(r.direcao, e2);
        double det = produtoEscalar(e1, p);

        // Verifica se o determinante é zero (raio paralelo ao triângulo)
        if (fabs(det) < 1e-8) {
            return false; // Sem interseção
        }

        double invDet = 1.0 / det;
        vetor<double> t = subtracao(r.origem, vertice1);
        double u = produtoEscalar(t, p) * invDet;

        // Verifica se u está dentro do triângulo
        if (u < 0.0 || u > 1.0) {
            return false;
        }

        vetor<double> q = produtoVetorial(t, e1);
        double v = produtoEscalar(r.direcao, q) * invDet;

        // Verifica se v está dentro do triângulo
        if (v < 0.0 || u + v > 1.0) {
            return false;
        }

        // Calcule o valor de t e a interseção
        double t_hit = produtoEscalar(e2, q) * invDet;

        // Verifica se a interseção está dentro dos limites
        if (t_hit < t_min || t_hit > t_max) {
            return false;
        }

        rec.t = t_hit;
        rec.p = raioAt(r, t_hit); // Calcula o ponto de interseção usando a função raioAt
        rec.normal = normal(); // Calcula a normal do triângulo
        return true; // Interseção encontrada
    }

    vetor<double> normal() const {
        // Calcula a normal do triângulo
        vetor<double> normal = produtoVetorial(vetor1, vetor2);
        return vetorUni(normal); // Normaliza a normal
    }

    vetor<double> vertice1, vertice2, vertice3;
    vetor<double> vetor1, vetor2; // Vetores para cálculo de norma
};

#endif
