//
// Created by Артем Парфенов on 27.03.2022.
//

#pragma once
#include "filters.h"

class Sharpening : public BaseFilter{
public:
    Image Apply(Image &img) override;

};