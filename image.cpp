//
// Created by Артем Парфенов on 23.03.2022.
//

#include "image.h"
#include <vector>
#include <algorithm>


static const int HEADER_SIZE = 14;
static const int DIB_HEADER_SIZE = 40;
static const size_t DIB_HEADER_SIZE_OFFSET = 0;
static const std::pair<char, char> FILE_EXTENSION = {'B', 'M'};
static const std::pair<size_t, size_t> FILE_EXTENSION_OFFSET = {0, 1};
static const std::vector<int> BYTES_QUANTITY = {8, 16, 24};
static const size_t BITS_PER_PIXEL_OFFSET = 14;
static const size_t SUPPORTED_BITS_PER_PIXEL = 24;
static const int COLOR_PLANES_QUANTITY = 1;
static const size_t COLOR_PLANES_OFFSET = 12;
static const int COMPRESSION_QUANTITY = 0;
static const size_t COMPRESSION_QUANTITY_OFFSET = 16;
static const size_t HEADER_SIZE_OFFSET = 2;
static const size_t WIDTH_OFFSET = 4;
static const size_t HEIGHT_OFFSET = 8;
static const std::pair<size_t, size_t> BORDERDS_OF_UNUSED_DIB = {16, 40};
static const std::vector<size_t> WIDTH_DIB_INDEXES = {4, 5, 6, 7};
static const std::vector<size_t> HEIGHT_DIB_INDEXES = {8, 9, 10, 11};
static const std::vector<size_t> MATRIX_READ_OFFSET = {3, 2, 1, 0};

int CountPadding(size_t wid) {
    return ((4 - (wid * 3) % 4) % 4);
}

Image::Image() : width_(0), height_(0) {}

