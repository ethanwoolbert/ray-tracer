#ifndef _COLOR_H
#define _COLOR_H

class Color {
    double red;
    double green;
    double blue;
    double special;
    
    public:

    Color ();
    Color (double, double, double, double);

    // Method functions
    // Get value of color functions
    double getColorRed() {
        return red;
    }
    double getColorGreen() {
        return green;
    }
    double getColorBlue() {
        return blue;
    }
    double getColorSpecial() {
        return special;
    }

    // Set value of color
    double setColorRed(double rValue) {
        red = rValue;
    }
    double setColorGreen(double gValue) {
        green = gValue;
    }
    double setColorBlue(double bValue) {
        blue = bValue;
    }
    double setColorSpecial(double sValue) {
        special = sValue;
    }

    // Average of colors or brightness
    double brightness() {
        return(red + green + blue) / 3;
    }

    // Basic operations with color
    Color colorScalar(double scalar) {
        return Color (red * scalar, green * scalar, blue * scalar, special);
    }

    Color colorAdd(Color color) {
        return Color (red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
    }

    Color colorAverage(Color color) {
        return Color ((red + color.getColorRed()) / 2, (green + color.getColorGreen()) / 2, (blue + color.getColorBlue()) / 2, special);
    }

    Color colorMultiply(Color color) {
        return Color (red * color.getColorRed(), green * color.getColorGreen(), blue * color.getColorBlue(), special);
    }

    // Clips extra light so that when increasing light levels, the colors do not go past 1 or less than 0
    Color clip() {
        double alllight = red + green + blue;
        double extralight = alllight - 3;

        if (extralight > 0) {
            red = red + extralight * (red / alllight);
            green = green + extralight * (green / alllight);
            blue = blue + extralight * (blue / alllight);
        }
        if (red > 1) {
            red = 1;
        }
        if (green > 1) {
            green = 1;
        }
        if (green > 1) {
            blue = 1;
        }
        if (red < 0) {
            red = 0;
        }
        if (green < 0) {
            green = 0;
        }
        if (blue < 0) {
            blue = 0;
        }

        return Color (red, green, blue, special);
    }
};

// Initialization -> no parameters
Color::Color () {
    red = 0.5;
    green = 0.5;
    blue = 0.5;
    special = 0;
}

// Initialization w/parameters
Color::Color (double r, double g, double b, double s) {
    red = r;
    green = g;
    blue = b;
    special = s;
}

#endif