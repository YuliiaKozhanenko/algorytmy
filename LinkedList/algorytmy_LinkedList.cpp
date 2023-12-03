//ALGO2 IS1 214A LAB01
//Yuliia Kozhanenko
//ky53726@zut.edu.pl

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>


template<typename T>
struct is_pointer {
    static const bool value = false;
};

template<typename T>
struct is_pointer<T*> {
    static const bool value = true;
};

struct SomeObject {

    int field_1;
    char field_2;

    friend bool operator==(const SomeObject& a, const SomeObject& b) {

        return a.field_1 == b.field_1 && a.field_2 == b.field_2;
    }

    friend bool operator<(const SomeObject& a, const SomeObject& b) {

        if (a.field_1 < b.field_1)
        {
            return true;
        }
        if (a.field_1 > b.field_1)
        {
            return false;
        }
        return a.field_2 < b.field_2;
    }

    friend bool operator>(const SomeObject& a, const SomeObject& b) {

        if (a.field_1 > b.field_1)
        {
            return true;
        }
        if (a.field_1 < b.field_1)
        {
            return false;
        }
        return a.field_2 > b.field_2;
    }

    void generateRandomValues() {

        std::random_device rd;
        std::uniform_int_distribution<> intDist(0, 10000);
        std::uniform_int_distribution<> charDist('a', 'z');

        field_1 = intDist(rd);
        field_2 = static_cast<char>(charDist(rd));
    }

    SomeObject() {
        generateRandomValues();
    }
};

template <typename T, bool isPtr>
struct UniversalComparator {

    int operator()(const T& a, const T& b) {

        if (a < b)
        {
            return -1;
        }
        if (a > b)
        {
            return 1;
        }
        return 0;
    }
};

template <typename T>
struct UniversalComparator<T*, true> {

    int operator()(const T* a, const T* b) {

        if (*a < *b)
        {
            return -1;
        }
        if (*a > *b)
        {
            return 1;
        }
        return 0;
    }
};

enum PrintOption {

    ALL_NODES,
    FIRST_N_NODES,
    LAST_N_NODES
};

template <typename T>
class LinkedList {

private:
    class Node {

    public:
        T data;
        Node* next;
        Node* previous;
    };
    
    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    T& operator[](int index);
    int getSize();
    void addBack(T data); //(a)
    void addFront(T data);//(b)
    void removeLast();//(c)
    void removeFirst();//(d)

    T/*&*/ getElement(int index) {//(e)

        if constexpr (is_pointer<T>::value)
        {
            // якщо T є вказівником
            T original = (*this)[index];

            if (original == nullptr)
            {
                return nullptr;
            }
            return new SomeObject(*original); // Deep copy
        }
        else
        {
            return (*this)[index];
        }
    }

    void setElement(int index, const T& newData) {//(f)

        if constexpr (is_pointer<T>::value)
        {
            delete (*this)[index];
            (*this)[index] = nullptr;
        }
        (*this)[index] = newData;
    }

    Node* findNode(const T& value);//(g)
    bool findAndRemoveNode(const T& value);//(h)
    void addSorted(const T& value);//(i)
    void clearList();//(j)
    void printLinkedList(PrintOption option, int n = 0);//(k)

    ~LinkedList() {

        clearList();
    }
};


template <typename T>
int LinkedList<T>::getSize() {

    int count = 0;
    Node* current = head;

    while (current != nullptr)
    {
        ++count;
        current = current->next;
    }

    return count;
}


template<typename T>
void LinkedList<T>::clearList() {

    Node* current = tail;

    while (current != nullptr)
    {
        Node* previous = current->previous;

        if constexpr (is_pointer<T>::value)
        {
            delete current->data;
            current->data = nullptr;
        }
        delete current;
        current = previous;
    }

    head = nullptr;
    tail = nullptr;
}

template <typename T>
void LinkedList<T>::removeFirst() {

    if (head == nullptr)
    {
        return;
    }

    Node* temp = head;

    if (head->next != nullptr)
    {
        head->next->previous = nullptr;
    }

    head = head->next;

    if (head == nullptr)
    {
        tail = nullptr;
    }

    if constexpr (is_pointer<T>::value)
    {
        delete temp->data;
        temp->data = nullptr;
    }
    delete temp;
}

template <typename T>
void LinkedList<T>::removeLast() {

    if (tail == nullptr)
    {
        return;
    }

    Node* temp = tail;

    if (tail->previous != nullptr)
    {
        tail->previous->next = nullptr;
    }

    tail = tail->previous;

    if (tail == nullptr)
    {
        head = nullptr;
    }

    if constexpr (is_pointer<T>::value)
    {
        delete temp->data;
        temp->data = nullptr;
    }
    delete temp;
}

