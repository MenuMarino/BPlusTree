#include <iostream>
#include <chrono>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;
    ifstream file;
    vector<string> files = {"French.txt", "German.txt", "Italian.txt", "Spanish.txt", "Portuguese.txt"};

    auto start = chrono::high_resolution_clock::now();
    auto keys = bt.build(files);
    auto end = chrono::high_resolution_clock::now();
    auto executionTime = chrono::duration_cast<chrono::milliseconds>(end - start);
    auto buildTime = executionTime.count();

    vector<string> rKeys(keys.begin(), keys.begin() + 1000);

    /// Print del arbol
//    cout << "================" << "\n";
//    bt.print();
//    cout << "================" << "\n";

    start = chrono::high_resolution_clock::now();
    for (const auto& key : rKeys) {
        auto beg = bt.find(key);
        if (beg.ptr) {
            cout << "\n==========================" << '\n';
            cout << key << '\n';

            if (!beg.ptr->registros[beg.index]->direccionesF.empty()) {
                file.open("French.txt", ios::binary);
                cout << "Traduccion(es) en Frances: " << '\n';
                for (auto direccion_offset : beg.ptr->registros[beg.index]->direccionesF) {
                    file.seekg(direccion_offset.first, ios::beg);
                    string buffer;
                    getline(file, buffer);
                    cout << buffer << '\n';
                }
                file.close();
            }

            if (!beg.ptr->registros[beg.index]->direccionesG.empty()) {
                file.open("German.txt", ios::binary);
                cout << "Traduccion(es) en Aleman: " << '\n';
                for (auto direccion_offset : beg.ptr->registros[beg.index]->direccionesG) {
                    file.seekg(direccion_offset.first, ios::beg);
                    string buffer;
                    getline(file, buffer);
                    cout << buffer << '\n';
                }
                file.close();
            }

            if (!beg.ptr->registros[beg.index]->direccionesS.empty()) {
                file.open("Spanish.txt", ios::binary);
                cout << "Traduccion(es) en Español: " << '\n';
                for (auto direccion_offset : beg.ptr->registros[beg.index]->direccionesS) {
                    file.seekg(direccion_offset.first, ios::beg);
                    string buffer;
                    getline(file, buffer);
                    cout << buffer << '\n';
                }
                file.close();
            }

            if (!beg.ptr->registros[beg.index]->direccionesI.empty()) {
                file.open("Italian.txt", ios::binary);
                cout << "Traduccion(es) en Italiano: " << '\n';
                for (auto direccion_offset : beg.ptr->registros[beg.index]->direccionesI) {
                    file.seekg(direccion_offset.first, ios::beg);
                    string buffer;
                    getline(file, buffer);
                    cout << buffer << '\n';
                }
                file.close();
            }

            if (!beg.ptr->registros[beg.index]->direccionesP.empty()) {
                file.open("Portuguese.txt", ios::binary);
                cout << "Traduccion(es) en Portugues: " << '\n';
                for (auto direccion_offset : beg.ptr->registros[beg.index]->direccionesP) {
                    file.seekg(direccion_offset.first, ios::beg);
                    string buffer;
                    getline(file, buffer);
                    cout << buffer << '\n';
                }
                file.close();
            }
            cout << "==========================" << '\n';
        }
    }
    end = chrono::high_resolution_clock::now();
    executionTime = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Tamaño " << ORDER << '\n';
    cout << "Tiempo del build: " << buildTime << "ms.\n";
    cout << "Tiempo del find: " << executionTime.count() << " ms. Numero de palabras a buscar: " << rKeys.size() << '\n';
    cout << "Reads: " << reads << ". Writes: " << writes << '\n';

    file.close();
    return 0;
}
