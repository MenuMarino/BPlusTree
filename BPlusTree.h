#ifndef B_PLUS_TREE
#define B_PLUS_TREE

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include "funciones.h"

int ORDER = 3;

const std::string indexfile = "index.dat";
#define FILESIZE getFileSize(indexfile)

// Retorna true cuando la primera palabra es menor
bool isGreater(const char* palabra1, const char* palabra2) {
    if (palabra1 == nullptr)
        return false;
    if (palabra2 == nullptr)
        return false;
    if (palabra1[0] == '\0')
        return false;
    if (palabra2[0] == '\0')
        return false;
    return strcmp(palabra1, palabra2) < 0;
}

bool isGreaterOrEqual(const char* palabra1, const char* palabra2) {
    if (palabra1 == nullptr)
        return false;
    if (palabra2 == nullptr)
        return false;
    if (palabra1[0] == '\0')
        return false;
    if (palabra2[0] == '\0')
        return false;
    return strcmp(palabra1, palabra2) <= 0;
}

struct Registro {
    char palabra[50]{};
//    vector<pair<unsigned long, unsigned long>> direcciones;

    Registro() = default;

    Registro(const string& palabra, unsigned long direccion, unsigned long offset) {
        strncpy(this->palabra, palabra.c_str(), 50);
//        direcciones.emplace_back(direccion, offset);
    }

    void print() {
        if (!this) return;
        cout << palabra << " ";
//        for (const auto& i : direcciones) {
//            cout << i.first << " ";
//            cout << i.second << ". ";
//        }
        cout << endl;
    }
};

template<typename T>
// ahora es un B+
class btree {
private:

    enum state_t { OVERFLOW, UNDERFLOW, B_OK };

    struct node {
        T* data; // keys
        size_t* children; // Hijos (posicion en el archivo de los hijos)
        Registro** registros; // solo los nodos hoja tienen esto
        size_t count{0}; // numero de keys que el nodo tiene
        int isLeaf{0}; // el nodo es hoja?
        long long next{-1}; // si el nodo es hoja, un puntero (posicion en el archivo de mi hermano derecho)
        long long prev{-1}; // si el nodo es hoja, un puntero (posicion en el archivo de mi hermano izquierdo)
        long long filePosition{-1}; // posicion en el file
        
        node() {
            data = (T*) calloc (ORDER + 1, sizeof(T));
            children = (size_t*) calloc (ORDER + 2, sizeof(size_t));
            registros = new Registro*[ORDER+2](); // el '()' hace lo mismo que calloc
        }

        void print() {
            cout << "Data: ";
            for (int i = 0; i < this->count+1; ++i) {
                cout << this->data[i] << " ";
            }
            cout << "\n";
            cout << "Children: ";
            for (int i = 0; i < this->count+2; ++i) {
                cout << this->children[i] << " ";
            }
            cout << "\n";
            cout << "Registros: \n";
            for (int i = 0; i < this->count+2; ++i) {
                this->registros[i]->print();
            }
            cout << "\n";
            cout << "Count: " << this->count << "\n";
            cout << "IsLeaf: " << this->isLeaf << "\n";
            cout << "Next: " << this->next << "\n";
            cout << "Prev: " << this->prev << "\n";
        }

        void insert_into(size_t index, Registro* registro) {
            if (!this) return;
            size_t j = this->count;
            if (this->registros[index] && registro && strcmp(this->registros[index]->palabra, registro->palabra) == 0) {
//                cout << "Son iguales" << endl;
//                registros[index]->direcciones.push_back(registro->direcciones[0]);
                return;
            }
            while (j > index) {
                registros[j+1] = registros[j];
                children[j+1] = children[j];
                data[j] = data[j-1];
                j--;
            }
            registros[j+1] = registros[j];
            children[j+1] = children[j];
            data[j] = registro->palabra;
            registros[j] = registro;
            this->count++;
        }

        void push_back(const T& value) {
            insert_into(this->count, value);
        }

