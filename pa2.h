#include <iostream>
#include <string>

using namespace std;

/*
 * Resources: https://www.youtube.com/watch?v=H5lkmKkfjD0&t=173s - Linked List tutorial
 *            Also used help from the posted lab documents on Canvas
 *
 *
 */


void printMenu();

class linkedList {
private:
    struct node {
        string data;
        node* next;
    };

    typedef struct node* nodePtr;

    nodePtr head;
    nodePtr curr;
    nodePtr temp;

public:
    linkedList();
    void add(string name);
    void printList();
    void createPages();
    bool checkDuplicate(string name);
    void insertBest(string name, int size, string algortithm);
    void insertWorst(string name, int size, string algorithm);
    void kill(string name);
    int countFrag();

};