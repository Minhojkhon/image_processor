//
// Created by Артем Парфенов on 27.03.2022.
//

#include "crystallize.h"
#include <random>
#include <numbers>
#include <cmath>

Image Crystallize::Apply(Image &img) {
    Image new_img(img.GetWidth(), img.GetHeight());
    std::vector<std::pair<size_t, size_t>> points;
    points.resize(cnt_);
    for (size_t i = 0; i < cnt_; ++i) {
        points[i].first = static_cast<double>(rand()) * (img.GetHeight() - 0) / RAND_MAX + 0;
        points[i].second = static_cast<double>(rand()) * (img.GetWidth() - 0) / RAND_MAX + 0;
    }
    std::vector<Color> colors(cnt_);
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            new_img.SetColor(j, i) = img.GetColor(j, i);
        }
    }
    for (size_t i = 0; i < cnt_; i++) {
        colors[i] = img.GetColor(points[i].second, points[i].first);
    }
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto d_mn = std::numeric_limits<int64_t>::max();
            size_t pos = 0;
            for (size_t r = 0; r < cnt_; ++r) {
                auto d =
                        (static_cast<int64_t>(i) - static_cast<int64_t>(points[r].first)) *
                        (static_cast<int64_t>(i) - static_cast<int64_t>(points[r].first)) +
                        (static_cast<int64_t>(j) - static_cast<int64_t>(points[r].second)) *
                        (static_cast<int64_t>(j) - static_cast<int64_t>(points[r].second));
                if (d < d_mn) {
                    d_mn = d;
                    pos = r;
                }
            }
            new_img.SetColor(j, i) = colors[pos];
        }
    }
    return new_img;
}