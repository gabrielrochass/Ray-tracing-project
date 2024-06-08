#ifndef MALHA_TRIANGULOS_H
#define MALHA_TRIANGULOS_H

#include "vector.h"
#include "triangulo.h"
#include "raio.h"
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

        bool hit(const raio<double>& r, double t_min, double t_max, hit_record& rec) const {
            hit_record temp_rec;
            bool acertou_algo = false;
            double t_mais_proximo = t_max;
            for (int i = 0; i < num_triangulos; i++){
                if (lista_triangulos[i].hit(lista_vertices[i], lista_normais[i], r, r, t_min, t_mais_proximo, temp_rec)){
                    acertou_algo = true;
                    t_mais_proximo = temp_rec.t;
                    //rec = temp_rec;

                    // Preenche o índice do triângulo atingido no objeto hit_record
                    rec.indice_tri = i;
                    temp_rec.normal = lista_normais[i];
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
