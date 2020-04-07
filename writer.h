#ifndef WRITER_H
#define WRITER_H

class TraceSimulator;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ulong;

class Writer {
    public:
        TraceSimulator *ts;
        int doWrite(uint index, uint pos) {}
        int doMissingWrite(uint index, ulong curTag) {}

        Writer() {}
        ~Writer() {}
};

class ALLOC_THROUGH_Writer: public Writer {
    public:
        TraceSimulator *ts;
        int doWrite(uint index, uint pos);
        int doMissingWrite(uint index, ulong curTag);

        ALLOC_THROUGH_Writer() {}
        ~ALLOC_THROUGH_Writer() {}
};

class ALLOC_BACK_Writer: public Writer {
    public:
        TraceSimulator *ts;
        int doWrite(uint index, uint pos);
        int doMissingWrite(uint index, ulong curTag);

        ALLOC_BACK_Writer() {}
        ~ALLOC_BACK_Writer() {}
};

class UNALLOC_THROUGH_Writer: public Writer {
    public:
        TraceSimulator *ts;
        int doWrite(uint index, uint pos);
        int doMissingWrite(uint index, ulong curTag);

        UNALLOC_THROUGH_Writer() {}
        ~UNALLOC_THROUGH_Writer() {}
};

class UNALLOC_BACK_Writer: public Writer {
    public:
        TraceSimulator *ts;
        int doWrite(uint index, uint pos);
        int doMissingWrite(uint index, ulong curTag);

        UNALLOC_BACK_Writer() {}
        ~UNALLOC_BACK_Writer() {}
};

#endif