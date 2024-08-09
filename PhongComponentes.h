#ifndef PHONGCOMPONENTES_H
#define PHONGCOMPONENTES_H

#include "vector.h"
#include "raio.h"
#include "cone.h"
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
vetor<double> calcularRefracao(const vetor<double>& V, const vetor<double>& N, double ni, double nt) {
   
    double cosO = produtoEscalar(N, V);
    double eta = ni / nt;
    double constante = 1 - eta * eta * (1 - cosO * cosO);
    if (constante < 0) {
        return calcularReflexao(V, N);
    }  
    double cosOt = sqrt(constante);
    return (eta*cosO - cosOt)*N - eta*V;
}

vetor<double> calcularIluminacaoPhong(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas,
    plano plano,
    cone_list cones,
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
        
        

        //Componentes de iluminação
        vetor<double> difusa = {0, 0, 0};
        vetor<double> especular = {0, 0, 0};
        difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd));
        especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }
    vetor<double> corDaEsfera = {1, 0, 0};
    vetor<double> corDoPlano = {0, 0, 0};

    hit_record rec;
    for(const auto& l : esferas.list) {
        if (l.hit(raio<double>(pontoIntersecao, normal(subtracao(pontoIntersecao, posicaoObservador))), 0.001, infinito, rec)) {
            corDaEsfera = rec.cor;
        }
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
        if (esferas.hit(raioReflexao, 0.001, infinito, recReflexao)) {
            corReflexao = calcularIluminacaoPhong(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, esferas,plano, cones, profundidade - 1);
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
        if (esferas.hit(raioRefracao, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhong(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, esferas,plano, cones, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        corRefracao = multiplicacaoPorEscalar(corRefracao, material.kt);
    }

   
    I = I + corReflexao + corRefracao;

    // Clamping final
    I.x = fmax(0.0, fmin(1.0, I.x));
    I.y = fmax(0.0, fmin(1.0, I.y));
    I.z = fmax(0.0, fmin(1.0, I.z));

    return multiplicacaoPorEscalar(produtoVetorial(I, corDaEsfera), 0.8);
    

}

vetor<double> calcularIluminacaoPhongCilindro(
    vetor<double> pontoIntersecao,
    vetor<double> Normal,
    vetor<double> posicaoObservador,
    iluminacao luz,
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas,
    cylinder_list cilindros,
    plano plano,
    int profundidade)
{
    Normal = normal(Normal);
    double diffn = material.n1 / material.n2;

    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // Calcula parâmetros da equação de Phong
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> L = normal(subtracao(luzes.luzes[i].posicao, pontoIntersecao)); // Direção da luz
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz

        
        vetor<double> difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd));
        
        
        vetor<double> especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }

    vetor<double> corDoCilindro = {0.5, 0.5, 1}; // cor padrão do cilindro, você pode atualizar isso conforme necessário
    hit_record rec;

    for (const auto& c : cilindros.list) {
        if (c.hit(raio<double>(pontoIntersecao, normal(subtracao(pontoIntersecao, posicaoObservador))), 0.001, infinito, rec)) {
            corDoCilindro = rec.cor;
        }
    }

    if(profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, corDoCilindro), 0.8);
    }

    // Reflexão
    if (material.kr > 0 && profundidade > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> refletido(pontoIntersecao, R);
        hit_record recReflexao;
        if (cilindros.hit(refletido, 0.001, infinito, recReflexao)) {
            I = I + multiplicacaoPorEscalar(calcularIluminacaoPhongCilindro(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, esferas, cilindros, plano, profundidade - 1), material.kr);
        }
    }

    // Refração
    if (material.kt > 0 && profundidade > 0) {
        vetor<double> T = calcularRefracao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)), material.n1, material.n2);
        raio<double> transmitido(pontoIntersecao, T);
        hit_record recRefracao;
        if (cilindros.hit(transmitido, 0.001, infinito, recRefracao)) {
            I = I + multiplicacaoPorEscalar(calcularIluminacaoPhongCilindro(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, esferas, cilindros, plano, profundidade - 1), material.kt);
        }
    }

    // Clamping final
    I.x = fmax(0.0, fmin(1.0, I.x));
    I.y = fmax(0.0, fmin(1.0, I.y));
    I.z = fmax(0.0, fmin(1.0, I.z));

    return I;

    
}

