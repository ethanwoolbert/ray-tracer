#ifndef _RAY_H
#define _RAY_H

#include "Vect.h"

class Ray {
    Vect origin;
    Vect direction;
    
    public:

    Ray ();
    Ray (Vect, Vect);

    // Method functions
    // Basic return value methods
    Vect getRayOrigin() {
        return origin;
    }
    Vect getRayDirection() {
        return direction;
    }
};

// Initialization w/no parameters
Ray::Ray () {
    origin = Vect(0, 0, 0);
    direction = Vect(1, 0, 0);
}


// Initialization w/parameters
Ray::Ray (Vect ori, Vect dir) {
    origin = ori;
    direction = dir;
}

#endif