//
// Created by Артем Парфенов on 27.03.2022.
//

#include "crop.h"
#include "filters.h"

Image Crop::Apply(Image &img) {
    width_ = std::min(width_, img.GetWidth());
    height_ = std::min(height_, img.GetHeight());
    img.file_.image_header_.Height = height_;
    img.file_.image_header_.Width = width_;
    Image res(width_, height_);
    res.file_.image_header_ = img.file_.image_header_;
    res.file_.file_header_ = img.file_.file_header_;
    for (size_t i = img.GetHeight() - height_; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < width_; ++j) {
            res.SetColor(j, i - img.GetHeight() + height_) = img.GetColor(j, i);
        }
    }
    return res;
}