#include <iostream>
#include "cmath"

using namespace std;

// Criando estrutura vetor
template<typename T> //carregar mais de um tipo de dados
struct vetor {
    T x, y, z;

    // Construtor do vetor
    vetor(T xRecebido, T yRecebido, T zRecebido) : x(xRecebido), y(yRecebido), z(zRecebido) {}

    // Construtor padrão (inicializa com zero)
    vetor() : x(0), y(0), z(0) {}


};

// Função soma
template<typename T>
vetor<T> soma(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
};

// Função subtração
template<typename T>
vetor<T> subtracao(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
};

// Função norma
template<typename T>
T norma(const vetor<T>& vector) {
    return sqrt(pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2));
};

// Função do Produto Escalar
template<typename T>
T produtoEscalar(const vetor<T>& v1, const vetor<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
};

// Função Vetor unitário
template<typename T>
vetor<T> vetorUni(const vetor<T>& v) {
    T norm = norma(v);
    

    return  vetor<T> (v.x / norm, v.y / norm, v.z / norm);;
};



// Função Produto Vetorial
template<typename T>
vetor<T> produtoVetorial(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

// Função multiplicação por escalar
template<typename T>
vetor<T> multiplicacaoPorEscalar(const vetor<T>& v, T escalar) {
    return vetor<T>(v.x * escalar, v.y * escalar, v.z * escalar);
}

template<typename T>
struct raio {
    vetor<T> origem;
    vetor<T> direcao;

    raio(vetor<T> comeco, vetor<T> direc) {
        this->origem = comeco;
        this->direcao = vetorUni(direc);
    }
};

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
    // Testando operações com vetores
    vetor<double> v1{1.0, 0.0, 0.0};
    vetor<double> v2{0.0, 1.0, 0.0};

    double normV1 = norma(v1);
    double normV2 = norma(v2);

    double dotProduct = produtoEscalar(v1, v2);

    vetor<double> unitV1 = vetorUni(v1);


    std::cout << "Norma de v1: " << normV1 << std::endl;
    std::cout << "Norma de v2: " << normV2 << std::endl;
    std::cout << "Produto escalar de v1 e v2: " << dotProduct << std::endl;
    std::cout << "Vetor unitário de v1: (" << unitV1.x << ", " << unitV1.y << ", " << unitV1.z << ")" << std::endl;

    // Criando uma câmera
    camera<double> minhaCamera;

    // Obtendo um raio da câmera com parâmetros u = 0.5 e v = 0.5
    raio<double> meuRaio = minhaCamera.getRaio(0.5, 0.5);

    // Exibindo as informações do raio
    std::cout  << "Origem do raio: (" << meuRaio.origem.x << ", " << meuRaio.origem.y << ", " << meuRaio.origem.z << ")" <<  std::endl;
    std::cout  << "Direção do raio: (" << meuRaio.direcao.x << ", " << meuRaio.direcao.y << ", " << meuRaio.direcao.z << ")" << std:: endl;


    return 0;
}
