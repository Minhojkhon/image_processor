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

    Color GetColor(size_t i, size_t j) ;

    Color &SetColor(size_t i, size_t j);

    void Read(const char *filepath);

    void Write(const char *filepath);

    template<typename type>
    static type BinaryRead(std::ifstream& in);

    template<typename type>
    static void BinaryWrite(std::ofstream& out, type& output);

    class BMP {
    public:
        BMP() = default;

        void Read(std::ifstream &in);

        void Save(std::ofstream &out);

        struct FileHeader {
            uint16_t Type;
            uint32_t Size;
            uint16_t Reserved1;
            uint16_t Reserved2;
            uint32_t OffBits;

            FileHeader() = default;

            FileHeader(std::ifstream& in);

            void Write(std::ofstream &out);
        };

        struct ImageHeader {
            uint32_t Size;
            uint32_t Width;
            uint32_t Height;
            uint16_t Planes;
            uint16_t BitsPerPixel;
            uint32_t Compression;
            uint32_t SizeImage;
            uint32_t XPelsPerMeter;
            uint32_t YPelsPerMeter;
            uint32_t ClrUsed;
            uint32_t ClrImportant;

            ImageHeader() = default;

            ImageHeader(std::ifstream &in);


            void Write(std::ofstream &out);
        };

        FileHeader file_header_;
        ImageHeader image_header_;
    };
    BMP file_;

private:
    size_t width_;
    size_t height_;
    std::vector<Color> colors_;
};
