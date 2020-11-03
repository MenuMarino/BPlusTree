#include <vector>
#include <iostream>
#include <cassert>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<char*> bt;

    auto r1 = new Registro("hola", 1, 1);
    bt.insert(r1);
    bt.print();

//    typedef btree<int>::iterator btree_iterator;
//    btree_iterator beg = bt.find("hola");
//    beg.ptr->registros[beg.index]->print();
//    (*beg)->print();
//    assert (beg.ptr->registros[beg.index]->palabra == "hola");
//
//    btree_iterator result = bt.find("adios");
//    result.ptr->registros[result.index]->print();
//    (*result)->print();
//    assert (result.ptr->registros[0]->palabra == "adios");

    return 0;
}
