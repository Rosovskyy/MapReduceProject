#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <map>
#include <future>
#include <math.h>


template<typename T>
void print(std::vector<T> const &v) {
    for (auto i: v) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
}

void printMap(std::map<std::string, int> map) {
    for (auto el : map) {
        std::cout << el.first << " " << el.second << std::endl;
    }
}


struct universalVector {
private:
    template<class T>
    static std::unordered_map<const universalVector *, std::vector <T>> items;
public:
    template<class T>
    void push_back(const T &_t) {
        items<T>[this].push_back(_t);
    }
};

template<class T>
std::unordered_map<const universalVector *, std::vector < T>> universalVector::items;


std::vector<std::string> read_file(std::string const &path) {
    std::vector<std::string> res;
    std::string num_1;

    std::ifstream file(path);
    if (file.is_open()) {
        while (file >> num_1) {
            res.push_back(num_1);
        }
        file.close();
    } else {
        throw std::invalid_argument("File not found");
    }
    if (res.empty()) {
        throw std::invalid_argument("File is empty");
    }
    return res;
}


std::map<std::string, int> wordCount(std::vector<std::string> data) {//, std::vector<std::map<std::string, int>> &res) {
    std::map<std::string, int> word_map;

    for(const std::string & word : data) {
        word_map[word]++;
    }

    printMap(word_map);

    return word_map;
}


void map(std::map<std::string, int> (*func)(std::vector<std::string>), std::string input) {//, std::vector<std::map<std::string, int>>), std::string input) {
    std::vector <std::string> data = read_file(input);
    unsigned int cores = std::thread::hardware_concurrency();

    std::vector<std::vector<std::map<std::string, int>>> res(cores, std::vector<std::map<std::string, int>>());
    std::vector <std::thread> threads;

    int step = floor(data.size() / cores);
    int start = 0, finish = 0;
    for (int i = 0; i < cores - 1; ++i) {
        threads.emplace_back(wordCount, slice(data, start, finish += step));
        start = finish + 1;
        finish++;
    }

    // std::map<std::string ,int> something;
    // threads.emplace_back([&], {something = wordCount(slice(data, finish, data.size()));});
    threads.emplace_back(wordCount, slice(data, finish, data.size()));

    for (int j = 0; j < cores; ++j) {
        threads[j].join();
    }

}

int main() {

    map(wordCount, "../input.txt");

    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Number of cores: " << cores / 2 << std::endl;

    return 0;
}


