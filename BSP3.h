#ifndef BSP_H
#define BSP_H

#include <vector>
#include "sphere.h"

#include "plano.h"
#include "sphere_list.h"
#include "malha_triangulos.h"
#include "triangulo.h"

using namespace std;

struct BSPNode {
    vector<sphere> spheres;
    vector<triangulo> triangles;
    vector<plano> planos;
    BSPNode* left;
    BSPNode* right;
    int axis; // 0 = x, 1 = y, 2 = z
    double median_value;

    BSPNode() : left(nullptr), right(nullptr), axis(-1), median_value(0) {}
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


BSPNode* buildBSP(vector<sphere_list>& esferas, vector<malha>& triangulos, vector<plano>& planos, int depth = 0) {
    if (esferas.empty() && triangulos.empty() && planos.empty()) {
        return nullptr;
    }

    BSPNode* node = new BSPNode();
    int axis = depth % 3; // Alterna entre x, y e z
    node->axis = axis;

    vector<double> values;
    sphere_list listinha =  esferas[0];
    for (const auto& esferasList : listinha.list) {
        
        values.push_back(getAxisValue(esferasList.center, axis));
        
    }

    malha lista_tri = triangulos[0];
    for (const auto& triangulo : lista_tri.lista_triangulos) {
       
        values.push_back(getAxisValue(triangulo.vertice1, axis));
        values.push_back(getAxisValue(triangulo.vertice2, axis));
        values.push_back(getAxisValue(triangulo.vertice3, axis));
        
    }

    for (const auto& plano : planos) {
        values.push_back(getAxisValue(plano.pontoNoPlano, axis));
    }

    if (!values.empty()) {
        sort(values.begin(), values.end());
        node->median_value = values[values.size() / 2];
    } else {
        node->median_value = 0;
    }

    vector<sphere_list> leftEsferas, rightEsferas;
    vector<malha> leftTriangles, rightTriangles;
    vector<plano> leftPlanos, rightPlanos;

    
   for (auto& esferasList : listinha.list){
        vector<sphere> leftList, rightList;
        
        if (getAxisValue(esferasList.center, axis) < node->median_value) {
            leftList.push_back(esferasList);
        } else {
            rightList.push_back(esferasList);
        }
        
   }
    //Faça um loop semelhante agora para triangulos
    for (auto& triangulo_elementos : lista_tri.lista_triangulos){
        vector<triangulo> leftList, rightList;
        if (getAxisValue(triangulo_elementos.vertice1, axis) < node->median_value) {
            leftList.push_back(triangulo_elementos);
        } else {
            rightList.push_back(triangulo_elementos);
        }
        
    }
    

    for (auto& plano : planos) {
        if (getAxisValue(plano.pontoNoPlano, axis) < node->median_value) {
            leftPlanos.push_back(plano);
        } else {
            rightPlanos.push_back(plano);
        }
    }
    // Ajuste para evitar lados vazios
    if (leftEsferas.empty() || rightEsferas.empty()) {
        leftEsferas = esferas;
        //rightEsferas.clear();
    }
    if (leftTriangles.empty() || rightTriangles.empty()) {
        leftTriangles = triangulos;
        //rightTriangles.clear();
    }
    if (leftPlanos.empty() || rightPlanos.empty()) {
        leftPlanos = planos;
        //rightPlanos.clear();
    }
    
    //Atualizar apropiadamente os ponteiros para esferas, triangulos e planos
    node->spheres = esferas[0].list;
    node->triangles = triangulos[0].lista_triangulos;
    node->planos = planos;

    node->left = buildBSP(leftEsferas, leftTriangles, leftPlanos, depth + 1);
    node->right = buildBSP(rightEsferas, rightTriangles, rightPlanos, depth + 1);

    return node;
}

#endif
