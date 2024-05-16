#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.cpp"
#include "raio.cpp"

using namespace std;

Vetor mult(double scalar, Vetor h) {
    return Vetor(h.x * scalar, h.y * scalar, h.z * scalar);
}

Vetor subtracaoVP(double scalar, Vetor h) {
    return Vetor(h.x - scalar, h.y - scalar, h.z - scalar);
}

bool hit(const Vetor& center, double radius, const Raio& raio) {
    Vetor oc = subtracao(raio.origem, center);
    double a = produtoEscalar(raio.direcao, raio.direcao);
    double b = produtoEscalar(raio.direcao, oc);
    double rr = radius * radius;
    double oocc = produtoEscalar(oc, oc);
    Vetor c = Vetor(oocc, rr, 0.0);
    double bb = 2.0 * b;
    Vetor ac = mult(a, c);
    Vetor ac4 = mult(4.0, ac);
    Vetor discriminant = ac4;
    if (discriminant.x <= 0) {
        return false;
    }
    else {
        return true;
    }
}

Vetor backgroundColor(const Vetor& dir) {
    double t = 0.5 * (dir.y + 1.0);
    return Vetor((1 - t) * 1.0 + t * 0.5,
                  (1 - t) * 1.0 + t * 0.7,
                  (1 - t) * 1.0 + t * 1.0);
}

Vetor raioColor(const Raio& raio, const Vetor& sphereCenter, double sphereRadius) {
    if (hit(sphereCenter, sphereRadius, raio)) {
        return Vetor(1.0, 0.0, 0.0); // Red color for hit
    }

    return backgroundColor(raio.direcao);
}

int main() {
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    
    vector<vector<Vetor>> image(imHeight, vector<Vetor>(imWidth));

    Vetor origin(0.0, 0.0, 0.0);
    Vetor lowerLeftCorner(-8.0, -4.5, -1.0);
    Vetor horizontal(16.0, 0.0, 0.0);
    Vetor vertical(0.0, 9.0, 0.0);
    Vetor sphereCenter(0.0, 0.0, -1.0);
    double sphereRadius = 0.5;

    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            Vetor gh = mult(v, vertical);
            Vetor uh = mult(u, horizontal);
            Vetor dir = soma(soma(lowerLeftCorner, uh), gh);
            Raio raio(origin, dir);
            image[j][i] = raioColor(raio, sphereCenter, sphereRadius);
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

    cout << "Image size " << imWidth << " x " << imHeight << endl;
    clog << "\rDone.                 \n";
    return 0;
}
