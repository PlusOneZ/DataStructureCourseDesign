//
// Created by Zhengyi on 2020/12/20.
//

#include "../../MyDS/Vector.h"
#include <random>
#include <limits>
#include <ctime>

template <class Type, class RandomMachine = std::uniform_int_distribution<Type>>
class SortingSystem {
    enum Sortings {
        Bubble, Selection, Insert, Shell,
        Quick, Heap, Merge, Radix, NoSort
    };

    static bool greater(Type t1, Type t2, Sortings s) {
        ++compareCount[s];
        return t1 > t2;
    };
    static bool less(Type t1, Type t2, Sortings s) {
        ++compareCount[s];
        return t1 < t2;
    };
    static bool equal(Type t1, Type t2, Sortings s) {
        ++compareCount[s];
        return t1 == t2;
    }
    static void swap(Type &t1, Type &t2, Sortings s) {
        Type t = t1;
        t1 = t2;
        t2 = t;
        ++swapCount[s];
        assignCount[s] += 3;
    }
    template<typename T>
    inline static const T& min(const T &a, const T &b, Sortings s = NoSort) {
        ++compareCount[s];
        return (a > b) ? b : a;
    }

public:
    SortingSystem() = default;

    static void bubbleSort(Type arr[], size_t n);
    static void selectionSort(Type arr[], size_t n);
    static void insertSort(Type arr[], size_t n);
    static void shellSort(Type arr[], size_t n);
    static void quickSort(Type arr[], size_t n);
    static void heapSort(Type arr[], size_t n);
    static void mergeSort(Type arr[], size_t n);
    static void radixSort(Type arr[], size_t n);
    void resize(int n);
    clock_t testAndClock(Sortings s);
    bool parseCommand(const std::string &cmd);
    void redesignateSize();
    void showTestResultOf(Sortings s, const std::string& name);
    void run();

private:
    // system
    void genRandom(size_t n);
    static void showMenu();

    // quick sort
    inline static void quickSort(Type arr[], Type *l, Type *r);
    inline static Type * partition(Type *first, Type *last, Type pivot);
    inline static const Type& medium(const Type &a, const Type &b, const Type &c);
    // heap sort
    inline static void maxHeapify(Type arr[], int start, int end);
    // radix sort
    inline static Type maxDigits(Type *arr, size_t n, const Type &radix);
    inline static void radixSort(Type arr[], size_t n, const Type& radix);

    static inline void showVec(Type arr[], size_t n);
    static inline void copy(Type *dest, Type *source, size_t size);
    static inline void clearCounters(Sortings s);

private:
    Type *array = nullptr;
    size_t curSize = 0;
    static unsigned long long compareCount[9];
    static unsigned long long swapCount[9];
    static unsigned long long assignCount[9];
};

