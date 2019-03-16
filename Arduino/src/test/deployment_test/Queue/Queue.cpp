#include "Queue.h"
#include <stdlib.h>

//Constructors
Queue :: Queue() {
    _max_size = MAX_SIZE_DEFAULT;
}
Queue :: Queue(int max_size) {
    _max_size = max_size;
}
void Queue :: init() {
    this->store = (int *)malloc(this->_max_size * sizeof(int));
    this->_size = 0;
    this->_start = 0;
}

//Getters
int* Queue :: getQueue() {
    return this->store;
}
int Queue :: getSize() {
    return this->_size;
}

//Methods
void Queue :: enq(int val) {
    if (this->_size == this->_max_size) {
        //blew the memory
        return;
    }
    this->store[(this->_start + this->_size) % this->_max_size] = val;
    this->_size += 1;
    return;
}

int Queue :: deq() {
    if (this->_size == 0) {
        return -1;
    }
    int r = this->store[this->_start];
    this->store[this->_start] = 0;
    this->_start = (this->_start + 1) % this->_max_size;
    this->_size -= 1;
    return r;
}

bool Queue :: isEmpty() {
    return (this->_size == 0);
//    if (this->_size == 0) {
//        return 1;
//    }
//    return 0;
}
