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
#include "textura.h"

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

// calcula a cor de um pixel
vetor<double> raioColor(const raio<double>& raio, const sphere_list& esferas, const vetor<double>& posicaoObservador, listaLuzes luzes, const phongComponentes& material, const phongComponentes& materialEsf) {
    hit_record rec;
    vetor<double> corFinal = {0.0, 0.0, 0.0};
    bool hitAnything = false;
    double t_closest = infinity;
    const sphere* hit_sphere = nullptr;

    plano plano1(vetor<double>{0.0, 0.0, -1.0}, vetor<double>{0.0, 1.0, 0.0});

    // Verifica a interseção com as esferas
    for (const auto& esfera : esferas.list) {
        hit_record temp_rec;
        if (esfera.hit(raio, 0, t_closest, temp_rec)) {
            t_closest = temp_rec.t;
            rec = temp_rec;
            hit_sphere = &esfera;
            hitAnything = true;
        }
    }

    if (hitAnything && hit_sphere) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(rec.normal);

        // UV mapping
        vetor<double> uv = hit_sphere->obterCoordenadasUV(p - hit_sphere->center);
        vetor<double> corTextura = hit_sphere->textura ? hit_sphere->textura->corTextura(uv.x, uv.y) : rec.cor; // se a esfera tiver textura, usa a cor da textura, senão usa a cor padrão
        corFinal = corTextura;
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

    // define a câmera
    vetor<double> posicaoDaCamera(0, 0, 1);
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);
    
    // carregar textura
    const Textura* textura1 = new Textura("texturas/terra.bmp");
    const Textura* textura2 = new Textura("texturas/parede-verde.bmp");
    const Textura* textura3 = new Textura("texturas/tapete.bmp");
    const Textura* textura4 = new Textura("texturas/moana.bmp");
    const Textura* textura5 = new Textura("texturas/jupter.bmp");


    sphere_list esferas;
    esferas.add(sphere(vetor<double>{0, 0, -1}, 1, textura1)); // Esfera central
    esferas.add(sphere(vetor<double>{1.7, 0, -1}, 0.7, textura3));
    esferas.add(sphere(vetor<double>{-1.7, 0, -1}, 0.7, textura5));


    // futuras esferas (?)
    // esferas.add(sphere(vetor<double>{0, 0, -1}, 1, vetor<double>{0, 1, 0})); // Esfera central
    // esferas.add(sphere(vetor<double>{1.5, 0, -1}, 0.7, vetor<double>{0, 1, 0})); 
    // esferas.add(sphere(vetor<double>{-1.5, 0, -1}, 0.7, vetor<double>{0, 0, 1})); 

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
            vetor<double> color = raioColor(r, esferas, camera.posicaoDaCamera, luzes, material, materialEsferas);
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