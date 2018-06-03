/*
 * Ava Nemanic
 * COP3503
 * March 19, 2018
 */



#include <iostream>
#include <string>
#include "pa2.h"
#include <ctgmath>
#include <iomanip>

using std::setw;

/*
 * Resources: http://www.cplusplus.com/reference/cmath/ceil/ - Calculates program size
 *      http://www.moreprocess.com/process-2/memory-management-using-linked-lists-first-next-best-worst-quick-fit-allocation-algorithms
 *
 */

using namespace std;


linkedList::linkedList() {
    head = nullptr;
    curr = nullptr;
    temp = nullptr;
}

//Adds program to list (Used to generate initial pages
//Resource: https://www.youtube.com/watch?v=H5lkmKkfjD0&t=173s - Linked list tutorial
void linkedList::add(string name) {
    nodePtr temp = new node;
    temp->next = nullptr;
    temp->data = name;

    if (head != nullptr) {
        curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = temp;
    } else {
        head = temp;
    }
}

//Creates 32 pages for the "empty" linked list at the beginning of the program
void linkedList::createPages() {
    for (int i = 0; i < 32; i++) {
        add("FREE");
    }
}


/*
 * Resources: http://faculty.cs.niu.edu/~mcmahon/CS241/c241man/node83.html - Output Formatting
 */
void linkedList::printList() {
    curr = head;
    int counter = 0; //Keeps track of number of nodes printed
    while (curr != nullptr) {
        if (counter == 8) {//Once 8 nodes printed, begins new line, resets counter
            cout << "\n";
            counter = 0;
        } else { //Used to format the printing of the list
            if (curr->data.length() < 4) { // Works well as long as string name is <= 4
                cout << curr->data << setw(5 - curr->data.length()) << " ";
                curr = curr->next;
                counter++;
            }
            else { //If name >4 just sets width as 1
                cout << curr->data << setw(1) << " ";
                curr = curr->next;
                counter++;
            }
        }
    }

}

//Kills selected program
void linkedList::kill(string name) {
    curr = head;
    int pagesReclaimed = 0;
    int freeSpace = 0; //Keeps track of number of spaces freed up

    //Loops until the end of the list
    while (curr->next != nullptr) {
        if (curr->data == name) { //If the current node contains the program, delete it and add a free space to the counter
            curr->data = "FREE";
            curr -> next;
            freeSpace++;
        }
        else { //Otherwise move on
            curr = curr->next;
        }
    }
    if (curr->data == name && curr->next == nullptr) { //Makes sure everything is deleted if program is at end of list
        curr->data = "FREE";
        freeSpace++;
    }

    //Just used for print statement
    if (freeSpace % 4 == 0) {
        pagesReclaimed = freeSpace / 4;
    } else {
        pagesReclaimed = (freeSpace / 4) + 1;
    }

    if (freeSpace != 0) {
        cout << "Program " << name << " successfully killed, " << pagesReclaimed << " pages reclaimed." << endl;
    } else {
        cout << "Program is not running" << endl;
    }
}

//Checks to see if the program is already running
bool linkedList::checkDuplicate(string name) {
    nodePtr temp = head; //Temporary node so nothing gets messed up
    while (temp->next != nullptr) {
        if (temp->data == name) { //Returns true if the current node has the same name as the desired program
            return true;
        }
        temp = temp->next;
    }
    return false;
}



/*Resources: https://www.geeksforgeeks.org/program-worst-fit-algorithm-memory-management/ - Worst-fit algorithm
 * https://www.geeksforgeeks.org/program-first-fit-algorithm-memory-management/ - Worst-fit algorithm
 */

//Uses worst-fit algorithm to add the new program to the list
//Finds the location of the largest amount of free space available in memory and adds the program there
void linkedList::insertWorst(string name, int size, string algorithm) {
    curr = head;

    int position = 0; //Final starting position of program
    int pages = 0; //Min amount of pages free
    int start = 0; //Starting index for particular program
    int index = 0; //Overall index
    int counter = 1; //Keeps track of spaces traversed

    //Loops until the end of the list
    while (curr->next != nullptr) {

        if (curr->data == "FREE") {
            //Resets the beginning index to the position after the last occupied node
            start = index;

            //Loops as long as the current node is free until the end of the list
            while (curr->data == "FREE") {
                if (curr->next == nullptr) {
                    break;
                }
                index++;
                counter++;
                curr = curr->next;
            }

            position = start;
            pages = counter;
            counter = 0; //resets the counter
        }

            //If memory location is occupied, loop until free space
        else {
            curr = curr->next;
            index++;
        }
    }

    int progSize = 0; //Used for print statement

    //Adds program to list if it will fit in amount of space available
    if (pages >= size) {
        curr = head;

        for (int i = 0; i < position; i++) {
            curr = curr->next; //Increments to the node at the beginning index of the free space
        }
        for (int i = 0; i < size; i++) {
            curr->data = name; //Sets the name of the current node to the desired program name for the amount of nodes specified in program size
            curr = curr->next;
        }

        //Used to get proper page # print statement
        if (size % 4 == 0) {
            progSize = size / 4;
        } else {
            progSize = (size / 4) + 1;
        }
        curr = nullptr; //Resets function for next program

        cout << "Program " << name << " added successfully: " << progSize << " page(s)"
                " used." << endl;
    }

    else { //If it doesn't fit, prints out error
        curr = nullptr;
        cout << "Error, Not enough memory for Program " << name << endl;
    }
}




