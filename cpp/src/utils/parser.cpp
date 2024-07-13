#include "parser.hpp"

class ParserException : public std::exception {
private:
    std::string _message;
public:
    ParserException(std::string exception_message) : _message(exception_message) {};
    std::string message() {return _message;};
};


void Parser::addSingleValueArgument(std::string arg_key, bool required, std::string default_val, SUPPORTED_ARG_TYPES arg_type) {
    try {
        if (posArgs.size() > 0) {
            if (!(posArgs[singleValKeyArgs.size() - 1]->isRequired()) && required) {

                throw ParserException("Developer Error. Unable to add positional argument. Cannot have a required positional argument after a non-required positional argument.");
            }
        }
    } catch(ParserException pe) {
        std::cout << pe.message() << std::endl;
        exit(-1);
    }
    SingleValueArgument *svarg = new SingleValueArgument(arg_key, required, default_val, arg_type);
    if (arg_key[0] == '-' && arg_key[1] == '-') singleValKeyArgs.insert({arg_key, svarg});
    else posArgs.push_back(svarg);

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
}


void Parser::parseArgs(int argc, char **argv) {
    if (argc == 1) {
        _verifyRequiredArgumentsSet(argc);
        return;
    }

    bool doneWithPositionArgs = false;
    int i = 1;
    int keyArgStartIndex = 0;
    while (i < argc) {
        if (argv[i][0] == '-' && !doneWithPositionArgs) {
            doneWithPositionArgs = true;
            keyArgStartIndex = i;
        }
        else if (!doneWithPositionArgs) {
            try {
                if (posArgs.size() <= i - 1) {
                    throw ParserException("Too many positional arguments given. Expected " + std::to_string(posArgs.size()) + " but received at least " + std::to_string(i) + " argument" + (i == 1 ? "" : "s") + ".");
                }
                posArgs[i-1]->setValue(argv[i]);
            } catch (ParserException pe) {
                std::cout << pe.message() << std::endl;
                exit(-1);
            }
        }
        else if (argv[i][0] == '-') {
            _parseKeyArg(argv, keyArgStartIndex, i-1);
            keyArgStartIndex = i;
        }
        i++;
    };

    if (i == argc && doneWithPositionArgs) {
        _parseKeyArg(argv, keyArgStartIndex, i-1);
    }

    _verifyRequiredArgumentsSet(argc);
    return;
}


void Parser::_parseKeyArg(char **argv, int arg_key_index, int last_value_index) {
    try {
        if (arg_key_index >= last_value_index){
            std::string output_message = "Unable to properly parse keyword argument ";
            output_message += argv[arg_key_index];
            output_message += ". Expected 1 value to be passed, but 0 were given.";
            throw ParserException(output_message);
        };
    } catch (ParserException pe) {
        std::cout << pe.message() << std::endl;
        exit(-1);
    }
        
    if (singleValKeyArgs.find(argv[arg_key_index]) != singleValKeyArgs.end()) {
        singleValKeyArgs[argv[arg_key_index]]->setValue(argv[last_value_index]);
        return;
    }

    if (multiValKeyArgs.find(argv[arg_key_index]) != multiValKeyArgs.end()) {
        for (int i = arg_key_index + 1; i <= last_value_index; i++) {
            multiValKeyArgs[argv[arg_key_index]]->pushValue(argv[i]);
        }
        multiValKeyArgs[argv[arg_key_index]]->checkValuesCount();
        return;
    }
}


void Parser::_verifyRequiredArgumentsSet(int argc) {
    try {
        for (SingleValueArgument *const req_svarg_ptr : requiredSingleValArgsPtrs) {
            if (req_svarg_ptr->getValue().empty()) {
                std::string error_message;
                if (singleValKeyArgs.find(req_svarg_ptr->getName()) != singleValKeyArgs.end())
                    error_message = "Keyword argument " + req_svarg_ptr->getName() + " is required but not given.";
                else {
                    std::string arg_name = req_svarg_ptr->getName();
                    for (int i = 0;i < posArgs.size(); i++) {
                        if (arg_name == posArgs[i]->getName()) {
                            error_message = "Not enough positional arguments given. Argument " + arg_name + " is required at position " + std::to_string(i+1);
                            error_message = error_message +  " but " + (argc-1 == 0 ? "" : "only ") + std::to_string(argc - 1) + (argc-1 == 1 ? " argument was" : " arguments were") + " given.";
                        }
                    }
                }
                throw ParserException(error_message);
            }
        }
    } catch (ParserException pe) {
        std::cout << pe.message() << std::endl;
        exit(-1);
    }
    for (MultiValueArgument *const req_mvarg_ptr : requiredMultiValArgsPtrs) {
        req_mvarg_ptr->checkValuesCount();
    }
}


