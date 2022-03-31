//
// Created by Артем Парфенов on 27.03.2022.
//

#pragma once

#include "filters.h"

class Crop : public BaseFilter {
public:
    Image Apply(Image &img) override;

    size_t height_;
    size_t width_;
};
