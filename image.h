//
// Created by Артем Парфенов on 23.03.2022.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>

struct Color {

    Color() : red_(0), green_(0), blue_(0) {};

    Color(double r, double g, double b) : red_(r), green_(g), blue_(b) {};

    double red_;
    double green_;
    double blue_;
};

class Image {
public:
    Image();

    Image(size_t width, size_t height);

    size_t GetWidth() const;

    size_t GetHeight() const;

    Color GetColor(size_t i, size_t j) const;

    Color &SetColor(size_t i, size_t j);

    void Read(const char *filepath);

    void Write(const char *filepath);



private:
    size_t width_;
    size_t height_;
    std::vector<Color> colors_;
};