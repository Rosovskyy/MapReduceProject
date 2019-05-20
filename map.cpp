#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include "time.h"
#include <functional>


std::vector<std::vector<int>> friends = { { 0, 1, 0, 1, 1, 0, 0, 0 },
                                          { 0, 0, 0, 1, 0, 0, 0, 1 },
                                          { 0, 0, 0, 1, 0, 1, 0, 0 },
                                          { 0, 0, 0, 0, 1, 0, 0, 1 },
                                          { 0, 0, 0, 0, 0, 1, 0, 0 },
                                          { 0, 0, 0, 0, 0, 0, 0, 0 },
                                          { 0, 0, 0, 0, 0, 0, 0, 1 },
                                          { 0, 0, 0, 0, 0, 0, 0, 0 } };
size_t m=0;
std::map<std::tuple<int, int>, std::vector<int>> f_friends(std::vector<int>& vec){
//    {std::mutex mut;
//    std::lock_guard{mut};
//    size_t n = m;
//    m++;}
    int n = 1;
    std::vector<int> friends_list;
    std::map<std::tuple<int, int>, std::vector<int>> pair_map;
    for(auto i=0;i<vec.size();i++){
        if(vec[i]!=0){
            friends_list.push_back(i);
        }
    }
    for (int i = 0; i < friends_list.size(); i++) {
        pair_map.insert(std::pair<std::tuple<int, int>, std::vector<int>>(std::tuple<int, int>(n, friends_list[i]),
                                                                          friends_list));
    }
    return pair_map;
}
std::vector<int> ffriend(std::vector<int> &vec, int n){
    n= n+1;
    std::vector<int> v;

    for(auto &e:vec) v.emplace_back((e+1)*n);

    return v;
}


template <typename Fn, typename Itr, typename Data2>
int mapp(Fn fn, int start, int finish,Itr input, Data2 &result, std::mutex &mut){
    for(auto i=start;i<finish;++i,input++){
        auto f = fn(*input);
//        result.insert(result.begin()+i,f);
    result[i] = f;
    }
    return 1;

}


template <typename Fn, typename Data, typename Data2>
void mapped(Fn fn, Data data, Data2 &result){

    auto begin = data.begin();
    auto end = data.end();

    int threads_number = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(threads_number);

    int size = end - begin;
    int rem = size%threads_number;
    int start = 0;
    int step = (size-rem)/threads_number;

    size_t i = 0;

    typedef typename Data::iterator Itr;

    std::mutex mut;

    try {
        for (auto itr = begin; itr != end; i++) {
            threads.emplace_back(mapp<Fn, Itr, Data2>, fn, start, start+step+(rem>0), itr, std::ref(result), std::ref(mut));
                itr += step + (rem > 0);
                rem--;
                start+=step+(rem>0);
        }

        for(auto &t:threads){
            t.join();
        }}catch (std::bad_alloc &e){
        std::cerr << e.what();
    }
}


int main(){

    std::map<std::tuple<int, int>, std::vector<int>> n;

    std::vector<std::map<std::tuple<int, int>, std::vector<int>>> result(8, n);

    mapped(f_friends, friends, result);

    return 0;
}