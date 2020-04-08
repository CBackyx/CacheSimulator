#ifndef MAIN_H
#define MAIN_H

// The Bocksizes collection(B)
enum BlockSizes {
    SMALL = 8,
    MID = 32,
    BIG = 64
};

// Replacement Policy
enum ReplacementPolicy {
    LRU,
    RANDOM,
    BINARY_TREE
};

// Write Policy
enum WritePolicy {
    ALLOC_THROUGH,
    ALLOC_BACK,
    UNALLOC_THROUGH,
    UNALLOC_BACK
};

// Organization
enum Origanization {
    FULL_ASS,
    ONE_WAY = 1,
    FOUR_WAY = 4,
    EIGHT_WAY = 8
};

#endif