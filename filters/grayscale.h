//
// Created by Артем Парфенов on 27.03.2022.
//

#pragma once
#include "filters.h"

class Grayscale : public BaseFilter {
public:
    Image Apply(Image &img) override;

};
