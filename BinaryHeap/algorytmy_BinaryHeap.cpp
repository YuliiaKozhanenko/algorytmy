//ALGO2 IS1 214A LAB05
//Yuliia Kozhanenko
//ky53726@zut.edu.pl

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <chrono>


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
		field_1 = rand() % 10001;
		field_2 = static_cast<char>('a' + rand() % 26);
	}

	SomeObject()
	{
		generateRandomValues();
	}
};

std::ostream& operator<<(std::ostream& os, const SomeObject& obj)
{

	os << "field_1: " << obj.field_1 << ", field_2: " << obj.field_2;
	return os;
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
			std::cout << "Index out of range\n";
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


	T getElement(int index) const //(b)
	{
		if (currentSize == 0)
		{
			std::cout << "Array is empty, generating and returning a random element\n";
			T randomElement = T();
			return randomElement;
		}
		if (index < 0 || index >= currentSize)
		{
			std::cout << "Index out of range, returning the first element\n";
			return array[0];
		}

		if constexpr (is_pointer<T>::value)
		{
			T original = array[index];
			if (original == nullptr)
			{
				return nullptr;
			}
			return new SomeObject(*original); // Deep copy
		}
		else
		{
			return array[index];
		}
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
	void clear(bool freeMemory = false) //(d)
	{
		if (freeMemory)
		{
			if constexpr (is_pointer<T>::value)
			{
				for (int i = 0; i < currentSize; ++i)
				{
					delete array[i];
				}
			}
		}

		delete[] array;
		maxSize = 1;
		currentSize = 0;
		array = new T[maxSize];
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
class BinaryHeap
{
public:
	BinaryHeap() = default;

	void addElement(const T& value)
	{
		array.addElement(value);
		heapifyUp(array.size() - 1);
	}
	T poll()
	{
		if (array.size() == 0)
		{
			return T();
		}

		T MaxValue = array.getElement(0);

		T lastElement = array.getElement(array.size() - 1);
		//array.removeElement(lastElement);
		array.removeElement(array.size() - 1);
		if (array.size() > 0)
		{
			array.setElement(0, lastElement);
			heapifyDown(0);
		}

		return MaxValue;
	}
	void clear()
	{
		array.clear(true);
	}
	bool isEmpty()
	{
		return array.size() == 0;
	}
	void print()
	{
		array.printArray();
	}
private:
	DynamicArray<T> array;

	int IndexOfParent(int index)
	{
		return (index - 1) / 2;
	}
	int IndexOfLeft(int index)
	{
		return 2 * index + 1;
	}
	int IndexOfRight(int index)
	{
		return 2 * index + 2;
	}

	void heapifyUp(int index)
	{
		int parentIndex = IndexOfParent(index);
		if constexpr (is_pointer<T>::value)
		{
			if (index && *array.getElement(parentIndex) < *array.getElement(index))
			{
				array.swap(parentIndex, index);
				heapifyUp(parentIndex);
			}
		}
		else
		{
			if (index && array.getElement(parentIndex) < array.getElement(index))
			{
				array.swap(parentIndex, index);
				heapifyUp(parentIndex);
			}
		}

	}
	void heapifyDown(int index)
	{
		int left = IndexOfLeft(index);
		int right = IndexOfRight(index);
		int largest = index;

		if constexpr (is_pointer<T>::value)
		{
			if (left < array.size() && *array.getElement(left) > *array.getElement(largest))
				largest = left;
			if (right < array.size() && *array.getElement(right) > *array.getElement(largest))
				largest = right;
		}
		else
		{
			if (left < array.size() && array.getElement(left) > array.getElement(largest))
				largest = left;
			if (right < array.size() && array.getElement(right) > array.getElement(largest))
				largest = right;
		}

		if (largest != index)
		{
			array.swap(largest, index);
			heapifyDown(largest);
		}
	}
};

int main()
{
	BinaryHeap<SomeObject*> heap;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; ++i)
	{
		SomeObject* newElement = new SomeObject();
		heap.addElement(newElement);
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "Time to insert 100 elements: " << ms_double.count() << " ms\n";

	heap.print();

	t1 = std::chrono::high_resolution_clock::now();
	while (!heap.isEmpty())
	{
		SomeObject* maxElement = heap.poll();
		//std::cout << "Root/max element: " << *maxElement << std::endl;
		delete maxElement;
	}
	t2 = std::chrono::high_resolution_clock::now();
	ms_double = t2 - t1;
	std::cout << "Time to remove 100 elements: " << ms_double.count() << " ms.\n";

	heap.clear();

	return 0;
}