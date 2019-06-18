#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <cmath>
#include <mutex>
#include <fstream>
#include <sstream>
#include <iterator>



template <typename Itr1, typename Itr2>
void arePrime(Itr1 begin, Itr1 end,  Itr2 result, std::mutex &m){
    std::vector<bool> results(end - begin);
    bool current_result = true;

    for (auto i = begin; i != end; ++i){
        if (*i % 2 == 0){
            current_result = false;
        }else{
            for (auto j = 2; j <= *i / 2; ++j){
                if (*i % j == 0) {
                    current_result = false;
                    break;
                }
            }
        }
        {
            std::lock_guard<std::mutex> lock(m);
            current_result ? *result = true : *result = false;
        }
        ++result;
        current_result = true;
    }
}
template<typename Fn, typename Itr1, typename Itr2>
void map(Fn *fn, Itr1 begin, Itr1 end, Itr2 res_begin){

    int am_of_threads = std::thread::hardware_concurrency(),
            size = end - begin, step = size / am_of_threads, left;
    std::mutex m;
    std::vector<bool> results(size);
    std::vector<std::thread> thread_container;
    thread_container.reserve(am_of_threads);
    Itr1 itr = begin;

    for (size_t i = 0; i< am_of_threads; ++i){
        (i == am_of_threads - 1) ? left = size % am_of_threads : left = 0;
        thread_container.emplace_back(fn, itr ,itr + step + left, res_begin,std::ref(m));
        itr += step;
        res_begin+= step;
    }

    size_t len = thread_container.size();
    for (size_t i = 0; i < len; ++i){
        thread_container[i].join();
    }

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
    if (!h.is_open()) throw "Cannot open file";
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


int main() {
    try {
        std::vector<int> numbers = file_reading<int>("../input.txt");
        std::vector<bool> result(numbers.size());

        map(arePrime<std::vector<int>::iterator, std::vector<bool>::iterator>,
            numbers.begin(),numbers.end(),result.begin());

        for (size_t i = 0; i < result.size(); ++i){
            std::cout << result[i]<< " ";
        }
    }catch (std::string &e){
        std::cout << e;
    }

    return 0;
}