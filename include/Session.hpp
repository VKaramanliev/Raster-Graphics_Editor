#ifndef SESSION_HPP
#define SESSION_HPP

#include "Image.hpp"
#include "Transformation.hpp"

#include <vector>


class Session {
    private:
        int id;
        vector<Image*> images;
        vector<Transformation> transformations;

        void applyTransformation(Image* image, const Transformation& transformation);
        void copy(const Session& other);

    public:
        Session(int id);

        Session(const Session& other);
        Session& operator=(const Session& other);

        Session(Session&& other) noexcept;
        Session& operator=(Session&& other) noexcept;

        void addImage(Image* image);
        void grayscale();
        void monochrome();
        void negative();
        void rotate(const string& direction);
        void collage(const string& direction, const string& image1, const string& image2, const string& outimage);

        void undo();
        void save();
        void saveas(const string& fileName);
        void info() const;

        int getId() const;

        ~Session();
};

#endif