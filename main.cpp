#include <vector>
#include <iostream>
#include <cassert>

#include "BPlusTree.h"

using namespace std;

int main() {
    btree<int> bt;
    auto r1 = new Registro(69, "Benjamin", 1234, "Pisco");
    auto r2 = new Registro(70, "Yanli", 5432, "Ica");
    auto r3 = new Registro(71, "Yeny", 6789, "Arequipa");
    auto r4 = new Registro(72, "Victor", 9876, "Huacho");
    auto r5 = new Registro(73, "Jose Maria", 1111, "Lambayeque");
    auto r6 = new Registro(74, "Benjamin", 1234, "Pisco");
    auto r7 = new Registro(75, "Yanli", 5432, "Ica");
    auto r8 = new Registro(76, "Yeny", 6789, "Arequipa");
    auto r9 = new Registro(77, "Victor", 9876, "Huacho");
    auto r10 = new Registro(78, "Jose Maria", 1111, "Lambayeque");
    auto r11 = new Registro(79, "Benjamin", 1234, "Pisco");
    auto r12 = new Registro(80, "Yanli", 5432, "Ica");
    auto r13 = new Registro(81, "Yeny", 6789, "Arequipa");
    auto r14 = new Registro(82, "Victor", 9876, "Huacho");
    auto r15 = new Registro(83, "Jose Maria", 1111, "Lambayeque");
    auto r16 = new Registro(84, "Jose Maria", 1111, "Lambayeque");
    auto r17 = new Registro(85, "Jose Maria", 1111, "Lambayeque");
    auto r18 = new Registro(86, "Jose Maria", 1111, "Lambayeque");
    auto r19 = new Registro(87, "Jose Maria", 1111, "Lambayeque");
    auto r20 = new Registro(88, "Jose Maria", 1111, "Lambayeque");
    auto r21 = new Registro(89, "Jose Maria", 1111, "Lambayeque");
    auto r22 = new Registro(90, "Jose Maria", 1111, "Lambayeque");
    auto r23 = new Registro(91, "Jose Maria", 1111, "Lambayeque");
    auto r24 = new Registro(92, "Jose Maria", 1111, "Lambayeque");
    auto r25 = new Registro(93, "Jose Maria", 1111, "Lambayeque");
    auto r26 = new Registro(94, "Jose Maria", 1111, "Lambayeque");
    auto r27 = new Registro(95, "Jose Maria", 1111, "Lambayeque");
    auto r28 = new Registro(96, "Jose Maria", 1111, "Lambayeque");
    auto r29 = new Registro(97, "Jose Maria", 1111, "Lambayeque");
    auto r30 = new Registro(98, "Jose Maria", 1111, "Lambayeque");
    auto r31 = new Registro(99, "Jose Maria", 1111, "Lambayeque");

    bt.insert(r1);
    bt.insert(r2);
    bt.insert(r3);
    bt.insert(r4);
    bt.insert(r5);
    bt.insert(r6);
    bt.insert(r7);
    bt.insert(r8);
    bt.insert(r9);
    bt.insert(r10);
    bt.insert(r11);
    bt.insert(r12);
    bt.insert(r13);
    bt.insert(r14);
    bt.insert(r15);
    bt.insert(r16);
    bt.insert(r17);
    bt.insert(r18);
    bt.insert(r19);
    bt.insert(r20);
    bt.insert(r21);
    bt.insert(r22);
    bt.insert(r23);
    bt.insert(r24);
    bt.insert(r25);
    bt.insert(r26);
    bt.insert(r27);
    bt.insert(r28);
    bt.insert(r29);
    bt.insert(r30);
    bt.print();

    typedef btree<int>::iterator btree_iterator;

    btree_iterator beg = bt.find(78);
    // beg.ptr->registros[beg.index]->print();
    (*beg)->print();
    assert (beg.ptr->registros[beg.index]->id == 78);

    btree_iterator result = bt.find(87);
    // result.ptr->registros[result.index]->print();
    (*result)->print();
    assert (result.ptr->registros[0]->id == 87);

    return 0;
}