Image::Image(size_t width, size_t height) : width_(width), height_(height) {
    colors_.resize(width * height);
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

/*

void Image::Read(const char *filepath) {
    std::ifstream file;
    file.open(filepath, std::ios::binary);

    if (!file.is_open()) {
        throw std::fstream::failure("file read error");
    }

    uint8_t header[HEADER_SIZE];
    file.read(reinterpret_cast<char *>(header), HEADER_SIZE);

    if (header[FILE_EXTENSION_OFFSET.first] != FILE_EXTENSION.first ||
        header[FILE_EXTENSION_OFFSET.second] != FILE_EXTENSION.second) {
        file.close();
        throw std::fstream::failure("wrong input file format");
    }

    uint8_t dib_header[DIB_HEADER_SIZE];

    try {
        file.read(reinterpret_cast<char *>(dib_header), DIB_HEADER_SIZE);
    }
    catch (...) {
        throw std::fstream::failure("unable to read");
    }
    if (dib_header[DIB_HEADER_SIZE_OFFSET] != DIB_HEADER_SIZE) {
        throw std::fstream::failure("wrong type of dib_header");
    }
    if (dib_header[COLOR_PLANES_OFFSET] != COLOR_PLANES_QUANTITY) {
        throw std::fstream::failure("unsupported quantity of color planes");
    }

    if (dib_header[BITS_PER_PIXEL_OFFSET] != SUPPORTED_BITS_PER_PIXEL) {
        throw std::fstream::failure("input file isn`t 24-bit");
    }

    if (dib_header[COMPRESSION_QUANTITY_OFFSET] != COMPRESSION_QUANTITY) {
        throw std::fstream::failure("unsupported quantity of compression");
    }


    width_ = dib_header[WIDTH_DIB_INDEXES[0]] + (dib_header[WIDTH_DIB_INDEXES[1]] << BYTES_QUANTITY[0]) +
             (dib_header[WIDTH_DIB_INDEXES[2]] << BYTES_QUANTITY[1]) +
             (dib_header[WIDTH_DIB_INDEXES[3]] << BYTES_QUANTITY[2]);
    height_ = dib_header[HEIGHT_DIB_INDEXES[0]] + (dib_header[HEIGHT_DIB_INDEXES[1]] << BYTES_QUANTITY[0]) +
              (dib_header[HEIGHT_DIB_INDEXES[2]] << BYTES_QUANTITY[1]) +
              (dib_header[HEIGHT_DIB_INDEXES[3]] << BYTES_QUANTITY[2]);
    const int padding = CountPadding(width_);

    colors_.resize(width_ * height_);

    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            uint8_t line[MATRIX_READ_OFFSET[0]];
            file.read(reinterpret_cast<char *>(line), MATRIX_READ_OFFSET[0]);

            colors_[i * width_ + j].red_ = static_cast<int>(line[MATRIX_READ_OFFSET[1]]);
            colors_[i * width_ + j].green_ = static_cast<int>(line[MATRIX_READ_OFFSET[2]]);
            colors_[i * width_ + j].blue_ = static_cast<int>(line[MATRIX_READ_OFFSET[3]]);
        }
        file.ignore(padding);
    }
}

void Image::Write(const char *filepath) {
    std::ofstream file;

    file.open(filepath, std::ios::out | std::ios::binary);


    if (!file.is_open()) {
        throw std::fstream::failure("file write error");
    }

    uint8_t header_garbage[4] = {0, 0, 0, 0};

    const int line_garbage = CountPadding(width_);

    const int size = HEADER_SIZE + DIB_HEADER_SIZE + width_ * height_ * 3 + line_garbage * height_;

    uint8_t header[HEADER_SIZE];

    header[FILE_EXTENSION_OFFSET.first] = FILE_EXTENSION.first;
    header[FILE_EXTENSION_OFFSET.second] = FILE_EXTENSION.second;
    header[HEADER_SIZE_OFFSET] = size;

    // header size bytes fill
    for (size_t i = 3; i < 6; ++i) {
        header[i] = size >> BYTES_QUANTITY[0] * (i - 2);
    }
    // application specific bytes
    for (size_t i = 6; i < 10; ++i) {
        header[i] = 0;
    }
    // byte, contains sum of headers
    header[10] = HEADER_SIZE + DIB_HEADER_SIZE;
    // unused bytes
    for (size_t i = 11; i < 14; ++i) {
        header[i] = 0;
    }

    uint8_t dib[DIB_HEADER_SIZE];

    dib[DIB_HEADER_SIZE_OFFSET] = DIB_HEADER_SIZE;
    // first bytes of dib fill
    for (size_t i = 1; i < 4; ++i) {
        dib[i] = header_garbage[i];
    }

    dib[WIDTH_OFFSET] = width_;
    // byte with width
    for (size_t i = 5; i < 8; ++i) {
        dib[i] = width_ >> BYTES_QUANTITY[0] * (i - 4);
    }

    dib[HEIGHT_OFFSET] = height_;
    // byte with height
    for (size_t i = 9; i < 12; ++i) {
        dib[i] = height_ >> BYTES_QUANTITY[0] * (i - 8);
    }

    dib[COLOR_PLANES_OFFSET] = COLOR_PLANES_QUANTITY;
    dib[COLOR_PLANES_OFFSET + 1] = 0; // useless byte
    dib[BITS_PER_PIXEL_OFFSET] = SUPPORTED_BITS_PER_PIXEL;
    dib[COMPRESSION_QUANTITY_OFFSET] = COMPRESSION_QUANTITY;

    for (size_t i = BORDERDS_OF_UNUSED_DIB.first; i < BORDERDS_OF_UNUSED_DIB.second; ++i) {
        dib[i] = 0;
    }

    file.write(reinterpret_cast<char *>(header), HEADER_SIZE);
    file.write(reinterpret_cast<char *>(dib), DIB_HEADER_SIZE);

    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            uint8_t red = static_cast<uint8_t>(std::clamp(GetColor(j, i).red_, 0., 255.));
            uint8_t green = static_cast<uint8_t>(std::clamp(GetColor(j, i).green_, 0., 255.));
            uint8_t blue = static_cast<uint8_t>(std::clamp(GetColor(j, i).blue_, 0., 255.));

            uint8_t line[3] = {blue, green, red};

            file.write(reinterpret_cast<char *>(line), MATRIX_READ_OFFSET[0]);
        }
        file.write(reinterpret_cast<char *>(header_garbage), line_garbage);
    }
}

*/

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

void Image::Write(const char *filepath) {
    std::ofstream str;
    str.open(filepath);
    file_.file_header_.Write(str);
    file_.image_header_.Write(str);
    uint8_t header_garbage[4] = {0, 0, 0, 0};
    auto line_garbage = CountPadding(width_);
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            uint8_t red = static_cast<uint8_t>(std::clamp(GetColor(j, i).red_, 0., 255.));
            uint8_t green = static_cast<uint8_t>(std::clamp(GetColor(j, i).green_, 0., 255.));
            uint8_t blue = static_cast<uint8_t>(std::clamp(GetColor(j, i).blue_, 0., 255.));

            uint8_t line[3] = {blue, green, red};

            str.write(reinterpret_cast<char *>(line), MATRIX_READ_OFFSET[0]);
        }
        str.write(reinterpret_cast<char *>(header_garbage), line_garbage);
    }
}


