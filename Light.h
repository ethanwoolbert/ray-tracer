#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "Color.h"
#include "Vect.h"

class Light : public Source{
    Vect position;
    Color color;
    
    public:

    Light ();
    Light (Vect, Color);

    // Method functions
    // Basic get value methods
    virtual Vect getLightPosition() {
        return position;
    }
    virtual Color getLightColor() {
        return color;
    }
};

Light::Light () {
    position = Vect(0, 0, 0);
    color = Color(1, 1, 1, 0);
}

Light::Light (Vect pos, Color col) {
    position = pos;
    color = col;
}

#endif