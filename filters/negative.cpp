//
// Created by Артем Парфенов on 27.03.2022.
//

#include "negative.h"

Image Negative::Apply(Image &img) {
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            img.SetColor(j, i).red_ = 255 - img.GetColor(j, i).red_;
            img.SetColor(j, i).blue_ = 255 - img.GetColor(j, i).blue_;
            img.SetColor(j, i).green_ = 255 - img.GetColor(j, i).green_;
        }
    }
    return img;
}