//Determines position of smallest amount of space a program can fit into and puts it there
void linkedList::insertBest(string name, int size, string algorithm) {
    curr = head;
    int position = 0; //Final starting position where program will be added
    int pages = 32; //Maximum amount of pages available to be used
    int index = 0; //Position in the list
    int counter = 1; //Keeps track of position for each loop


    //Loops until the end of the list
    while (curr->next != nullptr) {
        if (curr->data == "FREE") {
            while (curr->data == "FREE") {
                //Specific case for the end of the list, ends loop after incrementing for end of list
                if (curr->data == "FREE" && curr->next == nullptr) {
                    index++;
                    counter++;
                    break;
                } //Otherwise just stop the loop
                else {
                    index++;
                    counter++;
                    curr = curr->next;
                }
            }
            pages = counter; //Sets number of available pages to what was calculated
            position = index - counter; //Sets position to memory location with smallest number of free space that will work
        }

            //If there is already a program running, loop until free space
        else {
            curr = curr->next;
            index++; //Increments index location but does not count free page
        }
        counter = 0; //Resets counter
    }

    int progSize = 0; //Used for print statement

    //Same method of adding program as the worst-fit algorithm
    if (pages >= size) {
        curr = head;
        for (int i = 0; i < position; i++) {
            curr = curr->next;
        }
        for (int i = 0; i < size; i++) {
            curr->data = name;
            curr = curr->next;
        }
        curr = nullptr;

        if (size % 4 == 0) {
            progSize = size / 4;
        } else {
            progSize = (size / 4) + 1;
        }

        cout << "Program " << name << " added successfully: " << progSize << " page(s)"
                " used." << endl;
        curr = nullptr;
    }
    else {
        curr = nullptr;
        cout << "Error, Not enough memory for Program " << name << endl;
        return;
    }
}


//Counts the number of fragments
int linkedList::countFrag() {
    curr = head;
    int fragCount = 0;

    //Checks if the head contains a program
    if (curr->data != "FREE") {
        fragCount++; //If it does, you know there is at least one fragment
        curr = curr->next;
    }

    //Loops as long as the next memory location isn't null
    while (curr->next != nullptr) {
        if (curr->data == "FREE") { //Checks if the memory location is available, if so cycles to the next node
            curr = curr->next;
            if (curr->data != "FREE") { //If next node is not free, we know there must be a fragment
                fragCount++;
                curr = curr->next;
            } else { //If it's still free, continue the loop
                continue;
            }
        } else { //If the location is not free, continue to the next node
            curr = curr->next;
        }
    }

    //Returns amount of a fragments
    return fragCount;
}


//Prints the menu
void printMenu() {
    cout << "1. Add program \n2. Kill program \n3. Fragmentation \n4. Print memory "
            "\n5. Exit" << endl;
}


/*
 * Resources: http://www.cplusplus.com/forum/beginner/2957/ - error correcting for input
 */
int main(int argc, char **argv) {

    string algorithm = argv[1];

    //Checks to make sure the user inputted the correct arguments into the command line
    //If not, program terminates
    if (argc != 2 || (algorithm != "best" && algorithm != "worst")) {
        cout << "Error: Incorrect arguments. Please specify 'best' or 'worst' and try again." << endl;
        return 1;
    } else {
        cout << "Using " << algorithm << " fit algoritm" << endl;
    }

    string progName;
    linkedList list;
    int userInput = 0;
    bool runProg = true;
    string name;
    int progSize = 0;

    printMenu();
    list.createPages();

    //Used so this will run at least one time
    do {
        cout << "\n\nchoice - ";
        cin >> userInput;
        cout << endl;

        //Checks for invalid user input
        if (userInput != 1 && userInput != 2 && userInput != 3 && userInput != 4 && userInput != 5) {
            cout << "Invalid input. Please enter an integer from 1 to 5. Exiting program.";
            return 1;
        }
        if (cin.fail()) {
            cout << "Invalid input. Please enter an integer from 1 to 5. Exiting program." << endl;
            return 1;
        }

            //If user input is valid:
        else {

            //User chose to add a program
            if (userInput == 1) {
                int size = 0;

                cout << "Program name - ";
                cin >> name;

                if (cin.fail()) {
                    cout << "Invalid input. Please enter a string or number as program name. Exiting program." << endl;
                    return 1;
                }

                cin.clear();
                cout << endl;

                cout << "Program size (KB) - ";
                cin >> progSize;

                if (progSize == 0) {
                    cout << "Invalid input. You must create a program with a size of at least 1 KB. Exiting program.";
                    return 1;
                }

                if (cin.fail()) {
                    cout << "Invalid input. Please enter an integer from 1 to 32. Exiting program." << endl;
                    return 1;
                }

                cin.clear();
                cout << endl;

                //Checks to see if the program already exists
                bool duplicate = list.checkDuplicate(name);

                if (duplicate) {
                    cout << "Error, Program " << name << " already running." << endl;
                } else {
                    if (algorithm == "best") {
                        list.insertBest(name, progSize, algorithm);
                    } else if (algorithm == "worst") {
                        list.insertWorst(name, progSize, algorithm);
                    }
                }
            }

            //User chose to kill a program
            if (userInput == 2) {
                cout << "Program name - ";
                cin >> name;

                if (cin.fail()) {
                    cout << "Invalid input. Please enter the name of a program that is running. Exiting program." << endl;
                    return 1;
                }

                cin.clear();
                cout << endl;

                list.kill(name);
            }
        }

        //User chose to count fragments
        if (userInput == 3) {
            cout << "There are " << list.countFrag() << " fragments.";
        }

        //User chose to print out the linked list
        if (userInput == 4) {
            list.printList();
        }

        //User chose to exit the program
        if (userInput == 5) {
            cout << "Exiting program" << endl;
            runProg = false; //Ends the while loop
        }
    } while (runProg);

    return 0;
}