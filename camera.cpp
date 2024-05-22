#include <iostream>
#include <cmath>
#include "Ponto.h"
#include "vector.h"

using namespace std;

// Classe Camera
// câmera móvel -> pode ser posicionada em qualquer lugar do mundo e apontada para diferentes localizações
// atributos: posicaoDaCamera, mira, vUp, vOrtonomais (u, v, w), distanciaParaTela, larguraDaTela, alturadaTela
// posicao -> Ponto onde a câmera está
// mira -> Ponto para onde a câmera está apontada
// vUp -> vetor que indica a direção para cima da câmera 
// vOrtonomais -> 3 vetores ortogonais entre si que definem a base da câmera
// w -> direção oposta ao veor que vai de posicao para mira
// u -> vetor perpendicular a w e vUp
// v -> vetor perpendicular a u e w -> define o vetor para cima da câmera
// distanciaParaTela -> distância da câmera para a tela
// largura -> largura da tela
// altura -> altura da tela


// etapas:
// 1. inicializar os parâmetros da câmera
// 2. calcular os vetores u, v, w
// 3. calcular a base da câmera
// 4. gerar os raios primários


struct Camera {
    // define atributos
    vetor<double> posicaoDaCamera;
    vetor<double> mira;
    vetor<double> vUp;
    vetor<double> w, u, v;
    
    // construtor
    Camera(const vetor<double>& posicaoDaCamera_, const vetor<double>& mira_, const vetor<double>& vUp_, double distanciaParaTela_, double larguraDaTela_, double alturadaTela_) : 
    posicaoDaCamera(posicaoDaCamera_), mira(mira_), vUp(vUp_) {
        w = vetorUni(subtracao(mira, posicaoDaCamera));
        u = vetorUni(produtoVetorial(vUp, w));
        v = produtoVetorial(w, u);
    }

};

// testando a classe Camera
int main() {
    Camera cam(vetor<double>(0, 0, 0), vetor<double>(0, 0, -1), vetor<double>(0, 1, 0), 1, 2, 2);
    cout << "u: " << cam.u << endl;
    cout << "v: " << cam.v << endl;
    cout << "w: " << cam.w << endl;
    return 0;
}