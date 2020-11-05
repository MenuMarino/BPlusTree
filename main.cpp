#include <vector>
#include <iostream>
#include <cassert>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;
    auto keys = bt.build("file.db");
//    cout << "================" << endl;
//    bt.print();

    for (const auto& key : keys) {
        auto beg = bt.find(key);
        beg.ptr->registros[beg.index]->print();
    }

    return 0;
}
