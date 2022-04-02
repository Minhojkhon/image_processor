//
// Created by Артем Парфенов on 27.03.2022.
//

#include "filters.h"
#include "sharpening.h"

static const int FIVE = 5;

Image Sharpening::Apply(Image &img) {
    Image res(img.GetWidth(), img.GetHeight());
    res.file_.image_header_ = img.file_.image_header_;
    res.file_.file_header_ = img.file_.file_header_;
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            int red = 0, blue = 0, green = 0;
            for (auto elem: FOR_REPLACEMENT) {
                size_t x = std::min(img.GetHeight() - 1, j + elem.first);
                size_t y = std::min(img.GetWidth() - 1, i + elem.second);
                red -= img.GetColor(x, y).red_;
                blue -= img.GetColor(x, y).blue_;
                green -= img.GetColor(x, y).green_;
            }
            red += FIVE * img.GetColor(j, i).red_;
            blue += FIVE * img.GetColor(j, i).blue_;
            green += FIVE * img.GetColor(j, i).green_;
            red = std::clamp(red, 0, 255);
            blue = std::clamp(blue, 0, 255);
            green = std::clamp(green, 0, 255);
            res.SetColor(j, i).red_ = red;
            res.SetColor(j, i).green_ = green;
            res.SetColor(j, i).blue_ = blue;
        }
    }
    return res;
}