#include <iostream>
#include <cmath>
using namespace std;

// Define a estrutura Ponto
struct Ponto {
    double x, y, z;
    Ponto(double x, double y, double z) : x(x), y(y), z(z) {} // Construtor -> inicializa os valores de x, y e z
};

// Define operações com pontos
Ponto soma(const Ponto& p1, const Ponto& p2) {
    return Ponto(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Ponto subtracao(const Ponto& p1, const Ponto& p2) {
    return Ponto(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Ponto multiplicacaoPorEscalar(const Ponto& p, double escalar) {
    return Ponto(p.x * escalar, p.y * escalar, p.z * escalar);
}

double produtoEscalar(const Ponto& p1, const Ponto& p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Ponto produtoVetorial(const Ponto& p1, const Ponto& p2) {
    return Ponto(p1.y * p2.z - p1.z * p2.y,
                 p1.z * p2.x - p1.x * p2.z,
                 p1.x * p2.y - p1.y * p2.x);
}

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
        w = vetorUni(subtracao(origem, horizontal));
    }

    // Construtor personalizado
    camera(const vetor<T>& origin, const vetor<T>& lookAt,
           const vetor<T>& up, T aperture = 0.0, T focusDist = 1.0)
        : origem(origin),
          apontarPara(lookAt),
          distanciaFocal(focusDist) {
        w = (origem - apontarPara).vetorUni();
        u = up.produtoVetorial(w).vetorUni();
        v = w.produtoVetorial(u);
        horizontal = u.multiplicacaoPorEscalar(4.0 * focusDist);
        vertical = v.multiplicacaoPorEscalar(2.0 * focusDist);
        extremidadeInferiorEsquerda = origem
                                      .subtracao(horizontal.multiplicacaoPorEscalar(0.5))
                                      .subtracao(vertical.multiplicacaoPorEscalar(0.5))
                                      .subtracao(w.multiplicacaoPorEscalar(focusDist));
    }
    
    // Método para obter um raio da câmera
    raio<double> getRaio(double u, double v) {
        // Calcula a direção do raio usando os parâmetros u e v
        vetor<T> direcao = soma(soma(extremidadeInferiorEsquerda, vetor<T>(u, v, 0.0)), origem);
        return raio<T>(origem, vetorUni(direcao));
    }
};

int main() {
    Ponto p1 = Ponto(1, 2, 3); 
    Ponto p2 = Ponto(1, 1, 1);
    cout << "Coordenadas de p1: " << p1.x << " " << p1.y << " " << p1.z << endl; 
    cout << "Coordenadas de p2: " << p2.x << " " << p2.y << " " << p2.z << endl;

    // Testando operações com pontos
    cout << "Soma: " << soma(p1, p2).x << " " << soma(p1, p2).y << " " << soma(p1, p2).z << endl;
    cout << "Subtracao: " << subtracao(p1, p2).x << " " << subtracao(p1, p2).y << " " << subtracao(p1, p2).z << endl;
    cout << "Multiplicacao por Escalar: " << multiplicacaoPorEscalar(p2, 2).x << " " << multiplicacaoPorEscalar(p2, 2).y << " " << multiplicacaoPorEscalar(p2, 2).z << endl;
    cout << "Produto Escalar: " << produtoEscalar(p1, p2) << endl;
    cout << "Produto Vetorial: " << produtoVetorial(p1, p2).x << " " << produtoVetorial(p1, p2).y << " " << produtoVetorial(p1, p2).z << endl;

    // Testando operações com vetores
    vetor<double> v1(1.0, 0.0, 0.0);
    vetor<double> v2(0.0, 1.0, 0.0);
    
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
    std::cout  << "Direção do raio: (" << meuRaio.direcao.x << ", " << meuRaio.direcao.y << ", " << meuRaio.direcao.z << ")" <<  std::endl;

   
    return 0;
}
