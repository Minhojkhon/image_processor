//
// Created by Артем Парфенов on 27.03.2022.
//

#include "filters.h"
#include "grayscale.h"

Image Grayscale::Apply(Image &img) {
    auto res = Image(img.GetWidth(), img.GetHeight());
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto r = img.GetColor(j, i).red_;
            auto g = img.GetColor(j, i).green_;
            auto b = img.GetColor(j, i).blue_;
            auto man = 0.299f * r + 0.587f * g + 0.114f * b;
            res.SetColor(j, i).red_ = man;
            res.SetColor(j, i).green_ = man;
            res.SetColor(j, i).blue_ = man;

        }
    }
    return res;
}