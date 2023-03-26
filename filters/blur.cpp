//
// Created by Minhojkhon Babaev on 03.05.2022.
//

#include <cmath>
#include "filters.h"
#include "blur.h"
#include <numbers>


Image Blur::Apply(Image &img) {
    Image new_img(img.GetWidth(), img.GetHeight());
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            new_img.SetColor(j, i) = img.GetColor(j, i);
        }
    }
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto color = Color();
            for (size_t r = 0; r < arg_ * 6.; ++r) {
                int x = static_cast<int>(i) + static_cast<int>(r) - 3. * arg_;
                auto val_a = std::pow(2. * std::numbers::pi * arg_ * arg_, 0.5);
                auto val_b = std::pow(std::numbers::e,
                                      -static_cast<double>((x - i) *
                                                           (x - i) / 2. /
                                                           arg_ / arg_));
                x = std::clamp(x, 0, static_cast<int>(img.GetHeight()) - 1);
                j = std::clamp(static_cast<int>(j), 0, static_cast<int>(img.GetWidth()) - 1);
                color.blue_ += new_img.GetColor(j, x).blue_ / val_a * val_b;
                color.green_ += new_img.GetColor(j, x).green_ / val_a * val_b;
                color.red_ += new_img.GetColor(j, x).red_ / val_a * val_b;
            }
            new_img.SetColor(j, i) = color;
        }
    }
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto color = Color();
            for (size_t r = 0; r < arg_ * 6.; ++r) {
                int y = static_cast<int>(j) + static_cast<int>(r) - 3. * arg_;
                auto val_a = std::pow(2. * std::numbers::pi * arg_ * arg_, 0.5);
                auto val_b = std::pow(std::numbers::e,
                                      -static_cast<double>((y - j) *
                                                           (y - j) / 2. /
                                                           arg_ / arg_));
                y = std::clamp(y, 0, static_cast<int>(img.GetWidth()) - 1);
                i = std::clamp(static_cast<int>(i), 0, static_cast<int>(img.GetHeight()) - 1);
                color.blue_ += new_img.GetColor(y, i).blue_ / val_a * val_b;
                color.green_ += new_img.GetColor(y, i).green_ / val_a * val_b;
                color.red_ += new_img.GetColor(y, i).red_ / val_a * val_b;
            }
            img.SetColor(j, i) = color;
        }
    }
    return img;
}