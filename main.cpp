#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "database.h"

void addEntry(Database &database, Entry* entry);
void resize(Database &database);
void* addArray(Database &database, Entry *entry);
void list(Database &database);
void printArray(Entry* getEntry);
void init(Database &database);

int arrayCount = 0;

void init(Database &database) {
    database.capacity = 10;
    database.dataSize = 0;
    database.entries = new Entry* [database.capacity];
}

Entry *create(Type type, std::string key, void *value) {
    Entry* entry = new Entry;
    entry->type = type;
    entry->key = key;
    entry->value = value;
    return entry;
}

void addEntry(Database &database, Entry* entry) {
    if (database.dataSize == database.capacity) {
        resize(database);
    }

    database.entries[database.dataSize] = entry;
    database.dataSize++;
}

void resize(Database &database) {
    database.capacity *= 2;
    Entry** newEntries = new Entry* [database.capacity];
    for (int i = 0; i<database.dataSize; i++) {
        newEntries[i] = database.entries[i];
    }
    delete[] database.entries;
    database.entries = newEntries;
}

Entry *get(Database &database, std::string &key) {
    for (int i = 0; i < database.dataSize; i++) {
        if ((database.entries[i]->key) == key) {
            return database.entries[i];
            
            // return getEntry;
        }
    }
    return nullptr;
}

void* addArray(Database &database, Entry *entry) {
    entry->type = ARRAY;
    Array* array = new Array;
    array->type = INT;
    std::cout << "type (int, double, string, array): ";
    std::string arrayType;
    std::cin >> arrayType;

    std::cout <<"size: ";
    std::cin >> array->size;

    entry->value = new std::string;
    *static_cast<std::string*>(entry->value) += "[";
  
    if (arrayType == "int") {
        array->type = INT;
        array->items = new int [array->size];
        // std::string intItems;
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]: ";
            std::cin >> *(static_cast<int*>(array->items)+i);
        }
        for (int i=0; i< array->size; i++) {
            if (i>0) {
                *static_cast<std::string*>(entry->value) += ", ";
            }
            *static_cast<std::string*>(entry->value) += std::to_string(*(static_cast<int*>(array->items) + i));
        }
    }

    else if (arrayType == "double") {
        array->type = DOUBLE;
        array->items = new double [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]: ";
            std::cin >> *(static_cast<double*>(array->items)+i);
        }
        for (int i=0; i< array->size; i++) {
            if (i>0) {
                *static_cast<std::string*>(entry->value) += ", ";
            }
            *static_cast<std::string*>(entry->value) += std::to_string(*(static_cast<double*>(array->items) + i));
        }
    }

    else if (arrayType == "string") {
        array->type = STRING;
        array->items = new std::string [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]: ";
            std::cin.ignore();
            std::string stringInput;
            getline(std::cin, stringInput);
            *((static_cast<std::string*>(array->items) + i)) = stringInput;
        }
        for (int i=0; i< array->size; i++) {
            if (i>0) {
                *static_cast<std::string*>(entry->value) += ", ";
            }
            *static_cast<std::string*>(entry->value) += *(static_cast<std::string*>(array->items) + i);
        }
    }

    else if (arrayType == "array") {
        array->type = ARRAY;
        array->items = new void* [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]: ";
            Entry* newEntry = create(ARRAY, "array", nullptr);

            arrayCount++;
            void* arrayEntry = addArray(database, newEntry);
            ((void**)array->items)[i] = arrayEntry;

            arrayCount--;
        }
        for (int i=0; i< array->size; i++) {
            if (i>0) {
                *static_cast<std::string*>(entry->value) += ", ";
            }
            *static_cast<std::string*>(entry->value) += *(static_cast<void**>(array->items) + i);
            // *static_cast<std::string*>(static_cast<void*>(entry->value)) += *(static_cast<std::string*>(static_cast<void*>(array->items) + i));
            // *static_cast<std::string*>(entry->value) = *static_cast<std::string*>(static_cast<char*>(array->items) + i * sizeof(std::string));
        }
    }
    *static_cast<std::string*>(entry->value) += "]";

    // *static_cast<Array*>(entry->value) = array;
    if (arrayCount == 0) {
        addEntry(database, entry);
    }
    // return entry->value;
    return static_cast<std::string*>(entry->value);
}

