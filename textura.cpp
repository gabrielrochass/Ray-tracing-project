#include "textura.h"
#include <iostream> 
#include <fstream> // abre e lê arquivos
#include <stdexcept> // lançamento de exceções
#include <cmath> 

Textura::Textura() : largura(0), altura(0) {}

// carrega a textura de um arquivo
Textura::Textura(const string& arquivo) {
    ifstream arquivoTextura(arquivo, ios::binary); // abre o arquivo em modo binário
    if (arquivoTextura.is_open()) {
        arquivoTextura.seekg(18, ios::beg); // pula o cabeçalho do arquivo bmp de 18 bytes para chegar na largura e altura (imagem)
        arquivoTextura.read((char*)&largura, sizeof(largura)); // lê a largura da imagem e armazena em largura
        arquivoTextura.read((char*)&altura, sizeof(altura));
        arquivoTextura.seekg(54, ios::beg); // pula o cabeçalho do arquivo bmp de 54 bytes para chegar nos dados da imagem (paleta de cores)

        textura.resize(altura);

        // lê a paleta de cores da imagem e armazena em textura (matriz de cores)
        for (int i = 0; i < altura; i++) {
            textura[i].resize(largura);
            for (int j = 0; j < largura; j++) {
                uint8_t cor[3];
                arquivoTextura.read((char*)&cor, sizeof(cor));
                textura[i][j] = vetor<double>(cor[2] / 255.0, cor[1] / 255.0, cor[0] / 255.0);
            }
        }
        arquivoTextura.close();
    } else { // se não conseguir abrir o arquivo, lança uma exceção
        throw runtime_error("Erro ao abrir o arquivo de textura");
    }
}

// retorna a cor da textura em um ponto (u, v)
vetor<double> Textura::corTextura(double u, double v) const {
    // garante que u e v estão no intervalo [0, 1) calculando o resto da divisão por 1
    u = fmod(u, 1.0);
    v = fmod(v, 1.0);

    // converte u e v para coordenadas da textura
    int i = (1 - v) * altura;
    int j = u * largura;

    return textura[i][j]; // retorna a cor da textura no ponto (u, v)
}
