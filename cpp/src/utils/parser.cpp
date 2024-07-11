#include "parser.hpp"


//TODO: Add my own Exception and add throw/catches so no relying on 'return 1' as indicators of errors
void Parser::addSingleValueArgument(std::string arg_key, bool required, std::string default_val, SUPPORTED_ARG_TYPES arg_type) {
    SingleValueArgument *svarg = new SingleValueArgument(arg_key, required, default_val, arg_type);
    if (arg_key[0] == '-' && arg_key[1] == '-') singleValKeyArgs.insert({arg_key, svarg});
    else posArgs.push_back(*svarg);

    //TODO: Add check to verify previous posArg is required if this one is as well. If not, throw an error as that wouldn't make sense to have a required pos argument after a non-required pos argument (both should be required)
    if (required) {
        requiredSingleValArgsPtrs.push_back(svarg);
    }
};


void Parser::addMultiValueArgument(std::string arg_key, bool required, int min_vals, int max_vals, SUPPORTED_ARG_TYPES arg_type) {
    if (arg_key[0] != '-' && arg_key[1] != '-') return;
    MultiValueArgument *mvarg = new MultiValueArgument(arg_key, required, min_vals, max_vals, arg_type);
    multiValKeyArgs.insert({arg_key, mvarg});

    if (required) {
        requiredMultiValArgsPtrs.push_back(mvarg);
    }
};


int Parser::parseArgs(int argc, char **argv) {
    if (argc == 1) return 0;

    bool doneWithPositionArgs = false;
    int i = 1;
    int keyArgStartIndex = 0;
    while (i < argc) {
        if (argv[i][0] == '-' && !doneWithPositionArgs) {
            doneWithPositionArgs = true;
            keyArgStartIndex = i;
        }
        else if (!doneWithPositionArgs) {
            posArgs[i-1].setValue(argv[i]);
        }
        else if (argv[i][0] == '-') {
            if (_parseKeyArg(argv, keyArgStartIndex, i-1)) return 1;
            keyArgStartIndex = i;
        }
        i++;
    };

    if (i == argc && doneWithPositionArgs) {
        if(_parseKeyArg(argv, keyArgStartIndex, i-1)) return 1;
    }

    printArgumentList();
    if(_verifyRequiredArgumentsSet()) return 1;
    return 0;
};


int Parser::_parseKeyArg(char **argv, int arg_key_index, int last_value_index) {
    if (arg_key_index >= last_value_index) return 1;
    
    if (singleValKeyArgs.find(argv[arg_key_index]) != singleValKeyArgs.end()) {
        if (arg_key_index + 1 != last_value_index) {std::cout << "Indexes wrong!" << std::endl; return 1;}
        singleValKeyArgs[argv[arg_key_index]]->setValue(argv[last_value_index]);
        return 0;
    }

    if (multiValKeyArgs.find(argv[arg_key_index]) != multiValKeyArgs.end()) {
        for (int i = arg_key_index + 1; i <= last_value_index; i++) {
            multiValKeyArgs[argv[arg_key_index]]->pushValue(argv[i]);
        }
        if (multiValKeyArgs[argv[arg_key_index]]->checkValuesCount()) return 1;
        return 0;
    }
    std::cout << "DIDNT FIND IT" << std::endl;
    return 1;
}


int Parser::_verifyRequiredArgumentsSet() {
    for (SingleValueArgument *const req_svarg_ptr : requiredSingleValArgsPtrs) {
        if (req_svarg_ptr->getValue().empty()) return 1;
    }
    for (MultiValueArgument *const req_mvarg_ptr : requiredMultiValArgsPtrs) {
        if (req_mvarg_ptr->checkValuesCount()) return 1;
    }
    return 0;
}


Argument::Argument(std::string arg_name, bool required, SUPPORTED_ARG_TYPES arg_type) {
    _name = arg_name;
    _required = required;
    if (arg_type >= END_OF_SUPPORTED_ARG_TYPES || arg_type < 0) _arg_type=STRING_ARG;
    else _arg_type=arg_type; 
}


SingleValueArgument::SingleValueArgument(std::string arg_name, bool required, std::string default_value, SUPPORTED_ARG_TYPES arg_type) : Argument(arg_name, required, arg_type) {
    if (default_value != "--") _value = default_value;
};


void SingleValueArgument::setValue(std::string value) {
    _value = value;
};


std::string SingleValueArgument::getValue() {
    return _value;
};


MultiValueArgument::MultiValueArgument(std::string arg_name, bool required,  int min_vals, int max_vals, SUPPORTED_ARG_TYPES arg_type) : Argument(arg_name, required, arg_type) {
    _min_vals = min_vals;
    _max_vals = max_vals;
};


void MultiValueArgument::pushValue(std::string value) {
    _values.push_back(value);
};


int MultiValueArgument::checkValuesCount() {
    int values_count = _values.size();

    if (values_count < _min_vals || (values_count > _max_vals && _max_vals != -1)) return 1;
    return 0;
};


std::vector<std::string> MultiValueArgument::getValues() {
    return _values;
};


void Parser::printArgumentList() {
    std::cout << "Positional Arguments:" << std::endl;
    for (auto & posArg : posArgs) posArg.printInfo();
    std::cout << "Single Value Keyword Arguments:" << std::endl;
    for (auto & keySingleValArg : singleValKeyArgs) keySingleValArg.second->printInfo();
    std::cout << "Multi Value Keyword Arguments:" << std::endl;
    for (auto & keyMultiValArg : multiValKeyArgs) keyMultiValArg.second->printInfo();

};

void SingleValueArgument::printInfo() {
    std::cout << "~~Single Value Argument: " << _name << ", Value: " << _value << ", Required: " << (_required?"True":"False") << std::endl;
}

void MultiValueArgument::printInfo() {
    std::cout << "~~Multi Value Argument: " << _name << ", Values: [";
    for (std::string val : _values) std::cout << val << ", ";
    std::cout << "], Required: " << (_required?"True":"False") << ", Min Vals: "<< _min_vals << ", Max Vals:" << _max_vals << std::endl;
}