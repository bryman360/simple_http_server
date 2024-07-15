#include "utils.hpp"


#define COMPRESSION_BUFF_SIZE 65536


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

/*
Steps for gzip encoding:
1. Create a z_stream
2. Initiate the deflation using deflateInit2 (Note: for gzip-specific format, set windowBits = 15+16)
3. Set the z_stream next_in (to be a <Bytef *> to the input's first char address) and avail_in (to be size of input)
4. Allocate a character buffer that we will compress to, an output string, and an int variable holding the compression status
5. Perform the compression
    a. Assign z_stream next_out (to be a <Bytef *> to the compression buffer) and avail_out (to be the size of that buffer)
    b. Perform the deflate by pointing to the z_stream address and using Z_FINISH so it deflates all the data 
    c. Append the data from the compression buffer onto the output string
    d. Repeat all of the compression steps (Step 5) if the return value from the deflate is Z_OK (means more deflation needed)
6. Perform a deflateEnd using the z_stream
*/
std::string gzip_encoding(std::string str_to_be_encoded) {
    z_stream zstr;
    zstr.zalloc = Z_NULL;
    zstr.zfree = Z_NULL;
    zstr.opaque = Z_NULL;

    char compression_buffer[COMPRESSION_BUFF_SIZE];
    zstr.next_in = (Bytef *)str_to_be_encoded.data();
    zstr.avail_in = str_to_be_encoded.size();

    int ret;
    std::string outstring;

    if (deflateInit2(&zstr, Z_BEST_COMPRESSION, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error("deflateInit2 failed to initialize compression");
    }

    do {
        zstr.next_out = reinterpret_cast<Bytef *>(compression_buffer);
        zstr.avail_out = sizeof(compression_buffer);

        ret = deflate(&zstr, Z_FINISH);

        if (outstring.size() < zstr.total_out) {
            outstring.append(compression_buffer, zstr.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zstr);

    if (ret != Z_STREAM_END) {
        throw std::runtime_error("Failed compression of all data, deflate status return code (" + std::to_string(ret) + ")");
    }

    return outstring;
}