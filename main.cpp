#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.cpp"

using namespace std;

// Define a atmosfera, ou seja, a cor de fundo, com base na direção vertical do raio
template<typename T>
vetor<T> backgroundColor(const vetor<T>& dir) {
    T t = 0.5 * (dir.y + 1.0);
    return vetor<T>((1 - t) * 1.0 + t * 0.5,
                    (1 - t) * 1.0 + t * 0.7,
                    (1 - t) * 1.0 + t * 1.0);
}

// Determina de um raio intersecta ou não uma esfera
bool hitEsfera(const vetor<double>& centroDaEsfera, double raioDaEsfera, const raio<double>& raioDeInterseccao) {
    // calcula os coeficientes da equação quadrática -> a*x^2 + b*x + c = 0 -> usada para determinar se e onde o raio intersecta a esfera
    vetor<double> origemMenosCentro = subtracao(raioDeInterseccao.origem, centroDaEsfera); // vetor da origem do raio até o centro da esfera
    double a = produtoEscalar(raioDeInterseccao.direcao, raioDeInterseccao.direcao); // magnitude do vetor direção do raio ao quadrado
    double b = 2.0 * produtoEscalar(origemMenosCentro, raioDeInterseccao.direcao); // 2 * produto escalar do vetor da origem do raio até o centro da esfera com o vetor direção do raio
    double c = produtoEscalar(origemMenosCentro, origemMenosCentro) - raioDaEsfera * raioDaEsfera; // magnitude do vetor da origem do raio até o centro da esfera ao quadrado - raio da esfera ao quadrado

    // calcula o discriminante da equação quadrática
    double discriminante = b * b - 4 * a * c;
    return discriminante > 0; // se o discriminante for maior que 0, o raio intersecta a esfera
}

// define se um raio intersecta ou não um plano -> retorna true se intersecta, false caso contrário
bool hitPlano(const vetor<double>& pontoNoPlano, const vetor<double>& normal, const raio<double>& raioDeInterseccao) {
    double denominador = produtoEscalar(normal, raioDeInterseccao.direcao);

    // Verifica se o denominador é zero para evitar divisão por zero
    if (fabs(denominador) < 1e-6) {
        return false; // Raio é paralelo ao plano
    }

    // Calcula o ponto de interseção
    double t = -produtoEscalar(subtracao(raioDeInterseccao.origem, pontoNoPlano), normal) / denominador;

    // Verifica se o ponto de interseção está na direção positiva do raio
    return t >= 0;
}



// Retorna a cor de um raio que intersecta uma esfera -> se não intersecta, retorna a cor de fundo

template<typename T>
// Function to compute color of the raio
vetor<T> raioColor(const raio<T>& raio, const vetor<T>& sphereCenter, T sphereRadius) {
    double t = hitEsfera(sphereCenter, sphereRadius, raio);
    // se o raio intersecta o plano, retorna a cor verde
    vetor<T> pontoNoPlano{0.0, -1.25, -1.0};
    vetor<T> normal{0.0, 0.0, 0.0};
    if (t > 0.0) {
        vetor<T> p = raioAt(raio, t);
        vetor<T> N = vetorUni(subtracao(p, sphereCenter));
        vetor<T> color = mult(0.5, soma(vetor<T>(1.0, 1.0, 1.0), N));
        return color; 
    }
    else if(hitEsfera({0.0, 2.0, -1.0}, 2.0, raio)) { 
        return vetor<T>(0.0, 1.0, 0.0);
    }
    /*else if(hitPlano(pontoNoPlano, normal, raio)) { 
        return vetor<T>(0.0, 1.0, 0.0);
    }*/

    return backgroundColor(raio.direcao);
}



int main() {
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));

    vector<vector<vetor<double>>> image(imHeight, vector<vetor<double>>(imWidth));

    vetor origem{0.0, 0.0, 0.0};
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * 16.0 / 9.0;
    vetor horizontal{viewportWidth, 0.0, 0.0};
    vetor vertical{0.0, viewportHeight, 0.0};
    vetor h2 = mult(0.5, horizontal);
    vetor h2o = subtracao(origem, h2);
    vetor v2 = mult(0.5, vertical);
    vetor lente{0.0, 0.0, 1.0};
    vetor v2f = subtracao(v2, lente);
    vetor lowerLeftCorner = subtracao(h2o, v2f);

    // define esfera
    vetor sphereCenter{0.0, 0.0, -1.0};
    double sphereRadius = 0.5;


    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            vetor vv = mult(v, vertical);


            vetor vvo = subtracao(vv, origem);
            vetor uh = mult(u, horizontal);
            vetor loweruh = soma(uh, lowerLeftCorner);
            vetor dir = soma(loweruh, vvo);
            // dir = lowerleftcorner + u*horizontal + v*vertical - origin
            raio raio(origem, dir);
            image[j][i] = raioColor(raio, sphereCenter , sphereRadius);
        }
    }


    cout << "P3\n" << imWidth << " " << imHeight << "\n255\n";
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            int ir = int(255.99 * image[j][i].x);
            int ig = int(255.99 * image[j][i].y);
            int ib = int(255.99 * image[j][i].z);
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }



    clog << "\rDone.                 \n";

    return 0;
}
