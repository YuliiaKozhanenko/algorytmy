//ALGO2 IS1 214A LAB06
//Yuliia Kozhanenko
//ky53726@zut.edu.pl

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>
#include <iomanip>

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
};
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
enum PrintOption
{

	ALL_NODES,
	FIRST_N_NODES,
	LAST_N_NODES
};
template <typename T>
class HashTable;
template <typename T>
struct Pair;
template <typename T>
class LinkedList
{

public:
	class Node
	{
	public:
		T data;
		Node* next;
		Node* previous;
		Node() : data(T()), next(nullptr), previous(nullptr) {}
		explicit Node(const T& value) : data(value), next(nullptr), previous(nullptr) {}
	};

	Node* head;
	Node* tail;
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
	~LinkedList()
	{

		clearList();
	}
	LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr)
	{
		Node* current = other.head;
		while (current != nullptr)
		{
			addBack(current->data);
			current = current->next;
		}
	}
	LinkedList& operator=(const LinkedList& other)
	{
		if (this != &other)
		{
			clearList();

			Node* current = other.head;
			while (current != nullptr)
			{
				addBack(current->data);
				current = current->next;
			}
		}
		return *this;
	}
};
template <typename T>
int LinkedList<T>::getSize()
{

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
void LinkedList<T>::clearList()
{

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
void LinkedList<T>::removeFirst()
{

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
void LinkedList<T>::removeLast()
{

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
void LinkedList<T>::addFront(T data)
{

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
void LinkedList<T>::addBack(T data)
{

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
typename LinkedList<T>::Node* LinkedList<T>::findNode(const T& value)
{

	UniversalComparator<T, std::is_pointer<T>::value> comp;

	Node* current = head;

	while (current != nullptr && comp(current->data, value) != 0)
	{
		current = current->next;
	}

	return current;
}
template <typename T>
bool LinkedList<T>::findAndRemoveNode(const T& value)
{

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
void LinkedList<T>::addSorted(const T& value)
{

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
std::ostream& operator<<(std::ostream& os, const SomeObject& obj)
{

	os << "field_1: " << obj.field_1 << ", field_2: " << obj.field_2;
	return os;
}
template <typename T>
T& LinkedList<T>::operator[](int index)
{

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

	return head->data;
}
template <typename T>
class DynamicArray
{
private:
	T* array;
	int currentSize;
	int maxSize;
	double extenderCoefficient;
	void resize()
	{
		maxSize *= extenderCoefficient;
		T* newArray = new T[maxSize];

		for (int i = 0; i < currentSize; ++i)
		{
			newArray[i] = array[i];
		}
		delete[] array;
		array = newArray;
	}
	UniversalComparator<T, is_pointer<T>::value> comp;
public:
	DynamicArray(int initSize = 1, double ExtenderCoefficient = 2.0)
		: currentSize(0), maxSize(initSize), extenderCoefficient(ExtenderCoefficient)
	{
		array = new T[maxSize];
	}
	~DynamicArray()
	{
		clear();
		delete[] array;
	}
	DynamicArray(const DynamicArray& other) : maxSize(other.maxSize), currentSize(other.currentSize), extenderCoefficient(other.extenderCoefficient)
	{
		array = new T[maxSize];
		for (int i = 0; i < currentSize; ++i)
		{
			array[i] = other.array[i];
		}
	}
	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this != &other)
		{
			delete[] array;

			maxSize = other.maxSize;
			currentSize = other.currentSize;
			extenderCoefficient = other.extenderCoefficient;
			array = new T[maxSize];
			for (int i = 0; i < currentSize; ++i)
			{
				array[i] = other.array[i];
			}
		}
		return *this;
	}

	void addElement(const T& element) //(a)
	{
		if (currentSize == maxSize)
		{
			resize();
		}
		array[currentSize] = element;
		++currentSize;
	}
	void removeElement(const T& data)
	{
		int foundIndex = -1;
		for (int i = 0; i < currentSize; ++i)
		{
			if (comp(this->getElement(i), data) == 0)
			{
				foundIndex = i;
				break;
			}
		}
		// Jeśli element nie został znaleziony
		if (foundIndex == -1)
		{
			//std::cout << "Element not found\n";
			return;
		}
		if constexpr (is_pointer<T>::value)
		{
			delete array[foundIndex];
		}
		// Usunięcie elementu
		for (int i = foundIndex; i < currentSize - 1; ++i)
		{
			array[i] = array[i + 1];
		}
		// Aktualizacja rozmiaru
		--currentSize;
	}
	void removeElement(int index)
	{
		if (index < 0 || index >= currentSize)
		{
			return;
		}

		if constexpr (is_pointer<T>::value)
		{
			delete array[index];
		}

		for (int i = index; i < currentSize - 1; ++i)
		{
			array[i] = array[i + 1];
		}

		--currentSize;
	}
	//T* getElement(int index) const
	// {
	//    if (index < 0 || index >= currentSize)
	//    {
	//        // Logika obsługi błędnych indeksów
	//        std::cout << "Index out of range\n";
	//        return nullptr;
	//    }
	//    if constexpr (is_pointer<T>::value)
	//    {
	//        return array[index];  
	//    }
	//    else
	//    {
	//        return &array[index];  
	//    }
	//}
	T& getElement(int index)
	{
		if (index < 0 || index >= currentSize)
		{
			throw std::out_of_range("Index out of range");
		}
		return array[index]; //Zwrócenie referencji
	}
	void setElement(int index, const T& element) //(c)
	{
		if (index < 0 || index >= currentSize)
		{
			std::cout << "Index out of range\n";
			return;
		}

		if constexpr (is_pointer<T>::value)
		{
			delete array[index];
			array[index] = nullptr;
		}
		array[index] = element;
	}
	void clear()
	{
		if constexpr (std::is_pointer<T>::value)
		{
			for (int i = 0; i < currentSize; ++i)
			{
				delete array[i];
			}
		}
		delete[] array;
		array = new T[maxSize];
		currentSize = 0;
	}
	int size()
	{
		return currentSize;
	}
	void printArray(int n = -1) //(e)
	{
		std::cout << "\n------------------------------------------------";
		std::cout << "\nCurrent Size: " << currentSize;
		std::cout << ", Max Size: " << maxSize;
		std::cout << ", Array Address: " << array << '\n';

		if (array == nullptr)
		{
			std::cout << "The array is empty.\n";
		}

		if (currentSize == 0)
		{
			std::cout << "The array has no elements.\n";
		}

		if (n == -1 || n > currentSize)
		{
			std::cout << "\nPrinting all elements.\n";
			n = currentSize;
		}
		else
		{
			std::cout << "\nPrinting first " << n << " elements.\n";
		}

		for (int i = 0; i < n; ++i)
		{
			if constexpr (is_pointer<T>::value)
			{
				std::cout << "Element index: " << i << ", Data: " << *array[i] << '\n';
			}
			else
			{
				std::cout << "Element index: " << i << ", Data: " << array[i] << '\n';
			}
		}
	}
	void bubbleSort()  //(f)
	{
		bool swapped = false;

		for (int i = 0; i < currentSize; ++i)
		{
			swapped = false;

			for (int j = 0; j < currentSize - i - 1; ++j)
			{
				if (comp(array[j], array[j + 1]) > 0)
				{
					T temp = array[j];
					array[j] = array[j + 1];
					array[j + 1] = temp;
					swapped = true;
				}
			}

			if (!swapped)
			{
				break;
			}
		}
	}
	void swap(int index1, int index2)
	{
		if (index1 < 0 || index1 >= currentSize || index2 < 0 || index2 >= currentSize)
		{
			std::cout << "Index out of range\n";
			return;
		}
		T temp = getElement(index1);
		setElement(index1, getElement(index2));
		setElement(index2, temp);

	}
};
template <typename T>
struct Pair
{
	std::string key;
	T value;

	Pair() : key(""), value(T()) {}
	Pair(const std::string& k, const T& v) : key(k), value(v) {}
	bool operator==(const Pair& other) const
	{
		return key == other.key;
	}
	bool operator<(const Pair& other) const
	{
		return key < other.key;
	}
	bool operator>(const Pair& other) const
	{
		return key > other.key;
	}
};

template <typename T>
class HashTable
{
private:
	const double MAX_LOAD_FACTOR = 0.75;
	DynamicArray<LinkedList<Pair<T>>>* table;
	int currentTableSize;
	int maxTableSize;

	int hashFunction(const std::string& key)
	{
		long long hashValue = 0;
		const int base = 31;

		for (int i = 0; i < key.size(); ++i)
		{
			long long basePowered = 1;
			for (int j = 0; j < (key.size() - (i + 1)); ++j)
			{
				basePowered = (basePowered * base) % maxTableSize;
			}
			hashValue = (hashValue + basePowered * key[i]) % maxTableSize;
		}

		return hashValue;
	}

	/*void rehash()
	{
		DynamicArray<LinkedList<Pair<T>>> oldTable(*table);
		delete table;
		maxTableSize *= 2;
		table = new DynamicArray<LinkedList<Pair<T>>>(maxTableSize);

		for (int i = 0; i < maxTableSize; ++i)
		{
			table->addElement(LinkedList<Pair<T>>());
		}
		currentTableSize = 0;

		for (int i = 0; i < oldTable.size(); ++i)
		{
			LinkedList<Pair<T>>& list = oldTable.getElement(i);

			typename LinkedList<Pair<T>>::Node* current = list.head;
			while (current != nullptr)
			{
				int index = hashFunction(current->data.key);

				table->getElement(index).addBack(current->data);

				current = current->next;
				++currentTableSize;
			}
		}
	}*/
	void rehash()
	{
		maxTableSize *= 2;
		DynamicArray<LinkedList<Pair<T>>>* newTable = new DynamicArray<LinkedList<Pair<T>>>(maxTableSize);

		for (int i = 0; i < maxTableSize; ++i)
		{
			newTable->addElement(LinkedList<Pair<T>>());
		}

		for (int i = 0; i < currentTableSize/*maxTableSize/2*/; ++i)
		{
			LinkedList<Pair<T>>& list = table->getElement(i);
			typename LinkedList<Pair<T>>::Node* current = list.head;

			while (current != nullptr)
			{
				int index = hashFunction(current->data.key);
				newTable->getElement(index).addBack(current->data);
				current = current->next;
			}
		}

		delete table;
		table = newTable;
	}


public:
	HashTable(int size = 10) : maxTableSize(size), currentTableSize(0)
	{
		table = new DynamicArray<LinkedList<Pair<T>>>(size);

		for (int i = 0; i < size; ++i)
		{
			table->addElement(LinkedList<Pair<T>>());
		}
	}

	void add(const Pair<T>& pair)
	{
		/*int index = hashFunction(pair.key);

		table->getElement(index).addBack(pair);
		currentSize++;

		if (static_cast<double>(currentSize) / tableSize > MAX_LOAD_FACTOR)
		{
			rehash();
		}*/
		int index = hashFunction(pair.key);
		typename LinkedList<Pair<T>>::Node* node = table->getElement(index).findNode(Pair<T>(pair.key, T()));

		if (node != nullptr)
		{
			node->data.value = pair.value;
		}
		else
		{
			table->getElement(index).addBack(pair);
			currentTableSize++;

			if (currentTableSize > MAX_LOAD_FACTOR * maxTableSize)
			{
				rehash();
			}
		}
	}


	Pair<T>* search(const std::string& key)
	{
		int index = hashFunction(key);
		LinkedList<Pair<T>>& list = table->getElement(index);
		typename LinkedList<Pair<T>>::Node* node = list.findNode(Pair<T>(key, T()));
		if (node != nullptr)
		{
			return &(node->data);
		}

		return nullptr;
	}

	bool remove(const std::string& key)
	{
		int index = hashFunction(key);
		LinkedList<Pair<T>>& list = table->getElement(index);

		Pair<T> removingKeyPair(key, T());

		if (list.findAndRemoveNode(removingKeyPair))
		{
			currentTableSize--;
			return true;
		}

		return false;
	}

	void clear()
	{
		if (table != nullptr)
		{
			delete table;
			table = new DynamicArray<LinkedList<Pair<T>>>(maxTableSize);
			for (int i = 0; i < maxTableSize; ++i)
			{
				table->addElement(LinkedList<Pair<T>>());
			}
			currentTableSize = 0;
		}
	}
	void print()
	{
		std::cout << "hash table:\n";
		std::cout << "current_size: " << currentTableSize << "\n";
		std::cout << "max_size: " << maxTableSize << "\n";
		std::cout << "table:\n";
		std::cout << "{\n";
		for (int i = 0; i < table->size(); ++i)
		{
			LinkedList<Pair<T>>& list = table->getElement(i);
			if (list.head != nullptr) // Lista jest niepusta, jeśli head nie jest nullptr
			{
				std::cout << "  " << i << ": ";
				typename LinkedList<Pair<T>>::Node* current = list.head;
				while (current)
				{
					std::cout << current->data.key << " -> " << current->data.value;
					if (current = current->next)
					{
						std::cout << ", ";
					}
				}
				std::cout << ";\n";
			}
		}
		std::cout << "}\n";
		printStats();
	}

	void printStats()
	{
		int minSize = table->getElement(0).getSize();
		int maxSize = 0;
		int nonNullLists = 0;
		double averageSize = 0;

		for (int i = 0; i < maxTableSize; ++i)
		{
			LinkedList<Pair<T>>& list = table->getElement(i);
			if (list.getSize() != 0)
			{
				nonNullLists++;
				int size = list.getSize();
				if (size < minSize)
				{
					minSize = size;
				}
				if (size > maxSize)
				{
					maxSize = size;
				}
			}
		}

		if (nonNullLists)
		{
			averageSize = (double)currentTableSize / nonNullLists;
		}

		std::cout << "stats:\n";
		std::cout << "  list min size: " << minSize << "\n";
		std::cout << "  list max size: " << maxSize << "\n";
		std::cout << "  non-null lists: " << nonNullLists << "\n";
		std::cout << "  list average size: " << averageSize << std::endl;
	}
};


std::string random_key(int length)
{
	std::mt19937 gen(std::random_device{}());
	const char characters[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::uniform_int_distribution<> dist(0, sizeof(characters) - 2); // -2, bez terminatora '\0'

	std::string key;
	key.reserve(length);

	for (int i = 0; i < length; ++i)
	{
		key += characters[dist(gen)];
	}

	return key;
}

int main()
{
	//const int MAX_ORDER = 7; 
	//HashTable<int>* ht = new HashTable<int>();
	//for (int o = 1; o <= MAX_ORDER; ++o)
	//{
	//    int n = static_cast<int>(std::pow(10, o));
	//    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	//    for (int i = 0; i < n; ++i)
	//    {
	//        ht->add(Pair<int>(random_key(3), i));
	//    }
	//    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	//    std::cout << "Dodawanie " << n << " elementow zajelo: " << ms_double.count() << "ms\n";
	//    int m = 10000; 
	//    int hits = 0;
	//    t1 = std::chrono::high_resolution_clock::now();
	//    for (int i = 0; i < 100; ++i)
	//    {
	//        if (ht->search(random_key(3)) != nullptr)
	//        {
	//            ++hits;
	//        }
	//    }
	//    t2 = std::chrono::high_resolution_clock::now();
	//    // Wypis na ekran pomiarów czasowych i liczby trafień
	//    ms_double = t2 - t1;
	//    std::cout << "Wyszukiwanie " << m << " losowych kluczy zajelo: " << ms_double.count() << "ms, trafienia: " << hits << "\n";
	//    ht->printStats();
	//    int removeCount = 0;
	//    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
	//    for (int i = 0; i < m; ++i)
	//    {
	//        std::string key = random_key(3);
	//        if (ht->remove(key))
	//        {
	//            ++removeCount;
	//        }
	//    }
	//    std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
	//    // Wypis na ekran pomiarów czasowych i liczby usunięć
	//    std::chrono::duration<double, std::milli> ms_double_remove = t4 - t3;
	//    std::cout << "Usuwanie " << m << " losowych kluczy zajelo: " << ms_double_remove.count() << "ms, usuniecia: " << removeCount << "\n";
	//    //ht->print();
	//    ht->clear();
	//}
	//delete ht;



	HashTable<int> ht(10);

	std::cout << "Dodanie elementów\n";
	for (int i = 0; i < 7; ++i)
	{
		ht.add(Pair<int>(/*"key" + std::to_string(i)*/random_key(6), i));
	}
	ht.print();
	for (int i = 7; i < 15; ++i)
	{
		ht.add(Pair<int>(/*"key" + std::to_string(i)*/random_key(6), i));
	}
	ht.print();
	for (int i = 15; i < 30; ++i)
	{
		ht.add(Pair<int>(/*"key" + std::to_string(i)*/random_key(6), i));
	}
	ht.print();
	for (int i = 30; i < 50; ++i)
	{
		ht.add(Pair<int>(/*"key" + std::to_string(i)*/random_key(6), i));
	}
	ht.add(Pair<int>("key20", 50));
	ht.print();
	ht.add(Pair<int>("key20", 542434));
	ht.print();

	std::cout << "Wyszukiwanie elementu\n";
	Pair<int>* result = ht.search("key20");
	if (result != nullptr)
	{
		std::cout << "Znaleziono element z kluczem 'key20': " << result->value << "\n";
	}
	else
	{
		std::cout << "Element z kluczem 'key20' nie znaleziono\n";
	}
	/*for (int i = 0; i < 50; ++i)
	{
		Pair<int>* result = ht.search("key" + std::to_string(i));
		if (result != nullptr)
		{
			std::cout << "Znaleziono element z kluczem " << "key" + std::to_string(i) << ": " << result->value << "\n";
		}
		else
		{
			std::cout << "Element z kluczem " << "key" + std::to_string(i) << " nie znaleziono\n";
		}
	}*/

	std::cout << "Usuwanie elementa\n";
	if (ht.remove("key20"))
	{
		std::cout << "Element z kluczem 'key20' usunięto\n";
	}
	else
	{
		std::cout << "Element z kluczem 'key20' nie znaleziono dla usunięcia\n";
	}
	ht.print();
	/*for (int i = 0; i < 50; ++i)
	{
		if (ht.remove("key" + std::to_string(i)))
		{
			std::cout << "Znaleziono element z kluczem " << "key" + std::to_string(i) << " usunieto\n";
		}
		else
		{
			std::cout << "Element z kluczem " << "key" + std::to_string(i) << " nie znaleziono dla usuniecia\n";
		}
		ht.print();
	}*/

	std::cout << "Czyszczenie tablicy mieszającej\n";
	ht.clear();
	ht.print();

	return 0;
}