void add(Database &database, Entry *entry){
    std::cout << "key: ";
    std::cin >> entry->key;
    std::cout << "type (int, double, string, array): ";
    std::string inputType;
    std::cin >> inputType;
    std::cout << "value: ";
  
    if (inputType == "int") {
        entry->type = INT;
        entry->value = new int;
        std::cin >> *(static_cast<int*>(entry->value)); 
        addEntry(database, entry);
    }

    else if (inputType == "double") {
        entry->type = DOUBLE;
        entry->value = new double;
        std::cin >> *(static_cast<double*>(entry->value)); 
        addEntry(database, entry);
    }

    else if (inputType == "string") {
        entry->type = STRING;
        entry->value = new std::string;
        std::cin.ignore();
        std::string stringInput;
        getline(std::cin, stringInput);
        *(static_cast<std::string*>(entry->value)) = stringInput;
        addEntry(database, entry);
    }

    else if (inputType == "array") {
        addArray(database, entry);
    }
}

void list(Database &database) {
    for (int i = 0; i < database.dataSize; i++) {
        std::cout << database.entries[i]->key << ": ";
        if (database.entries[i]->type == INT) {
            std::cout << *static_cast<int*>(database.entries[i]->value);
        }
        if (database.entries[i]->type == DOUBLE) {
            std::cout << *static_cast<double*>(database.entries[i]->value);
        }
        else if (database.entries[i]->type == STRING) {
            std::cout << "\"" << *static_cast<std::string*>(database.entries[i]->value) << "\"";
        }
        else if (database.entries[i]->type == ARRAY) {
            std::cout << *static_cast<std::string*>(database.entries[i]->value);
        }
        std::cout << std::endl;
    }
}

void remove(Database &database, std::string &key) {
    int index = -1;
    for (int i = 0; i < database.dataSize; i++) {
        if (database.entries[i]->key == key) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        std::cout << "not found" << std::endl;
        return;
    }

    delete database.entries[index];
    database.entries[index] = database.entries[database.dataSize - 1];
    database.dataSize--;
}

void destroy(Database &database) {
    for (int i = 0; i< database.dataSize; i++) {
        delete database.entries[i];
    }
    delete[] database.entries;
}

void printArray(Entry* getEntry) {
    if (getEntry->type == INT) {
        std::cout << *static_cast<int*>(getEntry->value);
    }
    else if (getEntry->type == DOUBLE) {
        std::cout << *static_cast<double*>(getEntry->value);
    }
    if (getEntry->type == STRING) {
        std::cout << "\"" << *static_cast<std::string*>(getEntry->value) << "\"";
    }
    if (getEntry->type == ARRAY) {
        std::cout << *static_cast<std::string*>(getEntry->value);
    }
}
int main() {
    Database *database = new Database;

    init(*database);

    while(true) {
        std::cout << "command (list, add, get, del, exit): ";
        std::string command;
        std::cin >> command;
        std::string key;

        if (command == "list") {
            list(*database);
        }

        else if (command == "add") {
            Entry* entry = create(INT, "key", nullptr);
            add(*database, entry);
        }

        else if (command == "del") {
            std::cout << "key: ";
            std::cin >> key;
            remove(*database, key);
        }

        else if (command == "get") {
            std::cout << "key: ";
            std::cin >> key;
            Entry* getEntry = get(*database, key);

            if (getEntry != nullptr) {
                std::cout << getEntry->key << ": ";

                printArray(getEntry);
            }
            std::cout << std::endl;
        }

        else if (command == "exit") {
            destroy(*database);
            exit(0);
        }

        else {
            std::cout << "invalid command" << std::endl;
        }
        std::cout << std::endl;
    }
}