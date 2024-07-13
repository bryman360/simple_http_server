#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <string.h>


std::vector<std::string> split_string(std::string str, std::string delimiter = " ", int max_num_of_splits = -1);

//Currently only handles Linux pathing
std::string os_path_join(std::string path1, std::string path2);