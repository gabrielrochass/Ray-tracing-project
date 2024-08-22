#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"
#include "hit_record.h"
#include "raio.h"
#include "textura.h"
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

    // rec.cor = textura->corTextura(obterCoordenadasUV(rec.normal).x, obterCoordenadasUV(rec.normal).y);
    rec.cor = vetor<double>(1, 0, 0);
    
    return true;
}

vetor<double> sphere::obterCoordenadasUV(const vetor<double>& pontoDeIntersecao) const {
    const double pi = 3.14159265358979323846;
    double phi = atan2(pontoDeIntersecao.z, pontoDeIntersecao.x); // atan2 retorna o arco tangente (ângulo) entre o eixo x e o ponto de interseção no plano xz
    double theta = asin(pontoDeIntersecao.y); // asin retorna o arco seno (ângulo) entre o eixo y e o ponto de interseção no plano yz

    // mapeia os ângulos phi e theta para as coordenadas UV da textura normalizadas
    double u = 1 - (phi + pi) / (2 * pi); 
    double v = (theta + pi / 2) / pi;

    return vetor<double>(u, v, 0);
}

#endif