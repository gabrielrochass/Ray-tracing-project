#ifndef PONTO_H
#define PONTO_H

#include <cmath>

// Define a estrutura Ponto
template<typename T>
struct Ponto {
    T x, y, z;
    Ponto(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {} // Construtor -> inicializa os valores de x, y e z
};

// Define operações com pontos
template<typename T>
Ponto<T> somaPonto(const Ponto<T>& p1, const Ponto<T>& p2) {
    return Ponto<T>(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

template<typename T>
Ponto<T> subtracaoPonto(const Ponto<T>& p1, const Ponto<T>& p2) {
    return Ponto<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

template<typename T>
Ponto<T> multiplicacaoPorEscalarPonto(const Ponto<T>& p, double escalar) {
    return Ponto<T>(p.x * escalar, p.y * escalar, p.z * escalar);
}

template<typename T>
T produtoEscalarPonto(const Ponto<T>& p1, const Ponto<T>& p2) {
    return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

template<typename T>
Ponto<T> produtoVetorialPonto(const Ponto<T>& p1, const Ponto<T>& p2) {
    return Ponto<T>(p1.y * p2.z - p1.z * p2.y,
                    p1.z * p2.x - p1.x * p2.z,
                    p1.x * p2.y - p1.y * p2.x);
}

#endif // PONTO_H
