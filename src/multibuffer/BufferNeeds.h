//
// Created by G on 11/24/2024.
//

#ifndef CDB_BUFFERNEEDS_H
#define CDB_BUFFERNEEDS_H

class BufferNeeds {
public:
    auto static bestRoot(int avaiable, int size) -> int;
    auto static bestFactor(int available, int size) -> int;
};

#endif  // CDB_BUFFERNEEDS_H
