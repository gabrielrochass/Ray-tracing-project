#ifndef PHONGCOMPONENTES_H
#define PHONGCOMPONENTES_H

#include "vector.h"
#include "raio.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <memory>
#include <cmath>
using namespace std;


double infinito = std::numeric_limits<double>::infinity();

class phongComponentes{
    public:
        phongComponentes() {}
        phongComponentes(double ka, double kd, double ks, double n, double kr, double kt, double ni, double nt) {
            
            this->ka = min(ka, 1.0);
            this->kd = min(kd, 1.0);
            this->ks = min(ks, 1.0);
            this->n = n;
            this->kr = min(kr, 1.0);
            this->kt = min(kt, 1.0);
            this->ni = ni;
            this->nt = nt;
            
        }

        // as constantes variam de 0 a 1
        double ka; // ambiente -> o quanto um objeto reage à luz ambiente
        double kd; // difusa -> fosco
        double ks; // especular -> brilhante
        double n; // expoente especular -> o quanto rugoso é o objeto (quanto maior, mais brilhante)
        double kr; // reflexão
        double kt; // transmissão
        double ni; // índice de refração
        double nt; // índice de refração
};

struct iluminacao
{
    vetor<double> posicao;
    vetor<double> Ia;
    vetor<double> Id;
    vetor<double> Is;
    
};

struct listaLuzes
{
    vector<iluminacao> luzes;
    void addLuz(iluminacao luz) {
        luzes.push_back(luz);
    }
    iluminacao acessarLuz(int i) {
        return luzes[i];
    }
    
};

vetor<double> calcularRefracao( vetor<double> V,  vetor<double> N, double eta, vetor<double> L, phongComponentes material
   ) {
    
    double cosi = -produtoEscalar(V, N); // coseno do ângulo de incidência
    double etai = 1.0, etat = eta;
    vetor n2 = N;

    if (cosi < 0.0) {
        cosi = -cosi;
    } else {
        std::swap(etai, etat);
        n2 = multiplicacaoPorEscalar(N, -1.0);
    }

    double etaRatio = etai / etat;
    double k = 1.0 - etaRatio * etaRatio * (1.0 - cosi * cosi);
    
    // Se k for negativo, ocorre reflexão total interna
    if (k < 0.0) {
        return {0.0, 0.0, 0.0}; // Retornar vetor nulo
    } else {
        vetor primeiroTermo = multiplicacaoPorEscalar(V, etaRatio);
        vetor segundoTermo = multiplicacaoPorEscalar(n2, (etaRatio * cosi - sqrt(k)));
        return soma(primeiroTermo, segundoTermo);
    }
}
   /* 
    return (1 / eta) * V - ((1-material.nt) - (1-material.ni) / eta) * N; 
}*/

vetor<double> calcularIluminacaoPhong(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas
)
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    
    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> L = normal(subtracao(luzes.luzes[i].posicao, pontoIntersecao)); // Direção da luz
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz
        
        raio<double> raioSombra(pontoIntersecao, L);
        bool emSombra = false;
        hit_record temp_rec;
        if (esferas.hit(raioSombra, 0.001, infinito, temp_rec)) {
            emSombra = true;
        }

        // Componentes de iluminação
        vetor<double> difusa = {0, 0, 0};
        vetor<double> especular = {0, 0, 0};

        if (!emSombra) {
            difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd));
            especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n));
        }

        // Acumular iluminação resultante
        I = I + difusa + especular;
        if(i==0){
            double indiceRefracao = material.ni / material.nt;
            vetor<double> T = calcularRefracao(V, Normal, indiceRefracao, L, material);
            vetor<double> Ri = subtracao(produtoVetorial((Normal+Normal), produtoVetorial(Normal, V)), V);
            
            //Ri = 2 * N * (N * V) - V;
            //T = (1 / eta) * V - (cosOt - cosO / eta) * N;
            I = I + material.kr*Ri + material.kt*T;
        }
    }
    
    return I;
}
   
#endif
    
/*
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    // Iluminação ambiente = Ia * ka
    vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
    // Iluminação difusa = Id * (Normal * L) * kd
    vetor<double> especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    // Iluminação especular = Is * ks * (R * V)^n
*/

// phong
// 
// i = Ia * ka + Id * SOMATÓRIO[(N * L) * kd + Is * ks * (R * V)^n] + kr * R + kt * T
// i é a cor de cada pixel
// Ia é a intensidade da luz ambiente
// ka é a constante ambiente
// Id é a intensidade da luz difusa
// kd é a constante difusa
// Is é a intensidade da luz especular
// ks é a constante especular
// N é a normal do objeto
// L é a direção da luz
// R é a direção da reflexão da luz
// V é a direção para o observador
// n é o expoente especular

// i = ambiente + difusa + especular







