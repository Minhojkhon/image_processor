//
// Created by Артем Парфенов on 21.03.2022.
//

#include "argument_parser.h"
#include <iostream>

ArgCast::ArgCast(const char *data) : data_(data) {}

void ArgCast::operator()(double *s) {
    *s = std::stod(data_);
}

void ArgCast::operator()(size_t *s) {
    *s = std::stoull(data_);
}

void ArgCast::operator()(std::string *s) {
    *s = static_cast<std::string>(data_);
}

void Parser::AddPositionalArg(const std::string_view &name, Argument arg) {
    positional_args_.push_back({name, arg});
}

void Parser::AddKeywordArg(const std::string_view &name, const std::vector<NameOfArg> arg) {
    keyword_fields_[name] = arg;
}

void Parser::PrintHelp() const {
    std::cout << "required arguments: ";
    std::cout << positional_args_[0].first << " " << positional_args_[1].first << std::endl;
    std::cout << "keyword arguments: ";
    for (const auto& elem : keyword_fields_) {
        std::cout << elem.first << " ";
        if (!elem.second.empty()) {
            for (const auto& [name, value] : elem.second) {
                std::cout << name << " ";
            }
        }
    }
    std::cout << std::endl;
}


std::vector<std::string_view> Parser::Parse(size_t argc, char **argv) {
    std::vector<std::string_view> res;
    if (argc < 3) {
        throw std::logic_error("wrong argument");
    }
    for (size_t i = 0; i < positional_args_.size(); ++i) {
        if (!keyword_fields_.contains(argv[i + 1])) {

            throw std::logic_error("wrong argument");
        }
        ArgCast assign(argv[i + 1]);
        std::visit(assign, positional_args_[i].second);
    }
    for (size_t i = positional_args_.size() + 1; i < argc; ++i) {
        auto it = keyword_fields_.find(argv[i]);
        size_t n;
        try {
            n = it->second.size();
        }
        catch (...) {
            throw std::logic_error("wrong argument");
        }
        res.push_back(it->first);
        for (size_t j = 1; j <= n; ++j) {
            try {
                ArgCast assign(argv[i + j]);
                std::visit(assign, it->second[j - 1].second);
            }
            catch (...) {
                throw std::logic_error("wrong argument");
            }
        }
        i += it->second.size();
    }
    return res;
}