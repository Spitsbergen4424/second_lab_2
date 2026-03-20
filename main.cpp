#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime> 
#include <chrono>

using namespace std;
using namespace chrono;

struct List {
    int data;
    List *head;
    List *tail;
};

List *inputListFixed () {
    cout << "Введите длину списка\n";
    int length; 
    cin >> length;
    srand(time(0));

    auto start = steady_clock::now();

    List *curr = nullptr, *next = nullptr;
    for (int i=0; i<length; i++) {
        curr = new List;
        curr->data = rand() % 100;
        curr->tail = next;

        if (next) {
            next->head = curr;
        }
        next = curr;
    }
    if (curr)
        curr->head = nullptr;

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время создания списка: " << duration.count() << "\n";
    
    return curr;
}

List *inputListFree () {
    string line;
    cin.ignore();
    getline(cin, line);
    stringstream s(line);

    auto start = steady_clock::now();

    List *curr = nullptr, *next = nullptr;
    int buf;
    while (s >> buf) {
        curr = new List;
        curr->data = buf;
        curr->tail = next;

        if (next) {
            next->head = curr;
        }
        next = curr;
    }
    if (curr)
        curr->head = nullptr;

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время создания списка: " << duration.count() << "\n";
    
    return curr;
}

List *createList () {
    cout << "Как хотите задать список?\n 1) По количесву элементов\n 2) В свободном формате\n";
    int choise;
    cin >> choise;
    
    List *curr = nullptr;
    if (cin.fail())
        return curr;
    switch (choise) {
        case 1: 
            curr = inputListFixed();
            break; 
        case 2:
            curr = inputListFree();
            break;
        default:
            break;
    }
    return curr;
}

int lengthList (List *beg) {
    int length = 0;
    while (beg) {
        length++;
        beg = beg->tail;
    }
    return length;
}

List *listItemIndex(List *beg, int index, bool err = true) {
    auto start = steady_clock::now();

	while (beg && (index--))
		beg = beg -> tail;
	if (err && !beg)
		cout << "Элемент списка отсутствует \n";

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    if (err)
        cout <<"Время поиска элемента: " << duration.count() << "\n";
	return beg;
}

List *listItemValue(List *beg, int value, bool err = true) {
    auto start = steady_clock::now();

	while (beg && beg->data != value)
		beg = beg -> tail;
	if (err && !beg)
		cout << "Элемент списка отсутствует \n";

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    if (err)
        cout <<"Время поиска элемента: " << duration.count() << "\n";
	return beg;
}

void deleteItem(List *&beg, int index) {
    auto start = steady_clock::now();

    if(index >= lengthList(beg) || index < 0) {
        cout << "Неверный индекс\n";
        return;
    }

    List *item;
    if (!index) {
        item = beg -> tail;
        delete beg;
        beg = item;
        if (beg)
            beg -> head = nullptr;
        return;
    }
    item = listItemIndex(beg, index - 1, 0);
    List *dItem = item -> tail;
    item -> tail = dItem -> tail;
    if (item->tail)
        item -> tail -> head = item;
    delete dItem;
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время удаления элемента: " << duration.count() << "\n";
}

void insertItem ( List *&beg, int index, int value ) {
    auto start = steady_clock::now();

    if (index > lengthList(beg) || index < 0) {
        cout << "Неверный индекс\n";
        return;
    }

    List *item = new List;
    item -> data = value;
    if (!index || !beg) {
        beg -> head = item;
        item -> head = nullptr;
        item -> tail = beg;
        beg = item;
    }
    else {
        List *predItem = beg;
        --index;
        if (index+1 == lengthList(beg)) {
            while (predItem->tail) {
                predItem = predItem -> tail; 
            }
            predItem->tail = item;
            item->head = predItem;
            item->tail = nullptr;
        }
        else {
            while (predItem -> tail && (index--)) {
                predItem = predItem -> tail; 
            }
            item -> head = predItem;
            if (item->tail)
                item -> tail -> head = item;
            item -> tail = predItem -> tail;
            predItem -> tail = item;
        }
    }
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время вставки элемента: " << duration.count() << "\n";
}

