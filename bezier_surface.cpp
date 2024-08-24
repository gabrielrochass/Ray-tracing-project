#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "ponto.h"
#include "vector.h"
#include "camera.h"
#include "sphere_list.h"
#include "sphere.h"
#include "plano.h" 
#include "malha_triangulos.h"
#include "triangulo.h"
#include "raio.h"
#include "matriz4x4.h"
#include "phongComponentes.h"

using namespace std;

// Função para calcular o coeficiente binomial
int binomialCoeff(int n, int k) {
    int res = 1;
    if (k > n - k) k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

// Função para calcular um ponto na superfície de Bézier
template<typename T>
vetor<T> pontoBezier(double u, double v, const vector<vector<vetor<T>>>& pontosDeControle) {
    int n = pontosDeControle.size() - 1;
    int m = pontosDeControle[0].size() - 1;
    vetor<T> ponto(0.0, 0.0, 0.0);

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            double bernsteinU = binomialCoeff(n, i) * pow(u, i) * pow(1 - u, n - i);
            double bernsteinV = binomialCoeff(m, j) * pow(v, j) * pow(1 - v, m - j);
            ponto += bernsteinU * bernsteinV * pontosDeControle[i][j];
        }
    }

    return ponto;
}

// Função para gerar uma malha de triângulos a partir de uma superfície de Bézier
template<typename T>
malha gerarMalhaBezier(const vector<vector<vetor<T>>>& pontosDeControle, int u_cells, int w_cells) {
    malha malha;
    vector<vector<vetor<T>>> pontosSuperficie(u_cells, vector<vetor<T>>(w_cells));

    // Geração dos pontos na superfície de Bézier
    for (int i = 0; i < u_cells; ++i) {
        for (int j = 0; j < w_cells; ++j) {
            double u = static_cast<double>(i) / (u_cells - 1);
            double v = static_cast<double>(j) / (w_cells - 1);
            pontosSuperficie[i][j] = pontoBezier(u, v, pontosDeControle);
        }
    }

    // Triangulação dos pontos gerados
    for (int i = 0; i < u_cells - 1; ++i) {
        for (int j = 0; j < w_cells - 1; ++j) {
            vetor<T> v1 = pontosSuperficie[i][j];
            vetor<T> v2 = pontosSuperficie[i + 1][j];
            vetor<T> v3 = pontosSuperficie[i][j + 1];
            vetor<T> v4 = pontosSuperficie[i + 1][j + 1];

            // Cria dois triângulos para cada quadrado
            malha.add(triangulo(v1, v2, v3));
            malha.add(triangulo(v2, v4, v3));
        }
    }

    return malha;
}

const double infinity = numeric_limits<double>::infinity();

// Define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

vetor<double> raioColor(const raio<double>& raio, const malha& mundo,  const vetor<double>& posicaoObservador, listaLuzes luzes, const phongComponentes& material) {
    hit_record rec;

    plano plano1(vetor<double>{0.0, 0.0, -1.0}, vetor<double>{0.0, 0.0, 1.0});

    // Variável para armazenar a cor final do pixel
    vetor<double> corFinal = {0.0, 0.0, 0.0};

    if (mundo.hit(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        for (int i = 0; i < luzes.luzes.size(); i++) {
            corFinal = corFinal + calcularIluminacaoPhong(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, material, mundo);
        }
        return corFinal;
    } 
    vetor<double> direcao_uni = vetorUni(raio.direcao);
    return backgroundColor(direcao_uni);
}

int main() {
    // Define a imagem
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    vector<vector<vetor<double>>> image(imHeight, vector<vetor<double>>(imWidth));

    // Define a câmera
    vetor<double> posicaoDaCamera(0, 0, 1); // Mover para z=5 para ter uma visão mais clara
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);

    // Definindo os pontos de controle da superfície de Bézier
    vector<vector<vetor<double>>> pontosDeControle = {
        {vetor<double>(-1.0, 0.0, 0.0), vetor<double>(-0.5, 0.5, 0.0), vetor<double>(0.0, 0.0, 0.0)},
        {vetor<double>(0.5, -0.5, 0.0), vetor<double>(1.0, 0.0, 0.0), vetor<double>(1.5, 0.5, 0.0)},
    };

    // Parâmetros para a malha
    int u_cells = 50; // Aumente para uma resolução maior
    int w_cells = 50;

    // Gerando a malha da superfície de Bézier
    malha malha = gerarMalhaBezier(pontosDeControle, u_cells, w_cells);

    // Define a iluminação e o material
    iluminacao luz{
        vetor<double>(0.5, -0.5, -0.1), // posição da luz -> diagonal direita superior
        vetor<double>(0.1, 0.1, 0.1), // intensidade ambiente
        vetor<double>(0.7, 0.7, 0.7), // intensidade difusa
        vetor<double>(0.5, 0.5, 0.5)  // intensidade especular
    };

    iluminacao luz2{
        vetor<double>(-0.5, -0.5, -0.1), // posição da luz -> diagonal esquerda superior
        vetor<double>(0.1, 0.1, 0.1), // intensidade ambiente
        vetor<double>(0.7, 0.7, 0.7), // intensidade difusa
        vetor<double>(0.5, 0.5, 0.5)  // intensidade especular
    };

    listaLuzes luzes;
    luzes.addLuz(luz);
    luzes.addLuz(luz2);

    phongComponentes material(0.1, 0.4, 0.9, 10.0, 0.5, 0.5, 1.0, 1.0);
    phongComponentes materialDifusa(0.1, 0.5, 0.5, 10, 0.5, 0.5, 1.0, 1.5);

    // Define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, larguraDaViewport)), mult(0.5, alturaDaViewport)), mira);
    
    // Renderização da imagem
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = subtracao(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, larguraDaViewport), mult(v, alturaDaViewport))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            vetor<double> corPixel = raioColor(r, malha, camera.posicaoDaCamera, luzes, materialDifusa);

            image[j][i] = corPixel;
        }
    }

    // Salvar a imagem como um PPM
    cout << "P3\n" << imWidth << " " << imHeight << "\n255\n";
    for (int j = imHeight - 1; j >= 0; --j) {
        for (int i = 0; i < imWidth; ++i) {
            vetor<double> cor = image[j][i];
            cout << static_cast<int>(255.999 * cor.x) << ' '
                 << static_cast<int>(255.999 * cor.y) << ' '
                 << static_cast<int>(255.999 * cor.z) << '\n';
        }
    }

    return 0;
}
