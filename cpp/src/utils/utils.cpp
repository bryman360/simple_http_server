#include "utils.hpp"

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


std::string os_path_join(std::string path1, std::string path2) {
    if (path1 == "") {
        if (path2[0] != '/') return "./" + path2;
        else return path2;
    }
    if (path2 == "") {
        if (path1[0] != '/') return "./" + path1;
        else return path1;
    }
    if (path2[0] == '/') return path2;


    std::string output;
    if (path1[0] != '/') output += "./";
    output += path1;
    if(path1[path1.size() - 1] != '/') output += "/";
    output += path2;
    return output;

}