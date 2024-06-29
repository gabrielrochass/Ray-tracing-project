#ifndef PHONGCOMPONENTES_H
#define PHONGCOMPONENTES_H

#include "vector.h"
#include "raio.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <memory>
using namespace std;


const double infinito = std::numeric_limits<double>::infinity();

class phongComponentes{
    public:
        phongComponentes() {}
        phongComponentes(double ka, double kd, double ks, double n) {
            
            this->ka = min(ka, 1.0);
            this->kd = min(kd, 1.0);
            this->ks = min(ks, 1.0);
            this->n = n;
            
        }

        // as constantes variam de 0 a 1
        double ka; // ambiente -> o quanto um objeto reage à luz ambiente
        double kd; // difusa -> fosco
        double ks; // especular -> brilhante
        double n; // expoente especular -> o quanto rugoso é o objeto (quanto maior, mais brilhante)
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
     phongComponentes material,
     sphere_list esferas) 
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    // L = Normal de L', L' = posicao - pontoIntersecao
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    // V = Normal de V', V' = posicaoObservador - pontoIntersecao
    vetor<double> R = normal(subtracao(produtoVetorial(multiplicacaoPorEscalar(produtoVetorial(Normal, L), 2.0),Normal), L)); // Reflexão da luz
    // R = Normal de R', R' = 2 * (Normal * L) * Normal - L


    // Verificar se o ponto está na sombra
    raio<double> raioSombra(pontoIntersecao, L);
    bool emSombra = false;
    hit_record temp_rec;
    if (esferas.hit(raioSombra, 0.001, infinito, temp_rec)) {
        emSombra = true;
    }

    
/*
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    // Iluminação ambiente = Ia * ka
    vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
    // Iluminação difusa = Id * (Normal * L) * kd
    vetor<double> especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    // Iluminação especular = Is * ks * (R * V)^n
*/
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    vetor<double> difusa = {0, 0, 0};
    vetor<double> especular = {0, 0, 0};
    
    if (!emSombra) {
        difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
        especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    }
    // Iluminação resultante
    vetor<double> I = soma(soma(ambiente, difusa), especular);


    return I;
}

vetor<double> calcularIluminacaoPhongTri(
     vetor<double> pontoIntersecao, 
     vetor<double> Normal, 
     vetor<double> posicaoObservador,
     iluminacao luz, 
     phongComponentes material,
     malha triangulos) 
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    // L = Normal de L', L' = posicao - pontoIntersecao
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    // V = Normal de V', V' = posicaoObservador - pontoIntersecao
    vetor<double> R = normal(subtracao(produtoVetorial(multiplicacaoPorEscalar(produtoVetorial(Normal, L), 2.0),Normal), L)); // Reflexão da luz
    // R = Normal de R', R' = 2 * (Normal * L) * Normal - L


    // Verificar se o ponto está na sombra
    raio<double> raioSombra(pontoIntersecao, L);
    bool emSombra = false;
    hit_record temp_rec;
    if (triangulos.hit(raioSombra, 0.001, infinito, temp_rec)) {
        emSombra = true;
    }

    
/*
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    // Iluminação ambiente = Ia * ka
    vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
    // Iluminação difusa = Id * (Normal * L) * kd
    vetor<double> especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    // Iluminação especular = Is * ks * (R * V)^n
*/
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    vetor<double> difusa = {0, 0, 0};
    vetor<double> especular = {0, 0, 0};
    
    if (!emSombra) {
        difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
        especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    }
    // Iluminação resultante
    vetor<double> I = soma(soma(ambiente, difusa), especular);


    return I;
}

vetor<double> calcularIluminacaoPhongPlano(
     vetor<double> pontoIntersecao, 
     vetor<double> Normal, 
     vetor<double> posicaoObservador,
     iluminacao luz, 
     phongComponentes material,
     plano plano) 
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    // L = Normal de L', L' = posicao - pontoIntersecao
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    // V = Normal de V', V' = posicaoObservador - pontoIntersecao
    vetor<double> R = normal(subtracao(produtoVetorial(multiplicacaoPorEscalar(produtoVetorial(Normal, L), 2.0),Normal), L)); // Reflexão da luz
    // R = Normal de R', R' = 2 * (Normal * L) * Normal - L


    // Verificar se o ponto está na sombra
    raio<double> raioSombra(pontoIntersecao, L);
    bool emSombra = false;
    hit_record temp_rec;
    if (plano.hitPlano(raioSombra, 0.001, infinito, temp_rec)) {
        emSombra = true;
    }

    
/*
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    // Iluminação ambiente = Ia * ka
    vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
    // Iluminação difusa = Id * (Normal * L) * kd
    vetor<double> especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    // Iluminação especular = Is * ks * (R * V)^n
*/
    // Componentes de iluminação
    vetor<double> ambiente = multiplicacaoPorEscalar(luz.Ia, material.ka);
    vetor<double> difusa = {0, 0, 0};
    vetor<double> especular = {0, 0, 0};
    
    if (!emSombra) {
        difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
        especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
    }
    // Iluminação resultante
    vetor<double> I = soma(soma(ambiente, difusa), especular);


    return I;
}


#endif



// phong
// i = Ia * ka + Id * (N * L) * kd + Is * ks * (R * V)^n
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







