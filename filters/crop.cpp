//
// Created by Артем Парфенов on 27.03.2022.
//

#include "crop.h"
#include "filters.h"

Image Crop::Apply(Image &img) {
    width_ = std::min(width_, img.GetWidth());
    height_ = std::min(height_, img.GetHeight());
    Image res(width_, height_);
    for (size_t i = img.GetHeight() - height_; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < width_; ++j) {
            res.SetColor(j, i - img.GetHeight() + height_) = img.GetColor(j, i);
        }
    }
    return res;
}