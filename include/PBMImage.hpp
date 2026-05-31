#ifndef PBM_IMAGE_HPP
#define PBM_IMAGE_HPP

#include "Image.hpp"
#include <vector>

class PBMImage : public Image {
    private:
        vector<vector<int>> pixels;

    public:
        PBMImage(const string& name);

        void load() override;
        void save() const override;
        void saveas(const string& newName) const override;

        void negative() override;
        void rotate(const string& direction) override;
        Image* collage(const string& direction, const Image* other, const string& outName) const override;

        Image* clone() const override;
};

#endif