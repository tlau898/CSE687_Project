//
// Created by John Schurman on 4/20/19.
//

#ifndef PROJ1GIT2_NODE_H
#define PROJ1GIT2_NODE_H

template <class T>
class Node{
public:
    Node()= default;

    Node(T& _data) : data(_data), next(nullptr){}
    T& data;
    //Member is made public for convience
    //could make private and create getNext() method
    Node<T>* next;








};

#endif //PROJ1GIT2_NODE_H