        state_t insert(Registro* registro) {
            // binary_search
            size_t index = 0;
            auto value = registro->palabra;

            while (this->data[index] && index < this->count && isGreater(this->data[index], value)) {
                index += 1; 
            }

            if (this->children[index] == 0) {
                // this is a leaf node
                this->insert_into(index, registro);
                fstream myFile;
                myFile.open(indexfile, ios::binary | ios::in | ios::out);
                setWritePos(myFile, this->filePosition);
                writeNode(myFile, this);
                myFile.close();
            } else {
                fstream myFile;
                myFile.open(indexfile, ios::binary | ios::in | ios::out);
                setReadPos(myFile, this->children[index]);
                auto child = readNode(myFile);
                myFile.close();

                auto state = child->insert(registro);
                if (state == state_t::OVERFLOW) {
                    // split
                    myFile.open(indexfile, ios::binary | ios::in | ios::out);
                    setWritePos(myFile, child->filePosition);
                    writeNode(myFile, child);
                    myFile.close();
                    this->split(index);
                } else {
                    myFile.open(indexfile, ios::binary | ios::in | ios::out);
                    setWritePos(myFile, this->children[index]);
                    writeNode(myFile, child);
                    myFile.close();
                }

            }
            return this->count > ORDER ? OVERFLOW : B_OK;
        }

        void split(size_t position) {
            // leaf nodes / index nodes
            node* parent = this;
            fstream myFile;
            myFile.open(indexfile, ios::binary | ios::in | ios::out);
            setReadPos(myFile, this->children[position]);
            node* ptr = readNode(myFile);
            node* ptr_next = nullptr;
            node* ptr_prev = nullptr;
            if (ptr->next != -1) {
                setReadPos(myFile, ptr->next);
                ptr_next = readNode(myFile);
            }
            if (ptr->prev != -1) {
                setReadPos(myFile, ptr->prev);
                ptr_prev = readNode(myFile);
            }
            myFile.close();

            // 'ptr' es el nodo que va a ser spliteado
            node* child1 = new node();
            node* child2 = new node();
            child1->filePosition = FILESIZE;
            child2->filePosition = FILESIZE + sizeof(node);

            if (ptr->isLeaf != 0) {
                child1->isLeaf = 1;
                child2->isLeaf = 1;
            }

            size_t i = 0;
            for (; i < ptr->count / 2; i++) { // cambie aca
                child1->children[i] = ptr->children[i];
                child1->registros[i] = ptr->registros[i];
                child1->data[i] = ptr->data[i];
                child1->count++;
            }
            child1->children[i] = ptr->children[i];
            child1->registros[i] = ptr->registros[i];

            size_t mid = i;
            i += 1; 
            size_t j = 0;
            // B+
            if (ptr->isLeaf != 0) {
                child2->data[j] = ptr->data[mid];
                child2->registros[j] = ptr->registros[mid];
                child2->count++;
                ++j;
            }
            // B+
            for (; i < ptr->count; i++) {
                child2->children[j] = ptr->children[i];
                child2->registros[j] = ptr->registros[i];
                child2->data[j] = ptr->data[i];
                child2->count++;
                j++;
            }
            child2->children[j] = ptr->children[i];
            child2->registros[j] = ptr->registros[i];

            // update dll pointers
            child1->next = child2->filePosition;
            child2->prev = child1->filePosition;

            // FIXME: Esto da bug
//            if (ptr_prev) {
//                child1->prev = ptr_prev->filePosition;
//                ptr_prev->next = child1->filePosition;
//                parent->children[position - 1] = ptr_prev->filePosition;
//            }

            myFile.open(indexfile, ios::app | ios::binary | ios::out);
            writeNode(myFile, child1);
            myFile.close();

            myFile.open(indexfile, ios::app | ios::binary | ios::out);
            child2->filePosition = FILESIZE;
            writeNode(myFile, child2);
            myFile.close();
            // FIXME: Esto da bug
//            if (ptr_next) {
//                ptr_next->prev = child2->filePosition;
//                child2->next = ptr_next->filePosition;
//                parent->children[position + 2] = ptr_next->filePosition;
//            }

            parent->insert_into(position, ptr->registros[mid]);
            parent->children[position] = child1->filePosition;
            parent->children[position + 1] = child2->filePosition;

            myFile.open(indexfile, ios::app | ios::binary | ios::out);
            setWritePos(myFile, parent->filePosition);
            writeNode(myFile, parent);
            myFile.close();

            // Pavada
            myFile.open(indexfile, ios::binary | ios::out | ios::in);
            setReadPos(myFile, child1->filePosition);
            auto tmp = readNode(myFile);
            tmp->next = child2->filePosition;
            setWritePos(myFile, child1->filePosition);
            writeNode(myFile, tmp);
            myFile.close();
        }

