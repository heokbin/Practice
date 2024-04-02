#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

enum Type { INT, DOUBLE, STRING, ARRAY };

void addEntry(Entry* entry);
void resize();
void init(Database &database);
void* addArray(Database &database, Entry *entry);
void add(Database &database, Entry *entry);
void list(Database &database);
void remove(Database &database, std::string &key);
void destroy(Database &database);
void printArray(Entry* getEntry);
Entry *get(Database &database, std::string &key);
Entry *create(Type type, std::string key, void *value);

int arrayCount = 0;

struct Array {
    int size;
    Type type;
    void *items;
};

struct Entry {
    Type type;
    std::string key;
    void *value;
};

struct Entry {
    Type type;
    std::string key;
    void *value;
};

void init(Database &database) {
    database.capacity = 10;
    database.size = 0;
    Entry** entries = new Entry*[capacity];
}

Entry *create(Type type, std::string key, void *value) {
    Entry* entry = new Entry;
    entry->type = type;
    entry->key = key;
    entry->value = value;
    return entry;
}

struct Database {
    Database* database = new Database;
    //int capacity;
    //int size;
    init(database);

    void addEntry(Entry* entry) {
        if (size == capacity) {
            resize();
        }

        entries[size] = entry;
        size++;
    }

    void resize() {
        capacity *= 2;
        Entry** newEntries = new Entry* [capacity];
        for (int i = 0; i<size; i++) {
            newEntries[i] = entries[i];
        }
        delete[] entries;
        entries = newEntries;
    }
}

Entry *get(Database &database, std::string &key) {
    for (int i = 0; i < database.size; i++) {
        if (database.entries[i]->key == key) {
            // std::cout << database.entries[i]->key << ": " << database.entries[i]->value;
            return database.entries[i];
        }
        else {
            return nullptr;
        }
    }
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
  
    if (arrayType == "int") {
        array->type = INT;
        // entry->value = new int [array->size];
        array->items = new int [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]:";
            std::cin >> *(static_cast<int*>(array->items[i]));
            // entry->value[i] = array;
        }
    }

    else if (arrayType == "double") {
        array->type = DOUBLE;
        // entry->value = new double [array->size];
        array->items = new double [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]:";
            std::cin >> *(static_cast<double*>(array->items[i]));
        // entry->value[i] = array;
        }
    }

    else if (arrayType == "string") {
        array->type = STRING;
        // entry->value = new std::string [array->size];
        array->items = new std::string [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]:";
            std::cin.ignore();
            getline(std::cin, *(static_cast<std::string*>(array->items[i])));
            // entry->value[i] = array;
        }
    }

    else if (arrayType == "array") {
        array->type = ARRAY;
        array->items = new Entry* [array->size];
        for (int i = 0; i < array->size; i++) {
            std::cout << "item[" << i << "]:";
            Entry* newEntry = create(ARRAY, "array", nullptr);

            arrayCount++;
            newEntry = addArray(database, newEntry);
            static_cast<Array*>(array->items[i]) = *static_cast<Array*>(newEntry->value);
            arrayCount--;

            // entry->value[i] = array;
            // return entry->value;
        }
    }
    
    entry->value = array;
    // database.addEntry(entry);
    if (arrayCount == 0) {
        database.addEntry(entry);
    }
    return entry->value;
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
        database.addEntry(entry);
    }

    else if (inputType == "double") {
        entry->type = DOUBLE;
        entry->value = new double;
        std::cin >> *(static_cast<double*>(entry->value)); 
        Entry* entryResult = database.create(entry->type, entry->key, entry->value);
        database.addEntry(entryResult);
    }

    else if (inputType == "string") {
        entry->type = STRING;
        entry->value = new std::string;
        std::cin.ignore();
        getline(std::cin, *(static_cast<std::string*>(entry->value)));
        Entry* entryResult = database.create(entry->type, entry->key, entry->value);
        database.addEntry(entryResult);
    }

    else if (inputType == "array") {
        //entry->type = ARRAY;
        addArray(database, entry);
    }
}

void list(Database &database) {
    for (int i = 0; i < database.size; i++) {
        std::cout << database.entries[i]->key << ": ";
        if (database.entries[i]->type == INT) {
            std::cout << *static_cast<int*>(database.entries[i]->value) << std::endl;
        }
        if (database.entries[i]->type == DOUBLE) {
            std::cout << *static_cast<double*>(database.entries[i]->value) << std::endl;
        }
        else if (database.entries[i]->type == STRING) {
            std::cout << ": \"" << *static_cast<std::string*>(database.entries[i]->value) << "\"" <<std::endl;
        }
        else if (database.entries[i]->type == ARRAY) {
            std::cout << "[";
            Array* printArray = static_cast<Array*>(database.entries[i]->value);
            for (int j=0; j < printArray->size; j++) {
                if (j>0) {
                    std::cout << ",";
                }
                printArray(static_cast<Entry*>(printArray->items[j]));
            }
            std::cout << " ]" << std::endl;
        }
    }
}

void remove(Database &database, std::string &key) {
    int index = -1;
    for (int i = 0; i < database.size; i++) {
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
    database.entries[index] = database.entries[database.size - 1];
    database.size--;
}

void destroy(Database &database) {
    for (int i = 0; i< database.size; i++) {
        delete database.entries[i];
    }

    delete[] database.entries;
}

int main() {
    Database *database = new Database;
    init (database);

    while(true) {
        std::cout << "command (list, add, get, del, exit): ";
        std::string command;
        std::cin >> command;
        std::string key;

        if (command == "list") {
            list(database);
        }

        else if (command == "add") {
            Entry* entry = create(INT, "key", nullptr);
            add(database, entry);
        }

        else if (command == "del") {
            std::cout << "key: ";
            std::cin >> key;
            database.remove(database, key);
        }

        else if (command == "get") {
            std::cout << "key: ";
            std::cin >> key;
            Entry* getEntry = get(database, key);

            if (getEntry != nullptr) {
                std::cout << getEntry->key << ": ";

                printGet(getEntry);
            }

            
        }

        else if (command == "exit") {
            destroy(database);
            exit(0);
        }

        else {
            std::cout << "invalid command" << std::endl;
        }

        std::cout << std::endl;
    }
}

void printArray(Entry* getEntry) {
    std::cout << "[";
    if (getEntry->type == INT) {
        std::cout <<  getEntry->value;
    }
    else if (getEntry->type == DOUBLE) {
        std::cout <<  getEntry->value;
    }
    if (getEntry->type == STRING) {
        std::cout << "\"" << getEntry->value << "\"";
    }
    if (getEntry->type == ARRAY) {
        Array* getArray = static_cast<Array*>(getEntry->value);
        for (int i = 0; i < getArray->size; i++) {
            if (i > 0) {
                std::cout << ", ";
            }
            printArray(static_cast<Entry*>(getArray->items[i]));
        }
    }
    std::cout << "]" << std::endl;
}
#endif


// Entry* newEntry = database.create(ARRAY, "array", nullptr);
//         arrayCount++;
//         array->items[i] = addArray(database, newEntry);
//         static_cast<Array*>(array->items[i]) = *static_cast<Array*>(subEntry->value);
//         arrayCount--;