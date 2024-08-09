#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <memory>
//include "ponto.h"
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
#include "cone.h"

using namespace std;

const double infinity = numeric_limits<double>::infinity();

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

vetor<double> raioColor(const raio<double>& raio, const cone_list& cones, const vetor<double>& posicaoObservador, listaLuzes luzes, const phongComponentes& material, const cylinder_list& cilindros) {
    hit_record rec;
    vetor<double> corFinal = {0.0, 0.0, 0.0};
    sphere_list esferas;
    
    esferas.add(sphere(vetor<double>{0, 0, -1}, 0.5, vetor<double>{1, 0, 0}));
    if (cones.hit(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);
        
        for (int i = 0; i < luzes.luzes.size(); i++) {    
            corFinal = corFinal + calcularIluminacaoPhongCone(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, material, esferas, cones, plano{}, 2);
        }
        return corFinal;
    }
    else if(cilindros.hit(raio, 0, infinity, rec)){
        
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);
        
        for (int i = 0; i < luzes.luzes.size(); i++) {    
            corFinal = corFinal + calcularIluminacaoPhongCilindro(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, material, esferas, cilindros, plano{}, 2);
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

    // define a câmera
    vetor<double> posicaoDaCamera(0, 0, 0);
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);

    // define o mundo
    // define a iluminação e o material
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

    // define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, larguraDaViewport)), mult(0.5, alturaDaViewport)), mira);

    // criando cones com coordenas parecidas com as esferas
    // cone cone1(vetor<double>{-0.8, 0.0, -1}, vetor<double>{0, 0, 1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    // cone cone2(vetor<double>{0.5, -0.3, -1}, vetor<double>{0, 0, 1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    // cylinder cilindro1(vetor<double>{0, -0.3, -1.2}, vetor<double>{0, 0, 1}, 0.3, 0.5, vetor<double>{0, 0, 1});
    
    
    // cylinder cilindro2(vetor<double>{1.3, 0.0, -1.25}, vetor<double>{0, 0, 1}, 0.15, 0.5, vetor<double>{0, 1, 0});
    // cylinder cilindro3(vetor<double>{-1.4, 0.0, -1.15}, vetor<double>{0, 0, 1}, 0.1, 0.3, vetor<double>{1, 0, 0});
    //criar cilindros e cones de cabeça para baixo
    double angulo = 3.14 / 2; 
    // define a rotação eixo Z
    matriz4x4 rotacaoZ = matriz4x4::createRotationZ(angulo,false);
    matriz4x4 rotacaoY = matriz4x4::createRotationY(angulo,false);
    vetor<double> v1 = rotacaoZ.multMatrizVetor({-0.8, 0.0, -1});
    vetor<double> v2 = rotacaoZ.multMatrizVetor({0, 0, -1});
    vetor<double> v3 = rotacaoZ.multMatrizVetor({2, 0, -1});
    vetor<double> v4 = rotacaoZ.multMatrizVetor({0.5, -0.3, -1});

    vetor<double> v5 = rotacaoY.multMatrizVetor({0, -0.3, -1.2});
    vetor<double> v6 = rotacaoY.multMatrizVetor({1.3, 0.0, -1.25});
    vetor<double> v7 = rotacaoY.multMatrizVetor({-1.4, 0.0, -1.15});



    cone coneRotacionado1(v1, vetor<double>{0, 0, -1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    cone coneRotacionado2(v4, vetor<double>{0, 0, -1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    cylinder cilindroRotacionado1(v4, v2, 0.3, 0.5, vetor<double>{0, 0, 1});
    cylinder cilindroRotacionado2(v6, v5, 0.15, 0.5, vetor<double>{0, 1, 0});
    cone cone1(vetor<double>{-0.8, 0.0, -1}, vetor<double>{0, 0, -1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    cone cone2(vetor<double>{0.5, -0.3, -1}, vetor<double>{0, 0, -1}, 0.5, 3.14/6, vetor<double>{0.5, 0.5, 0.5});
    cylinder cilindro1(vetor<double>{0, -0.3, -1.2}, vetor<double>{0, 0, -1}, 0.3, 0.5, vetor<double>{0, 0, 1});
    cylinder cilindro2(vetor<double>{1.3, 0.0, -1.25}, vetor<double>{0, 0, -1}, 0.15, 0.5, vetor<double>{0, 1, 0});
    cylinder cilindro3(vetor<double>{-1.4, 0.0, -1.15}, vetor<double>{0, 0, -1}, 0.1, 0.3, vetor<double>{1, 0, 0});
    cylinder_list cilindros;
    cilindros.add(cilindro3);
    cilindros.add(cilindro2);
    cilindros.add(cilindro1);
    cilindros.add(cilindroRotacionado1);
    cilindros.add(cilindroRotacionado2);    
    // lista de cones
    cone_list cones;
    cones.add(cone1);
    cones.add(cone2);
    cones.add(coneRotacionado1);
    cones.add(coneRotacionado2);

    // define a cor do fundo
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = subtracao(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, larguraDaViewport), mult(v, alturaDaViewport))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            vetor<double> color = raioColor(r, cones, camera.posicaoDaCamera, luzes, material, cilindros);
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

    clog << "\rDone.                 \n";
    
    return 0;
}
