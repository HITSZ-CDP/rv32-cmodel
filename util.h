#ifndef __UTIL_H__
#define __UTIL_H__

#define _CONCAT_  x ## y
#define CONCAT(x, y)  _CONCAT_(x, y)
#define CONCAT_3(x, y, z) _CONCAT(_CONCAT_(x,y), z)

// Bitmanip
#define BITMASK(bits) ((1 << (bits)) - 1)
#define BITS(x, hi, lo) (((x) >> (lo)) & BITMASK((hi) - (lo) + 1)) // similar to x[hi:lo] in verilog

#endif
