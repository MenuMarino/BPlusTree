#include <vector>
#include <iostream>
#include <cassert>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;
    auto keys = bt.build("file.db");
    cout << "================" << endl;
    bt.print();
    int cont = 0;
    for (const auto& key : keys) {
        auto beg = bt.find(key);
        if (beg.ptr)
            beg.ptr->registros[beg.index]->print();
        else
            cont++;
    }
    //FIXME
    cout << "El find no encontró " << cont << " archivos." << endl;
    return 0;
}
