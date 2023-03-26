//
// Created by Minhojkhon Babaev on 03.05.2022.
//

#pragma once

#include "filters.h"

class Crystallize : public BaseFilter {
public:
    Image Apply(Image &img) override;

    size_t cnt_;
};
