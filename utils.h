#ifndef UTILS_H
#define UTILS_h

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ulong;

struct BitPos {
    uint first;
    uint second;
};

void setBits(uchar *target, uchar *source, uint t_begin, uint s_begin, uint num);
void setBits(uchar *target, ulong &source, uint t_begin, uint s_begin, uint num);
void setBits(ulong &target, uchar *source, uint t_begin, uint s_begin, uint num);
void setBits(ulong &target, ulong &source, uint t_begin, uint s_begin, uint num);
void setBits(uint &target, ulong &source, uint t_begin, uint s_begin, uint num);
void setBits(uint &target, uchar *source, uint t_begin, uint s_begin, uint num);
void setBits(uchar *target, uint &source, uint t_begin, uint s_begin, uint num);


#endif