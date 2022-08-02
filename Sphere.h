#ifndef _SPHERE_H
#define _SPHERE_H

#include "Object.h"
#include "Color.h"
#include "Vect.h"
#include "math.h"

class Sphere : public Object {
    Vect center;
    double radius;
    Color color;
    
    public:

    Sphere ();
    Sphere (Vect, double, Color);

    // Method functions
    // Functions return attributes of sphere
    Vect getSphereCenter() {
        return center;
    }
    double getSphereRadius() {
        return radius;
    }
    virtual Color getColor() {
        return color;
    }

    // Get the normal vector at any point on the sphere
    virtual Vect getNormalAt(Vect pos) {
        // Points away from center of sphere
        Vect normalVector = pos.vectAdd(center.negative()).normalize();
        return normalVector;
    }

    // Find the intersection between a ray and the sphere
    virtual double findIntersection(Ray ray) {
        Vect ray_direction = ray.getRayDirection();
        double ray_direction_x = ray_direction.getVectX();
        double ray_direction_y = ray_direction.getVectY();
        double ray_direction_z = ray_direction.getVectZ();

        Vect ray_origin = ray.getRayOrigin();
        double ray_origin_x = ray_origin.getVectX();
        double ray_origin_y = ray_origin.getVectY();
        double ray_origin_z = ray_origin.getVectZ();

        Vect sphere_center = center;
        double sphere_center_x = sphere_center.getVectX();
        double sphere_center_y = sphere_center.getVectY();
        double sphere_center_z = sphere_center.getVectZ();

        double a = 1;
        // Calculating values to plug into quadratic formula
        double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) +
        (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) +
        (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) +
        pow(ray_origin_y - sphere_center_y, 2) +
        pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);

        // Finds center of the roots
        double discriminant = (b * b) - (4 * c);

        // Checks if the ray intersects the sphere
        if (discriminant > 0) {
            // First root
            double root1 = (((-1 * b) - sqrt(discriminant)) / 2) - 0.000001;

            // If root intersects with sphere
            if (root1 > 0) {
                return root1;
            }
            // Otherwise, second root intersects
            else {
                double root2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
                return root2;
            }
        }
        else {
            // Ray does not intersect the sphere ever
            return -1;
        }
    }
};

// Sphere initialization when no parameters are given
Sphere::Sphere () {
    center = Vect(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

// Sphee initialization when parameters are given
Sphere::Sphere (Vect cValue, double rValue, Color colValue) {
    center = cValue;
    radius = rValue;
    color = colValue;
}

#endif