#ifndef BSP_H
#define BSP_H

#include <vector>
#include <algorithm>
#include <memory>
#include "vector.h"
#include "sphere_list.h"  
#include "malha_triangulos.h" 

using namespace std;

struct BSPNode {

    vector<sphere_list> spheres;
    vector<malha> triangles;
    int axis;  // Eixo de divisão
    double median_value;  // Valor do plano de corte
    BSPNode* left;
    BSPNode* right;

    //indicado em -1 para determinar indefinição
    BSPNode() : axis(-1), median_value(0), left(nullptr), right(nullptr) {}
};





double getAxisValue(const vetor<double>& point, int axis) { // retornar o valor do ponto no eixo
    if (axis == 0) return point.x;
    if (axis == 1) return point.y;
    if (axis == 2) return point.z;
    return 0;  
}

int chooseAxis(const vector<sphere_list>& spheres, const vector<malha>& triangles) { 
    double x_variance = 0, y_variance = 0, z_variance = 0;
    int i = 0;  // escolher o eixo com maior variância
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

    x_variance /= (spheres.size() + triangles.size());
    y_variance /= (spheres.size() + triangles.size());
    z_variance /= (spheres.size() + triangles.size());

    if (x_variance >= y_variance && x_variance >= z_variance) return 0;
    if (y_variance >= x_variance && y_variance >= z_variance) return 1;
    return 2;
}

//Como funciona a função buildBSP
//A função buildBSP é responsável por construir a BSP (Binary Space Partitioning) a partir de uma lista dos objetos da cena.
//A BSP é uma árvore binária que divide o espaço em partições, de forma que cada nó da árvore representa um plano de corte que divide o espaço em dois subespaços.
//recebe como parâmetro uma lista dos objetos, e retorna um ponteiro para o nó raiz da BSP.
//A função buildBSP é recursiva, e a cada chamada ela escolhe um eixo de divisão (x, y ou z) baseado na variância dos centros dos objetos da cena.
//Em seguida, ela ordena as esferas e triângulos de acordo com o eixo de divisão escolhido, e calcula a mediana dos valores dos centros dos objetos.
//A função então divide a lista em duas partes, uma contendo os objetos cujo centro é menor ou igual à mediana, e outra contendo os objetos cujo centro é maior que a mediana.
//termina quando a lista é vazia, ou quando o número de objetos é menor ou igual a 1, ou quando a profundidade máxima da BSP é atingida.

BSPNode* buildBSP(const vector<sphere_list>& spheres, const vector<malha>& triangles, int depth = 0, int max_depth = 10) {
    if (spheres.empty() && triangles.empty()) return nullptr;

    auto node = new BSPNode();
    if (spheres.size() + triangles.size() <= 1 || depth >= max_depth) {
        node->spheres = spheres;
        node->triangles = triangles;
        return node;
    }

    int axis = chooseAxis(spheres, triangles);
    node->axis = axis;

    // Comparator para esferas
    auto sphereComparator = [axis](const sphere_list& sphere1, const sphere_list& sphere2) {
        return getAxisValue(sphere1.list[0].center, axis) < getAxisValue(sphere2.list[0].center, axis);

    };

    // Comparator para malhas
    auto triangleComparator = [axis](const malha& malha1, const malha& malha2) {
        return getAxisValue(malha1.lista_triangulos[0].getCentro(), axis) < getAxisValue(malha2.lista_triangulos[0].getCentro(), axis);
    };
    
   

    // Ordenar as esferas
    vector<sphere_list> sortedSpheres = spheres;
    sort(sortedSpheres.begin(), sortedSpheres.end(), sphereComparator);

    // Ordenar as malhas
    vector<malha> sortedTriangles = triangles;
    std::sort(sortedTriangles.begin(), sortedTriangles.end(), triangleComparator);
    
    size_t median_index = (sortedSpheres.size() + sortedTriangles.size()) / 2;
    if (median_index < sortedSpheres.size()) {
        node->median_value = getAxisValue(sortedSpheres[0].list[median_index].center, axis);
    } else {
        node->median_value = getAxisValue(sortedTriangles[0].lista_triangulos[median_index - sortedSpheres.size()].getCentro(), axis);
    }

    vector<sphere_list> leftSpheres(sortedSpheres.begin(), sortedSpheres.begin() + median_index);
    vector<sphere_list> rightSpheres(sortedSpheres.begin() + median_index, sortedSpheres.end());

    vector<malha> leftTriangles(sortedTriangles.begin(), sortedTriangles.begin() + median_index - leftSpheres.size());
    vector<malha> rightTriangles(sortedTriangles.begin() + median_index - leftSpheres.size(), sortedTriangles.end());

    // Ajuste para evitar lados vazios
    if (leftSpheres.empty() || rightSpheres.empty()) {
        leftSpheres = sortedSpheres;
        //rightSpheres.clear();
    }
    if (leftTriangles.empty() || rightTriangles.empty()) {
        leftTriangles = sortedTriangles;
        //rightTriangles.clear();
    }

    node->left = buildBSP(leftSpheres, leftTriangles, depth + 1, max_depth);
    node->right = buildBSP(rightSpheres, rightTriangles, depth + 1, max_depth);

    return node;
}




#endif