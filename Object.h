#ifndef _OBJECT_H
#define _OBJECT_H

#include "Color.h"
#include "Vect.h"
#include "Ray.h"

// Super class for all scene objects
class Object {
    public:

    Object ();

    // Method functions
    // Basic return value methods
    virtual Color getColor() {
        return Color (0, 0, 0, 0);
    }

    virtual Vect getNormalAt(Vect intersection_position) {
        return Vect (0, 0, 0);
    }

    virtual double findIntersection(Ray ray) {
        return 0;
    }
};

Object::Object () {}

#endif