        void merge(size_t index){
            node *child = this->children[index];
            node *sibling = this->children[index+1];
            if(sibling!= nullptr){
                if(data[index] != sibling->data[0]){
                    child->data[child->count-1] = data[index];
                    for (int i=0; i<sibling->count; ++i)
                        child->data[i+child->count] = sibling->data[i];

                    if (!child->isLeaf) {
                        for(int i=0; i<sibling->count+1; ++i)
                            child->children[i+child->count] = sibling->children[i];
                    }

                    for (int i=index+1; i<count; ++i)
                        data[i-1] = data[i];


                    for (int i=index+2; i<=count; ++i)
                        children[i-1] = children[i];

                    child->count += sibling->count;
                    count--;
                }else{

                    for (int i=0; i<sibling->count; ++i)
                        child->data[i+child->count-1] = sibling->data[i];

                    if (!child->isLeaf) {
                        for(int i=0; i<sibling->count+1; ++i)
                            child->children[i+child->count-1] = sibling->children[i];
                    }

                    for (int i=index+1; i<count; ++i)
                        data[i-1] = data[i];


                    for (int i=index+2; i<=count; ++i)
                        children[i-1] = children[i];

                    child->count += sibling->count-1;
                    count--;
                }
            } else {
                std::cout << "derecho"; //TODO: Que falta?
            }

            delete(sibling);
        } 
    };

    static void writeNode(fstream& stream, node* nodo) {
        writeTArray<T>(stream, nodo->data, ORDER+1);
        writeTArray<size_t>(stream, nodo->children, ORDER+2);
        writeRegisterArray(stream, nodo->registros, ORDER+2);
        writeUnsignedLong(stream, nodo->count);
        writeInt(stream, nodo->isLeaf);
        writeLongLong(stream, nodo->next);
        writeLongLong(stream, nodo->prev);
        writeLongLong(stream, nodo->filePosition);
    }

    static node* readNode(fstream& stream) {
        node* nodo = new node();
        nodo->data = readTArray<T>(stream, ORDER+1);
        nodo->children = readTArray<size_t>(stream, ORDER+2);
        nodo->registros = readRegisterArray(stream, ORDER+2);
        nodo->count = readUnsignedLong(stream);
        nodo->isLeaf = readInt(stream);
        nodo->next = readLongLong(stream);
        nodo->prev = readLongLong(stream);
        nodo->filePosition = readLongLong(stream);
        return nodo;
    }

public:

    btree() {
        root.isLeaf = 1;
    }

    void insert(const string& name, unsigned long dir, unsigned long off) {
        auto registro = new Registro(name, dir, off);
        auto state = root.insert(registro);
        if (state == state_t::OVERFLOW) {
            // split root node
            split_root();
        }
    }

    void insert(Registro* registro) {
        auto state = root.insert(registro);
        if (state == state_t::OVERFLOW) {
            // split root node
            split_root();
        }
    }

    void remove(const T& value) {
        iterator target_node = find(value);
        if(target_node.ptr == nullptr) {
            return; 
        }

        size_t keys_count = target_node.ptr->count;
        int index = target_node.index;
        auto parent = target_node.parent;

        if (keys_count > (size_t)ORDER/2) {
            if (value == target_node.ptr->data[0]) {
                int parent_key_count = target_node.parent->count;
                for (int i = 0; i < parent_key_count; ++i) {
                    if (target_node.parent->data[i] == value) {
                        target_node.parent->data[i] = target_node.ptr->data[1];
                        for (int j = 0; j < target_node.ptr->count-1; ++j) {
                            target_node.ptr->data[j] = target_node.ptr->data[j+1];
                        }
                        target_node.ptr->count -= 1;
                        return;
                    }
                }
                for (int j = 0; j < target_node.ptr->count-1; ++j) {
                    target_node.ptr->data[j] = target_node.ptr->data[j+1];
                }
                target_node.ptr->count -= 1;
                return;
            } else { // caso 1b
                for (int i = index; i < keys_count-1; ++i) {
                    target_node.ptr->data[i] = target_node.ptr->data[i+1];
                }
                target_node.ptr->count -= 1;
                return;
            }
        } else {
            // Vecino izquierdo caso 2a
            if (index != 0 && parent->children[index - 1]->count > (size_t) ORDER/2) { // En caso tenga un hijo izquierdo
                auto left = parent->children[index - 1];
                std::cout << "Izq" << std::endl;
                T borrowed_value = left->data[left->count];
                left->data[left->count] = 0; // valor nulo?
                --left->count;

                // Ponerlo en el padre
                parent->data[index] = borrowed_value;

                // Copiar los valores a un temp para reescribirlos
                std::vector<T> tmp;
                for (size_t i = 0; i < keys_count; ++i) {
                    if (target_node.ptr->data[i] != value)
                        tmp.push_back(target_node.ptr->data[i]);
                }

                // Reescribir los valores
                target_node.ptr->data[0] = borrowed_value;
                for (size_t i = 0; i < keys_count - 1; ++i) {
                    target_node.ptr->data[i + 1] = tmp[i];
                }
                return;
            }

            // Vecino derecho caso 2b
            if (index != keys_count && parent->children[index + 1]->count > (size_t) ORDER/2) { 
                auto right = parent->children[index + 1];
                std::cout << "Der" << ORDER/2 << std::endl;
                T borrowed_value = right->data[0];
                --right->count;
                // Mover todos los valores del nodo de la derecha 1 posicion a la izquierda
                for (size_t i = 0; i < right->count; ++i) {
                    right->data[i] = right->data[i + 1];
                }
                right->data[right->count] = 0; // valor nulo?

                // Ponerlo en el padre
                parent->data[index] = borrowed_value;

                // Copiar los valores a un temp para reescribirlos
                std::vector<T> tmp;
                for (size_t i = 0; i < keys_count; ++i) {
                    if (target_node.ptr->data[i] != value)
                        tmp.push_back(target_node.ptr->data[i]);
                }

                // Reescribir los valores
                target_node.ptr->data[keys_count - 1] = borrowed_value;
                for (size_t i = 0; i < keys_count - 1; ++i) {
                    target_node.ptr->data[i] = tmp[i];
                }
                return;
            }

            // TODO: Merge caso 1c hacer merge con los vecinos
            std::cout << "Merge" << std::endl;
            std::cout << parent->data[0]<<std::endl;
            parent->merge(index);

        }

    }

