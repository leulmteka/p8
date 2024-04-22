#ifndef _EFILE_H_
#define _EFILE_H_


#include "io.h"
#include "file.h"
#include "ext2.h"

class EFile : public File
{
        uint32_t offset; //offset within file
        Node* node;
public:
        EFile(Node* node) : offset(0), node(node) {}
        bool isFile() override { return true; }
        bool isDirectory() override { return false; }
        off_t seek(off_t off) { 
                if(off < offset) {
                        offset = off;
                        return offset;
                }
                offset += off;
                return offset; }
        off_t size() { 
                
                return node->size_in_bytes(); 
                }
        
        ssize_t read(void *buffer, size_t n)
        {
                if((uint32_t)buffer <= 0x80000000 || (uint32_t)buffer > 0xFFFFFFFF) return -1;
                ssize_t read = node->read_all(offset, n, (char*)buffer);
                if(read != -1) offset += read;
                return read;
        }
        ssize_t write(void *buffer, size_t n)
        {
                return - 1;
        }
};

#endif