std::vector<std::string> Parser::operator[] (std::string argument) {
    if (multiValKeyArgs.find(argument) != multiValKeyArgs.end()) return multiValKeyArgs[argument]->getValues();

    std::vector<std::string> output;
    if (singleValKeyArgs.find(argument) != singleValKeyArgs.end()) {
        output.push_back(singleValKeyArgs[argument]->getValue());
        return output;
    }

    for (SingleValueArgument* posArg : posArgs) {
        if (posArg->getName() == argument) {
            output.push_back(posArg->getValue());
            return output;
        }
    }
    return output;
}


Argument::Argument(std::string arg_name, bool required, SUPPORTED_ARG_TYPES arg_type) {
    _name = arg_name;
    _required = required;
    if (arg_type >= END_OF_SUPPORTED_ARG_TYPES || arg_type < 0) _arg_type=STRING_ARG;
    else _arg_type=arg_type; 
}


std::string Argument::getName() { return _name;};


SingleValueArgument::SingleValueArgument(std::string arg_name, bool required, std::string default_value, SUPPORTED_ARG_TYPES arg_type) : Argument(arg_name, required, arg_type) {
    if (default_value != "--") _value = default_value;
}


void SingleValueArgument::setValue(std::string value) {
    _value = value;
}


std::string SingleValueArgument::getValue() {
    return _value;
}


bool SingleValueArgument::isRequired() {
    return _required;
}


MultiValueArgument::MultiValueArgument(std::string arg_name, bool required,  int min_vals, int max_vals, SUPPORTED_ARG_TYPES arg_type) : Argument(arg_name, required, arg_type) {
    _min_vals = min_vals;
    _max_vals = max_vals;
}


void MultiValueArgument::pushValue(std::string value) {
    _values.push_back(value);
}


void MultiValueArgument::checkValuesCount() {
    int values_count = _values.size();

    try {
        if (values_count < _min_vals || (values_count > _max_vals && _max_vals != -1)) {
            std::string output_message;
            if (_max_vals == -1) output_message = "Argument " + _name + " requires at least " + std::to_string(_min_vals) + " values, but only " + std::to_string(values_count) + (values_count == 1 ? " was" : " were") + " given.";
            else {
                output_message = "Argument " + _name + (_required ? " is required and" : "") + " requires between " + std::to_string(_min_vals) + " and " + std::to_string(_max_vals) + " values, but ";
                output_message += (values_count < _min_vals && values_count != 0 ? "only " : "") + std::to_string(values_count) + (values_count == 1 ? " was" : " were") + " given.";
            }
            throw ParserException(output_message);
        }
    } catch (ParserException pe) {
        std::cout << pe.message() << std::endl;
        exit(-1);
    }
}


std::vector<std::string> MultiValueArgument::getValues() {
    return _values;
}


void Parser::printArgumentList() {
    std::cout << "Positional Arguments:" << std::endl;
    for (auto & posArg : posArgs) posArg->printInfo();
    std::cout << "Single Value Keyword Arguments:" << std::endl;
    for (auto & keySingleValArg : singleValKeyArgs) keySingleValArg.second->printInfo();
    std::cout << "Multi Value Keyword Arguments:" << std::endl;
    for (auto & keyMultiValArg : multiValKeyArgs) keyMultiValArg.second->printInfo();

}

void SingleValueArgument::printInfo() {
    std::cout << "~~Single Value Argument: " << _name << ", Value: " << _value << ", Required: " << (_required?"True":"False") << std::endl;
}

void MultiValueArgument::printInfo() {
    std::cout << "~~Multi Value Argument: " << _name << ", Values: [";
    for (std::string val : _values) std::cout << val << ", ";
    std::cout << "], Required: " << (_required?"True":"False") << ", Min Vals: "<< _min_vals << ", Max Vals:" << _max_vals << std::endl;
}