    void print_leaves() {
        node* aux = &root;
        fstream myFile(indexfile, ios::binary | ios::in);
        while (aux->children[0] != 0) {
            setReadPos(myFile, aux->children[0]);
            aux = readNode(myFile);
        }
        head = aux->filePosition;
        setReadPos(myFile, head);
        node* leave = readNode(myFile);

        while (leave) {
            for (size_t i = 0; i < leave->count; ++i) {
                std::cout << leave->data[i] << " ";
            }
            if (leave->next != -1) {
                std::cout << " <-> ";
            }
            if (leave->next != -1) {
                setReadPos(myFile, leave->next);
                leave = readNode(myFile);
            } else {
                break;
            }
        }
        std::cout << "\n";
        myFile.close();
    }

    void print() {
        print(&root, 0);
        // std::cout << "________________________\n\n";
    }

    void in_order_print() {
        in_order_helper(&root);
    }

private:
    void print(node *ptr, int level) {
        if (ptr) {
            node* child = nullptr;
            fstream myFile;
            int i;
            for (i = ptr->count - 1; i >= 0; i--) {
                myFile.open(indexfile, ios::binary | ios::in | ios::out);
                setReadPos(myFile, ptr->children[i + 1]);
                child = readNode(myFile);
                myFile.close();

                if (!ptr->isLeaf)
                    print(child, level + 1);

                for (int k = 0; k < level; k++) {
                    std::cout << "\t\t";
                }
                if (ptr->isLeaf) {
                    std::cout << ptr->data[i] << "*" << "\n";
                } else {
                    std::cout << ptr->data[i] << "\n";
                }
            }
            myFile.open(indexfile, ios::binary | ios::in | ios::out);
            setReadPos(myFile, ptr->children[i + 1]);
            child = readNode(myFile);
            myFile.close();
            if (!ptr->isLeaf)
                print(child, level + 1);
        }
    }

    void in_order_helper(node* ptr) {
        if (!ptr)
            return;

        for (size_t i = 0; i < ptr->count; ++i) {
            in_order_helper(ptr->children[i]);
            std::cout << ptr->data[i] << ", ";
        }
        in_order_helper(ptr->children[ptr->count]);
    }
 
