#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <map>
#include <future>
#include <math.h>



template <typename first, typename second>
void printMap(std::map<first, second> map) {
    for (auto el : map) {
        std::cout << el.first << " " << el.second << std::endl;
    }
}

std::map<std::string, int> merge_two_maps(std::map<std::string, int> first, std::map<std::string, int> second) {//, std::vector<std::map<std::string, int>> &res) {
    for (auto el : first) {
        if (!(second.find(el.first) == second.end())) {
            first[el.first] += second[el.first];
        }
    }

    for (auto el : second) {
        if (first.find(el.first) == first.end()) {
            first[el.first] = el.second;
        }
    }

    return first;
}


template <typename Data, typename Fn>
Data reduced(std::vector<Data> res, Fn fn) {

    Data result = res[0];
    for (int i = 1; i < res.size(); ++i) {
        result = merge_two_maps(result, res[i]);
    }

    return result;

}

template <typename Data, typename Fn>
Data reduce(std::vector<Data> data, Fn fn) {

    unsigned int thread_num = std::thread::hardware_concurrency() * 2;
    std::vector<std::thread> all_threads;
    std::mutex m;


    std::vector<Data> results = data;

    int start = 0, finish = results.size() - 1;
    int length = finish - start;


    int number = floor(length / 2);
    int threadStart = 0, threadFinish = number;
    if (length % 2 == 0) {
        number++;
    }

    std::cout << number << std::endl;

    for (int i = 0; i < ceil(log2(data.size())); ++i) {
        int count = 0;
        for (int j = start; j < finish; j+=2) {
            if (j + 1 == results.size()) {
                count += 1;
                continue;
            }
            all_threads.emplace_back(fn, results[j], results[j+1], std::ref(results), std::ref(m));
        }

        for (int k = threadStart; k < threadFinish; ++k) {
            all_threads[k].join();
        }
        start = finish - count;
        finish += finish - count;
    }

    Data result = results[result.size() - 1];

    return result;

}

int main() {
    std::map<std::string, int> data1;
    data1["world"] = 2;
    data1["hello"] = 3;
    data1["allah"] = 1;
    std::map<std::string, int> data2;
    data2["world"] = 4;
    data2["hello"] = 1;
    data2["omg"] = 1;
    std::vector<std::map<std::string, int>> vec = {data1, data2};

    printMap(reduced(vec, merge_two_maps));

    return 0;
}


