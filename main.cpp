#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.cpp"

using namespace std;



template<typename T>
double hit(const vetor<T>& center, T radius, const raio<T>& raio) {
    double a = produtoEscalar(raio.direcao, raio.direcao);
    vetor<T> oc = subtracao(center, raio.origem);
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


template<typename T>

vetor<T> backgroundColor(const vetor<T>& dir) {
    T t = 0.5 * (dir.y + 1.0);
    return vetor<T>((1 - t) * 1.0 + t * 0.5,
                  (1 - t) * 1.0 + t * 0.7,
                  (1 - t) * 1.0 + t * 1.0);
}

template<typename T>
// Function to compute color of the raio
vetor<T> raioColor(const raio<T>& raio, const vetor<T>& sphereCenter, T sphereRadius) {

    double t = hit(sphereCenter, sphereRadius, raio);
    if (t > 0.0) {
        vetor<T> p = raioAt(raio, t);
        vetor<T> N = vetorUni(subtracao(p, sphereCenter));
        vetor<T> color = mult(0.5, soma(vetor<T>(1.0, 1.0, 1.0), N));
        return color; 
    }
    if (hit(sphereCenter, sphereRadius, raio)) {
        return vetor<T>(1.0, 0.0, 0.0); 

    }

    return backgroundColor(raio.direcao);
}


 
int main() {
    const int imWidth = 801;
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
    //vetor lowerLeftCorner = origem - horizontal/2 - vertical/2 - Vec3(0.0, 0.0, focallenght)
;
    
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