    void split_root() {
        node* _root = &root;
        node* child1 = new node();
        child1->filePosition = FILESIZE;
        node* child2 = new node();
        child2->filePosition = FILESIZE + sizeof(node);

        if (_root->isLeaf != 0) {
            child1->isLeaf = 1;
            child2->isLeaf = 1;
        }
        size_t i = 0;
        for (; i < _root->count / 2; i++) { // cambie aca
            child1->children[i] = _root->children[i];
            child1->registros[i] = _root->registros[i];
            child1->data[i] = _root->data[i];
            child1->count++;
        }

        child1->children[i] = _root->children[i];
        child1->registros[i] = _root->registros[i];

        size_t mid = i;
        i += 1;
        size_t j = 0;

        // B+
        if (_root->isLeaf != 0) {
            child2->data[j] = _root->data[mid];
            child2->registros[j] = _root->registros[mid];
            child2->count++;
            ++j;
        }
        // B+

        for (; i < _root->count; i++) {
            child2->children[j] = _root->children[i];
            child2->registros[j] = _root->registros[i];
            child2->data[j] = _root->data[i];
            child2->count++;
            j++;
        }
        child2->children[j] = _root->children[i];
        child2->registros[j] = _root->registros[i];

        // set dll pointers

        child1->next = child2->filePosition;
        child2->prev = child1->filePosition;

        if (_root->isLeaf != 0) {
            _root->isLeaf = false;
            head = child1->filePosition;
        }

        fstream myFile;
        myFile.open(indexfile, ios::app | ios::binary | ios::out);
        writeNode(myFile, child1);
        myFile.close();

        myFile.open(indexfile, ios::app | ios::binary | ios::out);
        child2->filePosition = FILESIZE;
        writeNode(myFile, child2);
        myFile.close();

        _root->data[0] = _root->data[mid];
        _root->children[0] = child1->filePosition;
        _root->children[1] = child2->filePosition;
        _root->count = 1;
        // Pavada
        myFile.open(indexfile, ios::binary | ios::out | ios::in);
        setReadPos(myFile, child1->filePosition);
        auto tmp = readNode(myFile);
        tmp->next = child2->filePosition;
        setWritePos(myFile, child1->filePosition);
        writeNode(myFile, tmp);
        myFile.close();
    }

public:

    struct iterator {
        node* ptr;
        node* parent;
        size_t index;

        iterator(node* ptr, size_t index, node* parent = nullptr): ptr{ptr}, index{index}, parent{parent} {}

        iterator& operator++(int) {
            if (index < ptr->count) {
                ++index;
            } else {
                fstream file(indexfile, fstream::binary | fstream::in);
                setReadPos(file, ptr->next);
                node* _next = readNode(file);
                file.close();
                ptr = _next;
                index = 0;
            }
            return *this;
        }

        Registro* operator*() {
            return ptr->registros[index];
        } 

        bool operator==(const iterator& i) {
            return this->ptr == i.ptr && this->index == i.index;
        }

        bool operator!=(const iterator& i) {
            return this->ptr != i.ptr || this->index != i.index;
        }

    };

    iterator find(const string& value) {
        return find_helper(&root, value.c_str(), &root);
    }

    iterator find_helper(node* ptr, const char* value, node* ptr_parent) {
        size_t index = 0;
        if (ptr->isLeaf) {
            // < porque si es igual se debe detener
            while (ptr->data[index] && index < ptr->count && isGreater(ptr->data[index], value) && index < ptr->count) {
                ++index;
            }
        } else {
            // <= porque si es igual ESE valor esta en el siguiente indice
            while (ptr->data[index] && index < ptr->count && isGreaterOrEqual(ptr->data[index], value) && index < ptr->count) {
                ++index;
            }
        }

        if (ptr->isLeaf) {
            if (index < ptr->count && strcmp(ptr->data[index], value) == 0) {
                return iterator(ptr, index, ptr_parent);
            }
            return iterator(nullptr, 0);
        }
        fstream file(indexfile, fstream::binary | fstream::in);
        setReadPos(file, ptr->children[index]);
        node* _nodo = readNode(file);
        file.close();
        return find_helper(_nodo, value, ptr);
    }

    vector<string> build(const string& filename){
        ifstream file(filename);
        vector<string> keys;
        // ofstream keys("keys.db");
        string line, key;
        unsigned int pgdir = 0, offset;
        while(getline(file,line)) {
            offset = (unsigned int) file.tellg() - pgdir;
            key = getFileNameFromRoute(line);
            keys.push_back(key);

            insert(key, pgdir, offset);
            pgdir = file.tellg();
            cout << "================" << endl;
            cout << "key: " << key << endl;
            print();
        }
        file.close();
        return keys;
    }

private:
    string getFileNameFromRoute(string s) {
        string delimiter = "/";

        size_t pos = 0;
        string token;
        while ((pos = s.find(delimiter)) != string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + delimiter.length());
        }

        if ((pos = s.find('.')) != string::npos) {
            token = s.substr(0, pos);
            return token;
        }
        return "error";
    }

    node root;
    size_t head = -1; // head of the dll
};

#endif