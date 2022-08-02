#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Camera.h"
#include "Sphere.h"
#include "Source.h"
#include "Object.h"
#include "Light.h"
#include "Plane.h"
#include "Color.h"
#include "Vect.h"
#include "Ray.h"

using namespace std;

// Creates the type for pixles
struct RGBType {
    double r;
    double g;
    double b;
};

// Declaring global variables -> needed for loop inside main
int pixelPos;

// I copied this function from the web/tutorial
// I had and still have no clue how to convert pixel values to a bmp file and as that was not the
// main focus of the project I figured I would copy the function to save time
// This function converts a list of pixel color values to a bmp image
void savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w * h;
	int s = 4 * k;
	int filesize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int ppm = dpi * m;
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);
	
	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);
	
	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);
	
	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);
	
	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);
	
	f = fopen(filename, "wb");
	
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	
	for (int i = 0; i < k; i++) {
		RGBType rgb = data[i];
		
		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;
		
		unsigned char color[3] = {(int)floor(blue), (int)floor(green), (int)floor(red)};
		
		fwrite(color, 1, 3, f);
	}
	
	fclose(f);
}

// Finds intersection that was closest to the camera
int winningObjectIndex(vector<double> object_intersectionsList) {
    int index_of_minimum_value;

    // Prevent unnecessary calculations -> when no objects intersect with ray
    if (object_intersectionsList.size() == 1) {
        if (object_intersectionsList.at(0) > 0) {
            return 0;
        }
        else {
            return -1;
        }
    }
    else if (object_intersectionsList.size() == 0) {
        return -1;
    }
    // If only one intersection with object
    
    // Finds which object is closer to camera/light
    else {
        double max = 0;
        // Find maximum value for intersection
        for (int i = 0; i < object_intersectionsList.size(); i++) {
            if (max < object_intersectionsList.at(i)) {
                max = object_intersectionsList.at(i);
            }
        }

        // Checks if there was an intersection
        if (max > 0) {
            for (int index = 0; index < object_intersectionsList.size(); index++) {
                if (object_intersectionsList.at(index) > 0 && object_intersectionsList.at(index) <= max) {
                    max = object_intersectionsList.at(index);
                    // Returns index of correct intersection
                    index_of_minimum_value = index;
                }
            }

            return index_of_minimum_value;
        }
        // No intersection
        else {
            return -1;
        }
    }
}

