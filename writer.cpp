#include "headers.h"

int ALLOC_THROUGH_Writer::doWrite(uint index, uint pos) {
    this->ts->r->doUpdate(index, pos); // Update the LRU stack
    return 0;
}

int ALLOC_BACK_Writer::doWrite(uint index, uint pos) {
    // Set dirty
    uchar* curc = this->ts->cache[index] + this->ts->entry_size * pos;
    curc[0] |= 0x2;

    this->ts->r->doUpdate(index, pos); // Update the LRU stack
    return 0;
}

int UNALLOC_THROUGH_Writer::doWrite(uint index, uint pos) {
    this->ts->r->doUpdate(index, pos); // Update the LRU stack
    return 0;
}

int UNALLOC_BACK_Writer::doWrite(uint index, uint pos) {
    // Set dirty
    uchar* curc = this->ts->cache[index] + this->ts->entry_size * pos;
    curc[0] |= 0x2;
    
    this->ts->r->doUpdate(index, pos); // Update the LRU stack
    return 0;
}

int ALLOC_THROUGH_Writer::doMissingWrite(uint index, ulong curTag) {
    // printf("h8\n");
    this->ts->r->doUpdate(index, this->ts->r->doReplace(index, curTag)); // Update the LRU stack
    return 0;
}

int ALLOC_BACK_Writer::doMissingWrite(uint index, ulong curTag) {
    this->ts->r->doUpdate(index, this->ts->r->doReplace(index, curTag)); // Update the LRU stack
    return 0;
}

int UNALLOC_THROUGH_Writer::doMissingWrite(uint index, ulong curTag) {
    return 0;
}

int UNALLOC_BACK_Writer::doMissingWrite(uint index, ulong curTag) {
    return 0;
}

