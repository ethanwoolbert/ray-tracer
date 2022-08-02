#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
    double x, y, z;
    
    public:

    Vect ();
    Vect (double, double, double);

    // Method functions
    // Basic get value methods
    double getVectX() {
        return x;
    }
    double getVectY() {
        return y;
    }
    double getVectZ() {
        return z;
    }

    // Vector operation
    Vect vectAdd (Vect vector) {
        return Vect (x + vector.getVectX(), y + vector.getVectY(), z + vector.getVectZ());
    }

    Vect vectMult (double scalar) {
        return Vect (x * scalar, y * scalar, z * scalar);
    }

    Vect negative () {
        return Vect(-x, -y, -z);
    }

    double magnitude () {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    Vect normalize () {
        double magnitude = sqrt((x * x) + (y * y) + (z * z));
        return Vect(x / magnitude, y / magnitude, z / magnitude);
    }

    double dotProduct (Vect vector) {
        return ((x * vector.getVectX()) + (y * vector.getVectY()) + (z * vector.getVectZ()));
    }

    Vect crossProduct (Vect vector) {
        return Vect (
            (y * vector.getVectZ() - z * vector.getVectY()),
            (z * vector.getVectX() - x * vector.getVectZ()),
            (x * vector.getVectY() - y * vector.getVectX()));
    }
};

// Initialization w/no parameters
Vect::Vect () {
    x = 0;
    y = 0;
    z = 0;
}

// Initialization w/parameters
Vect::Vect (double xp, double yp, double zp) {
    x = xp;
    y = yp;
    z = zp;
}

#endif