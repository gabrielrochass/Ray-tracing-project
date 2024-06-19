#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.h"
#include "sphere_list.h"
#include "sphere.h"
#include "plano.h" 
#include <limits>
#include <memory>
#include "malha_triangulos.h"
#include "triangulo.h"
#include "raio.h"
#include "matriz4x4.h"
#include "phongComponentes.h"
using namespace std;

const double infinity = std::numeric_limits<double>::infinity();

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

/*vetor<double> raioColor(const raio<double>& raio, malha mundo, sphere_list esferas) {
    hit_record rec;
    
    plano plan(vetor<double>{0.0, -1.25, -1.0}, vetor<double>{0.0, 1.0, 2});
    if(esferas.hit(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(subtracao(p, vetor<double>(0.5, 1.0, -1)));
        vetor<double> color = mult(0.5, soma(esferas.list[0].center, N));
        return color; 
    }
    else if(mundo.hit(raio, 0, infinity, rec)) {
        
        vetor<double> color = mult(0.65, soma(vetor<double>{1, 1, 1}, rec.normal));
        return color;
    }
    

    vetor<double> direcao_uni = vetorUni(raio.direcao);
    return backgroundColor(direcao_uni);
}
*/
vetor<double> raioColor(const raio<double>& raio, const malha& mundo, const sphere_list& esferas, const vetor<double>& posicaoObservador, const iluminacao& luz, const phongComponentes& material) {
    hit_record rec;

    if (esferas.hit(raio, 0, infinity, rec)) {
        vetor<double> p = raioAt(raio, rec.t);
        vetor<double> N = vetorUni(subtracao(p, vetor<double>(0.0, 0.0, -1)));
        return calcularIluminacaoPhong(p, N, posicaoObservador, luz, material);
    }
    else if (mundo.hit(raio, 0, infinity, rec)) {
        return calcularIluminacaoPhong(rec.p, rec.normal, posicaoObservador, luz, material);
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

    
    double angulo = M_PI / 4; 
    // define a rotação eixo Z
    matriz4x4 rotacaoZ = matriz4x4::createRotationZ(angulo,false);

    // define a rotação eixo X
    matriz4x4 rotacaoX = matriz4x4::createRotationX(angulo,false);

    //translação para a direita
    matriz4x4 trans = matriz4x4::createTranslation(-0.5, 0, 0);

    // define a rotação eixo Y
    matriz4x4 rotacaoY = matriz4x4::createRotationY(angulo,false);

    

    // define o mundo
    malha mundo;
    sphere_list esferas;
    
    esferas.add(sphere(vetor<double>{0.0, 0.0, -1}, 0.50));
    
    // adiciona triângulos à malha
    // criação dos vértices triângulo 1 rotacionado eixo Z
    vetor<double> v1 = rotacaoZ.multMatrizVetor({1, 0, -1});
    vetor<double> v2 = rotacaoZ.multMatrizVetor({1, -1, -1});
    vetor<double> v3 = rotacaoZ.multMatrizVetor({2, 0, -1});
    triangulo tri1(v1, v2, v3);

    // criação dos vértices triângulo 2 transladado para a direita
    vetor<double> v4 = trans.multMatrizVetor({0, 0 , -1});
    vetor<double> v5 = trans.multMatrizVetor({0, -1, -1});
    vetor<double> v6 = trans.multMatrizVetor({1, 0, -1});
    triangulo tri2(v4, v5, v6);

    // criação dos vértices triângulo 3 rotacionado eixo X
    vetor<double> v7 = rotacaoX.multMatrizVetor({-1, 0, -1});
    vetor<double> v8 = rotacaoX.multMatrizVetor({-1, -1, -1});
    vetor<double> v9 = rotacaoX.multMatrizVetor({0, 0, -1});
    triangulo tri3(v7, v8, v9);

    // criação dos vértices triângulo 4 rotacionado eixo Y
    vetor<double> v10 = rotacaoY.multMatrizVetor({-2, 0, -1});
    vetor<double> v11 = rotacaoY.multMatrizVetor({-2, -1, -1});
    vetor<double> v12 = rotacaoY.multMatrizVetor({-1, 0, -1});
    triangulo tri4(v10, v11, v12);

    

    mundo.add(tri1);
    mundo.add(tri2);
    mundo.add(tri3);
    mundo.add(tri4);
  
    //mundo.add(triangulo(vetor<double>{0, 0, -1}, vetor<double>{0, -1, -1}, vetor<double>{1, 0, -1})); 
    //mundo.add(triangulo(vetor<double>{-1, 0, -1}, vetor<double>{-1, -1, -1}, vetor<double>{0, 0, -1}));
    //mundo.add(triangulo(vetor<double>{-2, 0, -1}, vetor<double>{-2, -1, -1}, vetor<double>{-1, 0, -1})); 
    
    // parâmetros da classe triangulo: vetor<double> v0, vetor<double> v1, vetor<double> v2
    // cada vetor<double> é um ponto no espaço 3D

    // Define a iluminação e o material
    iluminacao luz{
        vetor<double>(1.0, 1.0, 1.0), // posição da luz
        vetor<double>(0.1, 0.1, 0.1), // intensidade ambiente
        vetor<double>(0.7, 0.7, 0.7), // intensidade difusa
        vetor<double>(0.5, 0.5, 0.5)  // intensidade especular
    };

    phongComponentes material(0.1, 0.7, 0.5, 10.0);

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
            vetor<double> color = raioColor(r, mundo, esferas, camera.posicaoDaCamera, luz, material);
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

// Adicione triângulos à malha
    // mundo.add(triangulo(vetor<double>{0, 0, -1}, vetor<double>{1, 0, -1}, vetor<double>{0, 1, -1}));
    // mundo.add(triangulo(vetor<double>{1, 0, -1}, vetor<double>{1, 1, -1}, vetor<double>{0, 1, -1}));

    // adiciona esferas ao mundo
    // esferas.add(sphere(vetor<double>{0, 0, -1}, 0.5));
    // esferas.add(sphere(vetor<double>{0, -1, -1}, 0.5));
    // esferas.add(sphere(vetor<double>{1, 0, -1}, 0.5));


 // mundo.add(triangulo(vetor<double>{-1, 0, -1}, vetor<double>{0, 0, -1}, vetor<double>{0, 1, -1})); // meio para baixo pra diretia
    // mundo.add(triangulo(vetor<double>{0, 0, -1}, vetor<double>{0, 1, -1}, vetor<double>{1, 0, -1})); 

    // mundo.add(triangulo(vetor<double>{0.8, 0, -1}, vetor<double>{0.8, -1, -1}, vetor<double>{1.8, 0, -1}));
    // mundo.add(triangulo(vetor<double>{-1, 0, -1}, vetor<double>{-1, -1, -1}, vetor<double>{0, 0, -1}));
    // mundo.add(triangulo(vetor<double>{-1, 0, -1}, vetor<double>{0, 1, -1}, vetor<double>{-1, 1, -1}));