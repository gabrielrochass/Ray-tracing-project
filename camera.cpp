#include <iostream>
#include <cmath>
#include "Ponto.h"
#include "vector.h"

using namespace std;

// Estrutura Camera
// câmera móvel -> pode ser posicionada em qualquer lugar do mundo e apontada para diferentes localizações
// atributos: posicaoDaCamera, mira, vUp, vOrtonomais (u, v, w), distanciaParaTela, larguraDaTela, alturadaTela
// mira -> Ponto para onde a câmera está apontada
// vUp -> vetor que indica a direção para cima da câmera 
// vOrtonomais -> 3 vetores ortogonais entre si que definem a base da câmera
// w -> direção oposta ao vetor que vai de posicao para mira
// u -> vetor perpendicular a w e vUp
// v -> vetor perpendicular a u e w -> define o vetor para cima da câmera


struct Camera {
    // define atributos
    vetor<double> posicaoDaCamera;
    vetor<double> mira;
    vetor<double> vUp;
    vetor<double> w, u, v;
    
    // construtor
    Camera(const vetor<double>& posicaoDaCamera_, const vetor<double>& mira_, const vetor<double>& vUp_) : posicaoDaCamera(posicaoDaCamera_), mira(mira_), vUp(vUp_) {
        w = vetorUni(subtracao(mira, posicaoDaCamera));
        u = vetorUni(produtoVetorial(vUp, w));
        v = produtoVetorial(w, u);
    }

};
