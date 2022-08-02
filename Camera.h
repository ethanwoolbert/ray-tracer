#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"

class Camera {
    Vect campos;
    Vect camdir;
    Vect camright;
    Vect camdown;
    
    public:

    Camera ();
    Camera (Vect, Vect, Vect, Vect);

    // Method function
    // Get basic values of camera
    Vect getCameraPosition() {
        return campos;
    }
    Vect getCameraDirection() {
        return camdir;
    }
    Vect getCameraRight() {
        return camright;
    }
    Vect getCameraDown() {
        return camdown;
    }
};

// Initialization w/ no parameters
Camera::Camera () {
    campos = Vect(0, 0, 0);
    camdir = Vect(0, 0, 1);
    camright = Vect(0, 0, 0);
    camdown = Vect(0, 0, 0);
}

// Initialization w/parameters
Camera::Camera (Vect position, Vect direction, Vect right, Vect down) {
    campos = position;
    camdir = direction;
    camright = right;
    camdown = down;
}

#endif