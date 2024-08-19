#ifndef BSP_H
#define BSP_H


#include <vector>
#include <algorithm>
#include <memory>
#include "vector.h"
#include "sphere_list.h"  
#include "malha_triangulos.h" 
#include "plano.h"  

using namespace std;

struct BSPNode {
    vector<sphere_list> spheres;
    vector<malha> triangles;
    vector<plano> planos;  // Adicionando suporte para planos
    int axis;  // Eixo de divisão
    double median_value;  // Valor do plano de secção
    BSPNode* left;
    BSPNode* right;

    BSPNode() : axis(-1), median_value(0), left(nullptr), right(nullptr) {}
};


double getAxisValue(const vetor<double>& point, int axis) {
    if (axis == 0) return point.x;
    if (axis == 1) return point.y;
    if (axis == 2) return point.z;
    return 0;  // Caso padrão
}

int chooseAxis(const vector<sphere_list>& spheres, const vector<malha>& triangles, const vector<plano>& planos) {
    double x_variance = 0, y_variance = 0, z_variance = 0;
    int i = 0;  
    for (const auto& sphere : spheres) {
        vetor<double> center = sphere.list[i].center;
        x_variance += center.x;
        y_variance += center.y;
        z_variance += center.z;
        i++;
    }
    int j = 0;
    for (const auto& triangle : triangles) {
        vetor<double> center = triangle.lista_triangulos[j].getCentro();
        x_variance += center.x;
        y_variance += center.y;
        z_variance += center.z;
        j++;
    }
    for (const auto& plano : planos) {
        x_variance += plano.normal.x;
        y_variance += plano.normal.y;
        z_variance += plano.normal.z;
    }

    x_variance /= (spheres.size() + triangles.size() + planos.size());
    y_variance /= (spheres.size() + triangles.size() + planos.size());
    z_variance /= (spheres.size() + triangles.size() + planos.size());

    if (x_variance >= y_variance && x_variance >= z_variance) return 0;
    if (y_variance >= x_variance && y_variance >= z_variance) return 1;
    return 2;
}


void insertionSortSpheres(sphere_list spheres, int axis) {
    for (size_t i = 1; i < spheres.list.size(); ++i) {
        sphere key = spheres.list[i];
        size_t j = i;
        
        // Compara usando o eixo específico
        while (j > 0 && getAxisValue(key.center, axis) < getAxisValue(spheres.list[j - 1].center, axis)) {
            spheres.list[j] = spheres.list[j - 1];
            --j;
        }
        spheres.list[j] = key;
    }
};

void insertionSortTriangles(malha triangles, int axis) {
    for (size_t i = 1; i < triangles.lista_triangulos.size(); ++i) {
        triangulo key = triangles.lista_triangulos[i];
        size_t j = i;

        // Compara usando o eixo específico
        while (j > 0 && getAxisValue(key.getCentro(), axis) < getAxisValue(triangles.lista_triangulos[j - 1].getCentro(), axis)) {
            triangles.lista_triangulos[j] = triangles.lista_triangulos[j - 1];
            --j;
        }
        triangles.lista_triangulos[j] = key;
    }
};

void insertionSortPlanes(vector<plano>& planes, int axis) {
    for (size_t i = 1; i < planes.size(); ++i) {
        plano key = planes[i];
        size_t j = i;

        while (j > 0 && getAxisValue(key.normal, axis) < getAxisValue(planes[j - 1].normal, axis)) {
            planes[j] = planes[j - 1];
            --j;
        }
        planes[j] = key;
    }
}


BSPNode* buildBSP(const vector<sphere_list>& spheres, const vector<malha>& triangles, const vector<plano>& planos, int depth = 0, int max_depth = 10) {
    if (spheres.empty() && triangles.empty() && planos.empty()) return nullptr;

    auto node = new BSPNode();
    if (spheres.size() + triangles.size() + planos.size() <= 1 || depth >= max_depth) {
        node->spheres = spheres;
        node->triangles = triangles;
        node->planos = planos;
        return node;
    }

    int axis = chooseAxis(spheres, triangles, planos);
    node->axis = axis;

    // Atualize os comparadores para incluir planos
    auto sphereComparator = [axis](const sphere_list& sphere1, const sphere_list& sphere2) {
        return getAxisValue(sphere1.list[0].center, axis) < getAxisValue(sphere2.list[0].center, axis);
    };

    auto triangleComparator = [axis](const malha& malha1, const malha& malha2) {
        return getAxisValue(malha1.lista_triangulos[0].getCentro(), axis) < getAxisValue(malha2.lista_triangulos[0].getCentro(), axis);
    };

    auto planoComparator = [axis](const plano& plano1, const plano& plano2) {
        return getAxisValue(plano1.normal, axis) < getAxisValue(plano2.normal, axis);
    };

    vector<sphere_list> sortedSpheres = spheres;
    insertionSortSpheres(sortedSpheres[0], axis);

    vector<malha> sortedTriangles = triangles;
    insertionSortTriangles(sortedTriangles[0], axis);

    vector<plano> sortedplanos = planos;
    //insertionSortPlanes(sortedplanos, axis);

    size_t median_index = (sortedSpheres.size() + sortedTriangles.size() ) / 2;
    if (median_index < sortedSpheres.size()) {
        node->median_value = getAxisValue(sortedSpheres[median_index].list[0].center, axis);
    } else if (median_index < sortedSpheres.size() + sortedTriangles.size()) {
        node->median_value = getAxisValue(sortedTriangles[median_index - sortedSpheres.size()].lista_triangulos[0].getCentro(), axis);
     } //else {
    //     node->median_value = getAxisValue(sortedplanos[median_index - sortedSpheres.size() - sortedTriangles.size()].normal, axis);
    // }

    vector<sphere_list> leftSpheres;
    vector<sphere_list> rightSpheres;
    vector<malha> leftTriangles;
    vector<malha> rightTriangles;
    vector<plano> leftplanos;
    vector<plano> rightplanos;

    for (const auto& sphere : sortedSpheres) {
        if (getAxisValue(sphere.list[0].center, axis) < node->median_value) {
            leftSpheres.push_back(sphere);
        } else {
            rightSpheres.push_back(sphere);
        }
    }

    for (const auto& triangle : sortedTriangles) {
        if (getAxisValue(triangle.lista_triangulos[0].getCentro(), axis) < node->median_value) {
            leftTriangles.push_back(triangle);
        } else {
            rightTriangles.push_back(triangle);
        }
    }

    // for (const auto& plano : sortedplanos) {
    //     if (getAxisValue(plano.normal, axis) < node->median_value) {
    //         leftplanos.push_back(plano);
    //     } else {
    //         rightplanos.push_back(plano);
    //     }
    // }

    // Ajuste para evitar lados vazios
    if (leftSpheres.empty() || rightSpheres.empty()) {
        leftSpheres = sortedSpheres;
        //rightSpheres.clear();
    }
    if (leftTriangles.empty() || rightTriangles.empty()) {
        leftTriangles = sortedTriangles;
        //rightTriangles.clear();
    }


    node->left = buildBSP(leftSpheres, leftTriangles, leftplanos, depth + 1, max_depth);
    node->right = buildBSP(rightSpheres, rightTriangles, rightplanos, depth + 1, max_depth);
    
    return node;
}






#endif

