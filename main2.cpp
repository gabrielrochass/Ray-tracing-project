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
#include "BSP.h"

using namespace std;

const double infinity = numeric_limits<double>::infinity();

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}




bool intersectRayBSP(const raio<double>& ray, BSPNode* node, double t_min, double t_max, hit_record& rec) {
    if (!node) return false;

    if (node->spheres.size() + node->triangles.size() > 0) {
        hit_record tempRec;
        bool hitAnything = false;
        double closestSoFar = t_max;

        for (const auto& sphere : node->spheres) {
            if (sphere.hit(ray, t_min, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        for (const auto& triangle : node->triangles) {
            if (triangle.hit(ray, t_min, closestSoFar, tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
    // tPlane é o valor de t onde o raio atinge o plano de secção
    double tPlane = (node->median_value - getAxisValue(ray.origem, node->axis)) / getAxisValue(ray.direcao, node->axis);

    BSPNode* firstChild;
    BSPNode* secondChild;

    // Se o raio está indo na direção do plano de secção, o primeiro filho é o da esquerda e o segundo filho é o da direita
    // Caso contrário, o primeiro filho é o da direita e o segundo filho é o da esquerda
    if (getAxisValue(ray.origem, node->axis) < node->median_value) {
        firstChild = node->left;
        secondChild = node->right;
    } else {
        firstChild = node->right;
        secondChild = node->left;
    }

    // Se o raio não atinge o plano de secção, ele não atinge nenhum dos filhos
    if (tPlane > t_max || tPlane <= 0) {
        return intersectRayBSP(ray, firstChild, t_min, t_max, rec);
    } else if (tPlane < t_min) { // Se o raio atinge o plano de secção, ele atinge ambos os filhos
        return intersectRayBSP(ray, secondChild, t_min, t_max, rec);
    } else { // Se o raio atinge o plano de secção, ele atinge ambos os filhos
        if (intersectRayBSP(ray, firstChild, t_min, tPlane, rec)) { // Verifica se o raio atinge o primeiro filho
            return true;
        }
        return intersectRayBSP(ray, secondChild, tPlane, t_max, rec); // Verifica se o raio atinge o segundo filho
    }
}

vetor<double> raioColor(const raio<double>& r, BSPNode& root, const vetor<double>& origem, const listaLuzes& luzes, const phongComponentes& material, const phongComponentes& materialEsferas) {
    hit_record rec;
    plano plano1(vetor<double>(0, 0, -1), vetor<double>(0, 0, 1));

    if (intersectRayBSP(r, &root, 0.001, infinity, rec)) {
        vetor<double> cor(0, 0, 0);
        for (const auto& luz : luzes.luzes) {
            cor += material.ka * luz.Ia;
            vetor<double> direcaoLuz = normal(subtracao(luz.posicao, rec.p));
            double difusa = produtoEscalar(rec.normal, direcaoLuz);
            if (difusa > 0) {
                cor += multiplicacaoPorEscalar(multiplicacaoPorEscalar(luz.Id, material.kd), difusa);

                //Formula DR = 2 * (N . L) * N - L
                vetor<double> direcaoRefletida = normal(subtracao(mult(2 * produtoEscalar(direcaoLuz, rec.normal), rec.normal), direcaoLuz));
                //Formula especular = R . V
                double especular = produtoEscalar(normal(subtracao(origem, rec.p)), direcaoRefletida); // Corrigir direção da câmera
                //Formula Ks * (R . V)^n
                if (especular > 0) {
                    cor += multiplicacaoPorEscalar(multiplicacaoPorEscalar(luz.Is, material.ks), pow(especular, material.n));
                }
            }
        }
        return cor;
    }
    else if (plano1.hitPlano(r, 0.001, infinity, rec)) {
        return vetor<double>(1, 1, 1);
    }
    return backgroundColor(r.direcao);
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

    
    double angulo = 3.14 / 4; 
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
    
    esferas.add(sphere(vetor<double>{0, 0, -1}, 0.5, vetor<double>{1, 0, 0})); // Esfera verde
    // esferas.add(sphere(vetor<double>{1, 0.5, -1}, 0.3, vetor<double>{0, 1, 0})); // Esfera azul
    // esferas.add(sphere(vetor<double>{-1, -0.5, -1}, 0.3, vetor<double>{0, 0, 1})); // Esfera vermelha
    esferas.add(sphere(vetor<double>{1, 0, -1}, 0.4, vetor<double>{0, 1, 0})); // Esfera azul
    esferas.add(sphere(vetor<double>{-1, 0, -1}, 0.4, vetor<double>{0, 0, 1})); // Esfera vermelha
    
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

    
    // adiciona os triangulos ao mundo
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

    //phongComponentes material(0.1, 0.1, 0.9, 100, 0.0, 0.0, 0.1, 1.5);
    // phongComponentes material(0.1, 0.9, 0.5, 10.0, 1.0, 1.0, 1.0, 10.5);
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
                                0, // kr
                                1, // kt
                                1.0, // n1
                                10.5 // n2
                                );
      
    // define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, larguraDaViewport)), mult(0.5, alturaDaViewport)), mira);
    //vetor cantoEsquerdoTela = origem - horizontal/2 - vertical/2 - mira
    vector<sphere_list> esferass;
    esferass.push_back(esferas);
    vector<malha> mundos;
    mundos.push_back(mundo);
    BSPNode* root = buildBSP(esferass, mundos);
    
    // define a cor do fundo
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = subtracao(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, larguraDaViewport), mult(v, alturaDaViewport))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            // vetor<double> color = raioColor(r, mundo, esferas, camera.posicaoDaCamera, luz, material);
            //vetor<double> color = raioColor(r, mundo, esferas, camera.posicaoDaCamera, luzes, material, materialEsferas);
            //Put the BSP structure in the new function raioColor
            vetor<double> color = raioColor(r, *root, camera.posicaoDaCamera, luzes, material, materialEsferas);
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