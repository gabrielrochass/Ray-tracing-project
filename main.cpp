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
#include "octree.cpp" // Include the header file for OctreeNode

using namespace std;

const double infinity = numeric_limits<double>::infinity();

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

// verifica se o ponto está em sombra
bool estaNaSombra(const vetor<double>& ponto, listaLuzes luzes, const OctreeNode& octree, const plano& plano1) {
    for (int i = 0; i < luzes.luzes.size(); i++) {
        vetor<double> luzPos = luzes.acessarLuz(i).posicao;
        raio<double> r(ponto, subtracao(luzPos, ponto));
        hit_record rec;

        // Verifica se há interseção com a octree ou plano
        bool intersecionouOctree = octree.intersectou(r, 0.001, infinity, rec);
        bool intersecionouPlano = plano1.hitPlano(r, 0.001, infinity, rec);

        // Se houver interseção com qualquer objeto, o ponto está na sombra para esta luz
        if (intersecionouOctree || intersecionouPlano) {
            return true;
        }
    }

    // Se nenhuma luz estiver obstruída, o ponto não está na sombra
    return false;
}

// raio color com octree
vetor<double> raioColor(const raio<double>& raio, const OctreeNode& octree, const vetor<double>& posicaoObservador, listaLuzes luzes, const phongComponentes& material, const phongComponentes& materialEsf) {
    hit_record rec;
    plano plano1(vetor<double>{0.0, 0.0, -1.0}, vetor<double>{0.0, 0.0, 1.0});
    vetor<double> corFinal = {0.0, 0.0, 0.0};

    if (octree.intersectou(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        for (int i = 0; i < luzes.luzes.size(); i++) {
            corFinal = corFinal + calcularIluminacaoPhong(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, materialEsf, octree, plano1, 1);
        }
        return corFinal;
    } else if (plano1.hitPlano(raio, 0.001, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        for (int i = 0; i < luzes.luzes.size(); i++) {
            corFinal = corFinal + calcularIluminacaoPhongPlano(p, N, posicaoObservador, luzes.acessarLuz(i), luzes, material, plano1, octree, 2);
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
    vetor<double> posicaoDaCamera(0, 0, 1); 
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);

    double angulo = 3.14 / 4; 
    // define a rotação eixo Z
    matriz4x4 rotacaoZ = matriz4x4::createRotationZ(angulo,false);

    // define a rotação eixo X
    matriz4x4 rotacaoX = matriz4x4::createRotationX(angulo,false);

    // translação para a direita
    matriz4x4 trans = matriz4x4::createTranslation(-0.5, 0, 0);

    // define a rotação eixo Y
    matriz4x4 rotacaoY = matriz4x4::createRotationY(angulo,false);

    // define o mundo
    malha mundo;
    sphere_list esferas;
    
    esferas.add(sphere(vetor<double>{0, 0, -1}, 0.5, vetor<double>{1, 0, 0})); // Esfera verde
    esferas.add(sphere(vetor<double>{1, 0, -1}, 0.4, vetor<double>{0, 1, 0})); // Esfera azul
    esferas.add(sphere(vetor<double>{-1, 0, -1}, 0.4, vetor<double>{0, 0, 1})); // Esfera vermelha

    // Cria o octree e adiciona as esferas
    OctreeNode octree(BoundingBox(vetor<double>{-1, -1, -1}, vetor<double>{1, 1, 1}));
    octree.inserirEsfera(make_shared<sphere>(vetor<double>{0, 0, -1}, 0.5, vetor<double>{1, 0, 0}));

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

    phongComponentes material(  
                                0.1, // ka
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
                                1, // kr
                                1, // kt
                                1.0, // n1
                                10.5 // n2
                                );

    // define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, larguraDaViewport)), mult(0.5, alturaDaViewport)), mira);

    // define a cor do fundo
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = subtracao(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, larguraDaViewport), mult(v, alturaDaViewport))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            vetor<double> color = raioColor(r, octree, camera.posicaoDaCamera, luzes, material, materialEsferas);
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
