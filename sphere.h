#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"
#include "hit_record.h"
#include "raio.h"
#include "textura.cpp"
using namespace std;


class sphere{
    public:
        sphere() {}
        sphere(vetor<double> c, double r, const Textura* t = nullptr) : center(c), radius(r), textura(t) {};

        bool hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;
        vetor<double> obterCoordenadasUV(const vetor<double>& pontoDeIntersecao) const;
        
    public:
        vetor<double> center;
        double radius;
        vetor<double> cor;
        const Textura* textura;
};

bool sphere::hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const{
    double a = produtoEscalar(raio.direcao, raio.direcao);
    vetor<double> oc = subtracao(center, raio.origem);
    double metadeb = produtoEscalar(raio.direcao, oc);
    double b = 2.0 * produtoEscalar(raio.direcao, oc);
    double c = produtoEscalar(oc, oc) - radius * radius;
    double discriminant = metadeb * metadeb -  a * c;

    if (discriminant < 0) {
        return false;
    } 
    auto sqrtd = sqrt(discriminant);

    auto root = (metadeb - sqrtd) / a;
    if (root <= t_min || t_max <= root) {
        root = (metadeb + sqrtd) / a;
        if(root <= t_min || t_max <= root){
            rec.cor = cor;
            return false;
        }
    }

    rec.t = root;
    rec.p = raioAt(raio, rec.t);
    rec.normal = vetorUni(subtracao(rec.p, center));

    if (textura != nullptr) {
        rec.cor = textura->corTextura(obterCoordenadasUV(rec.normal).x, obterCoordenadasUV(rec.normal).y);
    } else {
        rec.cor = cor;
    }
    
    return true;
}

vetor<double> sphere::obterCoordenadasUV(const vetor<double>& pontoDeIntersecao) const {
    double phi = atan2(pontoDeIntersecao.z, pontoDeIntersecao.x);
    double theta = asin(pontoDeIntersecao.y);

    double u = 1 - (phi + pi) / (2 * pi);
    double v = (theta + pi / 2) / pi;

    return vetor<double>(u, v, 0);
}

#endif