/*vetor<double> calcularIluminacaoPhongCone(
    vetor<double> pontoIntersecao,
    vetor<double> Normal,
    vetor<double> posicaoObservador,
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas,
    cone_list cones,
    plano plano,
    int profundidade)
{
    Normal = normal(Normal);
    double diffn = material.n1 / material.n2;

    // Inicializar vetor de iluminação resultante
    vetor<double> I = {0, 0, 0};
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // Calcula parâmetros da equação de Phong
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> L = normal(subtracao(luzes.luzes[i].posicao, pontoIntersecao)); // Direção da luz
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz

        
        vetor<double> difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd));
        
        
        vetor<double> especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }

    vetor<double> corDoCone = {0.5, 0.5, 1}; // cor padrão do cone, você pode atualizar isso conforme necessário
    

    hit_record rec;

    for (const auto& c : cones.list) {
        if (c.hit(raio<double>(pontoIntersecao, normal(subtracao(pontoIntersecao, posicaoObservador))), 0.001, infinito, rec)) {
            corDoCone = rec.cor;
        }
    }

    if (profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, corDoCone), 0.8);
    }

    // Reflexão
    vetor<double> corReflexao = {0, 0, 0};
    if (material.kr > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> refletido(pontoIntersecao, R);
        hit_record recReflexao;
        if (cones.hit(refletido, 0.001, infinito, recReflexao)) {
            corReflexao = calcularIluminacaoPhongCone(recReflexao.p, recReflexao.normal, posicaoObservador, luzes, material, esferas, cones, plano, profundidade - 1);
            corDoCone = produtoVetorial(corReflexao, recReflexao.cor);
        }
        corReflexao = multiplicacaoPorEscalar(corReflexao, material.kr);
    }
    // Refração
    vetor<double> corRefracao = {0, 0, 0};
    if (material.kt > 0) {
        vetor<double> T = calcularRefracao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)), material.n1, material.n2);
        raio<double> transmitido(pontoIntersecao, T);
         hit_record recRefracao;
        if (cones.hit(transmitido, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhongCone(recRefracao.p, recRefracao.normal, posicaoObservador, luzes, material, esferas, cones, plano, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        corRefracao = multiplicacaoPorEscalar(corRefracao, material.kt);
    }

    I = I + corReflexao + corRefracao;
    return  multiplicacaoPorEscalar(produtoVetorial(I, corDoCone), 0.8);
}*/

vetor<double> calcularIluminacaoPhongCone(
    vetor<double> pontoIntersecao,
    vetor<double> Normal,
    vetor<double> posicaoObservador,
    iluminacao luz,
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas,
    cone_list cones,
    plano plano,
    int profundidade)
{
    Normal = normal(Normal);
    vetor<double> I = {0, 0, 0};

    // Componente ambiente
    vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[0].Ia, material.ka);
    I = I + ambiente;

    // Componentes difusa e especular
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> L = normal(subtracao(luzes.luzes[i].posicao, pontoIntersecao));
        vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao));
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L);

        double cosNL = fmax(0.0, produtoEscalar(Normal, L));
        double cosRV = fmax(0.0, produtoEscalar(R, V));

        vetor<double> difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, cosNL * material.kd);
        vetor<double> especular = multiplicacaoPorEscalar(luzes.luzes[i].Is, pow(cosRV, material.n) * material.ks);

        I = I + difusa + especular;
    }

    if(profundidade <= 0) {
        return I;
    }

    // Reflexão
    if (material.kr > 0 && profundidade > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> refletido(pontoIntersecao, R);
        hit_record recReflexao;
        if (cones.hit(refletido, 0.001, infinito, recReflexao)) {
            I = I + multiplicacaoPorEscalar(calcularIluminacaoPhongCone(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, esferas, cones, plano, profundidade - 1), material.kr);
        }
    }

    // Refração
    if (material.kt > 0 && profundidade > 0) {
        vetor<double> T = calcularRefracao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)), material.n1, material.n2);
        raio<double> transmitido(pontoIntersecao, T);
        hit_record recRefracao;
        if (cones.hit(transmitido, 0.001, infinito, recRefracao)) {
            I = I + multiplicacaoPorEscalar(calcularIluminacaoPhongCone(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, esferas, cones, plano, profundidade - 1), material.kt);
        }
    }

    // Clamping final
    I.x = fmax(0.0, fmin(1.0, I.x));
    I.y = fmax(0.0, fmin(1.0, I.y));
    I.z = fmax(0.0, fmin(1.0, I.z));

    return I;
}



vetor<double> calcularIluminacaoPhongPlano(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    plano plano,
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
        
        

        //Componentes de iluminação
        vetor<double> difusa = {0, 0, 0};
        vetor<double> especular = {0, 0, 0};
        difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd));
        especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n));

        // Acumular iluminação resultante
        I = I + difusa + especular;
    }
    vetor<double> corDoPlano= {1,1,0};

    if (profundidade <= 0) {
        return multiplicacaoPorEscalar(produtoVetorial(I, corDoPlano), 0.8);
    }

    // Reflexão
    vetor<double> corReflexao = {0, 0, 0};
    if (material.kr > 0) {
        vetor<double> R = calcularReflexao(Normal, normal(subtracao(posicaoObservador, pontoIntersecao)));
        raio<double> raioReflexao(pontoIntersecao, R);
        hit_record recReflexao;
        if (plano.hitPlano(raioReflexao, 0.001, infinito, recReflexao)) {
            corReflexao = calcularIluminacaoPhongPlano(recReflexao.p, recReflexao.normal, posicaoObservador, luz, luzes, material, plano, esferas, profundidade - 1);
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
        if (plano.hitPlano(raioRefracao, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhongPlano(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, plano, esferas, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        else if (esferas.hit(raioRefracao, 0.001, infinito, recRefracao)) {
            corRefracao = calcularIluminacaoPhongPlano(recRefracao.p, recRefracao.normal, posicaoObservador, luz, luzes, material, plano, esferas, profundidade - 1);
            corRefracao = produtoVetorial(corRefracao, recRefracao.cor);
        }
        corRefracao = multiplicacaoPorEscalar(corRefracao, material.kt);
    }

   

    I = I + corReflexao + corRefracao;


    // Clamping final
    I.x = fmax(0.0, fmin(1.0, I.x));
    I.y = fmax(0.0, fmin(1.0, I.y));
    I.z = fmax(0.0, fmin(1.0, I.z));

    return multiplicacaoPorEscalar(produtoVetorial(I, corDoPlano), 0.8);
    

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







