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

/*vetor<double> calcularIluminacaoPhong(
     vetor<double> pontoIntersecao, 
     vetor<double> Normal, 
     vetor<double> posicaoObservador,
     iluminacao luz, 
     listaLuzes luzes,
     phongComponentes material,
     sphere_list esferas) 
{
    // Vetores de direção
    vetor<double> L = normal(subtracao(luz.posicao, pontoIntersecao)); // Direção da luz
    // Verificar se o ponto está na sombra
    bool emSombra = false;
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> L = normal(luzes.luzes[i].posicao - pontoIntersecao); // Direção da luz
        //vetor<double> L = normal(subtracao(luzes.luzes[i].posicao, pontoIntersecao)); // Direção da luz
        // L = Normal de L', L' = posicao - pontoIntersecao
        vetor<double> V = normal(posicaoObservador - pontoIntersecao); // Direção para o observador
        //vetor<double> V = normal(subtracao(posicaoObservador, pontoIntersecao)); // Direção para o observador
        // V = Normal de V', V' = posicaoObservador - pontoIntersecao
        vetor<double> R = normal(2 * produtoEscalar(Normal, L) * Normal - L); // Reflexão da luz
        //vetor<double> R = normal(subtracao(produtoVetorial(multiplicacaoPorEscalar(produtoVetorial(Normal, L), 2.0),Normal), L)); // Reflexão da luz
        // R = Normal de R', R' = 2 * (Normal * L) * Normal - L
        raio<double> raioSombra(pontoIntersecao, L);
        emSombra = false;
        hit_record temp_rec;
        if (esferas.hit(raioSombra, 0.001, infinito, temp_rec)) {
            emSombra = true;
        }
        // Componentes de iluminação
        vetor<double> ambiente = multiplicacaoPorEscalar(luzes.luzes[i].Ia, material.ka);
        vetor<double> difusa = {0, 0, 0};
        vetor<double> especular = {0, 0, 0};
        
        if (!emSombra) {
            difusa = multiplicacaoPorEscalar(luzes.luzes[i].Id, (produtoEscalar(Normal, L) * material.kd)) + difusa;
            //especular = multiplicacaoPorEscalar(luz.Is, material.ks * pow(produtoEscalar(R, V), material.n));
            
            especular = multiplicacaoPorEscalar(multiplicacaoPorEscalar(luzes.luzes[i].Is, material.ks), pow(produtoEscalar(R, V), material.n)) + especular;
    
            
        }
        // Iluminação resultante
        vetor<double> I = ambiente + difusa + especular;
        return I;
       
    }
}
     
*/
vetor<double> calcularIluminacaoPhong(
    vetor<double> pontoIntersecao, 
    vetor<double> Normal, 
    vetor<double> posicaoObservador,
    iluminacao luz, 
    listaLuzes luzes,
    phongComponentes material,
    sphere_list esferas) 
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
    }
    
    return I;
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
    
    


    



#endif



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







