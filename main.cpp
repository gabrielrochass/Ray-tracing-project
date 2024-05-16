#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "camera.cpp"

using namespace std;

template<typename T> 
vetor<T> mult(T scalar, vetor<T> h) {
    return vetor<T>(h.x * scalar, h.y * scalar, h.z * scalar);
}

template<typename T> 
vetor<T> subtracaoVP(T scalar, vetor<T> h) {
    return vetor<T>(h.x - scalar, h.y - scalar, h.z - scalar);
}

template<typename T>
// Function to compute hit between a sphere and a raio
bool hit(const vetor<T>& center, T radius, const raio<T>& raio) {
    vetor<T> oc = subtracao(raio.origem, center);
    T a = produtoEscalar(raio.direcao, raio.direcao);
    T b = produtoEscalar(raio.direcao, oc); // * 2.0 (lembrar de multiplicar depois)
    T rr = radius * radius;
    T oocc = produtoEscalar(oc, oc);
    //vetor<T> c = subtracaoVP(rr, oocc);
    vetor<T> c =  vetor<T>(oocc, rr, 00)occ, rr, 0.0);
    T bb = 2.0 *  b;
    vetor<T> ac = mult(a, c);
    vetor<T> ac4 = mult(4.0, ac);
    vetor<T> discriminant = ac4;
    if (discriminant.x <= 0) {
        return false;
    }
    else {
        return true;
    }
    
}




/*function Sphere(c::Vec3{T}, r::T) where T
    Sphere{T}(c, r)
end

function hit(sphere::Sphere, ray::Ray)
    # bhaskara
    a = normsquared(ray.direction)
    oc = sphere.center - ray.origin
    b = 2.0 * dot(ray.direction, oc)
    c = normsquared(oc) - sphere.radius^2
    discriminant = b*b - 4*a*c

    if discriminant <=0
        false
    else
        true
    end*/

template<typename T>
// Function to compute color based on the direction of the raio
vetor<T> backgroundColor(const vetor<T>& dir) {
    T t = 0.5 * (dir.y + 1.0);
    return vetor<T>((1 - t) * 1.0 + t * 0.5,
                  (1 - t) * 1.0 + t * 0.7,
                  (1 - t) * 1.0 + t * 1.0);
}

template<typename T>
// Function to compute color of the raio
vetor<T> raioColor(const raio<T>& raio, const vetor<T>& sphereCenter, T sphereRadius) {
    if (hit(sphereCenter, sphereRadius, raio)) {
        return vetor<T>(1.0, 0.0, 0.0); // Red color for hit
    }

    return backgroundColor(raio.direcao);
}

 
int main() {
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
    
    std::vector<std::vector<vetor<double>>> image(imHeight, std::vector<vetor<double>>(imWidth));

    vetor origin(0.0, 0.0, 0.0);
    vetor lowerLeftCorner(-8.0, -4.5, -1.0);
    vetor horizontal(16.0, 0.0, 0.0);
    vetor vertical(0.0, 9.0, 0.0);
    vetor sphereCenter(0.0, 0.0, -1.0);
    double sphereRadius = 0.5;

    

    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            double u = double(i) / (imWidth - 1);
            double v = 1.0 - double(j) / (imHeight - 1);
            vetor gh = mult(v, vertical);
            vetor uh = mult(u, horizontal);
            vetor dir = soma(soma(lowerLeftCorner, uh), gh);
            raio raio(origin, dir);
            image[j][i] = raioColor(raio, sphereCenter, sphereRadius);
        }
    }

    
    std::cout << "P3\n" << imWidth << " " << imHeight << "\n255\n";
    for (int j = 0; j < imHeight; ++j) {
        for (int i = 0; i < imWidth; ++i) {
            int ir = int(255.99 * image[j][i].x);
            int ig = int(255.99 * image[j][i].y);
            int ib = int(255.99 * image[j][i].z);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    

    std::cout << "Image size " << imWidth << " x " << imHeight << std::endl;
    std::clog << "\rDone.                 \n";
    return 0;
}