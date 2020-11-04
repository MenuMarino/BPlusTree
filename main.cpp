#include <vector>
#include <iostream>
#include <cassert>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;
    bt.build("file.db");
    bt.print();

//    typedef btree<int>::iterator btree_iterator;
//    auto beg = bt.find("DvorakLeft");
//    beg.ptr->registros[beg.index]->print();
//    (*beg)->print();

    return 0;
}
