//
// Created by Артем Парфенов on 27.03.2022.
//

#include "edge.h"
#include "filters.h"
#include "grayscale.h"

Image EdgeDetection::Apply(Image &img) {
    Image res(img.GetWidth(), img.GetHeight());
    Grayscale grayscale;
    res = grayscale.Apply(img);
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            int64_t color = 0;
            for (auto elem: FOR_REPLACEMENT) {
                size_t x = std::min(img.GetHeight() - 1, j + elem.first);
                size_t y = std::min(img.GetWidth() - 1, i + elem.second);
                color -= img.GetColor(x, y).blue_;
            }
            color += 4 * img.GetColor(j, i).blue_;
            if (color > arg_ * 255) {
                res.SetColor(j, i).red_ = 255;
                res.SetColor(j, i).green_ = 255;
                res.SetColor(j, i).blue_ = 255;
            } else {
                res.SetColor(j, i).red_ = 0;
                res.SetColor(j, i).green_ = 0;
                res.SetColor(j, i).blue_ = 0;
            }
        }
    }
    return res;
}