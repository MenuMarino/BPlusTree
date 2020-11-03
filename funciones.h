#ifndef PROYECTO_1_FUNCIONES_H
#define PROYECTO_1_FUNCIONES_H

#include <fstream>
using namespace std;

struct Registro;

long long getFileSize(const std::string& fileName) {
    ifstream file(fileName.c_str(), ifstream::in | ifstream::binary);

    if(!file.is_open()) return -1;

    file.seekg(0, ios::end);
    long long fileSize = file.tellg();
    file.close();

    return fileSize;
}

void writeCharArray(fstream& stream, const char* str, int len) {
    stream.write(str, len);
}

char* readCharArray(fstream& stream, int len) {
    char* buffer = new char[len+1];
    stream.read(buffer, len);
    buffer[len] = '\0';
    return buffer;
}

void writeInt(fstream& stream, int ival) {
    stream.write(reinterpret_cast<char*>(&ival), sizeof(int));
}

int readInt(fstream& stream) {
    int result;
    stream.read((char*) &result, sizeof(int));
    return result;
}

void writeLongLong(fstream& stream, long long ival) {
    stream.write(reinterpret_cast<char*>(&ival), sizeof(long long));
}

long long readLongLong(fstream& stream) {
    long long result;
    stream.read((char*) &result, sizeof(long long));
    return result;
}

size_t readUnsignedLong(fstream& stream) {
    size_t result;
    stream.read( (char*) &result, sizeof(size_t) );
    return result;
}

void writeUnsignedLong(fstream& stream, size_t val) {
    stream.write( reinterpret_cast<char*>(&val), sizeof(size_t) );
}

uint8_t readByte(fstream& stream) {
    uint8_t result;
    stream.read( (char*) &result, sizeof(uint8_t) );
    return result;
}

void writeByte(fstream& stream, uint8_t val) {
    stream.write( reinterpret_cast<char*>(&val), sizeof(uint8_t) );
}

void writeShort(fstream& stream, short ival) {
    stream.write(reinterpret_cast<char*>(&ival), sizeof(short));
}

short readShort(fstream& stream) {
    short result;
    stream.read((char*) &result, sizeof(short));
    return result;
}

template <typename T>
void writeTArray(fstream& stream, const T* array, int len) {
    stream.write( (char*) array, len * sizeof(T));
}

template <typename T>
T* readTArray (fstream& stream, int len) {
    T* buffer = new T[len];
    stream.read( (char*) buffer, len * sizeof(T));
    return buffer;
}

Registro** readRegisterArray(fstream& stream, int len) {
    Registro** registers = new Registro* [len];

    stream.read( (char*) registers, sizeof(Registro*) * len );

    return registers;
}

void writeRegisterArray(fstream& stream, Registro** array, int len) {
    stream.write( (char*) array, sizeof(Registro*) * len );
}

void setReadPos(fstream& file, int newPos) {
    file.seekg(newPos, ios::beg);
}

void setWritePos(fstream& file, int newPos) {
    file.seekp(newPos, ios::beg);
}

#endif //PROYECTO_1_FUNCIONES_H
