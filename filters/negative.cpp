//
// Created by Артем Парфенов on 27.03.2022.
//

#include "negative.h"

Image Negative::Apply(Image &img) {
    Image res(img.GetWidth(), img.GetHeight());
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            res.SetColor(j, i).red_ = 255 - img.GetColor(j, i).red_;
            res.SetColor(j, i).blue_ = 255 - img.GetColor(j, i).blue_;
            res.SetColor(j, i).green_ = 255 - img.GetColor(j, i).green_;
        }
    }
    return res;
}