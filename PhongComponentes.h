#ifndef PHONGCOMPONENTES_H
#define PHONGCOMPONENTES_H

#include "vector.h"
#include "raio.h"
#include "octree.cpp"  // Inclua o arquivo onde OctreeNode está definido
#include <vector>
#include <algorithm>
#include <limits>
#include <memory>
using namespace std;

const double infinito = std::numeric_limits<double>::infinity();

class phongComponentes {
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

    // Constantes variam de 0 a 1
    double ka; // Ambiente -> o quanto um objeto reage à luz ambiente
    double kd; // Difusa -> Fosco
    double ks; // Espacular -> Brilhante
    double n;  // Expoente especular -> O quanto rugoso é o objeto (quanto maior, mais brilhante)
    double kr; // Reflexão -> O quanto o objeto reflete a luz (espelhado)
    double kt; // Transmissão -> O quanto o objeto transmite a luz (transparente)
    double n1; // Índice de refração do meio 1
    double n2; // Índice de refração do meio 2
};

struct iluminacao {
    vetor<double> posicao;
    vetor<double> Ia;
    vetor<double> Id;
    vetor<double> Is;
};

struct listaLuzes {
    vector<iluminacao> luzes;
    void addLuz(iluminacao luz) {
        luzes.push_back(luz);
    }
    iluminacao acessarLuz(int i) const {
        return luzes[i];
    }
};

// Calcula vetor de reflexão
vetor<double> calcularReflexao(vetor<double> Normal, vetor<double> L) {
    return normal(2 * produtoEscalar(Normal, L) * Normal - L);
}

// Calcula vetor de refração
vetor<double> calcularRefracao(const vetor<double>& V, const vetor<double>& N, double ni, double nt) {
    double cosO = produtoEscalar(N, V);
    double eta = ni / nt;
    double constante = 1 - eta * eta * (1 - cosO * cosO);
    if (constante < 0) {
        return calcularReflexao(V, N);
    }
    double cosOt = sqrt(constante);
    return (eta * cosO - cosOt) * N - eta * V;
}

vetor<double> calcularIluminacaoPhong(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    const OctreeNode& octree,
    plano plano,
    int profundidade) 
{
    Normal = normal(Normal);

    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // Calcula parâmetros da equação de Phong
    for (const auto& luz : luzes.luzes) {
        vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz

        // Componentes de iluminação
        vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
        vetor<double> especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luz.Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }

    vetor<double> corDaEsfera = {1, 0, 0};
    vetor<double> corDoPlano = {0, 0, 0};

    hit_record rec;
    if (octree.intersectou(raio<double>(pontoIntersecao, normal(subtracao(pontoIntersecao, posicaoObservador))), 0.001, infinito, rec)) {
        corDaEsfera = rec.cor;
    }

    if (profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, corDaEsfera), 0.8);
    }

    // Reflexão
    vetor<double> corReflexao = {0, 0, 0};
    if (material.kr > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> raioReflexao(pontoIntersecao, R);
        hit_record recReflexao;
        if (octree.intersectou(raioReflexao, 0.001, infinito, recReflexao)) {
            corReflexao = calcularIluminacaoPhong(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, octree, plano, profundidade - 1);
            corDaEsfera = produtoVetorial(corReflexao, recReflexao.cor);
        }
        corReflexao = multiplicacaoPorEscalar(corReflexao, material.kr);
    }

    // Refração
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    vetor<double> corRefracao = {0, 0, 0};
    if (material.kt > 0) {
        vetor<double> R = calcularRefracao(normal(subtracao(posicaoObservador, pontoIntersecao)), Normal, material.n1, material.n2);
        raio<double> raioRefracao(pontoIntersecao, R);
        hit_record recRefracao;
        if (octree.intersectou(raioRefracao, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhong(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, octree, plano, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        corRefracao = multiplicacaoPorEscalar(corRefracao, material.kt);
    }

    I = I + corReflexao + corRefracao;
    return multiplicacaoPorEscalar(produtoVetorial(I, corDaEsfera), 0.8);
}

vetor<double> calcularIluminacaoPhongPlano(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    plano plano,
    const OctreeNode& octree,
    int profundidade) 
{
    Normal = normal(Normal);

    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // Calcula parâmetros da equação de Phong
    for (const auto& luz : luzes.luzes) {
        vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz

        // Componentes de iluminação
        vetor<double> difusa = multiplicacaoPorEscalar(luz.Id, (produtoEscalar(Normal, L) * material.kd));
        vetor<double> especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luz.Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }

    vetor<double> corDoPlano = {1, 1, 0};

    if (profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, corDoPlano), 0.8);
    }

    // Reflexão
    vetor<double> corReflexao = {0, 0, 0};
    if (material.kr > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> raioReflexao(pontoIntersecao, R);
        hit_record recReflexao;
        if (octree.intersectou(raioReflexao, 0.001, infinito, recReflexao)) {
            corReflexao = calcularIluminacaoPhongPlano(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, plano, octree, profundidade - 1);
            corDoPlano = produtoVetorial(corReflexao, recReflexao.cor);
        }
        corReflexao = multiplicacaoPorEscalar(corReflexao, material.kr);
    }

    // Refração
    vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
    vetor<double> corRefracao = {0, 0, 0};
    if (material.kt > 0) {
        vetor<double> R = calcularRefracao(normal(subtracao(posicaoObservador, pontoIntersecao)), Normal, material.n1, material.n2);
        raio<double> raioRefracao(pontoIntersecao, R);
        hit_record recRefracao;
        if (octree.intersectou(raioRefracao, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhongPlano(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, plano, octree, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        corRefracao = multiplicacaoPorEscalar(corRefracao, material.kt);
    }

    I = I + corReflexao + corRefracao;
    return multiplicacaoPorEscalar(produtoVetorial(I, corDoPlano), 0.8);
}

#endif // PHONGCOMPONENTES_H
