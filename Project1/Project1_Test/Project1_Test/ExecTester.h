//
// Created by John Schurman on 4/20/19.
//

#ifndef PROJ1GIT2_EXECTESTER_H
#define PROJ1GIT2_EXECTESTER_H

#include <iostream>

#include "Node.h"

template <class T>
class ExecTester{
private:

    //head of the linked list
    //will be passed in to the executor function to test
    Node<T>*head;


public:
    void insert(T &);
    ExecTester<T>();
    ~ExecTester<T>();
    Node<T> * getHead(){return head;}
    ExecTester<T>(const ExecTester<T> & );



};

template <class T>
ExecTester<T>::ExecTester() {
    head= nullptr;
}

/*
 * destructor that frees all allocated nodes
 */
template <class T>
ExecTester<T>::~ExecTester() {
    if(head == nullptr)return;
    Node<T> *temp=head;
    while(temp!= nullptr){
        Node<T>*prev = temp;
        temp=temp->next;
        free(prev);
    }
}
template <class T>
ExecTester<T>::ExecTester(const ExecTester<T> &ET) {
    head = ET.head;
}
template <class T>
void ExecTester<T>::insert(T & data) {
    Node<T> * node = new Node<T>(data);
    if(head== nullptr){
        head=node;
        return;
    }
    Node<T> * temp = head;
    while(temp->next!= nullptr){
        temp=temp->next;

    }
    temp->next=node;
}
#endif //PROJ1GIT2_EXECTESTER_H
