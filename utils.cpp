#include "headers.h"

void setBits(uchar *target, uchar *source, uint t_begin, uint s_begin, uint num) {
    BitPos tp;
    BitPos sp;

    while (num > 0) {
        tp.first = (t_begin >> 3);
        tp.second = (t_begin & 7);
        sp.first = (s_begin >> 3);
        sp.second = (s_begin & 7);
        if ((source[sp.first] >> sp.second) & 1) {
            target[tp.first] |= ((uchar)1 << tp.second);
        } else {
            target[tp.first] &= (~((uchar)1 << tp.second));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(uchar *target, ulong source, uint t_begin, uint s_begin, uint num) {
    BitPos tp;

    while (num > 0) {
        tp.first = (t_begin >> 3);
        tp.second = (t_begin & 7);
        if ((source >> s_begin) & 1) {
            target[tp.first] |= ((uchar)1 << tp.second);
        } else {
            target[tp.first] &= (~((uchar)1 << tp.second));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(ulong &target, uchar *source, uint t_begin, uint s_begin, uint num) {
    BitPos sp;

    while (num > 0) {
        sp.first = (s_begin >> 3);
        sp.second = (s_begin & 7);
        if ((source[sp.first] >> sp.second) & 1) {
            target |= ((ulong)1 << t_begin);
        } else {
            target &= (~((ulong)1 << t_begin));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(uchar *target, uint source, uint t_begin, uint s_begin, uint num) {
    BitPos tp;

    while (num > 0) {
        tp.first = (t_begin >> 3);
        tp.second = (t_begin & 7);
        if ((source >> s_begin) & 1) {
            target[tp.first] |= ((uchar)1 << tp.second);
        } else {
            target[tp.first] &= (~((uchar)1 << tp.second));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(uint &target, uchar *source, uint t_begin, uint s_begin, uint num) {
    BitPos sp;

    while (num > 0) {
        sp.first = (s_begin >> 3);
        sp.second = (s_begin & 7);
        if ((source[sp.first] >> sp.second) & 1) {
            target |= ((uint)1 << t_begin);
        } else {
            target &= (~((uint)1 << t_begin));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(ulong &target, ulong source, uint t_begin, uint s_begin, uint num) {
    while (num > 0) {
        if ((source >> s_begin) & 1) {
            target |= ((ulong)1 << t_begin);
        } else {
            target &= (~((ulong)1 << t_begin));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}

void setBits(uint &target, ulong source, uint t_begin, uint s_begin, uint num) {
    while (num > 0) {
        if ((source >> s_begin) & 1) {
            target |= ((uint)1 << t_begin);
        } else {
            target &= (~((uint)1 << t_begin));
        }
        --num;
        ++t_begin;
        ++s_begin;
    }

    return;
}