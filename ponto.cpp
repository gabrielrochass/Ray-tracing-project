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

    return 0;
}
