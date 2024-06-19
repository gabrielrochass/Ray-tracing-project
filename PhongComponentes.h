#ifndef PHONGCOMPONENTES_H
#define PHONGCOMPONENTES_H

#include "vector.h"
#include "raio.h"
#include <vector>
#include <algorithm>
using namespace std;

class phongComponentes{
    public:
        phongComponentes() {}
        phongComponentes(double ka, double kd, double ks, double n) {
            
            this->ka = min(ka, 1.0);
            this->kd = min(kd, 1.0);
            this->ks = min(ks, 1.0);
            this->n = n;
            
        }

        double ka;
        double kd;
        double ks;
        double n;
};

struct iluminacao
{
    vetor<double> posicao;
    vetor<double> Ia;
    vetor<double> Id;
    vetor<double> Is;
    
};

vetor<double> calcularIluminacaoPhong(
     vetor<double> pontoIntersecao, 
     vetor<double> Normal, 
     vetor<double> posicaoObservador,
     iluminacao luz, 
     phongComponentes material) 
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    vetor<double> R = normal(subtracao(produtoVetorial(multiplicacaoPorEscalar(produtoVetorial(Normal, L), 2.0),Normal), L)); // Reflexão da luz

    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
    vetor<double> especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));

    // Iluminação resultante
    vetor<double> I = soma(soma(ambiente, difusa), especular);
    return I;
}


#endif










