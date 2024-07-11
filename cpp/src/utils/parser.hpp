#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

enum SUPPORTED_ARG_TYPES {
    STRING_ARG,
    INT_ARG,
    FLOAT_ARG,
    BOOL_ARG,
    END_OF_SUPPORTED_ARG_TYPES
};


class Argument{
protected:
    std::string _name;
    bool _required;
    int _arg_type;
public:
    Argument(std::string arg_key, bool required, SUPPORTED_ARG_TYPES arg_type=STRING_ARG);

};


class SingleValueArgument : private Argument {
private:
    std::string _value;
public:
    void setValue(std::string value);
    std::string getValue();
    template <typename Type> Type getValue();
    SingleValueArgument(std::string arg_name, bool required, std::string default_value, SUPPORTED_ARG_TYPES arg_type=STRING_ARG);
    void printInfo();
};


class MultiValueArgument : private Argument {
private:
    std::vector<std::string> _values;
    int _min_vals;
    int _max_vals;
public:
    void pushValue(std::string value);
    int checkValuesCount();
    std::vector<std::string> getValues();
    MultiValueArgument(std::string arg_name, bool required,  int min_vals, int max_vals, SUPPORTED_ARG_TYPES arg_type=STRING_ARG);
    void printInfo();
};

class Parser{
private:
    std::vector<SingleValueArgument> posArgs;
    std::map<std::string, SingleValueArgument *> singleValKeyArgs;
    std::map<std::string, MultiValueArgument *> multiValKeyArgs;
    std::vector<SingleValueArgument *> requiredSingleValArgsPtrs;
    std::vector<MultiValueArgument *> requiredMultiValArgsPtrs;
    int _parseKeyArg(char **argv, int arg_key_index, int last_value_index);
    int _verifyRequiredArgumentsSet();
public:
    void addSingleValueArgument(std::string arg_name, bool required=false, std::string default_val="--", SUPPORTED_ARG_TYPES arg_type=STRING_ARG);
    void addMultiValueArgument(std::string arg_name, bool required=false, int min_vals=0, int max_args=-1, SUPPORTED_ARG_TYPES arg_type=STRING_ARG);
    int parseArgs(int argc, char **argv);
    void printArgumentList();
};