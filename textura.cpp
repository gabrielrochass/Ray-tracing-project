#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <cmath>
#include "vector.h"

using namespace std;

struct Textura {
    vector<vector<vetor<double>>> textura;
    int largura, altura;

    Textura() : largura(0), altura(0) {}

    Textura(const string& arquivo) {
        ifstream arquivoTextura(arquivo, ios::binary);

        // verifica se o arquivo foi aberto
        if (arquivoTextura.is_open()) {
            arquivoTextura.seekg(18, ios::beg);
            arquivoTextura.read(reinterpret_cast<char*>(&largura), sizeof(largura));
            arquivoTextura.read(reinterpret_cast<char*>(&altura), sizeof(altura));
            arquivoTextura.seekg(54, ios::beg);

            textura.resize(altura);
            for (int i = 0; i < altura; i++) {
                textura[i].resize(largura);
                for (int j = 0; j < largura; j++) {
                    uint8_t cor[3];
                    arquivoTextura.read(reinterpret_cast<char*>(cor), sizeof(cor));
                    textura[i][j] = vetor<double>(cor[2] / 255.0, cor[1] / 255.0, cor[0] / 255.0);
                }
            }
            arquivoTextura.close();
        } else {
            throw runtime_error("Erro ao abrir o arquivo de textura");
        }
    }

    vetor<double> corTextura(double u, double v) const {
        u = fmod(u, 1.0);
        v = fmod(v, 1.0);

        int i = static_cast<int>((1 - v) * altura);
        int j = static_cast<int>(u * largura);

        return textura[i][j];
    }
};
