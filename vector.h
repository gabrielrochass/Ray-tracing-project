#include <iostream>
#include <cmath>

#ifndef VECTOR_H
#define VECTOR_H

using namespace std;

// Criando estrutura vetor
template<typename T> // carregar mais de um tipo de dados
struct vetor {
    T x, y, z;
    vetor(T xRecebido, T yRecebido, T zRecebido) : x(xRecebido), y(yRecebido), z(zRecebido) {} // Construtor do vetor
    vetor() : x(0), y(0), z(0) {} // Construtor padrão (inicializa com zero)
    bool operator==(const vetor& other) const {
        return x == other.x && y == other.y && z == other.z;
    } //Resolvendo erro de compilação.
};

// Função soma
template<typename T>
vetor<T> soma(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
};

// Função subtração
template<typename T>
vetor<T> subtracao(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
};

// Função norma
template<typename T>
T norma(const vetor<T>& vector) {
    return sqrt(pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2));
};

// Função do Produto Escalar
template<typename T>
T produtoEscalar(const vetor<T>& v1, const vetor<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}; // retorna double 

// Função Vetor unitário
template<typename T>
vetor<T> vetorUni(const vetor<T>& v) {
    T norm = norma(v);
    return  vetor<T> (v.x / norm, v.y / norm, v.z / norm);;
};

// Função Produto Vetorial
template<typename T>
vetor<T> produtoVetorial(const vetor<T>& v1, const vetor<T>& v2) {
    return vetor<T>(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x);
}

// Função multiplicação por escalar
template<typename T>
vetor<T> multiplicacaoPorEscalar(const vetor<T>& v, T escalar) {
    return vetor<T>(v.x * escalar, v.y * escalar, v.z * escalar);
}

template<typename T>
bool operator==(const vetor<T>& v1, const vetor<T>& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}



template<typename T> 
vetor<T> mult(T scalar, vetor<T> h) {
    return vetor<T>(h.x * scalar, h.y * scalar, h.z * scalar);
}

template<typename T> 
vetor<T> subtracaoVP(T scalar, vetor<T> h) { // subtrai vetor por escalar
    return vetor<T>(h.x - scalar, h.y - scalar, h.z - scalar);
}


/*template<typename T>
struct raio { 
    vetor<T> origem;
    vetor<T> direcao;

    raio(vetor<T> comeco, vetor<T> direc) {
        this->origem = comeco;
        this->direcao = vetorUni(direc);
    }
};

template<typename T>
vetor<T> raioAt(raio<T> r, T t) { // calcula o ponto no espaço que o raio atinge 
    
    return soma(r.origem, mult(t, r.direcao));
}*/



#endif // VECTOR_H

/*int main2() {
    // Testando operações com vetores
    vetor <double> v1{1.0, 0.0, 0.0};
    vetor <double> v2{0.0, 1.0, 0.0};

    double normV1 = norma(v1);
    double normV2 = norma(v2);

    double dotProduct = produtoEscalar(v1, v2);

    vetor <double> unitV1 = vetorUni(v1);


   cout << "Norma de v1: " << normV1 <<endl;
   cout << "Norma de v2: " << normV2 <<endl;
   cout << "Produto escalar de v1 e v2: " << dotProduct <<endl;
   cout << "Vetor unitário de v1: (" << unitV1.x << ", " << unitV1.y << ", " << unitV1.z << ")" <<endl;

    


    return 0;
}*/
