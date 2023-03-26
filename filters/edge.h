//
// Created by Minhojkhon Babaev on 03.05.2022.
//

#pragma once

#include "filters.h"

class EdgeDetection : public BaseFilter{
public:
    Image Apply(Image &img) override;
    double arg_;
};
