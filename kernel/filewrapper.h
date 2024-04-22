#ifndef _FILEWRAPPER_H_
#define _FILEWRAPPER_H_

#include "io.h"
#include "file.h"
#include "ext2.h"

class FileWrapper : public File {
    Node* node;
    uint32_t curr = 0;
public:
    FileWrapper(Node* node) : node(node) {}

    bool isFile() override {
        return node->is_file();
    }

    bool isDirectory() override {
        return node->is_dir();
    }

    off_t seek(off_t offset) override {
        if (offset >= 0 && offset <= this->size()) {
            curr = offset;
            return curr;
        }
        return -1;
    }

    off_t size() override {
        return node->size_in_bytes();
    }

    ssize_t read(void* buffer, size_t n) override {
        if (curr >= this->size()) {
            return 0;
        }
        
        size_t max = this->size() - curr;
        size_t readSize;
        if(n > max){
            readSize = max;
        }else{
            readSize = n;
        }

        ssize_t readBytes = node->read_all(curr, readSize, (char*)buffer);
        if (readBytes > 0) {
            curr = curr + readBytes;
        }
        return readBytes;
    }

    ssize_t write(void* buffer, size_t n) override {
        return -1;
    }
};

#endif