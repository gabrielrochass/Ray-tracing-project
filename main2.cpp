#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.h"
#include "sphere_list.h"
#include "sphere.h"
#include <limits>

using namespace std;

const double infinity = std::numeric_limits<double>::infinity();



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

vetor<double> backgroundColor(const vetor<double>& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return vetor<double>((1 - t) * 1.0 + t * 0.5,
                  (1 - t) * 1.0 + t * 0.7,
                  (1 - t) * 1.0 + t * 1.0);
}
// define se um raio intersecta ou não um plano -> retorna true se intersecta, false caso contrário
//  ax + by + cz + d = 0 -> equação do plano
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

// Function to compute color of the raio
vetor<double> raioColor(const raio<double>& raio, const sphere_list& mundo) {
    hit_record rec;
    if(mundo.hit(raio, 0, infinity, rec)){
        return multiplicacaoPorEscalar(vetor<double>(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1),0.5);
    }
    else if(hitPlano({0.0, -1.25, -1.0}, {0.0, 1.0, 0.0}, raio)) { 
        return vetor<double>(0.0, 1.0, 0.0);
    }
    vetor<double> direcao_uni = vetorUni(raio.direcao);
    auto t = 0.5 * (direcao_uni.y + 1.0);
    return soma(multiplicacaoPorEscalar(vetor<double>(1.0, 1.0, 1.0), 1.0 - t), multiplicacaoPorEscalar(vetor<double>(0.5, 0.7, 1.0), t));
}




 
int main() {
    const int imWidth = 801;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    
    vector<vector<vetor<double>>> image(imHeight, vector<vetor<double>>(imWidth));

    sphere_list mundo;
    //mundo.add(sphere(vetor<double>(0, -100.5, -1), 100));
    mundo.add(sphere(vetor<double>(0, 0, -1), 0.5));  
    mundo.add(sphere(vetor<double>(1, +100.5, -1), 100)); 

    vetor<double> origem{0.0, 0.0, 0.0};
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * 16.0 / 9.0;
    vetor<double> horizontal{viewportWidth, 0.0, 0.0};
    vetor<double> vertical{0.0, viewportHeight, 0.0};
    vetor<double> h2 = mult(0.5, horizontal);
    vetor<double> h2o = subtracao(origem, h2);
    vetor<double> v2 = mult(0.5, vertical);
    vetor<double> lente{0, 0, -1};
    vetor<double> v2f = subtracao(v2, lente);
    vetor<double> lowerLeftCorner = subtracao(h2o, v2f);
    //vetor lowerLeftCorner = origem - horizontal/2 - vertical/2 - Vec3(0.0, 0.0, focallenght)
;
    
    vetor<double> sphereCenter{0.0, 0.0, -1.0};
    double sphereRadius = 0.5;
    

    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            raio<double> r(origem, soma(lowerLeftCorner, soma(mult(u, horizontal), mult(v, vertical))));
            vetor<double> color = raioColor(r, mundo);
            image[j][i] = color; 
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
