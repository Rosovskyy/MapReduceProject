#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <iterator>
#include <sstream>
#include <fstream>
#include <map>
#include <boost/locale.hpp>
//#include "map_functions.h"


typedef std::map<std::string, int> map;
std::string read_txt(std::string filename) {
    std::vector<std::string> temp_words_list;
    std::ifstream in(filename, std::ios_base::out);
    auto ss = std::ostringstream{};
    ss << in.rdbuf();
    return ss.str();
}


std::vector<std::string> preprocess(std::string text) {
    std::vector<std::string> words_list;
    boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, text.begin(), text.end());
    map.rule(boost::locale::boundary::word_any);
    for (auto it = map.begin(), e = map.end(); it != e; it++) {
        words_list.emplace_back(*it);
    }
    for (auto &el:words_list) {
        el = boost::locale::fold_case(boost::locale::normalize(el));
    }
    return words_list;
}


map words_count(std::string text) {
    map res;
    auto words_list = preprocess(text);
    for (auto &w:words_list) {
        ++res[w];
    }
    return res;
}

map merge(map first, map second) {
    for (auto &v: second) first[v.first] += v.second;
    return first;
}

map words_count_from_file(std::string filename) {
    map res;
    auto text = read_txt(filename);
    auto words_list = preprocess(text);
    for (auto &w:words_list) {
        res[w] += 1;
    }
    return res;
}

template <typename T>
T ConvertString( const std::string &data )//doesnt work for char and types like uint_8
{
    if( !data.empty( ))
    {
        T ret;
        std::istringstream iss( data );
        if( data.find( "0x" ) != std::string::npos )
        {
            iss >> std::hex >> ret;
        }
        else
        {
            iss >> std::dec >> ret;
        }

        if( iss.fail( ))
        {
            std::cout << "Convert error: cannot convert string '" << data << "' to value" << std::endl;
            return T( );
        }
        return ret;
    }
    return T( );
}

template <typename return_T>
std::vector<return_T> file_reading(const std::string &filename){
    std::ifstream h(filename, std::ios_base::out);
    if (!h.is_open()) throw std::exception();
    auto ss = std::ostringstream{};
    ss << h.rdbuf();
    std::istringstream iss(ss.str());
    std::vector<std::string> vector_str((std::istream_iterator<std::string>(iss)),
                                        std::istream_iterator<std::string>());

    size_t size = vector_str.size();
    std::vector<return_T> result_vector(size);
    for (size_t i = 0; i < size; ++i){
        result_vector[i] = ConvertString<return_T>(vector_str[i]);
    }

    return result_vector;

}

template <typename Vect_type>
void resultWriter(Vect_type vect){
    std::ofstream os("../output.txt");
    if (!os.is_open()){
        std::cout << "File wasn't opened";
    }
    for (auto &i: vect){
        os << i;
        os << " ";
    }
    os.close();
}