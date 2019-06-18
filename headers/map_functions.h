#ifndef MAPREDUCE_MAP_FUNCTIONS_H
#define MAPREDUCE_MAP_FUNCTIONS_H

//auto input_list = {"../input.txt", "../input3.txt"};

typedef std::map<std::string, int> map;

template <typename T>
T ConvertString( const std::string &data );

template <typename return_T>
std::vector<return_T> file_reading(const std::string &filename);

template <typename Vect_type>
void resultWriter(Vect_type vect);

std::string read_txt(std::string filename);

std::vector<std::string> preprocess(std::string text);

map words_count(std::string text);

map merge(map first, map second);

map words_count_from_file(std::string filename);

#endif //MAPREDUCE_MAP_FUNCTIONS_H
