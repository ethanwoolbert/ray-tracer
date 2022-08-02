#ifndef _SOURCE_H
#define _SOURCE_H

#include "Vect.h"
#include "Color.h"

// Super class for light
class Source {
    public:

    Source();

    // Basic return value methods
    virtual Vect getLightPosition() {
        return Vect(0, 0, 0);
    }
    virtual Color getLightColor() {
        return Color(1, 1, 1, 0);
    }
};

Source::Source() {}

#endif