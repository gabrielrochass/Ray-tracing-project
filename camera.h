#include <iostream>
#include <cmath>
#include "ponto.h"
#include "vector.h"

using namespace std;
#ifndef CAMERA_H
#define CAMERA_H

template<typename T>
// Define a classe Camera
class Camera {
public:
    vetor<T> horizontal, vertical, lowerleftcorner, origem;
    const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));
public:
    Camera(double vfov) {
        double theta = vfov * M_PI / 180.0;
        double h = tan(theta / 2.0);
        double aspectratio = 16.0 / 9.0;
        double viewportheight = 2.0 ;
        double viewportwidth = viewportheight * aspectratio;
        vetor<double> horizontal {viewportwidth, 0.0, 0.0};
        vetor<double> vertical {0.0, viewportheight, 0.0};
        vetor<double> origem {0.0, 0.0, 0.0};
        vetor<double> h2 = mult(0.5, horizontal);
        vetor<double> h2o = subtracao(origem, h2);
        vetor<double> v2 = mult(0.5, vertical);
        double lente = 1.0;
        vetor<double> v2f = subtracao(v2, {0.0, 0.0, lente});
        vetor<double> lowerLeftCorner = subtracao(h2o, v2f);
        // # lowerleftcorner = origin - horizontal/2 - vertical/2 - Vec3(0.0, 0.0, focallenght)
            
    }
    Camera<T> cam, double s, double t
    vetor<T> tv = mult(t, cam.vertical);
        double u = double(i) / (imWidth - 1);
        double v = 1.0 - double(j) / (imHeight - 1);
        vetor vv = mult(v, vertical);


        vetor vvo = subtracao(vv, origem);
        vetor uh = mult(u, horizontal);
        vetor loweruh = soma(uh, lowerLeftCorner);
        vetor dir = soma(loweruh, vvo);
        
        return raio<T>(cam.origem, dir);
    
};

#endif // CAMERA_H
