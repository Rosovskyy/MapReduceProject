#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include "time.h"

std::vector<std::vector<int>> friends = { { 0, 1, 0, 1, 1, 0, 0, 0 },
                                { 0, 0, 0, 1, 0, 0, 0, 1 },
                                { 0, 0, 0, 1, 0, 1, 0, 0 },
                                { 0, 0, 0, 0, 1, 0, 0, 1 },
                                { 0, 0, 0, 0, 0, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0, 1 },
                                { 0, 0, 0, 0, 0, 0, 0, 0 } };
std::mutex mut;
 template<typename Itr>
void f_friends(std::vector<std::vector<int>>::iterator begin, std::vector<std::vector<int>>::iterator end, int n, Itr res_begin){
    std::vector<int> friends_list;
    std::map<std::tuple<int, int>, std::vector<int>> pair_map;
    for(auto itr = begin;itr!=end;itr++) {
        int k = 0;
        for (auto itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++, k++) {
            if (*itr2 != 0) friends_list.push_back(k);
        }
        for (int i = 0; i < friends_list.size(); i++) {
            pair_map.insert(std::pair<std::tuple<int, int>, std::vector<int>>(std::tuple<int, int>(n, friends_list[i]),
                                                                              friends_list));
        }
        mut.lock();
        *res_begin = pair_map;
        res_begin++;
        mut.unlock();
        pair_map.clear();
        friends_list.clear();
        n++;
    }
}

template<typename Fn, typename Itr1, typename Itr2>
void mymap(Fn fn, Itr1 begin, Itr1 end, Itr2 res_begin){
    int i = 0;
    int threads_number = 4;
    std::vector<std::thread> threads;
    int size = end - begin;
    int rem = size%threads_number;
    int step = (size-rem)/threads_number;
    for(auto itr = begin; itr != end;i++){
        threads.emplace_back(std::thread(fn, itr, itr+step+rem, i, res_begin));
        itr+=step+rem;
        res_begin+=step+rem;
        rem = 0;
    }

    for(auto &t:threads){
        t.join();
    }
}

int main(){
    auto start = get_current_time_fenced();
    std::vector<std::map<std::tuple<int, int>, std::vector<int>>> result(8);
    mymap(f_friends<std::vector<std::map<std::tuple<int, int>, std::vector<int>>>::iterator>, friends.begin(), friends.end(), result.begin());
    auto finish = get_current_time_fenced();
    std::cout<<to_us(finish-start);

}