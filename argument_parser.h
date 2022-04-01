//
// Created by Артем Парфенов on 21.03.2022.
// most of the code is taken from seminars

#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <exception>
#include <memory>

using Argument = std::variant<std::string*, double*, size_t*>;
using NameOfArg = std::pair<std::string_view, Argument>;
using StrToRefs = std::vector<NameOfArg>;
using StrToArgs = std::unordered_map<std::string_view, std::vector<NameOfArg>>;

struct ArgCast{
    ArgCast (const char* data);

    void operator()(std::string* s);
    void operator()(size_t* s);
    void operator()(double* s);

private:
    const char* data_;
};

class Parser {
public:
    Parser() = default;

    void AddKeywordArg(const std::string_view& name, const std::vector<NameOfArg> arg);

    std::vector<std::string_view> Parse(size_t argc, char** argv);

    void AddPositionalArg(const std::string_view& name, Argument arg);

    void PrintHelp() const;

private:
    StrToRefs positional_args_;
    StrToArgs keyword_fields_;
};




