#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {
    Vect normal;
    double distance;
    Color color;
    
    public:

    Plane ();
    Plane (Vect, double, Color);

    // Method functions
    // Basic return value functions
    Vect getPlaneNormal() {
        return normal;
    }
    double getPlaneDistance() {
        return distance;
    }
    virtual Color getColor() {
        return color;
    }

    // Get normal of plane
    virtual Vect getNormalAt(Vect point) {
        return normal;
    }

    // Find intersection with plane
    virtual double findIntersection(Ray ray) {
        Vect ray_direction = ray.getRayDirection();

        // Gets direction of camera or light ray
        double a = ray_direction.dotProduct(normal);

        if (a == 0) {
            // The ray is parallel to the plane
            return -1;
        }
        else {
            // If the ray intersects the plane then the point it intersects is returned
            double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
            return -1 * b / a;
        }
    }
};

Plane::Plane () {
    normal = Vect(0, 0, 0);
    distance = 0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane (Vect norValue, double disValue, Color colValue) {
    normal = norValue;
    distance = disValue;
    color = colValue;
}

#endif