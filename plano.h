#ifndef PLANO_H
#define PLANO_H

#include <iostream>
#include <cmath>
#include <vector>
#include "ponto.h"
#include "vector.h"
#include "sphere_list.h"
#include "sphere.h"
#include "raio.h"
#include <limits>


using namespace std;


class plano {
    public:
        vetor<double> pontoNoPlano;
        vetor<double> normal;

        plano() {}
        plano(vetor<double> ponto, vetor<double> normal) : pontoNoPlano(ponto), normal(normal) {}
        //bool hitPlano(const plano& plano, const raio<double>& raioDeInterseccao);
        bool hitPlano(const raio<double>& r, double t_min, double t_max, hit_record& rec) const;
};


bool plano::hitPlano(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
        double denom = produtoEscalar(normal, r.direcao);
        if (fabs(denom) > 1e-6) { // evitar divisão por zero
            double t = produtoEscalar(subtracao(pontoNoPlano, r.origem), normal) / denom;
            if (t >= t_min && t <= t_max) {
                rec.t = t;
                rec.p = raioAt(r, t);
                rec.normal = normal;
                return true;
            }
        }
        return false;
}
#endif
