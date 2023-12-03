//ALGO2 IS1 214A LAB04
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
class RedBlackTree
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
		bool color = true;

		Node(T val, int idx, Node* par = nullptr)
			: data(val), index(idx), leftChild(nullptr), rightChild(nullptr), parent(par) {}

		std::string getNodeIndexOrNULL()
		{
			//Było tylko this, jeśli używać nullptr zamiast NIL
			if (this->index != -1)
			{//
				return std::to_string(this->index);
			}
			else
			{//
				return "NIL";
			}//
		}

	};

public:
	Node* root;
	UniversalComparator<T, is_pointer<T>::value> comp;

	RedBlackTree() : root(nullptr), currentIndex(0)
	{
		//
		NIL = new Node(T(), -1, nullptr); // specjalne znaczenie indeksu 
		NIL->color = true;      // true == czarny
		NIL->leftChild = NIL;   
		NIL->rightChild = NIL;
		NIL->parent = NIL;
		root = NIL;
		//
	}

	~RedBlackTree()
	{
		clear();
		//
		delete NIL;
		//
	}

	int height() const
	{
		return height(root);
	}
	int blackHeight() const
	{
		return blackHeight(root);
	}

	bool add(const T& data)
	{
		Node* newNode = new Node(data, ++currentIndex); // Створюємо новий вузол
		newNode->color = RED; // Нові вузли спочатку червоні
		//
		newNode->leftChild = NIL; // Ініціалізація NIL вузлів
		newNode->rightChild = NIL;
		//
		if (/*!*/root == NIL)
		{
			root = newNode;
			root->color = BLACK; // Корінь завжди чорний
			root->parent = NIL; // Батько кореня вказує на NIL
		}
		else
		{
			Node* current = root;
			Node* parent = /*nullptr*/NIL;

			while (current != /*nullptr*/NIL)
			{
				parent = current;
				if (comp(data, current->data) < 0)
				{
					current = current->leftChild;
				}
				else if (comp(data, current->data) > 0)
				{
					current = current->rightChild;
				}
				else
				{
					// Елемент вже існує
					delete newNode; // Важливо очистити пам'ять
					return false;
				}
			}

			// Зв'язуємо новий вузол з його батьком
			newNode->parent = parent;
			if (comp(data, parent->data) < 0)
			{
				parent->leftChild = newNode;
			}
			else
			{
				parent->rightChild = newNode;
			}

			fixInsert(newNode);
		}
		return true;
	}


	Node* search(const T& data)
	{
		Node* current = root;
		//
		while (current != /*nullptr*/NIL)
		{//
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
		//
		return /*nullptr*/NIL;
		//
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
		//
		root = /*nullptr*/NIL;
		//
	}

	bool isNIL(Node* node) const
	{
		return node == NIL;
	}
	friend std::ostream& operator<<(std::ostream& os, const Node& node)
	{
		std::string colour = "";
		if (node.color)
		{
			colour = "black";
		}
		else
		{
			colour = "red";
		}
		os << "(" << node.index << ": [" << colour << ", p: " << node.parent->getNodeIndexOrNULL();
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
	static const bool BLACK = true;
	static const bool RED = false;
	int currentIndex;

	int size(Node* node) const
	{
		//
		if (/*!*/node == NIL)
		{//
			return 0;
		}
		else
		{
			return 1 + size(node->leftChild) + size(node->rightChild);
		}
	}

	int height(Node* node)
	{
		//
		if (/*!*/node == NIL)
		{//
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
	int blackHeight(Node* node) const
	{
		//
		if (node == /*nullptr*/NIL)
		{//
			return 1; // NIL czarny
		}
		int leftBlackHeight = blackHeight(node->leftChild);
		int rightBlackHeight = blackHeight(node->rightChild);

		int addHeight;
		if (node->color == BLACK)
		{
			addHeight = 1;
		}
		else
		{
			addHeight = 0;
		}

		if (leftBlackHeight > rightBlackHeight)
		{
			return leftBlackHeight + addHeight;
		}
		else
		{
			return rightBlackHeight + addHeight;
		}
	}

	void preOrderTravel(Node* node, std::ostream& os) const
	{
		//
		if (node != NIL)
		{//
			os << *node << "\n"; // pokazujemy obecny węzeł
			preOrderTravel(node->leftChild, os);  // Rekursywne wywołanie dla lewego poddrzewa
			preOrderTravel(node->rightChild, os); // Rekursywne wywołanie dla prawego poddrzewa
		}
	}
	void inOrderTravel(Node* node, std::ostream& os) const
	{
		//
		if (node != NIL)
		{//
			inOrderTravel(node->leftChild, os);  // Rekursywne wywołanie dla lewego poddrzewa
			os << *node << "\n"; // pokazujemy obecny węzeł
			inOrderTravel(node->rightChild, os); // Rekursywne wywołanie dla prawego poddrzewa
		}
	}
	void postOrderDelete(Node* node, bool freeMemory = false)
	{
		//
		if (node != NIL)
		{//
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

	void leftRotate(Node* node)
	{
		Node* newRoot = node->rightChild;
		node->rightChild = newRoot->leftChild;
		if (newRoot->leftChild != NIL)
		{
			newRoot->leftChild->parent = node;
			newRoot->parent = node->parent;
		}
		if (node->parent == NIL)
		{
			root = newRoot;
		}
		else if (node == node->parent->leftChild)
		{
			node->parent->leftChild = newRoot;
		}
		else
		{
			node->parent->rightChild = newRoot;
		}
		newRoot->leftChild = node;
		node->parent = newRoot;

	}
	void rightRotate(Node* node)
	{
		Node* newRoot = node->leftChild;
		node->leftChild = newRoot->rightChild;
		if (newRoot->rightChild != NIL)
		{
			newRoot->rightChild->parent = node;
			newRoot->parent = node->parent;
		}
		if (node->parent == NIL)
		{
			root = newRoot;
		}
		else if (node == node->parent->rightChild)
		{
			node->parent->rightChild = newRoot;
		}
		else
		{
			node->parent->leftChild = newRoot;
		}
		newRoot->rightChild = node;
		node->parent = newRoot;
	}
	void fixInsert(Node* node)
	{
		Node* uncle;
		while (node->parent->color == RED && node != root)
		{
			if (node->parent == node->parent->parent->leftChild)
			{
				uncle = node->parent->parent->rightChild;
				if (uncle->color == RED)
				{
					node->parent->color = BLACK;
					uncle->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				}
				else
				{
					if (node == node->parent->rightChild)
					{
						node = node->parent;
						leftRotate(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					rightRotate(node->parent->parent);
				}
			}
			else
			{
				uncle = node->parent->parent->leftChild;
				if (uncle->color == RED)
				{
					node->parent->color = BLACK;
					uncle->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				}
				else
				{
					if (node == node->parent->leftChild)
					{
						node = node->parent;
						rightRotate(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					leftRotate(node->parent->parent);
				}
			}
			root->color = BLACK;
		}
	}

protected:
	Node* NIL;
	//friend class Node;
};



int main()
{
	const int MAX_ORDER = 7;
	RedBlackTree<SomeObject*>* bst = new RedBlackTree<SomeObject*>();

	for (int o = 1; o <= MAX_ORDER; ++o)
	{

		int n = std::pow(10, o);
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < n; ++i)
		{
			SomeObject* so = new SomeObject();
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
			SomeObject temp;
			if (!bst->isNIL(bst->search(&temp))/* != nullptr*/)
			{
				hits++;
			}
		}

		t2 = std::chrono::high_resolution_clock::now();
		ms_double = t2 - t1;
		std::cout << "Search for " << m << " elements executed in " << ms_double.count() << " ms, hits: " << hits << "\n";

		bst->clear(true);
	}



	RedBlackTree<SomeObject*> bst2;
	// Test add method
	std::cout << "Adding elements:" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		SomeObject* so = new SomeObject();
		bst2.add(so);
		std::cout << "Added: " << so->field_1 << ", " << so->field_2 << std::endl;
	}
	bst2.print();

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

	bst2.add(searchObj);
	if (bst2.add(anotherSearchObj))
	{
		std::cout << "Copy of searchObj has been added\n";
	}
	else
	{
		std::cout << "Copy of searchObj has not been added\n";
	}
	bst2.add(so1);
	bst2.add(so2);
	bst2.add(so3);

	bst2.print();
	RedBlackTree<SomeObject*>::Node* result = bst2.search(searchObj);
	if (!bst->isNIL(result))
	{
		std::cout << "Found: " << result->data->field_1 << ", " << result->data->field_2 << std::endl;
	}
	else
	{
		std::cout << "Element not found in the tree." << std::endl;
	}

	// Test preOrderTravel and inOrderTravel
	std::cout << "Pre-order travel:" << std::endl;
	bst2.preOrderTravel(std::cout);
	std::cout << "\nIn-order travel:" << std::endl;
	bst2.inOrderTravel(std::cout);

	// Test print method
	std::cout << "\nBinary Search Tree:" << std::endl;
	bst2.print();

	// Test clear method
	bst2.clear(true);
	std::cout << "\nTree after clearing:" << std::endl;
	bst2.print();

	/*RedBlackTree<int>* rbt = new RedBlackTree<int>();
	rbt->add(13);
	rbt->add(8);
	rbt->add(17);
	rbt->add(1);
	rbt->add(15);
	rbt->add(11);
	rbt->add(25);
	rbt->add(6);
	rbt->add(22);
	rbt->add(27);
	rbt->print();
	std::cout << "Black height: " << rbt->blackHeight()-1;*/
	return 0;
}