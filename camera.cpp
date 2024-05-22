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
    /*const int imWidth = 800;
    const int imHeight = static_cast<int>(imWidth / (16.0 / 9.0));*/
public:
    Camera(double vfov) {
        double theta = vfov * M_PI / 180.0;
        double h = tan(theta / 2.0);
        double aspectratio = 16.0 / 9.0;
        vetor origem{0.0, 0.0, 0.0};
        double viewportHeight = 2.0;
        double viewportWidth = viewportHeight * 16.0 / 9.0;
        vetor horizontal{viewportWidth, 0.0, 0.0};
        vetor vertical{0.0, viewportHeight, 0.0};
        vetor h2 = mult(0.5, horizontal);
        vetor h2o = subtracao(origem, h2);
        vetor v2 = mult(0.5, vertical);
        vetor lente{0.0, 0.0, 1.0};
        vetor v2f = subtracao(v2, lente);
        vetor lowerLeftCorner = subtracao(h2o, v2f);
        // # lowerleftcorner = origin - horizontal/2 - vertical/2 - Vec3(0.0, 0.0, focallenght)
            
    }
    
        
        
    
};

#endif // CAMERA_H