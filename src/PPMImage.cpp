#include "../include/PPMImage.hpp"

#include <iostream>
#include <fstream>

PPMImage::PPMImage(const string& name) : Image(name), maxValue(255) {}

bool PPMImage::load() {
    ifstream file(name);

    if (!file.is_open()) {
        cout << "Cannot open file: " << name << endl;
        return false;
    }

    string format;
    file >> format;

    if (format != "P3") {
        cout << "Invalid file format!" << endl;
        file.close();
        return false;
    }

    file >> width >> height >> maxValue;
    pixels.clear();
    pixels.resize(height, vector<RGB>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file >> pixels[i][j].r >> pixels[i][j].g >> pixels[i][j].b;
        }
    }

    file.close();
    return true;
}

void PPMImage::save() const {
    ofstream file(name);

    if (!file.is_open()) {
        cout << "Cannot save file: " << name << endl;
        return;
    }

    file << "P3" << endl;
    file << width << " " << height << endl;
    file << maxValue << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file << pixels[i][j].r << " "
                 << pixels[i][j].g << " "
                 << pixels[i][j].b << " ";
        }

        file << endl;
    }

    file.close();
}

void PPMImage::saveas(const string& newName) const {
    ofstream file(newName);

    if (!file.is_open()) {
        cout << "Cannot save file: " << name << endl;
        return;
    }

    file << "P3" << endl;
    file << width << " " << height << endl;
    file << maxValue << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file << pixels[i][j].r << " "
                 << pixels[i][j].g << " "
                 << pixels[i][j].b << " ";
        }

        file << endl;
    }

    file.close();
}

void PPMImage::grayscale() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int gray = (pixels[i][j].r + pixels[i][j].g + pixels[i][j].b) / 3;

            pixels[i][j].r = gray;
            pixels[i][j].g = gray;
            pixels[i][j].b = gray;
        }
    }
}

void PPMImage::monochrome() {
    grayscale();

    int middle = maxValue / 2;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int value;

            if (pixels[i][j].r > middle) {
                value = maxValue;
            } else {
                value = 0;
            }

            pixels[i][j].r = value;
            pixels[i][j].g = value;
            pixels[i][j].b = value;
        }
    }
}

void PPMImage::negative() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixels[i][j].r = maxValue - pixels[i][j].r;
            pixels[i][j].g = maxValue - pixels[i][j].g;
            pixels[i][j].b = maxValue - pixels[i][j].b;
        }
    }
}

void PPMImage::rotate(const string& direction) {
    vector<vector<RGB>> rotated;

    if (direction == "right") {
        rotated.resize(width, vector<RGB>(height));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated[j][height - 1 - i] = pixels[i][j];
            }
        }
    } else if (direction == "left") {
        rotated.resize(width, vector<RGB>(height));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated[width - 1 - j][i] = pixels[i][j];
            }
        }
    } else {
        cout << "Invalid rotate direction. You should use only left or right!" << endl;
        return;
    }

    pixels = rotated;
    
    int oldWidth = width;
    width = height;
    height = oldWidth;
}

Image* PPMImage::collage(const string& direction, const Image* other, const string& outName) const {
    const PPMImage* second = (const PPMImage*) other;

    PPMImage* result = new PPMImage(outName);
    result->maxValue = maxValue;

    if (direction == "horizontal") {
        result->width = width * 2;
        result->height = height;
        result->pixels.resize(height, vector<RGB>(width * 2));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result->pixels[i][j] = pixels[i][j];
                result->pixels[i][j + width] = second->pixels[i][j];
            }
        }
    }
    else if (direction == "vertical") {
        result->width = width;
        result->height = height * 2;
        result->pixels.resize(height * 2, vector<RGB>(width));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result->pixels[i][j] = pixels[i][j];
                result->pixels[i + height][j] = second->pixels[i][j];
            }
        }
    }
    else {
        cout << "Invalid direction!" << endl;
        delete result;
        return nullptr;
    }

    return result;
}

Image* PPMImage::clone() const {
    return new PPMImage(*this);
}