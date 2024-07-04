#include <vector>
#include <string>


std::vector<std::string> split_string(std::string str, std::string delimiter, int max_num_of_splits) {
    std::vector<std::string> output;
    int start, end = -1*delimiter.size();
    int num_of_splits = 0;
    if (max_num_of_splits == -1) max_num_of_splits = str.length();
    do {
        start = end + delimiter.size();
        end = str.find(delimiter, start);
        output.push_back(str.substr(start, end - start));
        num_of_splits += 1;
    } while (end != -1 && num_of_splits < max_num_of_splits);
    if (end != -1) output.push_back(str.substr(end + delimiter.size(), str.length() - 1));
    return output;
};