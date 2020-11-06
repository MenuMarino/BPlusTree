#include <iostream>
#include <unordered_map>

#include "BPlusTree.h"

using namespace std;

vector<string> notFound;

int main() {
    btree<char*> bt;
    unordered_map<string, bool> hash;
    int cont = 0;
    ifstream file ("file.db", ios::binary);

    auto keys = bt.build("file.db");
    /// Print del arbol
//    cout << "================" << "\n";
//    bt.print();
//    cout << "================" << "\n";

    for (const auto& key : keys) {
        if (hash[key]) { continue; } // Ya busco a este key
        hash[key] = true; // Lo encuentre o no lo va a marcar.

        auto beg = bt.find(key);
        if (beg.ptr) {
            cout << '\n';
            beg.ptr->registros[beg.index]->print();
            for (auto direccion_offset : beg.ptr->registros[beg.index]->direcciones) {
                file.seekg(direccion_offset.first, ios::beg);
                string buffer;
                getline(file, buffer);
                cout << "Ruta: " << buffer << '\n';
            }
        }
        else{
            cont++;
            notFound.push_back(key);
        }
    }
    //FIXME
    cout << "El find no encontró " << cont << " archivos." << endl;
    for (const auto& i : notFound) {
        cout << i << endl;
    }

    file.close();
    return 0;
}
