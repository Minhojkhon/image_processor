//
// Created by Minhojkhon Babaev on 03.05.2022.
//

#include "image.h"
#include <vector>
#include <algorithm>

static const size_t SUPPORTED_BITS_PER_PIXEL = 24;
static const int COMPRESSION_QUANTITY = 0;

static const std::vector<size_t> MATRIX_READ_OFFSET = {3, 2, 1, 0};

int CountPadding(size_t wid) {
    return ((4 - (wid * 3) % 4) % 4);
}

Image::Image() : width_(0), height_(0) {}

Image::Image(size_t width, size_t height) : width_(width), height_(height) {
    colors_.resize(width * height);
    file_.image_header_.Height = height;
    file_.image_header_.Width = width;
}


size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

Color Image::GetColor(size_t i, size_t j) {
    return colors_[j * width_ + i];
}

Color &Image::SetColor(size_t i, size_t j) {
    return colors_[j * width_ + i];
}



template<typename type>
type Image::BinaryRead(std::ifstream& in) {
    type input;
    in.read(reinterpret_cast<char*>(&input), sizeof(type));
    return input;
}
template<typename type>
void Image::BinaryWrite(std::ofstream& out, type &output) {
    out.write(reinterpret_cast<char*>(&output), sizeof(type));
}

static const auto TYPE_OFFSET = 0x4D42;
static const std::pair<int, int> RESERVED_OFFSET = {0, 0};

Image::BMP::FileHeader::FileHeader(std::ifstream &in) {
    Type = BinaryRead<uint16_t>(in);
    if (Type != TYPE_OFFSET) {
        throw std::invalid_argument("Wrong file extension");
    }
    Size = BinaryRead<uint32_t>(in);
    Reserved1 = BinaryRead<uint16_t>(in);
    Reserved2 = BinaryRead<uint16_t>(in);
    if (Reserved1 != RESERVED_OFFSET.first || Reserved2 != RESERVED_OFFSET.second) {
        throw std::invalid_argument("Wrong reserved bytes quantity");
    }
    OffBits = BinaryRead<uint32_t>(in);
}

void Image::BMP::FileHeader::Write(std::ofstream &out) {
    BinaryWrite(out, Type);
    BinaryWrite(out, Size);
    BinaryWrite(out, Reserved1);
    BinaryWrite(out, Reserved2);
    BinaryWrite(out, OffBits);
}

static const int PLANES_QUANTITY = 1;

Image::BMP::ImageHeader::ImageHeader(std::ifstream &in) {
    Size = BinaryRead<uint32_t>(in);
    Width = BinaryRead<uint32_t>(in);
    Height = BinaryRead<uint32_t>(in);
    Planes = BinaryRead<uint16_t>(in);
    if (Planes != PLANES_QUANTITY) {
        throw std::invalid_argument("Wrong quantity of image planes");
    }
    BitsPerPixel = BinaryRead<uint32_t>(in);
    if (BitsPerPixel != SUPPORTED_BITS_PER_PIXEL) {
        throw std::invalid_argument("Unsupported number of bits per pixel");
    }
    Compression = BinaryRead<uint32_t>(in);
    if (Compression != COMPRESSION_QUANTITY) {
        throw std::invalid_argument("Unsupported compression");
    }
    SizeImage = BinaryRead<uint32_t>(in);
    XPelsPerMeter = BinaryRead<uint32_t>(in);
    YPelsPerMeter = BinaryRead<uint32_t>(in);
    ClrUsed = BinaryRead<uint32_t>(in);
    ClrImportant = BinaryRead<uint32_t>(in);
}

void Image::BMP::ImageHeader::Write(std::ofstream &out) {
    BinaryWrite(out, Size);
    BinaryWrite(out, Width);
    BinaryWrite(out, Height);
    BinaryWrite(out, Planes);
    BinaryWrite(out, BitsPerPixel);
    BinaryWrite(out, Compression);
    BinaryWrite(out, SizeImage);
    BinaryWrite(out, XPelsPerMeter);
    BinaryWrite(out, YPelsPerMeter);
    BinaryWrite(out, ClrUsed);
    BinaryWrite(out, ClrImportant);
}

void Image::BMP::Read(std::ifstream &in) {
    file_header_ = FileHeader(in);
    image_header_ = ImageHeader(in);
}



void Image::Read(const char *filepath) {
    file_ = BMP();
    std::ifstream str;
    str.open(filepath);
    file_.Read(str);
    width_ = file_.image_header_.Width;
    height_ = file_.image_header_.Height;
    colors_.resize(width_ * height_);
    const int padding = CountPadding(width_);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            uint8_t line[MATRIX_READ_OFFSET[0]];
            str.read(reinterpret_cast<char *>(line), MATRIX_READ_OFFSET[0]);

            colors_[i * width_ + j].red_ = static_cast<int>(line[MATRIX_READ_OFFSET[1]]);
            colors_[i * width_ + j].green_ = static_cast<int>(line[MATRIX_READ_OFFSET[2]]);
            colors_[i * width_ + j].blue_ = static_cast<int>(line[MATRIX_READ_OFFSET[3]]);
        }
        str.ignore(padding);
    }
}

void Image::BMP::Save(std::ofstream &out) {
    file_header_.Write(out);
    image_header_.Write(out);
}

static uint8_t HEADER_GARBAGE[4] = {0, 0, 0, 0};

void Image::Write(const char *filepath) {
    std::ofstream str;
    str.open(filepath);
    file_.Save(str);
    auto line_garbage = CountPadding(width_);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            uint8_t red = static_cast<uint8_t>(std::clamp(GetColor(j, i).red_, 0., 255.));
            uint8_t green = static_cast<uint8_t>(std::clamp(GetColor(j, i).green_, 0., 255.));
            uint8_t blue = static_cast<uint8_t>(std::clamp(GetColor(j, i).blue_, 0., 255.));

            uint8_t line[3] = {green, red, blue};

            str.write(reinterpret_cast<char *>(line), MATRIX_READ_OFFSET[0]);
        }
    }
    str.write(reinterpret_cast<char *>(HEADER_GARBAGE), line_garbage);
}


