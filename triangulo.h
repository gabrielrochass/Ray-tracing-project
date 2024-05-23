#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "vector.h"
#include "hit_record.h"

#include <bits/stdc++.h>

using namespace std;

class triangulo{
    public:
        triangulo() {}
        triangulo(vetor<double> v1, vetor<double> v2, vetor<double> v3) : vertice1(v1), vertice2(v2), vertice3(v3) {
            vetor1 = subtracao(vertice2, vertice1);
            vetor2 = subtracao(vertice3, vertice1);
            vetor3 = subtracao(vertice3, vertice2);
        };

        bool hit(const vetor<double>& pontoNoPlano, const vetor<double>& normal, const raio<double>& raioDeInterseccao, const raio<double>& raio, double t_min, double t_max, hit_record& rec) const{
                double denominador = produtoEscalar(normal, raioDeInterseccao.direcao); // usado pra determinar se o raio é paralelo ao plano
                // Verifica se o denominador é zero para evitar divisão por zero
                if (fabs(denominador) < 1e-6) { // 0
                    return false; // Raio é paralelo ao plano -> não intersecta
                }
                // Calcula a distância para o ponto de interseção ao longo do raio
                double t = -produtoEscalar(subtracao(raioDeInterseccao.origem, pontoNoPlano), normal) / denominador;
                if (t <0){
                    return false;
                } 

                // Calculando as coordenadas baricêntricas
                denominador = (vertice2.y - vertice3.y) * (vertice1.x - vertice3.x) + (vertice3.x - vertice2.x) * (vertice1.y - vertice3.y);

                double lambda1 = ((vertice2.y - vertice3.y) * (pontoNoPlano.x - vertice3.x) + (vertice3.x - vertice2.x) * (pontoNoPlano.y - vertice3.y)) / denominador;
                double lambda2 = ((vertice3.y - vertice1.y) * (pontoNoPlano.x - vertice3.x) + (vertice1.x - vertice3.x) * (pontoNoPlano.y - vertice3.y)) / denominador;
                double lambda3 = 1.0 - lambda1 - lambda2;

                // Verificando se o ponto de interseção está dentro do triângulo
                if (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0) {
                    // O ponto de interseção está dentro do triângulo
                    return true;
                }
                return false;
            }
        
    public:
        vetor<double> vetor1;
        vetor<double> vetor2;
        vetor<double> vetor3;
        vetor<double> vertice1;
        vetor<double> vertice2;
        vetor<double> vertice3;
};

#endif