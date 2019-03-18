#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE_DEFAULT 10

class Queue {
  //Internal variables
  private:
    int *store;
    int _max_size;
    int _size;
    int _start;

  public:
    //Constructors
    Queue();
    Queue(int max_size);
    void init();
    
    //Getters
    int* getQueue();
    int getSize();
    
    //Methods
    void enq(int val);
    int deq();
    bool isEmpty();
    char* toString();
};

#endif  /* QUEUE_H */
