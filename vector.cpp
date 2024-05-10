#include <iostream>
#include "cmath"
using namespace std;

// Criando estrutura vetor
template<typename T> //carregar mais de um tipo de dados
struct vetor {
    T x, y, z;

    void criar(T xRecebido, T yRecebido, T zRecebido) { // Construtor do vetor
        x = xRecebido;
        y = yRecebido;
        z = zRecebido;
    }


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

    return vetor<T>(v.x / norma(v), v.y / norma(v), v.z / norma(v));

};

// Função Produto Vetorial
template<typename T>
vetor<T> produtoVetorial(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
};



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
               horizontal(4.0, 0.0, 0.0),
               vertical(0.0, 2.0, 0.0),
               extremidadeInferiorEsquerda(-2.0, -1.0, -1.0),
               u(produtoVetorial(horizontal, w)),
               v(produtoVetorial(w,u)),
               distanciaFocal(1.0) {
        w = vetorUni(subtracao(origem, apontarPara));
    }

    // Construtor personalizado
    camera(const vetor<T>& origin, const vetor<T>& lookAt,
           const vetor<T>& up, T aperture = 0.0, T focusDist = 1.0)
        : origem(origin),
          apontarPara(lookAt),
          distanciaFocal(focusDist) {
        w = vetorUni(subtracao(origem, apontarPara));
        u = vetorUni(produtoVetorial(up, w));
        v = produtoVetorial(w, u);
        horizontal = multiplicacaoPorEscalar(u , 4.0 * focusDist);
        vertical = multiplicacaoPorEscalar(v, 2.0 * focusDist);
        extremidadeInferiorEsquerda = (
                                      subtracao(origem, multiplicacaoPorEscalar(0.5, horizontal)),
                                      subtracao(origem, multiplicacaoPorEscalar(0.5, vertical)),
                                      subtracao(origem, multiplicacaoPorEscalar(focusDist, w))
                                      );
    }
    
    // Método para obter um raio da câmera
    raio<double> getRaio(double u, double v) {
        // Calcula a direção do raio usando os parâmetros u e v
        vetor<T> direcao = soma(soma(extremidadeInferiorEsquerda, vetor<T>(u, v, 0.0)), origem);
        return raio<T>(origem, vetorUni(direcao));
    }
};

int main() {
    // Testando operações com vetores
    vetor<double> v1(1.0, 0.0, 0.0);
    vetor<double> v2(0.0, 1.0, 0.0);

    double normV1 = norma(v1);
    double normV2 = norma(v2);

    double dotProduct = produtoEscalar(v1, v2);

    vetor<double> unitV1 = vetorUni(v1);


    cout << "Norma de v1: " << normV1 << endl;
    cout << "Norma de v2: " << normV2 << endl;
    cout << "Produto escalar de v1 e v2: " << dotProduct << endl;
    cout << "Vetor unitário de v1: (" << unitV1.x << ", " << unitV1.y << ", " << unitV1.z << ")" << endl;

    // Criando uma câmera
    camera<double> minhaCamera;

    // Obtendo um raio da câmera com parâmetros u = 0.5 e v = 0.5
    raio<double> meuRaio = minhaCamera.getRaio(0.5, 0.5);

    // Exibindo as informações do raio
    cout  << "Origem do raio: (" << meuRaio.origem.x << ", " << meuRaio.origem.y << ", " << meuRaio.origem.z << ")" <<  endl;
    cout  << "Direção do raio: (" << meuRaio.direcao.x << ", " << meuRaio.direcao.y << ", " << meuRaio.direcao.z << ")" <<  endl;


    return 0;
}
