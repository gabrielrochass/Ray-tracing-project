#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"

using namespace std;

struct hit_record {
    vetor<double> p;
    vetor<double> normal;
    double t;
};

class sphere{
    public:
        sphere() {}
        sphere(vetor<double> c, double r) : center(c), radius(r) {};

        bool hit(
            const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;
        
    public:
        vetor<double> center;
        double radius;
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
            return false;
        }
    }

    rec.t = root;
    rec.p = raioAt(raio, rec.t);
    rec.normal = multiplicacaoPorEscalar(subtracao(rec.p, center), 1.0 / radius);

    return true;
}

#endif