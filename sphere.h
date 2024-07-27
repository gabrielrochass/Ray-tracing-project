#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"
#include "hit_record.h"
#include "raio.h"
#include "octree.cpp"
using namespace std;

class sphere {
public:
    sphere() {}
    sphere(vetor<double> c, double r) : center(c), radius(r) {}

    bool hit(
        const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;

    BoundingBox boundingBox() const {
        return BoundingBox(subtracao(center, vetor<double>{radius, radius, radius}),
                           soma(center, vetor<double>{radius, radius, radius}));
    }

public:
    vetor<double> center;
    double radius;
};

bool sphere::hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const {
    double a = produtoEscalar(raio.direcao, raio.direcao);
    vetor<double> oc = subtracao(center, raio.origem);
    double metadeb = produtoEscalar(raio.direcao, oc);
    double b = 2.0 * produtoEscalar(raio.direcao, oc);
    double c = produtoEscalar(oc, oc) - radius * radius;
    double discriminant = metadeb * metadeb - a * c;

    if (discriminant < 0) {
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    auto root = (metadeb - sqrtd) / a;
    if (root <= t_min || t_max <= root) {
        root = (metadeb + sqrtd) / a;
        if (root <= t_min || t_max <= root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = raioAt(raio, rec.t);
    rec.normal = vetorUni(subtracao(rec.p, center));
    return true;
}

#endif
