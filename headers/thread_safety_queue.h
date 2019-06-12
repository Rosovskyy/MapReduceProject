
#ifndef MAPREDUCE_THREAD_SAFETY_QUEUE_H
#define MAPREDUCE_THREAD_SAFETY_QUEUE_H

#include <iostream>
#include <vector>
#include <mutex>
template <typename T>
class QueueNode{
private:
    T data;
    QueueNode *pntr = nullptr;
public:
    QueueNode(){};
    QueueNode(T dat){data = dat;};
    QueueNode* getLink(){return pntr;}
    void setValue(T value){data = value;}
    void setLink(QueueNode *link) { pntr = link;}
    T getValue(){return data;}
};

template <typename Input_T>
class SafetyQueue{
private:
    QueueNode<Input_T> *tail = nullptr;
    QueueNode<Input_T> *head = nullptr;
    size_t len = 0;
    std::mutex mtx;
public:
    bool isEmpty(){
        std::lock_guard<std::mutex> lock(mtx);
        return head==nullptr;
    }
    void enque(Input_T val){
        std::lock_guard<std::mutex> lock(mtx);
        if (head != nullptr){
            auto *pointer = new QueueNode<Input_T>(val);
            tail->setLink(pointer);
            tail = pointer;
        }else{
            auto *new_node = new QueueNode<Input_T>(val);
            head = new_node;
            tail = new_node;
        }
        len++;
    };
    Input_T deque(){
        std::lock_guard<std::mutex> lock(mtx);
        auto old_head = head->getValue();
        auto *new_head = head->getLink();
        delete head;
        head = new_head;
        len--;
        return old_head;
    };
    void printContent(){
        std::lock_guard<std::mutex> lock(mtx);
        auto iterator = head;
        while(iterator != nullptr){
            std::cout << iterator->getValue();
            iterator = iterator->getLink();
        }
    };
    void printSize(){
        std::lock_guard<std::mutex> lock(mtx);
        std::cout<<len<<std::endl;
    };

};
#endif //MAPREDUCE_THREAD_SAFETY_QUEUE_H