template <class T, class R>
unsigned long long SortingSystem<T, R>::compareCount[9];
template <class T, class R>
unsigned long long SortingSystem<T, R>::swapCount[9];
template <class T, class R>
unsigned long long SortingSystem<T, R>::assignCount[9];

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::bubbleSort(Type *arr, size_t n) {
    Type temp;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (greater(arr[j], arr[j+1], Bubble)) {
                swap(arr[j], arr[j+1], Bubble);
            }
        }
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::selectionSort(Type *arr, size_t n) {
    for (int i = 0; i < n; ++i) {
        int minIndex = i;
        for (int j = i; j < n; ++j) {
            if (greater(arr[minIndex], arr[j], Selection))
                swap(arr[minIndex], arr[j], Selection);
        }
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::insertSort(Type *arr, size_t n) {
    int j;
    for (int i = 0; i < n; ++i) {
        Type temp = arr[i];
        for (j = i; j > 0 && less(temp, arr[j - 1], Insert); --j) {
            arr[j] = arr[j - 1];
            ++assignCount[Insert];
        }
        arr[j] = temp;
        ++assignCount[Insert];
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::shellSort(Type *arr, size_t n) {
    int j;
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; i++) {
            Type temp = arr[i];
            for (j = i; j >= gap && less(temp, arr[j - gap], Shell); j -= gap) {
                arr[j] = arr[j - gap];
                ++assignCount[Shell];
            }
            arr[j] = temp;
            ++assignCount[Shell];
        }
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::quickSort(Type *arr, size_t n) {
    quickSort(arr, arr, arr + n - 1);
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::quickSort(Type *arr, Type *l, Type *r) {
    if (l >= r)
        return;
    auto mid = l + (r - l) / 2;
    auto &pivot = medium(*l, *mid, *r);
    ++assignCount[Quick];
    auto cut = partition(l, r, pivot);
    quickSort(arr, l, cut);
    quickSort(arr, cut + 1, r);
}

template <class Type, class RandomMachine>
Type * SortingSystem<Type, RandomMachine>::partition(Type *first, Type *last, Type pivot) {
    while (true) {
        while (less(*first, pivot, Quick)) ++first;
        while (less(pivot, *last, Quick))  --last;
        if (first >= last) return last;
        swap(*first, *last, Quick);
        ++first; --last;
    }
}

template <class Type, class RandomMachine>
const Type &
SortingSystem<Type, RandomMachine>::medium(const Type &a, const Type &b, const Type &c) {
    if (less(a, b, Quick))
        if      (less(b, c, Quick)) return b;
        else if (less(a, c, Quick)) return c;
        else                           return a;
    else if (less(a, c, Quick)) return a;
    else if (less(b, c, Quick)) return c;
    else                           return b;
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::heapSort(Type *arr, size_t n) {
    for (int i = n/2 - 1; i >= 0; i--)
        maxHeapify(arr, i, n - 1);
    for (int i = n-1; i > 0; i--) {
        swap(arr[0], arr[i], Heap);
        maxHeapify(arr, 0, i - 1);
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::maxHeapify(Type *arr, int start, int end) {
    int parent = start;
    int child = parent * 2 + 1;
    while (child <= end) {
        if (child + 1 <= end && less(arr[child], arr[child + 1], Heap)) {
            child++;
        }
        if (greater(arr[parent], arr[child], Heap)) {
            return;
        } else {
            swap(arr[parent], arr[child], Heap);
            parent = child;
            child = parent * 2 + 1;
        }
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::mergeSort(Type *arr, size_t n) {
    Type *copy = arr;
    Type *cache = new Type[n];
    for (size_t step = 1; step < n; step += step) {
        for (size_t start = 0; start < n; start += 2 * step) {
            size_t low = start;
            size_t mid = min(start + step, n), high = min(start + 2 * step, n);
            size_t k = low;
            size_t leftStart = low, leftEnd = mid;
            size_t rightStart = mid, rightEnd = high;
            // Merge
            while (leftStart < leftEnd && rightStart < rightEnd) {
                cache[k++] = less(copy[leftStart], copy[rightStart], Merge)
                        ? copy[leftStart++] : copy[rightStart++];
                ++assignCount[Merge];
            }
            // dealing with left parts
            while (leftStart < leftEnd) {
                cache[k++] = copy[leftStart++];
                ++assignCount[Merge];
            }
            while (rightStart < rightEnd) {
                cache[k++] = copy[rightStart++];
                ++assignCount[Merge];
            }
        }
        // Swap merged array with unmerged one,
        Type *temp = copy;
        copy = cache;
        cache = temp;
    }
    if (copy != arr) {
        // make sure arr is the result.
        for (int i = 0; i < n; i++) {
            cache[i] = copy[i];
            ++assignCount[Merge];
        }
        cache = copy;
    }
    delete[] cache;
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::radixSort(Type *arr, size_t n) {
    if (!(typeid(Type) == typeid(unsigned) ||
          typeid(Type) == typeid(unsigned long) ||
          typeid(Type) == typeid(unsigned long long)
    )) {
        std::cerr << "Radix sort is only for positive numeric value." << std::endl;
        return;
    }
    radixSort(arr, n, static_cast<Type>(10));
}

template <class Type, class RandomMachine>
Type SortingSystem<Type, RandomMachine>::maxDigits(Type *arr, size_t n,
                                           const Type &radix) {
    Type max = arr[0];
    for (int i = 1; i < n; ++i) {
        if (less(max, arr[i], Radix)){
            max = arr[i];
            ++assignCount[Radix];
        }
    }
    Type d = static_cast<Type>(1);
    while (max >= radix) {
        max /= radix;
        ++d;
    }
    return d;
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::radixSort(Type *arr, size_t n, const Type &radix) {
    if (radix <= 0) {
        std::cerr << "Invalid radix" << std::endl;
        return;
    }
    Type d = maxDigits(arr, n, radix);
    Type *tmp = new Type[n];
    Type *count = new Type[radix];
    long long i, j, k;
    Type curRadix = static_cast<Type>(1); ++assignCount[Radix];
    for(i = 1; i <= d; i++) {
        for (j = 0; j < radix; j++) {
            count[j] = static_cast<Type>(0);
            ++assignCount[Radix];
        }
        for (j = 0; j < n; j++) {
            k = (arr[j] / curRadix) % radix;
            ++count[k];
        }
        for (j = 1; j < radix; j++) {
            count[j] = count[j - 1] + count[j];
        }
        for (j = n - 1; j >= 0; j--) {
            k = (arr[j] / curRadix) % radix;
            tmp[count[k] - 1] = arr[j];
            ++assignCount[Radix];
            --count[k];
        }
        for (j = 0; j < n; j++) {
            arr[j] = tmp[j];
            ++assignCount[Radix];
        }
        curRadix = curRadix * radix;
    }
    delete []tmp;
    delete []count;
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::showVec(Type *arr, size_t n) {
    using std::cout;
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << "\t";
        if (++cnt == 5) { cout << std::endl; cnt = 0; }
    }
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::copy(Type *dest, Type *source, size_t size) {
    memcpy(dest, source, size * sizeof(Type));
}

template <class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::genRandom(size_t n) {
    if (array != nullptr) {
        delete[](array);
        array = nullptr;
    }
    array = new Type[n];
    RandomMachine machine(std::numeric_limits<Type>::min(), std::numeric_limits<Type>::max());
    std::default_random_engine e(std::clock());
    for (size_t i = 0; i < n; ++i) {
        array[i] = machine(e);
    }
}

template<class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::resize(int n) {
    curSize = n;
    delete[](array);
    array = nullptr;
    genRandom(n);
}

template<class Type, class RandomMachine>
clock_t
SortingSystem<Type, RandomMachine>::testAndClock(SortingSystem::Sortings s) {
    auto cpy = new Type[curSize];
    copy(cpy, array, curSize);
    auto start = std::clock();
    switch (s) {
        case Bubble:
            bubbleSort(cpy, curSize);
            break;
        case Selection:
            selectionSort(cpy, curSize);
            break;
        case Insert:
            insertSort(cpy, curSize);
            break;
        case Shell:
            shellSort(cpy, curSize);
            break;
        case Quick:
            quickSort(cpy, curSize);
            break;
        case Heap:
            heapSort(cpy, curSize);
            break;
        case Merge:
            mergeSort(cpy, curSize);
            break;
        case Radix:
            radixSort(cpy, curSize);
            break;
        default:
            return 0;
    }
    auto end = std::clock();
    return end - start;
}

template<class Type, class RandomMachine>
bool SortingSystem<Type, RandomMachine>::parseCommand(const std::string &cmd) {
    if (cmd.empty()) {
        return true;
    }
    if (cmd.length() > 1) {
        std::cout << "Command " << cmd << " Not valid. Command Menu: \n";
        showMenu();
        return true;
    }

    switch (cmd[0] - '0') {
        case 0:
            redesignateSize();
            break;
        case 1:
            showTestResultOf(Bubble, "Bubble");
            break;
        case 2:
            showTestResultOf(Selection, "Selection");
            break;
        case 3:
            showTestResultOf(Insert, "Straight Insertion");
            break;
        case 4:
            showTestResultOf(Shell, "Shell");
            break;
        case 5:
            showTestResultOf(Quick, "Quick");
            break;
        case 6:
            showTestResultOf(Heap, "Heap");
            break;
        case 7:
            showTestResultOf(Merge, "Merge");
            break;
        case 8:
            showTestResultOf(Radix, "Radix");
            break;
        case 9:
            return false;
        default:
            std::cout << "Command " << cmd << " not valid, try again!\n";
    }
    return true;
}

template<class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::redesignateSize() {
    using std::cin; using std::cout;
    size_t s;
    cout << "Enter new size of random data: ";
    cin >> s;
    while (s <= 0) {
        cout << "Size should be positive! Try again: ";
        cin.clear();
        cin.ignore(std::numeric_limits<int>::max(), '\n');
        cin >> s;
    }
    resize(s);
}

template<class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::showMenu() {
    std::cout << "***********************************************\n"
                 "**        Sorting Algorithm Comparison       **\n"
                 "**-------------------------------------------**\n"
                 "**     0. Re-designate size of random data   **\n"
                 "**     1.          Bubble Sort               **\n"
                 "**     2.       Selection Sort               **\n"
                 "**     3.       Insertion Sort (Straight)    **\n"
                 "**     4.           Shell Sort               **\n"
                 "**     5.           Quick Sort               **\n"
                 "**     6.            Heap Sort               **\n"
                 "**     7.           Merge Sort               **\n"
                 "**     8.           Radix Sort               **\n"
                 "**     9.              EXIT                  **\n"
                 "***********************************************\n";
    std::cout << std::endl;
}

template<class Type, class RandomMachine>
void
SortingSystem<Type, RandomMachine>::
showTestResultOf(SortingSystem::Sortings s, const std::string &name) {
    using std::cout;
    clearCounters(s);
    cout << name << " Sort:\n";
    auto t = testAndClock(s);
    cout << "Time consumption:\t\t" << t << " clocks  =  "
         << (double)t / (double)CLOCKS_PER_SEC << " sec.\n";
    cout << "Comparison times:\t\t" << compareCount[s] << "\n";
    cout << "Swapping times:  \t\t" << swapCount[s] << "\n";
    cout << "Assignment times:\t\t" << assignCount[s] << "\n";
    cout << std::endl;
}

template<class Type, class RandomMachine>
void
SortingSystem<Type, RandomMachine>::clearCounters(SortingSystem::Sortings s) {
    swapCount[s] = 0;
    assignCount[s] = 0;
    compareCount[s] = 0;
}

template<class Type, class RandomMachine>
void SortingSystem<Type, RandomMachine>::run() {
    using std::cout; using std::cin;
    cout << "Welcome to Sorting Comparison System, this is menu:\n";
    showMenu();
    cout << "Firstly, ";
    redesignateSize();
    cout << "Enter Your first command: ";
    std::string cmd;
    std::getline(cin, cmd);
    while (parseCommand(cmd)) {
        cout << "Enter your next command: ";
        std::getline(cin, cmd);
        cout << std::endl;
    }
    cout << "\nBye~" << std::endl;
}

int main() {
    SortingSystem<long> system;
    system.run();
    return 0;
}



