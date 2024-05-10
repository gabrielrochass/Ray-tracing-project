#include <iostream>
#include <cmath>
#include "ponto.cpp"
#include "vector.cpp"

using namespace std;

template<typename T>
class camera {
public:
    vetor<T> origem;
    vetor<T> apontarPara;
    vetor<T> horizontal;
    vetor<T> vertical;
    vetor<T> extremidadeInferiorEsquerda;
    vetor<T> u, v, w;
    T lenteR;
    T distanciaFocal;

    // Construtor padrão
    camera() : origem(0.0, 0.0, 0.0),
               apontarPara(0.0, 0.0, -1.0),
               distanciaFocal(1.0),
               lenteR(0.0) {
        // Calcula o vetor de direção w (apontando de origem para apontarPara)
        
        w = vetorUni(subtracao(origem, apontarPara));
        /*w (Direção de Visão): Define para onde a câmera está apontando.
        u (Orientação Horizontal): Define a direção horizontal da câmera em relação à sua direção de visão.
        v (Orientação Vertical): Define a direção vertical da câmera em relação à sua direção de visão.*/
        
        // Calcula os vetores u e v usando up e w
        vetor<T> up(0.0, 1.0, 0.0); // Vetor up padrão
        u = vetorUni(produtoVetorial(up, w));
        v = produtoVetorial(w, u);

        // Calcula os vetores horizontal e vertical com base na distância focal
        horizontal = multiplicacaoPorEscalar(u, 4.0 * distanciaFocal);
        vertical = multiplicacaoPorEscalar(v, 2.0 * distanciaFocal);

        // Calcula a extremidade inferior esquerda da tela
        vetor<T> half_horizontal = multiplicacaoPorEscalar(horizontal, 0.5);
        vetor<T> half_vertical = multiplicacaoPorEscalar(vertical, 0.5);
        
        extremidadeInferiorEsquerda = subtracao(subtracao(subtracao(origem, half_horizontal), half_vertical), w);
    }

    // Método para obter um raio da câmera
    raio<T> getRaio(double uVal, double vVal) const {
        // Calcula a direção do raio usando os parâmetros uVal e vVal
        vetor<T> direction = soma(soma(extremidadeInferiorEsquerda, multiplicacaoPorEscalar(horizontal, uVal)),
                                   soma(multiplicacaoPorEscalar(vertical, vVal), origem));
        return raio<T>(origem, vetorUni(direction));
    }
};

int main() {
    // Cria uma câmera
    camera<double> cam;

    // Obtem um raio da câmera
    raio<double> r = cam.getRaio(0.5, 0.5);

    // Imprime a origem e a direção do raio
    cout << "Origem do raio: (" << r.origem.x << ", " << r.origem.y << ", " << r.origem.z << ")" << std::endl;
    cout << "Direção do raio: (" << r.direcao.x << ", " << r.direcao.y << ", " << r.direcao.z << ")" << std::endl;

    return 0;
}

/*// Criando uma câmera
    camera<double> minhaCamera;

    // Obtendo um raio da câmera com parâmetros u = 0.5 e v = 0.5
    raio<double> meuRaio = minhaCamera.getRaio(0.5, 0.5);

    // Exibindo as informações do raio
   cout  << "Origem do raio: (" << meuRaio.origem.x << ", " << meuRaio.origem.y << ", " << meuRaio.origem.z << ")" << endl;
   cout  << "Direção do raio: (" << meuRaio.direcao.x << ", " << meuRaio.direcao.y << ", " << meuRaio.direcao.z << ")" << endl;*/