//
// Created by Артем Парфенов on 27.03.2022.
//

#pragma once

#include "filters.h"

class Blur : public BaseFilter{
public:
    Image Apply(Image &img) override;
    double arg_;
};