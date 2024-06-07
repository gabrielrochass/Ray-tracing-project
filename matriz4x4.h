#include <iostream>
#include <cmath>
#include "vector.h" 
#include "triangulo.h"

#ifndef MATRIX_H
#define MATRIX_H


class matriz4x4 {
public:
    double m[4][4];

    matriz4x4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == j) m[i][j] = 1;
                else m[i][j] = 0;
            }
        }
    }

    
    matriz4x4(double values[4][4]) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = values[i][j];
            }
        }
    }

    // Operador de acesso aos elementos da matriz
    const double* operator[](int index) const {
        return m[index];
    }

    double* operator[](int index) {
        return m[index];
    }

    // Multiplicação de matriz por vetor
    vetor<double> multMatrizVetor (const vetor<double>& v) const  {
        double x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        double y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        double z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        double w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];
        return vetor<double>(x / w, y / w, z / w);
    }


    // Função para criar uma matriz de translação
    static matriz4x4 createTranslation(double tx, double ty, double tz) {
        matriz4x4 result;
        result[0][3] = tx;
        result[1][3] = ty;
        result[2][3] = tz;
        result[3][3] = 1; 
        result[2][2] = 1;
        result[1][1] = 1;
        result[0][0] = 1;
    
        return result;
    }

    // Multiplicação de matriz por outra matriz
    matriz4x4 operator *(const matriz4x4& outra) const {
        matriz4x4 resultado;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                resultado[i][j] = 0;
                for (int k = 0; k < 4; ++k) {
                    resultado[i][j] += m[i][k] * outra[k][j];
                }
            }
        }
        return resultado;
    }

    // Função para criar uma matriz de escala
    static matriz4x4 createScale(double sx, double sy, double sz) {
        matriz4x4 result;
        result[0][0] = sx;
        result[1][1] = sy;
        result[2][2] = sz;
        return result;
    }

    // Função para criar uma matriz de rotação em torno do eixo Z horario
    static matriz4x4 createRotationZ(double angle) {
        matriz4x4 result;
        double cosA = std::cos(angle);
        double sinA = std::sin(angle);
        result[0][0] = cosA;
        result[0][1] = -sinA;
        result[1][0] = sinA;
        result[1][1] = cosA;
        result[2][2] = 1; 
        result[3][3] = 1;
        return result;
    }

     // Função para criar uma matriz de rotação em torno do eixo X
    static matriz4x4 createRotationX(double angle) {
        matriz4x4 result;
        double cosA = std::cos(angle);
        double sinA = std::sin(angle);
        result[0][0] = 1;
        result[1][1] = cosA;
        result[1][2] = -sinA;
        result[2][1] = sinA;
        result[2][2] = cosA;
        result[3][3] = 1;
        return result;
    }

     // Função para criar uma matriz de rotação em torno do eixo Y
    static matriz4x4 createRotationY(double angle) {
        matriz4x4 result;
        double cosA = std::cos(angle);
        double sinA = std::sin(angle);
        result[0][0] = cosA;
        result[0][2] = -sinA;
        result[1][1] = 1;
        result[2][0] = sinA;
        result[2][2] = cosA;
        result[3][3] = 1;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const matriz4x4& maz) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                os << maz.m[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    
};

#endif 

/*
int main() {
    // Exemplo de vetor
   vetor<double> v(1, 2, 3);
   std::cout << "Vector: " << v << std::endl;

    // Exemplo de matriz identidade
    matriz4x4 mat1;
    cout << "Matrix 1 (Identity):\n" << mat1 << endl;

    // Exemplo de matriz de translação
    matriz4x4 translation = matriz4x4::createTranslation(3, 4, 5);
    cout << "Translation Matrix:\n" << translation << endl;

    // Exemplo de matriz de rotação
    double angle = M_PI / 4; // 45 graus
    matriz4x4 rotation = matriz4x4::createRotationZ(angle);
    cout << "Rotation Matrix (45 degrees around Z):\n" << rotation << endl;

    // Multiplicação de matriz por vetor
    vetor<double> transformedVector = translation.multMatrizVetor(v);
    cout << "Transformed Vector by Translation: " << transformedVector << endl;

    // Multiplicação de duas matrizes
    matriz4x4 combined = translation * rotation;
    cout << "Combined Translation and Rotation Matrix:\n" << combined << endl;

    // Multiplicação da matriz combinada por vetor
    vetor<double> finalVector =  combined.multMatrizVetor(finalVector)
    cout << "Final Transformed Vector: " << finalVector << endl;

    return 0;
}
*/