// A lot of the math in this function I did not come up with on my own. I used the tutorial and various raytracing documents online
Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects,
int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {
    // Gets the color of the closes object to the screen
    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    // Get the normal vector or angle of the object at that point
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

    // Scales the color based on the ambient light
    Color new_color = winning_object_color.colorScalar(ambientlight);

    // Loops through each light source
    for (int light_index = 0; light_index < light_sources.size(); light_index++) {
        // Calculates the direction or the light 
        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

        // Get the cosine of the angle between the light ray and camera ray
        float cosine_angle = winning_object_normal.dotProduct(light_direction);

        // If the angle is not 90 degrees
        if (cosine_angle > 0) {
            // Initialize shadow bool
            bool shadowed = false;

            // Gets the distance from the intersection to the light
            Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            // Created the shadow ray
            Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

            // Creates a list to check for intersection other objects
            vector<double> secondary_intersectionsList;

            // Loops thriough the objects in the scene and checks for intersectionsList
            for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
                secondary_intersectionsList.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            }

            // If there is an intersection, then say that the point is in shadow
            for (int i = 0; i < secondary_intersectionsList.size(); i++) {
                if (secondary_intersectionsList.at(i) > accuracy) {
                    if (secondary_intersectionsList.at(i) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                }
                break;
            }

            // If the point is not shadowed
            if (shadowed == false) {
                // Increase the light level by the angle and number of light sources
                new_color = new_color.colorAdd(winning_object_color.colorMultiply(
                    light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

                // Checks if the object is reflective
                if (winning_object_color.getColorSpecial() > 0) {
                    double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
                    // Calculates the reflection direction of light from the object
                    Vect vectAdd = winning_object_normal.vectMult(dot1).vectAdd(intersecting_ray_direction);
                    Vect scalar = vectAdd.vectMult(2);
                    Vect vectAdd2 = intersecting_ray_direction.negative().vectAdd(scalar);
                    Vect reflection_direction = vectAdd2.normalize();

                    // Updates the color based on the reflections
                    double light_reflection = reflection_direction.dotProduct(light_direction);
                    if (light_reflection > 0) {
                        light_reflection = pow(light_reflection, 10);
                        new_color = new_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(
                            light_reflection * winning_object_color.getColorSpecial()));
                    }
                }
            }
        }
    }

    // Clips the final color so that the color stays in bounds see the color class
    return new_color.clip();
}

int main() {
    cout << "rendering..." << endl;

    // Variable declarations
    int width = 3840;
    int height = 2160;
    int dpi = 72;
    int n = width * height;
     
    // List of pixels for specified size image
    RGBType *pixels = new RGBType[n];

    // Basic deffinitions for producing image
    double aspectratio = (double)width/(double)height;
    double accuracy = 0.000001;
    double ambientlight = 0.2;

    //Declaring unit vectors
    Vect origin (0, 0, 0);
    Vect X (1, 0, 0);
    Vect Y (0, 1, 0);
    Vect Z (0, 0, 1);

    // Setting up the camera
    Vect campos (10, 5, -4);

    Vect look_at (1.5, 2, 0);
    Vect diff_btw (campos.getVectX() - look_at.getVectX(),
    campos.getVectY() - look_at.getVectY(),
    campos.getVectZ() - look_at.getVectZ());

    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossProduct(camdir).normalize();
    Vect camdown = camright.crossProduct(camdir);

    Camera scene_cam (campos, camdir, camright, camdown);

    // Color Deffinitions
    Color white_light (1.0, 1.0, 1.0, 0);
    Color blue_light (0, 0, 0.5, 0);
    Color bright_light (1000, 1, 1, 0);
    Color plane_white (0.5, 0.5, 0.1, 0);
    Color green (0.5, 1.0, 0.5, 0.3);
    Color red (1, 0.5, 0.5, 0.3);
    Color gray (0.5, 0.5, 0.5, 0);
    Color black (0, 0, 0, 0);
    Color light_blue (0.1, 0.3, 0.5, 0.3);
    Color colorless (0, 0, 0, 0);

    // Creating light sources in the scene and stacking them
    Vect light_position (-7, 10, -10);
    // Vect second_light_position (7, 10, 10);
    Light scene_light(light_position, white_light);
    // Light second_light(second_light_position, white_light);

    vector<Source*> light_sources;

    light_sources.push_back(dynamic_cast<Source*>(&scene_light));
    //light_sources.push_back(dynamic_cast<Source*>(&second_light));

    // Creating objects in the scene and stacking them
    Sphere scene_sphere (origin, 1, light_blue);

    Plane scene_plane (Y, -1, gray);
    Plane second_plane (Z, -1, gray);

    vector<Object*> scene_objects;

    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));

    // Creating variables that will act as the ofset so the camera looks through every pixel
    double xoffset, yoffset;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // Find index of specific pixel
            pixelPos = x + y * width;

            // Set xoffset and yoffset for different image sizes
            if (height > width) {
                // Image is taller than it is wide
                xoffset = (x + 0.5) / width;
                yoffset = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
            }
            else if (width > height) {
                // Image is wider than it is tall
                xoffset = ((x + 0.5) / width) * aspectratio - (((width - height) / (double)height) / 2);
                yoffset = ((height - y) + 0.5) / height;
            }
            else {
                // The image is square
                xoffset = (x + 0.5) / width;
                yoffset = ((height - y) + 0.5) / height;
            }

            // Creation of rays starting at the camera
            Vect cam_ray_origin = scene_cam.getCameraPosition();
            Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xoffset - 0.5).vectAdd(camdown.vectMult(yoffset - 0.5))).normalize();

            Ray cam_ray (cam_ray_origin, cam_ray_direction);

            // Initializes empty list for intersectionsList
            vector<double> intersectionsList;

            // Loops through objects and find the intersectionsList with the cam ray
            for (int i = 0; i < scene_objects.size(); i++) {
                intersectionsList.push_back(scene_objects.at(i)->findIntersection(cam_ray));
            }

            // Gets the intersection that was closest to the camera
            int index_of_winning_object = winningObjectIndex(intersectionsList);

            // Checks if any objects were hit
            if (index_of_winning_object == -1) {
                // Sets background to black -> Can be changed for different looking image
                pixels[pixelPos].r = 0;
                pixels[pixelPos].g = 0;
                pixels[pixelPos].b = 0;
            }
            else {
                // Index corresponds to an object
                if (intersectionsList.at(index_of_winning_object) > accuracy) {
                    // Determine the position and direction of the vectors at the point of intersection
                    Vect intersection_point = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersectionsList.at(index_of_winning_object)));
                    Vect intersecting_ray_direction = cam_ray_direction;

                    // Gets the new color of the pixel based on interestions and lighting
                    Color intersection_color = getColorAt(intersection_point, intersecting_ray_direction, scene_objects,
                    index_of_winning_object, light_sources, accuracy, ambientlight);

                    // Set the pixel to that new color
                    pixels[pixelPos].r = intersection_color.getColorRed();
                    pixels[pixelPos].g = intersection_color.getColorGreen();
                    pixels[pixelPos].b = intersection_color.getColorBlue();
                }
            }
        }
    }

    // Saves the image to a bmp file
    savebmp("scene.bmp", width, height, dpi, pixels);

    return 0;
}