#include <mutex>
#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <thread>

template <typename Iter_T>
void sorting(Iter_T beg, Iter_T end){
    std::sort(beg, end);
}

template <typename Iter_T>
struct Test_struct{
    std::function<void(Iter_T,Iter_T)> method = sorting<Iter_T>;
    Test_struct operator()(Iter_T begin, Iter_T end){
        return method(begin, end);
    }
};

template <typename Fn_T,typename Iter_T>
void parallelize(Fn_T *fn, Iter_T begin, Iter_T end){
    int am_of_threads = std::thread::hardware_concurrency(),
            size = end - begin, step = (size / am_of_threads), left;
    if (am_of_threads > size) {step = 1; am_of_threads = size;}
    std::vector<std::thread> thread_container;
    thread_container.reserve(am_of_threads);
    auto iter = begin;

    for (size_t i = 0; i < am_of_threads; ++i){
        (i == am_of_threads - 1) ? left = size % am_of_threads : left = 0;
        thread_container.emplace_back(fn, iter, iter + step + left);
        iter += step;
    }
    size_t len = thread_container.size();
    for (size_t i = 0; i < len; ++i){
        thread_container[i].join();
    }
}

template<typename Fn_T,typename Iter_T>
/*Map on Iterators*/
void map(Fn_T *fn, Iter_T begin, Iter_T end){
    parallelize(fn, begin, end);
}

template<typename Fn_T,typename Array_T>
/*Map on Arrays/Vectors*/
void map(Fn_T *fn, Array_T &array){
    parallelize(fn, array.begin(), array.end());
}


//int main() {
//    std::vector<int> input_vector{0,-3,4,10,-7,12,4,13,42,12,41,2,451,0}, input_vector2(input_vector);
//    map(sorting<std::vector<int>::iterator>, input_vector);
//    for (auto &i: input_vector){
//        std::cout << i<<" ";
//    }
//    std::cout << "\n";
//    map(sorting<std::vector<int>::iterator>, input_vector2.begin(), input_vector2.end());
//    for (auto &i: input_vector){
//        std::cout << i<<" ";
//    }
//}