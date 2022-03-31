//
// Created by Артем Парфенов on 22.03.2022.
//

#pragma once

#include "../image.h"
#include "../argument_parser.h"

const std::vector<std::pair<int, int>> FOR_REPLACEMENT = {{-1, 0},
                                                          {0,  -1},
                                                          {1,  0},
                                                          {0,  1}};

class BaseFilter {
public:
    virtual Image Apply(Image &img);

    virtual ~BaseFilter() {}
};


