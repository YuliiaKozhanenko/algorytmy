//ALGO2 IS1 214A LAB02
//Yuliia Kozhanenko
//ky53726@zut.edu.pl

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
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

std::ostream& operator<<(std::ostream& os, const SomeObject& obj) {

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

    /*T& operator[](int index)
    {
        if (index < 0 || index >= currentSize)
        {
            return;
        }
        return array[index];
    }*/
};

int main() 
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    DynamicArray<SomeObject*>* da = new DynamicArray<SomeObject*>();
    const int order = 7;
    const int n = pow(10, order);

    clock_t t1 = clock();
    double max_time_per_element = 0.0;

    for (int i = 0; i < n; ++i)
    {
        SomeObject* so = new SomeObject();
        clock_t t1_element = clock();

        da->addElement(so);

        clock_t t2_element = clock();
        double time_per_element = (double)(t2_element - t1_element) / CLOCKS_PER_SEC;

        if (time_per_element > max_time_per_element)
        {
            max_time_per_element = time_per_element;
            std::cout << "New max time " << max_time_per_element << "sec at index " << i << std::endl;
        }
    }

    clock_t t2 = clock();
    double total_time = (double)(t2 - t1) / CLOCKS_PER_SEC;
    double amortized_time = total_time / n;

    da->printArray(50);
    std::cout << "Total time: " << total_time << " sec" << std::endl;
    std::cout << "Amortized time: " << std::fixed << std::setprecision(10) << amortized_time << " sec" << std::endl;

    da->clear(true);
    delete da;

    DynamicArray<SomeObject*> arr;

    for (int i = 0; i < 15; ++i)
    {
        SomeObject* so = new SomeObject();
        arr.addElement(so);
    }

    arr.printArray(10);

    SomeObject* element = arr.getElement(5);
    if (element != nullptr)
    {
        std::cout << "\nElement at index 5: " << *element << std::endl;
    }

    SomeObject* newElement = new SomeObject();
    arr.setElement(5, newElement);
    SomeObject* replacedElement = arr.getElement(5);

    if (replacedElement != nullptr)
    {
        std::cout << "\nReplaced element at index 5: " << *replacedElement << std::endl;
    }

    arr.printArray();
    arr.clear(true);

    for (int i = 0; i < 20; ++i)
    {
        SomeObject* so = new SomeObject();
        arr.addElement(so);
    }
    arr.bubbleSort();

    arr.printArray(30);

    return 0;
}
