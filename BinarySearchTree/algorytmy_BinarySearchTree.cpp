//ALGO2 IS1 214A LAB03
//Yuliia Kozhanenko
//ky53726@zut.edu.pl

#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <limits>
#include <string>

template<typename T>
struct is_pointer
{
    static const bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
    static const bool value = true;
};

struct SomeObject
{

    int field_1;
    char field_2;

    friend bool operator==(const SomeObject& a, const SomeObject& b)
    {

        return a.field_1 == b.field_1 && a.field_2 == b.field_2;
    }

    friend bool operator<(const SomeObject& a, const SomeObject& b)
    {

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

    friend bool operator>(const SomeObject& a, const SomeObject& b)
    {

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

    void generateRandomValues()
    {

        std::random_device rd;
        std::uniform_int_distribution<> intDist(0, 10000);
        std::uniform_int_distribution<> charDist('a', 'z');

        field_1 = intDist(rd);
        field_2 = static_cast<char>(charDist(rd));
    }

    SomeObject()
    {
        generateRandomValues();
    }

    SomeObject(const SomeObject& other) = default;
};

std::ostream& operator<<(std::ostream& os, const SomeObject& obj)
{

    os << "field_1: " << obj.field_1 << ", field_2: " << obj.field_2;
    return os;
}

template <typename T, bool isPtr>
struct UniversalComparator
{

    int operator()(const T& a, const T& b)
    {

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
struct UniversalComparator<T*, true>
{

    int operator()(const T* a, const T* b)
    {

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

//preorder korzeń->lewe dziecko->prawe dziecko
//inorder lewe dziecko->korzeń->prawe dziecko
//postorder liście lewe->liście prawe->korzeń

template<typename T>
class BinarySearchTree
{
public:
    class Node
    {
    public:
        T data;
        int index;
        Node* leftChild;
        Node* rightChild;
        Node* parent;

        Node(T val, int idx, Node* par = nullptr) 
            : data(val), index(idx), leftChild(nullptr), rightChild(nullptr), parent(par) {}

        std::string getNodeIndexOrNULL()
        {
            if (this)
            {
                return std::to_string(this->index);
            }
            else
            {
                return "NULL";
            }
        }
        
    };

public:
    Node* root;
    int currentIndex;
    UniversalComparator<T, is_pointer<T>::value> comp;

    BinarySearchTree() : root(nullptr), currentIndex(0) {}

    ~BinarySearchTree()
    {
        clear();
    }

    void add(const T& data)
    {
        if (!root)
        {
            root = new Node(data, currentIndex);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr)
        {
            parent = current;
            if (comp(data, current->data) < 0)
            {
                current = current->leftChild;
            }
            else
            {
                current = current->rightChild;
            }
        }

        if (comp(data, parent->data) < 0)
        {
            parent->leftChild = new Node(data, ++currentIndex, parent);
        }
        else
        {
            parent->rightChild = new Node(data, ++currentIndex, parent);
        }
    }

    Node* search(const T& data)
    {
        Node* current = root;
        while (current != nullptr)
        {
            if (comp(data, current->data) == 0)
            {
                // Znaleziono węzeł
                return current;
            }
            else if (comp(data, current->data) < 0)
            {
                current = current->leftChild;
            }
            else
            {
                current = current->rightChild;
            }
        }
        
        return nullptr;
    }

    void remove(Node* nodeToRemove)
    {
        if (!nodeToRemove) return;

        if (nodeToRemove->leftChild && nodeToRemove->rightChild)
        {
            // Zamiana węzła i usunięcie 
            Node* replacementNode;
            if (height(nodeToRemove->leftChild) > height(nodeToRemove->rightChild))
            {
                replacementNode = findMax(nodeToRemove->leftChild);
            }
            else
            {
                replacementNode = findMin(nodeToRemove->rightChild);
            }

            //Zamieniamy indeks i danie węzła, który usuwamy, na dane ta indeks zamiany
            if constexpr (is_pointer<T>::value)
            {
                delete nodeToRemove->data;
            }
            nodeToRemove->data = replacementNode->data;
            nodeToRemove->index = replacementNode->index;
            if (is_pointer<T>::value)
            {
                replacementNode->data = nullptr;
            }
            remove(replacementNode);
        }
        else
        {
            // Węzeł ma jedno dziecko albo nie ma żadnego
            Node* child;
            if (nodeToRemove->leftChild)
            {
                child = nodeToRemove->leftChild;
            }
            else
            {
                child = nodeToRemove->rightChild;  // TO może być nullptr, jak nie ma lewego dziecka
            }

            // Jeżeli węzeł nie jest korzeniem, aktualizujemy jego parenta
            if (nodeToRemove->parent)
            {
                if (nodeToRemove == nodeToRemove->parent->leftChild)
                {
                    nodeToRemove->parent->leftChild = child;
                }
                else
                {
                    nodeToRemove->parent->rightChild = child;
                }
            }
            if (child)
            {
                child->parent = nodeToRemove->parent;
            }
            if (nodeToRemove == root)
            {
                root = child;
            }

            delete nodeToRemove;
        }
    }

    void preOrderTravel(std::ostream& os)
    {
        preOrderTravel(root, os);
    }

    void inOrderTravel(std::ostream& os)
    {
        inOrderTravel(root, os);
    }

    void print()
    {
        std::cout << "size: " << size(root) << "\n";
        std::cout << "height: " << height(root) << "\n";
        std::cout << "{\n";
        preOrderTravel(root, std::cout);
        std::cout << "}\n";
    }

    /*int size()
    {
        return size(root);
    }*/

    void clear(bool freeMemory = false)
    {
        postOrderDelete(root, freeMemory); 
        root = nullptr; 
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << "(" << node.index << ": [p: " << node.parent->getNodeIndexOrNULL();
        os << ", l: " << node.leftChild->getNodeIndexOrNULL();
        os << ", r: " << node.rightChild->getNodeIndexOrNULL();
        os << "], data: ";

        if constexpr (std::is_pointer<T>::value)
        {
            if (node.data)
            {
                os << "(" << *node.data << "))";
            }
            else
            {
                os << "(" << "NULL" << "))";
            }

        }
        else
        {
            os << "(" << node.data << "))";
        }

        return os;
    }

private:

    /*std::string getNodeIndexOrNULL(const Node* node)
    {
        if (node)
        {
            return std::to_string(node->index);
        }
        else
        {
            return "NULL";
        }
    }*/

    void postOrderDelete(Node* node, bool freeMemory = false)
    {
        if (node)
        {
            postOrderDelete(node->leftChild);
            postOrderDelete(node->rightChild);

            if (freeMemory)
            {
                if constexpr (is_pointer<T>::value)
                {
                    delete node->data;
                    node->data = nullptr;
                }
            }
            delete node;
        }
    }

    int size(Node* node) const
    {
        if (!node)
        {
            return 0;
        }
        else
        {
            return 1 + size(node->leftChild) + size(node->rightChild);
        }
    }

    int height(Node* node)
    {
        if (!node)
        {
            return 0;
        }
        int leftHeight = height(node->leftChild);
        int rightHeight = height(node->rightChild);

        if (leftHeight > rightHeight)
        {
            return leftHeight + 1;
        }
        else
        {
            return rightHeight + 1;
        }
    }

    Node* findMin(Node* node)
    {
        while (node && node->leftChild)
        {
            node = node->leftChild;
        }
        return node;
    }

    Node* findMax(Node* node)
    {
        while (node && node->rightChild)
        {
            node = node->rightChild;
        }
        return node;
    }

    void preOrderTravel(Node* node, std::ostream& os) const
    {
        if (node)
        {
            os << *node << "\n"; // pokazujemy obecny węzeł
            preOrderTravel(node->leftChild, os);  // Rekursywne wywołanie dla lewego poddrzewa
            preOrderTravel(node->rightChild, os); // Rekursywne wywołanie dla prawego poddrzewa
        }
    }
    void inOrderTravel(Node* node, std::ostream& os) const
    {
        if (node)
        {
            inOrderTravel(node->leftChild, os);  // Rekursywne wywołanie dla lewego poddrzewa
            os << *node << "\n"; // pokazujemy obecny węzeł
            inOrderTravel(node->rightChild, os); // Rekursywne wywołanie dla prawego poddrzewa
        }
    }
};

int main()
{
    /*
    const int MAX_ORDER = 7;
    BinarySearchTree<SomeObject*>* bst = new BinarySearchTree<SomeObject*>();

    for (int o = 1; o <= MAX_ORDER; ++o)
    {

        int n = std::pow(10, o);
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; ++i)
        {
            SomeObject* so = new SomeObject(); // Dynamic allocation of new SomeObject
            bst->add(so);
        }

        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "Added " << n << " elements in " << ms_double.count() << " ms\n";

        int m = std::pow(10, 4);
        int hits = 0;
        t1 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < m; ++i)
        {
            SomeObject temp; // Temporary object for search
            if (bst->search(&temp) != nullptr)
            {
                hits++;
            }
        }

        t2 = std::chrono::high_resolution_clock::now();
        ms_double = t2 - t1;
        std::cout << "Search for " << m << " elements executed in " << ms_double.count() << " ms, hits: " << hits << "\n";

        bst->clear(true); // Clear the tree
    }
    */
     BinarySearchTree<SomeObject*> bst;

    // Test add method
    std::cout << "Adding elements:" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        SomeObject* so = new SomeObject();
        bst.add(so);
        std::cout << "Added: " << so->field_1 << ", " << so->field_2 << std::endl;
    }
    bst.print();

    // Test search method
    SomeObject* searchObj = new SomeObject();
    searchObj->field_1 = 555;
    searchObj->field_2 = 'h';
    SomeObject* anotherSearchObj = new SomeObject(*searchObj);
    SomeObject* so1 = new SomeObject();
    so1->field_1 = 555;
    so1->field_2 = 'z';
    SomeObject* so2 = new SomeObject();
    so2->field_1 = 554;
    so2->field_2 = 'h';
    SomeObject* so3 = new SomeObject();
    so3->field_1 = 540;
    so3->field_2 = 'z';

    bst.add(searchObj);
    bst.add(anotherSearchObj);
    bst.add(so1);
    bst.add(so2);
    bst.add(so3);
    
    bst.print();
    BinarySearchTree<SomeObject*>::Node* result = bst.search(searchObj);
    if (result)
    {
        std::cout << "Found: " << result->data->field_1 << ", " << result->data->field_2 << std::endl;
    } else
    {
        std::cout << "Element not found in the tree." << std::endl;
    }

    // Test remove method
    //BinarySearchTree<SomeObject*>::Node* nodeToRemove = bst.search(searchObj);
    if (result)
    {
        bst.remove(result);
        std::cout << "After remove:\n";
        bst.print();
    } else
    {
        std::cout << "Element not found for removal." << std::endl;
    }

    // Test preOrderTravel and inOrderTravel
    std::cout << "Pre-order travel:" << std::endl;
    bst.preOrderTravel(std::cout);
    std::cout << "\nIn-order travel:" << std::endl;
    bst.inOrderTravel(std::cout);

    // Test print method
    std::cout << "\nBinary Search Tree:" << std::endl;
    bst.print();

    // Test clear method
    bst.clear();
    std::cout << "\nTree after clearing:" << std::endl;
    bst.print();
}
