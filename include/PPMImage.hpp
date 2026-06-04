#ifndef PPM_IMAGE_HPP
#define PPM_IMAGE_HPP

#include "Image.hpp"
#include <vector>

struct RGB {
    int r;
    int g;
    int b;
};

class PPMImage : public Image {
    private:
        vector<vector<RGB>> pixels;
        int maxValue;

        bool isGrayscale() const;
        bool isMonochrome() const;

    public:
        PPMImage(const string& name);

        bool load() override;
        void save() const override;
        void saveas(const string& newName) const override;

        void grayscale() override;
        void monochrome() override;
        void negative() override;
        void rotate(const string& direction) override;
        Image* collage(const string& direction, const Image* other, const string& outName) const override;

        Image* clone() const override;
};

#endif