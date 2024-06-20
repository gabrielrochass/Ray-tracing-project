// hit_record.h
#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vector.h"

struct hit_record {
    vetor<double> p;
    vetor<double> normal;
    double t;
    double indice_tri;
    
    
};

#endif // HIT_RECORD_H