template <typename T>
void LinkedList<T>::addFront(T data) {

    Node* newNode = new Node();
    newNode->data = data;
    newNode->previous = nullptr;
    newNode->next = head;

    if (head != nullptr)
    {
        head->previous = newNode;
    }

    head = newNode;

    if (tail == nullptr)
    {
        tail = newNode;
    }
}

template <typename T>
void LinkedList<T>::addBack(T data) {

    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    newNode->previous = tail;

    if (tail != nullptr)
    {
        tail->next = newNode;
    }

    tail = newNode;

    if (head == nullptr)
    {
        head = newNode;
    }
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::findNode(const T& value) {

    UniversalComparator<T, std::is_pointer<T>::value> comp;

    Node* current = head;

    while (current != nullptr && comp(current->data, value) != 0)
    {
        current = current->next;
    }

    return current;
}

template <typename T>
bool LinkedList<T>::findAndRemoveNode(const T& value) {

    UniversalComparator<T, std::is_pointer<T>::value> comp;

    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) 
    {

        if (comp(current->data, value) == 0)
        {
            if (previous != nullptr)
            {
                previous->next = current->next;
            }
            else
            {
                head = current->next;
            }

            if (current->next != nullptr)
            {
                current->next->previous = previous;
            }
            else
            {
                tail = previous;
            }

            if constexpr (is_pointer<T>::value)
            {
                delete current->data;
                current->data = nullptr;
            }
            delete current;
            return true;  // Has been found and removed
        }

        previous = current;
        current = current->next;
    }

    return false;  // Not found
}

template<typename T>
void LinkedList<T>::addSorted(const T& value) {

    UniversalComparator<T, std::is_pointer<T>::value> comp;

    Node* newNode = new Node();
    newNode->data = value;
    newNode->next = nullptr;
    newNode->previous = nullptr;

    if (head == nullptr)
    {
        head = tail = newNode;
        return;
    }

    Node* current = head;

    while (current != nullptr && comp(current->data, value) < 0)
    {
        current = current->next;
    }

    // Insert at the beginning
    if (current == head)
    {
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
    }
    // Insert at the end
    else if (current == nullptr)
    {
        newNode->previous = tail;
        tail->next = newNode;
        tail = newNode;
    }
    // Insert in the middle
    else
    {
        newNode->next = current;
        newNode->previous = current->previous;
        current->previous->next = newNode;
        current->previous = newNode;
    }

}


template<typename T>
void LinkedList<T>::printLinkedList(PrintOption option, int n)
{
    std::cout << "\n------------------------------------------------";
    std::cout << "\nSize: " << getSize();
    std::cout << ", Head: " << head;
    std::cout << ", Tail: " << tail << '\n';

    if (head == nullptr && tail == nullptr)
    {
        std::cout << "The list is empty.\n";
        return;
    }

    if (head == tail)
    {
        std::cout << "The list has only one element.\n";
    }

    Node* current;
    int count = 0;

    switch (option)
    {
    case ALL_NODES:

        current = head;
        while (current != nullptr)
        {
            if constexpr (is_pointer<T>::value)
            {
                std::cout << "Node address: " << current << ", Data: " << *current->data << '\n';
            }
            else
            {
                std::cout << "Node address: " << current << ", Data: " << current->data << '\n';
            }
            current = current->next;
        }
        break;

    case FIRST_N_NODES:

        if (n > getSize())
        {
            std::cout << "n exceeds the list size. Printing all nodes.\n";
            n = getSize();
        }
        current = head;
        while (current != nullptr && count < n)
        {
            if constexpr (is_pointer<T>::value)
            {
                std::cout << "Node address: " << current << ", Data: " << *current->data << '\n';
            }
            else
            {
                std::cout << "Node address: " << current << ", Data: " << current->data << '\n';
            }
            current = current->next;
            count++;
        }
        break;

    case LAST_N_NODES:

        if (n > getSize())
        {
            std::cout << "n exceeds the list size. Printing all nodes.\n";
            n = getSize();
        }
        current = tail;
        while (current != nullptr && count < n)
        {
            if constexpr (is_pointer<T>::value)
            {
                std::cout << "Node address: " << current << ", Data: " << *current->data << '\n';
            }
            else
            {
                std::cout << "Node address: " << current << ", Data: " << current->data << '\n';
            }
            current = current->previous;
            count++;
        }
        break;
    }
}

std::ostream& operator<<(std::ostream& os, const SomeObject& obj) {

    os << "field_1: " << obj.field_1 << ", field_2: " << obj.field_2;
    return os;
}

