#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <memory>
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
#include "textura.cpp"

using namespace std;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

// mapeia coordenadas esféricas para coordenadas uv
vetor<double> calularCoordenadasUV(const vetor<double>& pontoDeIntersecao) {
    double phi = atan2(pontoDeIntersecao.z, pontoDeIntersecao.x);
    double theta = asin(pontoDeIntersecao.y);

    double u = 1 - (phi + pi) / (2 * pi);
    double v = (theta + pi / 2) / pi;

    return vetor<double>(u, v, 0);
} 

// calcula a cor de um pixel
vetor<double> raioColor(const raio<double>& raio, const sphere_list& esferas, const vetor<double>& posicaoObservador, listaLuzes luzes, const phongComponentes& material, const phongComponentes& materialEsf, const Textura& textura) {
    hit_record rec;
    vetor<double> corFinal = {0.0, 0.0, 0.0};

    plano plano1(vetor<double>{0.0, 0.0, -1.0}, vetor<double>{0.0, 1.0, 0.0});

    if (esferas.hit(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        // uv mapping
        vetor<double> uv = calularCoordenadasUV(N);
        vetor<double> corTextura = textura.corTextura(uv.x, uv.y);
        corFinal = corTextura;

        for (int i = 0; i < luzes.luzes.size(); i++) {
            // corFinal = corFinal + calcularIluminacaoPhong(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, materialEsf, esferas, plano1, 1);
        }
        return corFinal;

    } else if (plano1.hitPlano(raio, 0.001, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        for (int i = 0; i < luzes.luzes.size(); i++) {
            corFinal = corFinal + calcularIluminacaoPhongPlano(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, material, plano1, esferas, 2);
        }
        return corFinal;
    }

    vetor<double> direcao_uni = vetorUni(raio.direcao);
    return backgroundColor(direcao_uni);
}


int main() {
    // define a imagem
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    vector<vector<vetor<double>>> image(imHeight, vector<vetor<double>>(imWidth));

    // carregar textura
    // Textura textura1("texturas/deserto.bmp");
    // Textura textura1("texturas/moana.bmp");
    // Textura textura1("texturas/parede-verde.bmp");
    Textura textura1("texturas/tapete.bmp");
  
    // define a câmera
    vetor<double> posicaoDaCamera(0, 0, 1);
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);

    sphere_list esferas;
    esferas.add(sphere(vetor<double>{0, 0, -1}, 1, vetor<double>{0, 1, 0})); // Esfera central

    // futuras esferas (?)
    // esferas.add(sphere(vetor<double>{1, 0, -1}, 0.4, vetor<double>{0, 1, 0})); 
    // esferas.add(sphere(vetor<double>{-1, 0, -1}, 0.4, vetor<double>{0, 0, 1})); 

    // Define a iluminação e o material
    iluminacao luz{
        vetor<double>(0.5, -0.5, -0.1), // posição da luz -> diagonal direita superior
        vetor<double>(0.8, 0.8, 0.8), // intensidade ambiente
        vetor<double>(0.7, 0.7, 0.7), // intensidade difusa
        vetor<double>(0.5, 0.5, 0.5)  // intensidade especular
    };

    iluminacao luz2{
        vetor<double>(-0.5, -0.5, -0.1), // posição da luz -> diagonal esquerda superior
        vetor<double>(0.8, 0.8, 0.8), // intensidade ambiente
        vetor<double>(0.7, 0.7, 0.7), // intensidade difusa
        vetor<double>(0.5, 0.5, 0.5)  // intensidade especular
    };

    listaLuzes luzes;
    luzes.addLuz(luz);
    luzes.addLuz(luz2);

    phongComponentes material(  0.1, // ka
                                0.3, // kd
                                0.9, // ks
                                10.0, // n
                                1.0, // kr
                                0.5, // kt
                                1.0, // n1
                                10.5 // n2
                                );
    phongComponentes materialEsferas(  
                                0.1, // ka
                                0.3, // kd
                                0.9, // ks
                                10.0, // n
                                0.5, // kr
                                0.5, // kt
                                1.0, // n1
                                10.5 // n2
                                );


    // define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, larguraDaViewport)), mult(0.5, alturaDaViewport)), mira);
    //vetor cantoEsquerdoTela = origem - horizontal/2 - vertical/2 - mira
    
    
    // define a cor do fundo
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = subtracao(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, larguraDaViewport), mult(v, alturaDaViewport))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            // vetor<double> color = raioColor(r, mundo, esferas, camera.posicaoDaCamera, luz, material);
            vetor<double> color = raioColor(r, esferas, camera.posicaoDaCamera, luzes, material, materialEsferas, textura1); 
            image[j][i] = color;
        }
    }

    // output
    cout << "P3\n" << imWidth << " " << imHeight << "\n255\n";
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            int ir = int(255.99 * image[j][i].x);
            int ig = int(255.99 * image[j][i].y);
            int ib = int(255.99 * image[j][i].z);
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    clog << "\rDone.\n";   
    return 0;
}