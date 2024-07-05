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
        phongComponentes(double ka, double kd, double ks, double n, double kr, double kt, double n1, double n2) {
            
            this->ka = min(ka, 1.0);
            this->kd = min(kd, 1.0);
            this->ks = min(ks, 1.0);
            this->n = n;
            this->kr = min(kr, 1.0);
            this->kt = min(kt, 1.0);
            this->n1 = n1;
            this->n2 = n2;
        }

        // as constantes variam de 0 a 1
        double ka; // ambiente -> o quanto um objeto reage à luz ambiente
        double kd; // difusa -> fosco
        double ks; // especular -> brilhante
        double n; // expoente especular -> o quanto rugoso é o objeto (quanto maior, mais brilhante)
        double kr; // reflexão -> o quanto o objeto reflete a luz (espelhado)
        double kt; // transmissão -> o quanto o objeto transmite a luz (transparente)
        double n1; // índice de refração do meio 1
        double n2; // índice de refração do meio 2

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

// calcula vetor de reflexão
vetor<double> calcularReflexao(vetor<double> Normal, vetor<double> L) {
    return normal(2 * produtoEscalar(Normal, L) * Normal - L);
}

// calcula vetor de refração
vetor<double> calcularRefração(vetor<double> Normal, vetor<double> L, double n1, double n2) {
    double n = n1 / n2;
    double cosI = -produtoEscalar(Normal, L);
    double sinT2 = n * n * (1.0 - cosI * cosI);
    if (sinT2 > 1.0) {
        return {0, 0, 0};
    }
    double cosT = sqrt(1.0 - sinT2);
    return normal(n * L + (n * cosI - cosT) * Normal);
}

vetor<double> calcularIluminacaoPhong(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas,
    int profundidade) 
{
    Normal = normal(Normal);
    double diffn = material.n1 / material.n2;
    
    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // calcula parâmetros da equação de phong
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
    }
    vector<double> corDaEsfera = {1, 0, 0};

    if (profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, vetor<double>{corDaEsfera[0], corDaEsfera[1], corDaEsfera[2]}), 0.3);
    }

    // Reflexão
    if (material.kr > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> raioReflexao(pontoIntersecao, R);
        hit_record rec;
        if (esferas.hit(raioReflexao, 0.001, infinito, rec)) {
            I = I + material.kr * calcularIluminacaoPhong(rec.p, rec.normal, posicaoObservador, luz, luzes, material, esferas, profundidade - 1);
        }
    }

    // Refração
    if (material.kt > 0) {
        vetor<double> R = calcularRefração(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)), 1.0, 1.5);
        raio<double> raioRefração(pontoIntersecao, R);
        hit_record rec;
        if (esferas.hit(raioRefração, 0.001, infinito, rec)) {
            I = I + material.kt * calcularIluminacaoPhong(rec.p, rec.normal, posicaoObservador, luz, luzes, material, esferas, profundidade - 1);
        }
    }

    return multiplicacaoPorEscalar(produtoVetorial(I, vetor<double>{corDaEsfera[0], corDaEsfera[1], corDaEsfera[2]}), 0.3);

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







