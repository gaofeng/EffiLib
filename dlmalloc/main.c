#include <stdio.h>

/*
由于这里认为x为无符号数，故使用取负运算符会导致产生警告，故改为效果等价的取补码方式。
*/

/* isolate the least set bit of a bitmap */
//#define least_bit(x)         ((x) & -(x))
#define least_bit(x)         ((x) & (~x + 1))

/* mask with all bits to left of least bit of x on */
//#define left_bits(x)         ((x<<1) | -(x<<1))
#define left_bits(x)         ((x<<1) | (~(x<<1) + 1))

/* mask with all bits to left of or equal to least bit of x on */
//#define same_or_left_bits(x) ((x) | -(x))
#define same_or_left_bits(x) ((x) | (~x + 1))

typedef unsigned int u32;
typedef unsigned long long u64;


//http://chessprogramming.wikispaces.com/BitScan

/**
 * bitScanForward
 * @author Gerd Isenberg
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int bitScanForward(u64 bb) {
   u32 lsb;
   if (!bb)
   {
       return 64;
   }
   bb &= (~bb + 1); // LS1B-Isolation
   lsb = bb | (bb>>32);
   return (((((((((((bb>>32) !=0)  * 2)
                 + ((lsb & 0xffff0000) !=0)) * 2)
                 + ((lsb & 0xff00ff00) !=0)) * 2)
                 + ((lsb & 0xf0f0f0f0) !=0)) * 2)
                 + ((lsb & 0xcccccccc) !=0)) * 2)
                 + ((lsb & 0xaaaaaaaa) !=0);
}

#define compute_bit2idx(X, I)\
{\
    unsigned int Y = X - 1;\
    unsigned int K = Y >> (16-4) & 16;\
    unsigned int N = K;        Y >>= K;\
    N += K = Y >> (8-3) &  8;  Y >>= K;\
    N += K = Y >> (4-2) &  4;  Y >>= K;\
    N += K = Y >> (2-1) &  2;  Y >>= K;\
    N += K = Y >> (1-0) &  1;  Y >>= K;\
    I = (u32)(N + Y);\
}

int bitScanReverse(u64 bb)
{
    union {
        double d;
        struct {
            unsigned int mantissal : 32;
            unsigned int mantissah : 20;
            unsigned int exponent : 11;
            unsigned int sign : 1;
        };
    } ud;
    ud.d = (double)(bb & ~(bb >> 32));  // avoid rounding error
    return ud.exponent - 1023;
}

void main(void)
{
    u32 a;
    u32 b;
    u32 leftbits;
    u32 leastbit;
    u32 sameorleftbits;

    a = 144;
    b = bitScanForward(a);
    b = bitScanReverse(a);

    leftbits = left_bits(a);
    leastbit = least_bit(a);
    sameorleftbits = same_or_left_bits(a);
    compute_bit2idx(a, b);
}