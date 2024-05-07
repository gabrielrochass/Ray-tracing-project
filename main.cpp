#include <iostream>
#include <C:\Users\gabri\Downloads\eigen-3.4.0\eigen-3.4.0\Eigen\Core>
using namespace std;
using namespace Eigen;

// Define a estrutura Ponto
struct Ponto {
    Vector3d coordenadas;
    Ponto(double x, double y, double z) : coordenadas(x, y, z) {} // Construtor -> inicializa os valores de x, y e z
};

// Define operações com pontos
Ponto soma(const Ponto& p1, const Ponto& p2) {
    Vector3d somatorio = p1.coordenadas + p2.coordenadas;
    return Ponto(somatorio(0), somatorio(1), somatorio(2));
}

Ponto subtracao(const Ponto& p1, const Ponto& p2) {
    Vector3d subtraido = p1.coordenadas - p2.coordenadas;
    return Ponto(subtraido(0), subtraido(1), subtraido(2));
}

Ponto multiplicacaoPorEscalar(const Ponto& p, double escalar) {
    Vector3d multiplicacao_vetor = p.coordenadas * escalar;
    return Ponto(multiplicacao_vetor(0), multiplicacao_vetor(1), multiplicacao_vetor(2));
}

double produtoEscalar(const Ponto& p1, const Ponto& p2) {
    return p1.coordenadas.dot(p2.coordenadas);
}

Ponto produtoVetorial(const Ponto& p1, const Ponto& p2) {
    Vector3d produto_vetorial = p1.coordenadas.cross(p2.coordenadas);
    return Ponto(produto_vetorial(0), produto_vetorial(1), produto_vetorial(2));
}

int main() {
    Ponto p1 = Ponto(1, 2, 3); // Ponto (1, 2, 3)
    Ponto p2 = Ponto(1, 1, 1);
    cout << p1.coordenadas << endl; // Imprime 1 2 3

//    testando operações com pontos
    cout << soma(p1, p2).coordenadas << endl;
    cout << subtracao(p1, p2).coordenadas << endl;
    cout << multiplicacaoPorEscalar(p2, 2).coordenadas << endl;
    cout << produtoEscalar(p1,p2) << endl;
    cout << produtoVetorial(p1,p2).coordenadas << endl;

    return 0;
}
