// definindo pontos e operações sobre os pontos
#include <iostream>
#include <Eigen/Core>
using namespace std;

// struct Ponto {
//     double x, y, z;
//     Ponto(double x, double y, double z) : x(x), y(y), z(z) {} // construtor -> inicializa os valores de x, y e z
// };



// int main() {
//     Ponto point = Ponto(1, 2, 3); // ponto (1, 2, 3)
//     cout << point.x << " " << point.y << " " << point.z << endl; // imprime 1 2 3
//     return 0;
// }

struct Ponto {
    Eigen::Vector3d vetor;
    Ponto(double x, double y, double z) : vetor(x, y, z) {} // construtor -> inicializa os valores de x, y e z
};

int main() {
    Ponto point = Ponto(1, 2, 3); // ponto (1, 2, 3)
    cout << point.vetor << endl; // imprime 1 2 3
    return 0;
}