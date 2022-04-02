
#include "argument_parser.h"
#include "filters/blur.h"
#include "filters/crop.h"
#include "filters/crystallize.h"
#include "image.h"
#include "filters/edge.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/sharpening.h"
#include <iostream>

static const std::string_view BLUR_STR = "-blur";
static const std::string_view CROP_STR = "-crop";
static const std::string_view NEG_STR = "-neg";
static const std::string_view GS_STR = "-gs";
static const std::string_view SHARP_STR = "-sharp";
static const std::string_view EDGE_STR = "-edge";
static const std::string_view CR_STR = "-cr";



int main(int argc, char **argv) {
    Parser parser;

    std::string input_filename;
    std::string output_filename;
    Blur blur;
    Crop crop;
    Negative neg;
    Grayscale gray;
    Sharpening sharp;
    EdgeDetection edge;
    Crystallize crystallize;


    std::vector<std::string_view> params;

    parser.AddPositionalArg("input filename", &input_filename);
    parser.AddPositionalArg("output filename", &output_filename);
    parser.AddKeywordArg(BLUR_STR, {{"sigma", &blur.arg_}});
    parser.AddKeywordArg(CROP_STR, {{"width",  &crop.width_},
                                    {"height", &crop.height_}});
    parser.AddKeywordArg(NEG_STR, {});
    parser.AddKeywordArg(GS_STR, {});
    parser.AddKeywordArg(SHARP_STR, {});
    parser.AddKeywordArg(EDGE_STR, {{"threshold", &edge.arg_}});
    parser.AddKeywordArg(CR_STR, {{"count", &crystallize.cnt_}});
    try {
        params = parser.Parse(argc, argv);
    }
    catch (...) {
        parser.PrintHelp();
        return 0;
    }

    Image img;
    img.Read(input_filename.c_str());

    for (auto &param: params) {
        if (param == BLUR_STR) {
            img = blur.Apply(img);
        }
        if (param == NEG_STR) {
            img = neg.Apply(img);
        }
        if (param == GS_STR) {
            img = gray.Apply(img);
        }
        if (param == SHARP_STR) {
            img = sharp.Apply(img);
        }
        if (param == EDGE_STR) {
            img = edge.Apply(img);
        }
        if (param == CR_STR) {
            img = crystallize.Apply(img);
        }
        if (param == CROP_STR) {
            img = crop.Apply(img);
        }
    }

    img.Write(output_filename.c_str());

    return 0;
}
