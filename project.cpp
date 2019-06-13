#include <iostream>
#include "headers/map.h"
auto input_list = {"../txt/input.txt", "../txt/input2.txt", "../txt/input3.txt", "../txt/input4.txt", "../txt/input5.txt", "../txt/input6.txt", "../txt/input7.txt", "../txt/input8.txt"};
int main() {
    std::string output = "../txt/output.txt";
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);
    std::wcout.imbue(loc);
    std::vector<map> result(8, map());
    std::vector<map> result2;
    MapReduce mapper{MapReduce(8)};
    auto start = get_current_time_fenced();
    mapper.mapped(words_count_from_file, input_list, result);
    mapper.reduce(merge, result, result2);
    auto finish = get_current_time_fenced();
    std::cout<<to_us(finish-start)<<std::endl;
    std::ofstream out_put(output);
    for (auto &v: result2) {
        for (auto &j: v) {
            out_put << j.first << " " << j.second << std::endl;
        }
    }
    out_put.close();
    return 0;
}
