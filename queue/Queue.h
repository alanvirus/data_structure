#include "../List/List.h"
template <typename T> class Queue: public List<T>{
    public:
        void enqueue(T const& e){
            this->insertAsLast(e);
        } 
        T dequeue() {
            return remove(first());
        }
        T& front(){
            return first()->data;
        }
};

