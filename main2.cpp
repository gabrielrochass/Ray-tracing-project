#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.cpp"
#include "sphere_list.h"
#include "sphere.h"
#include <limits>

using namespace std;

const double infinity = std::numeric_limits<double>::infinity();

// calcula a interseção de um raio com uma esfera
double hit(const vetor<double>& center, double radius, const raio<double>& raio) {
    double a = produtoEscalar(raio.direcao, raio.direcao);
    vetor<double> oc = subtracao(center, raio.origem);
    double metadeb = produtoEscalar(raio.direcao, oc);
    double b = 2.0 * produtoEscalar(raio.direcao, oc);
    double c = produtoEscalar(oc, oc) - radius * radius;
    double discriminant = metadeb * metadeb -  a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-metadeb - sqrt(discriminant)) /  a;
    }
}

// define a cor do fundo
vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                         (1 - t) * 1.0 + t * 0.7,
                         (1 - t) * 1.0 + t * 1.0);
}

// define se um raio intersecta ou não um plano -> retorna true se intersecta, false caso contrário
bool hitPlano(const vetor<double>& pontoNoPlano, const vetor<double>& normal, const raio<double>& raioDeInterseccao) {
    double denominador = produtoEscalar(normal, raioDeInterseccao.direcao); // usado pra determinar se o raio é paralelo ao plano
    // Verifica se o denominador é zero para evitar divisão por zero
    if (fabs(denominador) < 1e-6) { // 0
        return false; // Raio é paralelo ao plano -> não intersecta
    }
    // Calcula a distância para o ponto de interseção ao longo do raio
    double t = -produtoEscalar(subtracao(raioDeInterseccao.origem, pontoNoPlano), normal) / denominador;
    return t >= 0; // Se t for maior ou igual a zero, o raio intersecta o plano
} //**************

// define a cor do pixel de acordo com a interseção do raio com a esfera
vetor<double> raioColor(const raio<double>& raio, const sphere_list& mundo) {
    hit_record rec;
    if(mundo.hit(raio, 0, infinity, rec)){
        return multiplicacaoPorEscalar(vetor<double>(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1),0.5);
    }
    else if(hitPlano({0.0, -1.25, -1.0}, {0.0, 1.0, 0.0}, raio)) {
        return vetor<double>(1.0, 1.0, 1.0);
    }
    vetor<double> direcao_uni = vetorUni(raio.direcao);
    return backgroundColor(direcao_uni);
}


int main() {
    // define a imagem
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    vector<vector<vetor<double>>> image(imHeight, vector<vetor<double>>(imWidth));

    // define o mundo
    sphere_list mundo;
    //mundo.add(sphere(vetor<double>(0, -100.5, -1), 100));
    mundo.add(sphere(vetor<double>(0, 0, -1), 0.5));
    mundo.add(sphere(vetor<double>(1, +100.5, -1), 100));

    // define a câmera
    vetor<double> posicaoDaCamera(0, 0, 0);
    vetor<double> mira(0, 0, -1);
    vetor<double> vUp(0, 1, 0);
    Camera camera(posicaoDaCamera, mira, vUp);

    // define a viewport
    const vetor<double> larguraDaViewport(32.0 / 9.0, 0.0, 0.0);
    const vetor<double> alturaDaViewport(0.0, 2.0, 0.0);
    vetor<double> horizontal{larguraDaViewport.x, 0.0, 0.0};
    vetor<double> vertical{0.0, alturaDaViewport.y, 0.0};
    //vetor<double> cantoEsquerdoTela = subtracao(vetor{-16.0 / 9.0, 0.0, 0.0}, vetor{0.0, -1.0, 1.0});
    vetor<double> cantoEsquerdoTela = subtracao(subtracao(subtracao(camera.posicaoDaCamera, mult(0.5, horizontal)), mult(0.5, vertical)), mira);
    //vetor cantoEsquerdoTela = origem - horizontal/2 - vertical/2 - mira
    // define a esfera
    vetor<double> sphereCenter{0.0, 0.0, -1.0};
    double sphereRadius = 0.5;

    // define a cor do fundo
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            
            vetor<double> direcaoDoRaio = soma(camera.posicaoDaCamera, soma(cantoEsquerdoTela, soma(mult(u, horizontal), mult(v, vertical))));
            raio<double> r(camera.posicaoDaCamera, direcaoDoRaio);
            vetor<double> color = raioColor(r, mundo);
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
