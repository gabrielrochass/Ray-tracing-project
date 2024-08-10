#include <cmath>
#include <vector>
#include "malha_triangulos.h"
#include "vector.h"
#include "triangulo.h"
#include "matriz4x4.h"

const double PI = 3.14159265358979323846;

double angulo = PI / 8; 

matriz4x4 rotacaoX = matriz4x4::createRotationX(angulo,false);

malha gerarToro(malha& m, double R, double r, int num_theta, int num_phi) {
    // std::vector<vetor<double>> vertices;
    std::vector<triangulo> triangulos;
    std::vector<vector<vetor<double>>> vertices(num_phi);

    double dtheta = 2 * PI / num_theta;
    double dphi = 2 * PI / num_phi;

    // Gerar vértices
    for (int i = 0; i < num_theta; ++i) {
        double theta = i * dtheta;
        for (int j = 0; j < num_phi; ++j) {
            double phi = j * dphi;

            double x = (R + r * cos(phi)) * cos(theta);
            double y = r * sin(phi);
            double z = (R + r * cos(phi)) * sin(theta);

            vertices[i].push_back(vetor<double>(x, y, z));
            //vertices.push_back(rotacaoX.multMatrizVetor(vetor<double>(x, y, z)));
        }
    }

    // Gerar triângulos
    for (int i = 0; i < num_theta; i++) {
        for (int j = 0; j < num_phi; j++) {

            vetor<double> p1 =  vertices[i][j];
            vetor<double> p2 = vertices[(i+1) % num_theta][j];
            vetor<double> p3 = vertices[i][(j+1) % num_phi];
            vetor<double> p4 = vertices[(i+1) % num_theta][(j+1) % num_phi];

            // Adicionar dois triângulos para cada quadrado
            triangulos.push_back(triangulo(p1,p2,p3));
            triangulos.push_back(triangulo(p2,p3,p4));
        }
    }

    // Adicionar triângulos à malha
    for (const auto& t : triangulos) {
        m.add(t);
    }

    return m;
}
