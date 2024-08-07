#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include "raio.h"
#include "sphere.h"

// Define a estrutura do BoundingBox
struct BoundingBox {
    vetor<double> min, max;
    BoundingBox() {}
    BoundingBox(const vetor<double>& a, const vetor<double>& b) : min(a), max(b) {}
};

// Define a estrutura do OctreeNode
struct OctreeNode {
    BoundingBox box;
    std::vector<std::shared_ptr<OctreeNode>> filhos;
    std::vector<std::shared_ptr<sphere>> esferas;

    OctreeNode(const BoundingBox& box_) : box(box_) {}

    bool ehFolha() const {
        return filhos.empty();
    }

    void subdivide();
    void inserirEsfera(std::shared_ptr<sphere> esfera);
    bool intersectou(const raio<double>& r, double t_min, double t_max, hit_record& rec) const;
    bool intersectouBox(const BoundingBox& box, const raio<double>& r, double t_min, double t_max) const;
};

void OctreeNode::subdivide() {
    vetor<double> mid = multiplicacaoPorEscalar(soma(box.min, box.max), 0.5);
    vetor<double> min = box.min;
    vetor<double> max = box.max;

    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(min, mid)));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(mid.x, min.y, min.z), vetor<double>(max.x, mid.y, mid.z))));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(mid.x, mid.y, min.z), vetor<double>(max.x, max.y, mid.z))));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(min.x, mid.y, min.z), vetor<double>(mid.x, max.y, mid.z))));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(min.x, min.y, mid.z), vetor<double>(mid.x, mid.y, max.z))));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(mid.x, min.y, mid.z), vetor<double>(max.x, mid.y, max.z))));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(mid, max)));
    filhos.push_back(std::make_shared<OctreeNode>(BoundingBox(vetor<double>(min.x, mid.y, mid.z), vetor<double>(mid.x, max.y, max.z))));

    for (auto& esfera : esferas) {
        for (auto& filho : filhos) {
            if (filho->box.min.x <= esfera->center.x && esfera->center.x <= filho->box.max.x &&
                filho->box.min.y <= esfera->center.y && esfera->center.y <= filho->box.max.y &&
                filho->box.min.z <= esfera->center.z && esfera->center.z <= filho->box.max.z) {
                filho->inserirEsfera(esfera);
            }
        }
    }

    esferas.clear();
}

void OctreeNode::inserirEsfera(std::shared_ptr<sphere> esfera) {
    if (filhos.empty()) {
        esferas.push_back(esfera);
        if (esferas.size() > 8) {
            subdivide();
        }
    } else {
        for (auto& filho : filhos) {
            if (filho->box.min.x <= esfera->center.x && esfera->center.x <= filho->box.max.x &&
                filho->box.min.y <= esfera->center.y && esfera->center.y <= filho->box.max.y &&
                filho->box.min.z <= esfera->center.z && esfera->center.z <= filho->box.max.z) {
                filho->inserirEsfera(esfera);
            }
        }
    }
}

bool OctreeNode::intersectou(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
    if (!intersectouBox(box, r, t_min, t_max)) return false;

    bool acertouAlguem = false;
    double maisPerto = t_max;

    if (ehFolha()) {
        for (auto& esfera : esferas) {
            hit_record temp_rec;
            if (esfera->hit(r, t_min, maisPerto, temp_rec)) {
                acertouAlguem = true;
                maisPerto = temp_rec.t;
                rec = temp_rec;
            }
        }
    } else {
        for (auto& filho : filhos) {
            hit_record temp_rec;
            if (filho->intersectou(r, t_min, maisPerto, temp_rec)) {
                acertouAlguem = true;
                maisPerto = temp_rec.t;
                rec = temp_rec;
            }
        }
    }

    return acertouAlguem;
}

bool OctreeNode::intersectouBox(const BoundingBox& box, const raio<double>& r, double t_min, double t_max) const {
    vetor<double> invD = {1.0 / r.direcao.x, 1.0 / r.direcao.y, 1.0 / r.direcao.z};
    vetor<double> t0 = produtoVetorial(subtracao(box.min, r.origem), invD);
    vetor<double> t1 = produtoVetorial(subtracao(box.max, r.origem), invD);
    vetor<double> tmin = {std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z)};
    vetor<double> tmax = {std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z)};
    double tminMax = std::max(tmin.x, std::max(tmin.y, tmin.z));
    double tmaxMin = std::min(tmax.x, std::min(tmax.y, tmax.z));
    return tminMax <= tmaxMin;
}

#endif // OCTREE_H
