// utiliza uv mapping para texturizar um objeto
// mapeamento: converter ponto de interseção de coordenadas esféricas para coordenadas uv
// cálculo da cor do pixel com base no arquivo uploadado

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include "vector.h"

using namespace std;

// define struct textura
struct Textura {
    vector<vector<vetor<double>>> textura;
    int largura, altura;

    Textura(const string& arquivo);
    vetor<double> corTextura(double u, double v) const;
};

// construtor da struct textura
Textura::Textura(const string& arquivo) {
    ifstream arquivoTextura(arquivo, ios::binary);

    // verifica se o arquivo foi aberto
    if (arquivoTextura.is_open()) {
        arquivoTextura.seekg(18, ios::beg);
        arquivoTextura.read((char*)&largura, sizeof(largura));
        arquivoTextura.read((char*)&altura, sizeof(altura));
        arquivoTextura.seekg(54, ios::beg);

        textura.resize(altura);
        for (int i = 0; i < altura; i++) {
            textura[i].resize(largura);
            for (int j = 0; j < largura; j++) {
                uint8_t cor[3];
                arquivoTextura.read((char*)&cor, sizeof(cor));
                textura[i][j] = vetor<double>(cor[2] / 255.0, cor[1] / 255.0, cor[0] / 255.0);
            }
        }
        arquivoTextura.close();
    } else {
        throw runtime_error("Erro ao abrir o arquivo de textura");
    }
}

// mapeamento de coordenadas esféricas para coordenadas uv
vetor<double> Textura::corTextura(double u, double v) const {
    u = fmod(u, 1.0);
    v = fmod(v, 1.0);

    int i = (1 - v) * altura;
    int j = u * largura;

    return textura[i][j];
}




