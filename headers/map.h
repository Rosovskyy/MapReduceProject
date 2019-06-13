//
// Created by vaitovich on 6/13/2019.
//

#ifndef MAPREDUCE_MAP_H
#define MAPREDUCE_MAP_H
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include "../headers/time.h"
#include <functional>
#include <boost/locale.hpp>
#include <boost/locale/boundary.hpp>
#include "boost/asio.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/thread.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/thread/detail/thread_group.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/thread_pool.hpp>
#include "../headers/map.h"
#include "../headers/map_functions.h"

class MapReduce {
private:
    int threads_number;
    std::vector<std::thread> threads;
    std::mutex mtx;
    boost::asio::thread_pool pool;
    boost::asio::thread_pool pool2;


    template<typename Fn, typename Itr, typename Data2>
    static void parallel_map(Fn fn, std::mutex &mtx, int start, int finish, Itr input, Data2 &result) {
        Data2 local_result{};
        for (auto i = start; i < finish; ++i, input++) {
            local_result.emplace_back(fn(*input));
        }
        std::lock_guard lock(mtx);
        for (auto &v: local_result) result.push_back(v);
    }

    template<typename Fn, typename Data, typename Data2>
    static void parallel_reduce(Fn fn, Data &data, std::mutex &mtx, size_t start, size_t step, Data2 &result) {
        Data2 local_result{};
        auto begin = data.begin();
        auto end = data.end();
        for (auto i = start; i < (start + step), (begin + 1) != end; i++, begin++) {
            local_result.emplace_back(fn(*begin, *(begin + 1)));
        }
        std::lock_guard lock(mtx);
        for (auto &v: local_result) result.push_back(v);
    }

public:
    explicit MapReduce(int threads_num = 1) : pool(threads_num), pool2(threads_num) {
        threads_number = threads_num;
    }

    template<typename Fn, typename Data, typename Data2>
    void mapped(Fn fn, Data &data, Data2 &result) {
        auto begin = data.begin();
        auto end = data.end();
        const int size = end - begin;
        int rem = size % threads_number;
        int start = 0;
        const int step = (size - rem) / threads_number;
        size_t i = 0;
        typedef typename Data::iterator Itr;
        for (auto itr = begin; itr != end; ++i) {
            boost::asio::post(pool, boost::bind(parallel_map<Fn, Itr, Data2>, fn, std::ref(mtx), start, start + step + (rem > 0), itr,
                                                std::ref(result)));
            itr += step + (rem > 0);
            rem--;
            start += step + (rem > 0);
        }
        pool.join();
    }

    template<typename Fn, typename Data, typename Data2>
    void reduce(Fn fn, Data &data, Data2 &result) {
        // data must be vector of elements to be reduced
        // result must be type having push_back method
        int rem = data.size() % threads_number;
        size_t step = (data.size() - rem) / threads_number;
        size_t start = 0;
        for (auto i = 0; i < threads_number, i + 1 < data.size(); ++i) {
            boost::asio::post(pool2,boost::bind(parallel_reduce<Fn, Data, Data2>, fn, std::ref(data), std::ref(mtx), start,
                                                step + (rem > 0), std::ref(result)));
            start += step + (rem > 0);
            rem--;
        }
        pool2.join();
    }

    template<typename Fn, typename Itr1>
    void map(Fn *fn, Itr1 begin, Itr1 end){

        int am_of_threads = 1,
                size = end - begin, step = size / am_of_threads, left;

        std::mutex m;
        std::vector<bool> results(size);
        std::vector<std::thread> thread_container;
        thread_container.reserve(am_of_threads);
        Itr1 itr = begin;

        for (size_t i = 0; i< am_of_threads; ++i){
            (i == am_of_threads - 1) ? left = size % am_of_threads : left = 0;
            thread_container.emplace_back(fn, itr ,itr + step + left,std::ref(m));
            itr += step;
        }

        size_t len = thread_container.size();
        for (size_t i = 0; i < len; ++i){
            thread_container[i].join();
        }

    }
};

#endif //MAPREDUCE_MAP_H
