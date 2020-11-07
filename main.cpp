#include <iostream>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;
    ifstream file;
    vector<string> files = {"French.txt", "German.txt", "Italian.txt", "Spanish.txt", "Portuguese.txt"};

    auto keys = bt.build(files);
    /// Print del arbol
//    cout << "================" << "\n";
//    bt.print();
//    cout << "================" << "\n";

    for (const auto& key : keys) {
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

    file.close();
    return 0;
}
