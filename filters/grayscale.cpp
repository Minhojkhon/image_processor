//
// Created by Minhojkhon Babaev on 03.05.2022.
//

#include "filters.h"
#include "grayscale.h"

Image Grayscale::Apply(Image &img) {
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto r = img.GetColor(j, i).red_;
            auto g = img.GetColor(j, i).green_;
            auto b = img.GetColor(j, i).blue_;
            auto man = 0.299f * r + 0.587f * g + 0.114f * b;
            img.SetColor(j, i).red_ = man;
            img.SetColor(j, i).green_ = man;
            img.SetColor(j, i).blue_ = man;

        }
    }
    return img;
}