template <typename T>
T& LinkedList<T>::operator[](int index) {

    if (index < 0 || index >= getSize())
    {
        std::cout << "Incorrect index" << std::endl;
        //throw std::out_of_range("Incorrect index");
        return head->data;
    }
    Node* current;
    int i;

    // From which end of the list to start
    if (index <= getSize() / 2)
    {
        current = head;
        i = 0;

        while (current != nullptr)
        {
            if (i == index)
            {
                return current->data;
            }
            current = current->next;
            ++i;
        }

    }
    else
    {
        current = tail;
        i = getSize() - 1;

        while (current != nullptr)
        {
            if (i == index)
            {
                return current->data;
            }
            current = current->previous;
            --i;
        }
    }

    // Nigdy nie musi być wykonane, ale jest potrzebne dla kompilacji
    // Nigdy nie musi być wykonane, ale jest potrzebne dla kompilacji
    //throw std::out_of_range("Incorrect index");
    return head->data;
}

int main() {

    std::chrono::high_resolution_clock::time_point start
        = std::chrono::high_resolution_clock::now();

    LinkedList<SomeObject*>* newList = new LinkedList<SomeObject*>();

    SomeObject* so1 = new SomeObject();
    SomeObject* so2 = new SomeObject();
    SomeObject* so3 = new SomeObject();
    SomeObject* so4 = new SomeObject();
    SomeObject* so5 = new SomeObject();
    SomeObject* so6 = new SomeObject();

    std::chrono::high_resolution_clock::time_point end
        = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "\nTotal time to create six objects: " << duration.count() << " s\n";

    std::cout << "so1:\n" << (*so1) << std::endl;
    std::cout << "so2:\n" << (*so2) << std::endl;
    std::cout << "so3:\n" << (*so3) << std::endl;
    std::cout << "so4:\n" << (*so4) << std::endl;
    std::cout << "so5:\n" << (*so5) << std::endl;
    std::cout << "so6:\n" << (*so6) << std::endl;

    start = std::chrono::high_resolution_clock::now();

    newList->addBack(so1);
    newList->addBack(so2);
    newList->addFront(so3);
    newList->printLinkedList(ALL_NODES);
    newList->addSorted(so4);
    newList->printLinkedList(ALL_NODES);
    newList->setElement(1, so5);
    newList->printLinkedList(ALL_NODES);
    newList->addFront(so6);
    newList->printLinkedList(ALL_NODES);

    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "\nTotal time to add six objects in different ways: " << duration.count() << " s\n";



    SomeObject* so7 = newList->getElement(4);
    std::cout << "\nso7:\n" << (*so7) << std::endl;
    newList->addSorted(so7);
    newList->printLinkedList(ALL_NODES);


    start = std::chrono::high_resolution_clock::now();
    SomeObject* so8 = new SomeObject();
    so8->field_1 = 50605;
    so8->field_2 = 'Y';
    newList->setElement(1, so8);
    newList->printLinkedList(ALL_NODES);

    SomeObject* so9 = new SomeObject(*so8);
    newList->addFront(so9);
    newList->printLinkedList(ALL_NODES);

    newList->findAndRemoveNode(so8);
    newList->printLinkedList(ALL_NODES);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "\nTotal time spent on "
        << "creating two identical data objects, "
        << "adding them to the list, and removing one of them by value : "
        << duration.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    SomeObject* so10 = new SomeObject();
    so10->field_1 = 50605;
    so10->field_2 = 'Y';
    std::cout << "Adress of Node you were looking for: " << newList->findNode(so10) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "\nTotal time spent searching for one node by data "
        << duration.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    newList->removeLast();
    newList->printLinkedList(ALL_NODES);
    newList->removeFirst();
    newList->printLinkedList(ALL_NODES);
    newList->removeFirst();
    newList->printLinkedList(ALL_NODES);
    newList->removeLast();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "\nTotal time spent deleting four nodes " << duration.count() << " s\n";

    newList->printLinkedList(ALL_NODES);


    //LinkedList<SomeObject>* ll = new LinkedList<SomeObject>();
    //SomeObject so;
    //so.field_1 = 123;
    //so.field_2 = 'a';
    //ll->addBack(so);
    //SomeObject so1;
    //so1.field_1 = 223;
    //so1.field_2 = 'b';
    //ll->addBack(so1);
    //SomeObject so3;
    //so3.field_1 = 122;
    //so3.field_2 = 'c';
    //ll->addBack(so3);
    ////ll->clearList();
    //LinkedList<SomeObject*>* ll2 = new LinkedList<SomeObject*>();
    //SomeObject* aaa = new SomeObject();
    //aaa->field_1 = 333;
    //aaa->field_2 = 'm';
    //std::cout << std::endl << (*aaa);
    //ll2->addBack(aaa);
    //SomeObject* abb = new SomeObject();
    //abb->field_1 = 14324;
    //abb->field_2 = 'h';
    //std::cout << std::endl << (*abb);
    //ll2->addFront(abb);
    //SomeObject* acc= new SomeObject();
    //acc->field_1 = 2324;
    //acc->field_2 = 'i';
    //std::cout << std::endl << (*acc);
    //ll2->addFront(acc);
    //ll->printLinkedList(FIRST_N_NODES,5);
    //ll2->printLinkedList(ALL_NODES);
    //delete ll;
    return 0;
}