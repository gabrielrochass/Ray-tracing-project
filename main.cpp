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
template<typename T>
struct vetor {
    T x, y, z;

    void criar(T xRecebido, T yRecebido, T zRecebido) {
        x = xRecebido;
        y = yRecebido;
        z = zRecebido;
    }
};

// Função soma
template<typename T>
Vec3<T> soma(const Vec3<T>& v1, const Vec3<T>& v2) {
    return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Função subtração
template<typename T>
Vec3<T> subtracao(const Vec3<T>& v1, const Vec3<T>& v2) {
    return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Função norma
template<typename T>
T norma(const vetor<T>& vector) {
    return sqrt(pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2));
}

// Função do Produto Escalar
template<typename T>
T produtoEscalar(const vetor<T>& v1, const vetor<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Função Vetor unitário 
template<typename T>
vetor<T> vetorUni(const vetor<T>& v) {
     
    return vetor<T>(v.x / norma(v), v.y / norma(v), v.z / norma(v));
    
}

int main() {
    Ponto p1 = Ponto(1, 2, 3); // Ponto (1, 2, 3)
    Ponto p2 = Ponto(1, 1, 1);
    cout << "Coordenadas de p1: " << p1.x << " " << p1.y << " " << p1.z << endl; // Imprime 1 2 3
    cout << "Coordenadas de p2: " << p2.x << " " << p2.y << " " << p2.z << endl;

    // Testando operações com pontos
    cout << "Soma: " << soma(p1, p2).x << " " << soma(p1, p2).y << " " << soma(p1, p2).z << endl;
    cout << "Subtracao: " << subtracao(p1, p2).x << " " << subtracao(p1, p2).y << " " << subtracao(p1, p2).z << endl;
    cout << "Multiplicacao por Escalar: " << multiplicacaoPorEscalar(p2, 2).x << " " << multiplicacaoPorEscalar(p2, 2).y << " " << multiplicacaoPorEscalar(p2, 2).z << endl;
    cout << "Produto Escalar: " << produtoEscalar(p1, p2) << endl;
    cout << "Produto Vetorial: " << produtoVetorial(p1, p2).x << " " << produtoVetorial(p1, p2).y << " " << produtoVetorial(p1, p2).z << endl;

    // Testando operações com vetores
    vetor<double> v1(3.0, 4.0, 0.0);
    vetor<double> v2(0.6, 0.8, 0.0);
    
    double normV1 = norma(v1);
    double normV2 = norma(v2);
   
    double dotProduct = produtoEscalar(v1, v2);
    
    vetor<double> unitV1 = vetorUni(v1);

    
    std::cout << "Norma de v1: " << normV1 << std::endl;
    std::cout << "Norma de v2: " << normV2 << std::endl;
    std::cout << "Produto escalar de v1 e v2: " << dotProduct << std::endl;
    std::cout << "Vetor unitário de v1: (" << unitV1.x << ", " << unitV1.y << ", " << unitV1.z << ")" << std::endl;

    return 0;
}
