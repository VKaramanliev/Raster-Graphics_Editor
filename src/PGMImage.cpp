#include "../include/PGMImage.hpp"

#include <iostream>
#include <fstream>

PGMImage::PGMImage(const string& name) : Image(name), maxValue(255) {}

bool PGMImage::load() {
    ifstream file(name);

    if (!file.is_open()) {
        cout << "Cannot open file: " << name << endl;
        return false;
    }

    string format;
    file >> format;

    if (format != "P2") {
        cout << "Invalid file format!" << endl;
        file.close();
        return false;
    }

    file >> width >> height >> maxValue;
    pixels.clear();
    pixels.resize(height, vector<int>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file >> pixels[i][j];
        }
    }

    file.close();
    return true;
}

void PGMImage::save() const {
    saveas(name);
}

void PGMImage::saveas(const string& newName) const {
    ofstream file(newName);

    if (!file.is_open()) {
        cout << "Cannot save file: " << newName << endl;
        return;
    }

    file << "P2" << endl;
    file << width << " " << height << endl;
    file << maxValue << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file << pixels[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

void PGMImage::negative() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixels[i][j] = maxValue - pixels[i][j];
        }
    }
}

void PGMImage::rotate(const string& direction) {
    vector<vector<int>> rotated;

    if (direction == "right") {
        rotated.resize(width, vector<int>(height));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated[j][height - 1 - i] = pixels[i][j];
            }
        }
    } else if (direction == "left") {
        rotated.resize(width, vector<int>(height));

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

Image* PGMImage::collage(const string& direction, const Image* other, const string& outName) const {
    const PGMImage* second = (const PGMImage*) other;

    PGMImage* result = new PGMImage(outName);
    result->maxValue = maxValue;

    if (direction == "horizontal") {
        result->width = width * 2;
        result->height = height;
        result->pixels.resize(height, vector<int>(width * 2));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result->pixels[i][j] = pixels[i][j];
                result->pixels[i][j + width] = second->pixels[i][j];
            }
        }
    } else if (direction == "vertical") {
        result->width = width;
        result->height = height * 2;
        result->pixels.resize(height * 2, vector<int>(width));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result->pixels[i][j] = pixels[i][j];
                result->pixels[i + height][j] = second->pixels[i][j];
            }
        }
    } else {
        cout << "Invalid direction!" << endl;
        delete result;
        return nullptr;
    }

    return result;
}

Image* PGMImage::clone() const {
    return new PGMImage(*this);
}