#include "../include/Session.hpp"

#include <iostream>

Session::Session(int id) : id(id) {}

void Session::copy(const Session& other) {
    id = other.id;
    transformations = other.transformations;

    for (Image* image : other.images) {
        images.push_back(image->clone());
    }
}

Session::Session(const Session& other) {
    copy(other);
}

Session& Session::operator=(const Session& other) {
    if (this != &other) {
        for (Image* image : images) {
            delete image;
        }
        images.clear();

        copy(other);
    }

    return *this;
}

Session::Session(Session&& other) noexcept {
    id = other.id;

    images = move(other.images);
    transformations = move(other.transformations);

    other.images.clear();
}

Session& Session::operator=(Session&& other) noexcept {
    if (this != &other) {
        for (Image* image : images) {
            delete image;
        }

        id = other.id;
        images = move(other.images);
        transformations = move(other.transformations);

        other.images.clear();
    }

    return *this;
}

void Session::addImage(Image* image) {
    images.push_back(image);
    cout << "Image " << image->getName() << " added" << endl;
}

void Session::grayscale() {
    transformations.push_back(Transformation("grayscale", ""));
    cout << "Grayscale transformation added." << endl;
}

void Session::monochrome() {
    transformations.push_back(Transformation("monochrome", ""));
    cout << "Monochrome transformation added." << endl;
}

void Session::negative() {
    transformations.push_back(Transformation("negative", ""));
    cout << "Negative transformation added." << endl;
}

void Session::rotate(const string& direction) {
    if (direction != "left" && direction != "right") {
        cout << "Invalid rotation!" << endl;
        return;
    }

    transformations.push_back(Transformation("rotate", direction));
    cout << "Rotate " << direction << " transformation added." << endl;
}

void Session::collage(const string& direction, const string& image1, const string& image2, const string& outimage) {
    Image* first = nullptr;
    Image* second = nullptr;

    for (Image* image : images) {
        if (image->getName() == image1) {
            first = image;
        }
        
        if (image->getName() == image2) {
            second = image;
        }
    }

    if (first == nullptr || second == nullptr) {
        cout << "Images not found in the current session!" << endl;
        return; 
    }

    if (first->getWidth() != second->getWidth() || first->getHeight() != second->getHeight()) {
        cout << "Cannot make a collage from different dimensions!" << endl;
        return;
    }

    string firstName = first->getName();
    string secondName = second->getName();

    if (firstName.substr(firstName.find_last_of('.')) != secondName.substr(secondName.find_last_of('.'))) {
        cout << "Cannot make a collage from different types!" << endl;
        return;
    }

    Image* result = first->collage(direction, second, outimage);

    if (result != nullptr) {
        images.push_back(result);
        cout << "New collage " << outimage << " created" << endl;
    }
}

void Session::applyTransformation(Image* image, const Transformation& transformation) {
    if (transformation.name == "grayscale") {
        image->grayscale();
    } else if (transformation.name == "monochrome") {
        image->monochrome();
    } else if (transformation.name == "negative") {
        image->negative();
    } else if (transformation.name == "rotate") {
        image->rotate(transformation.argument);
    }
}

void Session::undo() {
    if (transformations.empty()) {
        cout << "No transformations to undo." << endl;
        return;
    }

    transformations.pop_back();
}

void Session::save() {
    for (Image* image : images) {
        for (const Transformation& transformation : transformations) {
            applyTransformation(image, transformation);
        }

        image->save();
    }

    transformations.clear();
    cout << "Session saved!" << endl;
}

void Session::saveas(const string& fileName) {
    if (images.empty()) {
        cout << "No images." << endl;
        return;
    }

    Image* copy = images[0]->clone();

    for (const Transformation& transformation : transformations) {
        applyTransformation(copy, transformation);
    }

    copy->saveas(fileName);

    delete copy;

    transformations.clear();
    cout << "Image saved as " << fileName << endl;
}

void Session::info() const {
    cout << "Name of images in the session: ";

    for (Image* image : images) {
        cout << image->getName() << ", ";
    }
    cout << endl;

    cout << "Pending transformations: ";

    if (transformations.empty()) {
        cout << "None" << endl;
        return;
    }

    for (const Transformation& transformation : transformations) {
        cout << transformation.toString() << ", ";
    }
    cout << endl;
}

int Session::getId() const {
    return id;
}

Session::~Session() {
    for (Image* image : images) {
        delete image;
    }

    images.clear();
}