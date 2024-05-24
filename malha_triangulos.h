#ifndef MALHA_TRIANGULOS_H
#define MALHA_TRIANGULOS_H

#include "vector.h"
#include "triangulo.h"
#include <vector>
#include <algorithm>

using namespace std;

class malha{
    public:
        malha() {}
        malha(triangulo t) {add(t);}

        void limpar() {
            lista_vertices.clear();
            lista_triangulos.clear();
            lista_normais.clear();
            num_triangulos = 0;
            num_vertices = 0;
        }
        void add(triangulo t) {
            lista_triangulos.push_back(t);   
            num_triangulos++;
            for(auto v : {t.vertice1, t.vertice2, t.vertice3}){
                if (find(lista_vertices.begin(), lista_vertices.end(), v) == lista_vertices.end()){
                    lista_vertices.push_back(v);
                    num_vertices++;
                }
            }
            lista_normais.push_back(produtoVetorial(t.vetor1,t.vetor2));
        }

        bool hit(const vetor<double>& pontoNoPlano, const vetor<double>& normal, const raio<double>& raio, double t_min, double t_max, hit_record& rec){
            hit_record temp_rec;
            bool acertou_algo = false;
            auto mais_perto = t_max;

            for (const auto& l : lista_triangulos) {
                if (l.hit(pontoNoPlano, normal, raio, raio, t_min, t_max, rec)) {
                    acertou_algo = true;
                    mais_perto = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return acertou_algo;
        }
    public:
        vector<vetor<double>> lista_vertices;
        vector<triangulo> lista_triangulos;
        vector<vetor<double>> lista_normais;
        vector<vetor<double>> lista_normais_vertices;
        int num_triangulos;
        int num_vertices;

};

#endif