void swapItem (List *&beg, int first, int second) {
    auto start = steady_clock::now();

    if (first >= lengthList(beg) || second >= lengthList(beg) || first < 0 || second < 0) {
        cout << "Неверный индекс\n";
        return;
    }
    if (first > second) 
        swap (first, second);

    List *buf = nullptr, *curr = beg;
    int counter = 0;
    if (first+1 == second) {
        while (counter != first+1) {
            if (counter == first) {
                if (curr->head) {
                    curr->head->tail = curr->tail;
                }
                if (curr->tail->tail) {
                    curr->tail->tail->head = curr;
                }
                buf = curr->tail->tail;
                curr->tail->tail = curr->tail->head;
                curr->tail->head = curr->head;
                curr->head = curr->tail;
                curr->tail = buf;
            }
            curr = curr->tail;
            counter++; 
        }
    }
    else {
        while (counter != second+1) {
            if (counter == first) {
                buf = curr;
            }
            if (counter == second) {
                if (curr->tail) {
                    curr->tail->head = buf;
                }
                curr->head->tail = buf;
                if (buf->head) {
                    buf->head->tail = curr;
                }
                buf->tail->head = curr;
                List *tailBuf, *headBuf;
                tailBuf = curr->tail;
                headBuf = curr->head;
                curr->tail = buf->tail;
                curr->head = buf->head;
                buf->tail = tailBuf;
                buf->head = headBuf;
            }
            curr = curr->tail;
            counter++;
        }
    }
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время перестановки элементов: " << duration.count() << "\n";
}

void deleteList (List *&beg) {
    List *next;
    while (beg) {
        next = beg->tail;
        delete beg;
        beg = next;
    }
}
// -------------------
// DYNAMIC ARRAY
// -------------------

int *inputArrayFixed (int &length) {
    cout << "Введите длину массива\n";
    int length; 
    cin >> length;
    srand(time(0));

    auto start = steady_clock::now();

    int *curr = new int[length];
    for (int i=0; i<length; i++) {
        curr[i] = rand() % 100;
    }

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время создания массива: " << duration.count() << "\n";
    
    return curr;
}

int *inputArrayFree (int &length) {
    string line;
    cin.ignore();
    getline(cin, line);
    stringstream s(line);

    auto start = steady_clock::now();

    int in, length = 0;
    int *curr = new int[length];
    while (s >> in) {
        int *buf = new int[length+1];
        for (int i=0; i< length; i++) {
            buf[i] = curr[i];
        }
        delete[] curr;
        curr = buf;
        curr[length] = in;
        length++;
    }
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время создания массива: " << duration.count() << "\n";
    
    return curr;
}

int *createArray (int &length) {
    cout << "Как хотите задать массив?\n 1) По количесву элементов\n 2) В свободном формате\n";
    int choise;
    cin >> choise;
    
    int *curr = nullptr;
    if (cin.fail())
        return curr;
    switch (choise) {
        case 1: 
            curr = inputArrayFixed(length);
            break; 
        case 2:
            curr = inputArrayFree(length);
            break;
        default:
            break;
    }
    return curr;
}

int arrayItemValue(int *beg, int length, int value, bool err = true) {
    auto start = steady_clock::now();
    int index = 0;
	while (index != length && *(beg+index) != value)
		index++;
	if (err && index == length)
		cout << "Элемент списка отсутствует \n";

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    if (err)
        cout <<"Время поиска элемента: " << duration.count() << "\n";
	return index;
}

void deleteItemArray(int *&beg, int &length, int index) {
    auto start = steady_clock::now();

    if(index >= length || index < 0) {
        cout << "Неверный индекс\n";
        return;
    }

    if (length == 0) {
        cout << "Массив пустой\n";
        return;
    }

    int *buf = new int[--length];
    int delIndex = 0;
    for (int i=0; i<index; i++) {
        buf[i] = *(beg + i);
    }
    for (int i=index; i<length; i++) {
        buf[i] = *(beg + i +1);
    }
    delete[] beg;
    beg = buf;

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<"Время удаления элемента: " << duration.count() << "\n";
}

void insertItemArray(int *&arr, int &length, int index, int value) {
    auto start = steady_clock::now();

    if (index > length || index < 0) {
        cout << "Неверный индекс\n";
        return;
    }

    int *newArr = new int[length + 1];
    
    for (int i = 0; i < index; i++)
        newArr[i] = arr[i];

    newArr[index] = value;

    for (int i = index; i < length; i++)
        newArr[i + 1] = arr[i];
    
    delete[] arr;
    arr = newArr;
    length++;

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Время вставки элемента: " << duration.count() << " мкс\n";
}

void swapItemArray(int *arr, int length, int first, int second) {
    auto start = steady_clock::now();

    if (first >= length || second >= length || first < 0 || second < 0) {
        cout << "Неверный индекс\n";
        return;
    }

    int buf = arr[first];
    arr[first] = arr[second];
    arr[second] = buf;

    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Время перестановки элементов: " << duration.count() << " мкс\n";
}

int main() {
    List *list = createList();

    bool continueFlag = true;
    int choise1;
    while (continueFlag) {
        cout << "Что хотите сделать?\n 0) выйти\n 1) перезаписать список\n";
        cout << " 2) вставить элемент\n 3) удалить элемент\n 4) поменять элементы местами\n 5) найти элемент\n";
        cout << " 6) динамический массив\n";
        cin >> choise1;
        switch (choise1) {
        case 0: {
            continueFlag = false;
            break;
        }
        case 1: {
            deleteList(list);
            list = createList();
            break;
        }
        case 2: {
            int index, value; 
            cout << "Какой элемент хотите вставить?\n";
            cin >> value;
            cout << "На какую позицию?\n";
            cin >> index;
            insertItem(list, index, value);
            break;
        }
        case 3: {
            if (!list) {
                cout << "Список пустой\n";
                break;
            }
            int index;
            cout << "Какой элемент хотите удалить?\n";
            cin >> index;
            deleteItem(list, index);
            break;
        }
        case 4: {
            if (!list) {
                cout << "Список пустой\n";
                break;
            }
            int first, second; 
            cout << "Какие элементы хотите поменять?\n";
            cin >> first >> second;
            swapItem(list, first, second);
            break;
        }
        case 5: {
            if (!list) {
                cout << "Список пустой\n";
                break;
            }
            int choise2;
            cout << "Как хотите искать элемент?\n 1) по значению\n 2) по индексу\n";
            cin >> choise2;
            int buf;
            List *element = list;
            switch (choise2)
            {
            case 1:
                cout << "Введите значение\n";
                cin >> buf;
                element = listItemValue(element, buf, 0);
                break;
            case 2:
                cout << "Введите индекс\n";
                cin >> buf;
                element = listItemIndex(element, buf, 0);
                break;
            default:
                break;
            }
            if (element) 
                cout << "Ваш элемент: " << element->data << '\n'; 
            break;
        }
        case 6: {
            cout << "\nРАБОТА С МАССИВОМ\n";
            
            int arrLength = 0;
            int *arr = createArray(arrLength);
            
            if (!arr) {
                cout << "Ошибка создания массива\n";
                break;
            }
            
            bool arrMenu = true;
            while (arrMenu) {
                cout << "\nЧто хотите сделать?\n";
                cout << " 0) Вернуться в главное меню\n";
                cout << " 1) Вставить элемент\n";
                cout << " 2) Удалить элемент по индексу\n";
                cout << " 3) Удалить элемент по значению\n";
                cout << " 4) Поменять элементы местами\n";
                cout << " 5) Найти элемент по индексу\n";
                cout << " 6) Вывести массив\n";
                
                int choice;
                cin >> choice;
                
                switch (choice) {
                    case 0:
                        arrMenu = false;
                        break;
                        
                    case 1: {
                        int index, value;
                        cout << "Значение: ";
                        cin >> value;
                        cout << "Индекс: ";
                        cin >> index;
                        insertItemArray(arr, arrLength, index, value);
                        break;
                    }
                    
                    case 2: {
                        if (arrLength == 0) {
                            cout << "Массив пуст\n";
                            break;
                        }
                        int index;
                        cout << "Индекс для удаления: ";
                        cin >> index;
                        deleteItem(arr, arrLength, index);
                        arrLength--; 
                        break;
                    }
                    
                    case 3: {
                        if (arrLength == 0) {
                            cout << "Массив пуст\n";
                            break;
                        }
                        int value;
                        cout << "Значение для удаления: ";
                        cin >> value;
                        deleteItemArrayByValue(arr, arrLength, value);
                        if (arrLength > 0) arrLength--;
                        break;
                    }
                    
                    case 4: {
                        if (arrLength < 2) {
                            cout << "Недостаточно элементов\n";
                            break;
                        }
                        int first, second;
                        cout << "Индексы элементов: ";
                        cin >> first >> second;
                        swapItemArray(arr, arrLength, first, second);
                        break;
                    }
                    
                    case 5: {
                        if (arrLength == 0) {
                            cout << "Массив пуст\n";
                            break;
                        }
                        auto start = steady_clock::now();
                        
                        int index;
                        cout << "Индекс: ";
                        cin >> index;
                        
                        if (index >= arrLength || index < 0) {
                            cout << "Элемент отсутствует\n";
                        } else {
                            cout << "Элемент: " << arr[index] << "\n";
                        }
                        
                        auto end = steady_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Время поиска элемента: " << duration.count() << " мкс\n";
                        break;
                    }
                    
                    case 6: {
                        cout << "Массив: ";
                        for (int i = 0; i < arrLength; i++)
                            cout << arr[i] << " ";
                        cout << "\n";
                        break;
                    }
                    
                    default:
                        cout << "Неверный выбор\n";
                }
            }
            
            delete[] arr;
            break;
        }
        default:
            break;
        }
    }
    deleteList(list);
}