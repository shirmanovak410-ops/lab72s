//
// Created by Ксения on 08.11.2025.
/*
1. Добавить элемент
2. Отобразить список
3. Найти элемент
4. Удалить первое вхождение
5. Удалить все вхождения
6. Очистить список
7. Сохранить в файл
8. Загрузить из файла
0. Выйти
программа принимает значения -t -c -e

параметр -t отвечает за тип связанного списка
1 - однонаправленный связанный список
2 - двунаправленный связный список
3 - однонаправленный кольцевой связный список
4 - двунаправленный кольцевой связный список

параметр -c отвечает за изначальное количество элементов в списке

параметр -e принимает изначальные элементы связанного списка
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
struct node {
    int data;
    node* next;
    node* prev;
    node(int value) : data(value), next(nullptr), prev(nullptr) {}
};
class LinkedList {
private:
    node* head;
    node* tail;
    int listType;
    int size;
public:
    LinkedList(int type) : head(nullptr), tail(nullptr), listType(type), size(0) {}
    ~LinkedList() {
        clear();
    }
    bool isEmpty() const {
        return head == nullptr;
    }
    void addElement(int data) {
        node* newNode = new node(data);
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
            if (listType == 3 || listType == 4) {
                newNode->next = head;
                if (listType == 4) {
                newNode->prev = head;
                }
            }
        } else {
            switch (listType) {
                case 1:
                    tail->next = newNode;
                    tail = newNode;
                    break;
                case 2:
                    newNode->prev = tail;
                    tail->next = newNode;
                    tail = newNode;
                    break;
                case 3:
                    newNode->next = head;
                    tail->next = newNode;
                    tail = newNode;
                    break;
                case 4:
                    newNode->next = head;
                    newNode->prev = tail;
                    tail->next = newNode;
                    head->prev = newNode;
                    tail = newNode;
                    break;
            }
        }
        size++;
    }
    void display() const {
        if (isEmpty()) {
            std::cout << "List is empty" << std::endl;
            return;
        }
        node* current = head;
        if (listType == 1 || listType == 2) {
            while (current != nullptr) {
                std::cout << current->data;
                current = current->next;
                if (current != nullptr) {
                    if (listType == 1) std::cout << " > ";
                    else std::cout << " <-> ";
                }
            }
        } else {
            do {
                std::cout << current->data;
                current = current->next;
                if (current != head) {
                    if (listType == 3) std::cout << " >0 ";
                    else std::cout << " <>0 ";
                }
            } while (current != head);
            std::cout << " (head)";
        }
        std::cout << std::endl;
    }
    std::vector<int> findElement(int data) const {
        std::vector<int> positions;
        if (isEmpty()) return positions;
        node* current = head;
        int position = 0;
        if (listType == 1 || listType == 2) {
            while (current != nullptr) {
                if (current->data == data) {
                    positions.push_back(position);
                }
                current = current->next;
                position++;
            }
        } else {
            do {
                if (current->data == data) {
                    positions.push_back(position);
                }
                current = current->next;
                position++;
            } while (current != head);
        }
        return positions;
    }
    bool deleteElement(int data, bool deleteAll = false) {
        if (isEmpty()) return false;
        bool deleted = false;
        node* current = head;
        if (listType == 1 || listType == 2) {
            node* prev = nullptr;
            while (current != nullptr) {
                if (current->data == data) {
                    node* toDelete = current;
                    if (current == head) {
                        head = head->next;
                        if (head != nullptr && listType == 2) {
                            head->prev = nullptr;
                        }
                    } else if (current == tail) {
                        tail = prev;
                        if (listType == 2) {
                            tail->next = nullptr;
                        }
                    } else {
                        prev->next = current->next;
                        if (listType == 2) {
                            current->next->prev = prev;
                        }
                    }
                    current = current->next;
                    delete toDelete;
                    size--;
                    deleted = true;
                    if (!deleteAll) return true;
                } else {
                    prev = current;
                    current = current->next;
                }
            }
        }
        return deleted;
    }
    void clear() {
        if (isEmpty()) return;
        if (listType == 1 || listType == 2) {
            node* current = head;
            while (current != nullptr) {
                node* next = current->next;
                delete current;
                current = next;
            }
        } else {
            tail->next = nullptr;
            node* current = head;
            while (current != nullptr) {
                node* next = current->next;
                delete current;
                current = next;
            }
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        std::string typeSymbol;
        switch (listType) {
            case 1: typeSymbol = ">"; break;
            case 2: typeSymbol = "<>"; break;
            case 3: typeSymbol = ">0"; break;
            case 4: typeSymbol = "<>0"; break;
        }
        file << "type:" << typeSymbol << std::endl;
        file << "elements:";
        if (!isEmpty()) {
            node* current = head;
            if (listType == 1 || listType == 2) {
                while (current != nullptr) {
                    file << current->data;
                    current = current->next;
                    if (current != nullptr) file << ",";
                }
            } else {
                do {
                    file << current->data;
                    current = current->next;
                    if (current != head) file << ",";
                } while (current != head);
            }
        }
        file << std::endl;
        file.close();
        return true;
    }
    bool loadFromFile(const std::string& filename) {
        if (!isEmpty()) {
            std::cout << "List already exists" << std::endl;
            return false;
        }
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Cannot open file" << std::endl;
            return false;
        }
        try {
            std::string line;
            int fileType = 0;
            std::vector<int> elements;
            if (!std::getline(file, line)) return false;
            if (line.find("type:") == 0) {
                std::string typeStr = line.substr(5);
                if (typeStr == ">") fileType = 1;
                else if (typeStr == "<>") fileType = 2;
                else if (typeStr == ">0") fileType = 3;
                else if (typeStr == "<>0") fileType = 4;
                else return false;
            }
            if (!std::getline(file, line)) return false;
            if (line.find("elements:") == 0) {
                std::string elementsStr = line.substr(9);
                if (!elementsStr.empty()) {
                    std::stringstream ss(elementsStr);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        elements.push_back(std::stoi(token));
                    }
                }
            }
            file.close();
            listType = fileType;
            for (int element : elements) {
                addElement(element);
            }
            return true;
        } catch (...) {
            clear();
            return false;
        }
    }
};
bool parseArguments(int argc, char* argv[], int& listType, int& count, std::vector<int>& elements) {
    if (argc < 6) return false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-t") {
            if (i + 1 >= argc) return false;
            listType = std::stoi(argv[++i]);
            if (listType < 1 || listType > 4) return false;
        } else if (arg == "-c") {
            if (i + 1 >= argc) return false;
            count = std::stoi(argv[++i]);
            if (count < 0) return false;
        } else if (arg == "-e") {
            if (i + 1 < argc) {
                std::string elementsStr = argv[++i];
                if (!elementsStr.empty()) {
                    std::stringstream ss(elementsStr);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        elements.push_back(std::stoi(token));
                    }
                }
            }
        } else {
            return false;
        }
    }
    if (listType == 0 || count == -1) return false;
    if (static_cast<int>(elements.size()) != count) return false;
    return true;
}
void printMenu() {
    std::cout << "\nLinked List Menu:" << std::endl;
    std::cout << "1. Add element" << std::endl;
    std::cout << "2. Display list" << std::endl;
    std::cout << "3. Find element" << std::endl;
    std::cout << "4. Delete first occurrence" << std::endl;
    std::cout << "5. Delete all occurrences" << std::endl;
    std::cout << "6. Clear list" << std::endl;
    std::cout << "7. Save to file" << std::endl;
    std::cout << "8. Load from file" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";
}
int main(int argc, char* argv[]) {
    int listType = 0;
    int count = -1;
    std::vector<int> elements;
    LinkedList* list = nullptr;
    if (argc > 1) {
        if (!parseArguments(argc, argv, listType, count, elements)) {
            std::cout << "Invalid arguments" << std::endl;
            return 1;
        }
        list = new LinkedList(listType);
        for (int element : elements) {
            list->addElement(element);
        }
        std::cout << "List created: type " << listType << ", elements " << count << std::endl;
    } else {
        std::cout << "List type (1-4): ";
        std::cin >> listType;
        if (listType < 1 || listType > 4) {
            std::cout << "Invalid type" << std::endl;
            return 1;
        }
        std::cout << "Number of elements: ";
        std::cin >> count;
        if (count < 0) {
            std::cout << "Invalid count" << std::endl;
            return 1;
        }
        list = new LinkedList(listType);
        for (int i = 0; i < count; i++) {
            int element;
            std::cout << "Element " << (i + 1) << ": ";
            std::cin >> element;
            list->addElement(element);
        }
        std::cin.ignore();
    }
    int choice;
    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case 1: {
                int element;
                std::cout << "Element to add: ";
                std::cin >> element;
                list->addElement(element);
                break;
            }
            case 2:
                list->display();
                break;
            case 3: {
                int element;
                std::cout << "Element to find: ";
                std::cin >> element;
                std::vector<int> positions = list->findElement(element);
                if (positions.empty()) {
                    std::cout << "Not found" << std::endl;
                } else {
                    std::cout << "Found at positions: ";
                    for (size_t i = 0; i < positions.size(); i++) {
                        std::cout << positions[i];
                        if (i < positions.size() - 1) std::cout << ", ";
                    }
                    std::cout << std::endl;
                }
                break;
            }
            case 4: {
                int element;
                std::cout << "Element to delete: ";
                std::cin >> element;
                if (list->deleteElement(element, false)) {
                    std::cout << "Deleted" << std::endl;
                } else {
                    std::cout << "Not found" << std::endl;
                }
                break;
            }
            case 5: {
                int element;
                std::cout << "Element to delete all: ";
                std::cin >> element;
                if (list->deleteElement(element, true)) {
                    std::cout << "All occurrences deleted" << std::endl;
                } else {
                    std::cout << "Not found" << std::endl;
                }
                break;
            }
            case 6:
                list->clear();
                std::cout << "List cleared" << std::endl;
                break;
            case 7: {
                std::string filename;
                std::cout << "Filename to save: ";
                std::getline(std::cin, filename);
                if (list->saveToFile(filename)) {
                    std::cout << "Saved" << std::endl;
                } else {
                    std::cout << "Save failed" << std::endl;
                }
                break;
            }
            case 8: {
                std::string filename;
                std::cout << "Filename to load: ";
                std::getline(std::cin, filename);
                if (list->loadFromFile(filename)) {
                    std::cout << "Loaded" << std::endl;
                } else {
                    std::cout << "Load failed" << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Goodbye! Have a nice day:)" << std::endl;
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    } while (choice != 0);
    delete list;
    return 0;
}