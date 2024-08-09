#ifndef CONE_H
#define CONE_H


#include <cmath>
#include "vector.h"
#include "triangulo.h"
#include "raio.h"
#include "matriz4x4.h"
#include <vector>
#include <algorithm>

using namespace std;


struct cone {
    vetor<double> apex;  // Vértice do cone
    vetor<double> axis;  // Direção do eixo do cone
    double height;       // Altura do cone
    double angle;        // Ângulo de abertura do cone
    vetor<double> cor;   // Cor do cone

    cone(vetor<double> a, vetor<double> ax, double h, double ang, vetor<double> c)
        : apex(a), axis(vetorUni(ax)), height(h), angle(ang), cor(c) {}

    bool hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const;
};


    
bool cone::hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const {
    vetor<double> co = subtracao(raio.origem, apex);
    double tanThetaSquared = tan(angle) * tan(angle);

    // Coeficientes da equação quadrática
    double a = raio.direcao.x * raio.direcao.x + raio.direcao.z * raio.direcao.z - tanThetaSquared * raio.direcao.y * raio.direcao.y;
    double b = 2.0 * (co.x * raio.direcao.x + co.z * raio.direcao.z - tanThetaSquared * co.y * raio.direcao.y);
    double c = co.x * co.x + co.z * co.z - tanThetaSquared * co.y * co.y;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;  // Nenhuma interseção

    double sqrtd = sqrt(discriminant);

    double t1 = (-b - sqrtd) / (2 * a);
    double t2 = (-b + sqrtd) / (2 * a);

    double t = t1 < t2 && t1 > t_min && t1 < t_max ? t1 : t2;

    if (t < t_min || t > t_max) return false;

    double y = raio.origem.y + t * raio.direcao.y;

    // Verifique se a interseção ocorre dentro da altura do cone
    if (y < apex.y || y > apex.y + height) return false;

    rec.t = t;
    rec.p = raioAt(raio, t);

    // Calcule a normal
    vetor<double> normal = {
        rec.p.x - apex.x,
        -tanThetaSquared * (rec.p.y - apex.y),
        rec.p.z - apex.z
    };

    rec.normal = vetorUni(normal);
    rec.cor = cor;

    return true;
}





// Definição da classe cone_list
class cone_list {
public:
    cone_list() {}
    cone_list(cone c) { add(c); }

    void clear() { list.clear(); }
    void add(cone c) { list.push_back(c); }

    bool hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const;

public:
    vector<cone> list;
};

bool cone_list::hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    
    for (const auto& c : list) {
        if (c.hit(r, t_min, closest_so_far, temp_rec)) {
            
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


class cylinder {
public:
    vetor<double> base_center;
    vetor<double> axis;
    double radius;
    double height;
    vetor<double> cor;

    cylinder() {}
    cylinder(const vetor<double>& bc, const vetor<double>& a, double r, double h, const vetor<double>& color) 
        : base_center(bc), axis(vetorUni(a)), radius(r), height(h), cor(color) {}

    bool hit(const raio<double>& raio, double t_min, double t_max, hit_record& rec) const;
};


bool cylinder::hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
    // Extrair a origem e a direção do raio
    vetor<double> pos = r.origem;
    vetor<double> dir = r.direcao;

    // Subtrair o centro da base do cilindro da origem do raio
    vetor<double> oc = pos - base_center;

    // Vetor eixo do cilindro normalizado
    vetor<double> ca = axis;

    // Cálculo dos coeficientes para a equação quadrática da superfície lateral
    double a = dir.x * dir.x + dir.z * dir.z;
    double b = 2.0 * (dir.x * oc.x + dir.z * oc.z);
    double c = oc.x * oc.x + oc.z * oc.z - radius * radius;

    // Calcular o discriminante
    double discriminant = b * b - 4.0 * a * c;

    // Verificar se o discriminante é negativo (sem interseção real)
    if (discriminant >= 0) {
        // Calcular as raízes (t1 e t2) da equação quadrática
        double sqrtd = sqrt(discriminant);
        double t1 = (-b - sqrtd) / (2.0 * a);
        double t2 = (-b + sqrtd) / (2.0 * a);

        // Escolher a menor raiz dentro dos limites [t_min, t_max]
        double t = (t1 < t_min || t1 > t_max) ? t2 : t1;
        if (t >= t_min && t <= t_max) {
            // Calcular a coordenada y do ponto de interseção
            double y = pos.y + t * dir.y;

            // Verificar se o ponto de interseção está dentro dos limites do cilindro
            if (y >= base_center.y && y <= base_center.y + height) {
                // Registrar as informações da interseção com a superfície lateral
                rec.t = t;
                rec.p = raioAt(r, t);
                rec.normal = vetor<double>((rec.p.x - base_center.x) / radius, 0, (rec.p.z - base_center.z) / radius);
                rec.cor = cor;
                return true;
            }
        }
    }

    // Verificar interseções com as tampas (superior e inferior)
    double t_caps[2];
    bool hit_caps[2] = { false, false };

    // Tampa de baixo
   // Utilizando a equação paramétrica do raio para encontrar a interseção com a tampa de baixo
   // pos + t * dir = base_center + t_bottom * axis
   // detectando o plano em que se encontra a tampa de baixo

    double t_bottom = (base_center.y - pos.y) / dir.y;
    if (t_bottom >= t_min && t_bottom <= t_max) {
        vetor<double> p_bottom = raioAt(r, t_bottom);
        vetor<double> d_bottom = p_bottom - base_center;
        if (d_bottom.x * d_bottom.x + d_bottom.z * d_bottom.z <= radius * radius) {
            t_caps[0] = t_bottom;
            hit_caps[0] = true;
        }
    }

    // Tampa de cima
    // Utilizando a equação paramétrica do raio para encontrar a interseção com a tampa de cima
    // pos + t * dir = base_center + height * axis + t_top * axis
    // detectando o plano em que se encontra a tampa de cima
    double t_top = (base_center.y + height - pos.y) / dir.y;
    if (t_top >= t_min && t_top <= t_max) {
        vetor<double> p_top = raioAt(r, t_top);
        vetor<double> d_top = p_top - (base_center + multiplicacaoPorEscalar(axis, height));
        if (d_top.x * d_top.x + d_top.z * d_top.z <= radius * radius) {
            t_caps[1] = t_top;
            hit_caps[1] = true;
        }
    }

    // Escolher a interseção válida mais próxima
    bool hit_any = false;
    if (hit_caps[0] && (!hit_caps[1] || t_caps[0] < t_caps[1])) {
        rec.t = t_caps[0];
        rec.p = raioAt(r, rec.t);
        rec.normal = vetor<double>(0, -1, 0); // Normal para a tampa inferior
        rec.cor = cor;
        hit_any = true;
    } else if (hit_caps[1]) {
        rec.t = t_caps[1];
        rec.p = raioAt(r, rec.t);
        rec.normal = vetor<double>(0, 1, 0); // Normal para a tampa superior
        rec.cor = cor;
        hit_any = true;
    }

    return hit_any;
}

class cylinder_list {
public:
    cylinder_list() {}
    cylinder_list(cylinder c) { add(c); }

    void clear() { list.clear(); }
    void add(cylinder c) { list.push_back(c); }

    bool hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const;

public:
    vector<cylinder> list;
};

bool cylinder_list::hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    
    for (const auto& c : list) {
        if (c.hit(r, t_min, closest_so_far, temp_rec)) {
            
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif