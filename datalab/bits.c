/* 
 * CS:APP Data Lab 
 * 
 * <gaofanfei>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  int m = (~x)&y;
  int n = (~y)&x;
  int p = (~m)&(~n);
  int q = ~p;
  return q;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 *   思路：
 *        x        0111 1111   
 *        x+1      1000 0000
 *        x^(x+1)  1111 1111
 *    
 *        x        1111 1111
 *        x+1      0000 0000
 *        !!(x+1)  0000 0000
 */
int isTmax(int x) {
  int m = !(~((x + 1) ^ x));
  int n = !!(x+1);
  return m & n;
  
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 *   思路：
 *        n         1010 1010
 *        x & n     1010 1010
 *        x&n ^ n   0000 0000
 */
int allOddBits(int x) {
  int n = 0xAA + (0xAA<<8) + (0xAA<<16) + (0xAA<<24);
  return !((x&n) ^ n);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 *   思路：
 *        补码的相反数等于各位取反再加1
 */
int negate(int x) {
  return ~x+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 *   思路：
 *        首先检测x是否是0x3?，检测方法: x >> 4，检查是否为0x3
 *        然后检测个位是否为0~9，检测方法：0xxx，或者1000 1001(这两个单独处理) 
 */
int isAsciiDigit(int x) {
  int m = !((x >> 4) ^ 0x3);
  int t = x & 0xf;
  int n = !(t>>3) | !(t ^ 0x8) | !(t ^ 0x9);
  return m & n;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 *   思路：利用算术右移的特性
 *                  !x     (!x)<<31     ((!x)<<31)>>31
 *        x不为0     0     0x00000000    0x00000000
 *        x为0       1     0x10000000    0xFFFFFFFF
 */
int conditional(int x, int y, int z) {
  int cy = ((~(!x)) << 31) >> 31;
  int cz = ((!x) << 31) >> 31;
  return (cy & y) | (cz & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 *   思路：分类讨论
 *        
 */
int isLessOrEqual(int x, int y) {
  int sx = (x >> 31) & 1;
  int sy = (y >> 31) & 1;

  int m = ((x + ~y + 1) >> 31) & 1;  // x - y符号位
  int c1 = sx & sy & m;   // x > 0, y > 0, x < y
  int c2 = !sx & !sy & m;   // x < 0, y < 0, x < y
  int c3 = sx & !sy;    // x < 0, y > 0
  int equal = !(x ^ y);  // x == y 

  return c1 | c2 | c3 | equal;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 *   思路：
 *        0的相反数还是0，二者符号位都为0，而其余数与其相反数的符号位不同
 */
int logicalNeg(int x) {
  int sx = (x >> 31) & 1;
  int y = ~x + 1;
  int sy = (y >> 31) & 1;

  return (~(sx | sy) + 2);
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 *  思路：二分搜索  
 *        特别地，对于正数，找最高的1所在的位置，加上符号位0即为最少数字
 *        对于负数，按位取反，找最高的1所在的位置，加上一个符号位
 * 
 *        例子：
 *        0000 0100 1000 1010    最高的1所在的位置是第11位，加上一个符号位，得12      
 *        
 *        1111 1110 0101 1010    按位取反
 *        0000 0001 1010 0101    最高的1所在的位置是第9位，加上一个符号位，得10
 * 
 *        寻找最高的1所在的位置使用二分搜索：
 *        1.检查高16位是否有1，如果有，将x右移16位屏蔽低位；如果没有，x保持不变
 *        2.检查x低16位中的高8位中是否有1，如果有，将x右移8位屏蔽低16位中的低8位；如果没有，x保持不变
 */
int howManyBits(int x) {
  int b16,b8,b4,b2,b1,b0;
  int sign = x >> 31;
  x = (sign & ~x) | (~sign & x);

  b16 = !!(x >> 16) << 4;
  x >>= b16;
  b8 = !!(x >> 8) <<3;
  x >>= b8;
  b4 = !!(x >> 4) << 2;
  x >>= b4;
  b2 = !!(x >> 2) << 1;
  x >>= b2;
  b1 = !!(x >> 1);
  x >>= b1;
  b0 = x;

  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 *   思路：
 *        规格化数    exp != 0 && exp != 0xFF     exp++
 *        非规格化数  exp = 0     1.frac = 0, 代表0，return uf      2.frac!=0，代表接近0的数，**frac<<1**   (注意，这里不是exp++)
 *        特殊       exp = 0xFF  1.frac = 0, infinity, return uf   2.frac!=0, 代表NaN, return uf
 */
unsigned floatScale2(unsigned uf) {
  int sign = (uf >> 31) & 1;
  int exp = (uf >> 23) & 0xff;
  int frac = uf & 0x7fffff;

  if (exp == 0xff)  //special
  {
      return uf;
  }
  else if (exp == 0)   //denormalized
  {
    if (frac == 0)  //0
      return uf;
    else  //close to 0
    {
      frac <<= 1;
    }
  }
  else  //normalized
  {
    exp++;
  }
  return sign << 31 | exp << 23 | frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 *   思路:
 *        (-1)^S * M * 2^E
 *        M = 1.frac
 *        E = exp - Bias (当exp = 0时，1 - Bias)
 *        
 *        当E < 0时，代表uf表示的是小数，返回0
 *        当E >= 31时，M中最高位至少移动到第32位，占据了符号位，out of range
 *        当 E >=0 && E < 31时，先把隐含的1在frac第24位表示出来，然后根据E的值对frac进行移位
 *                                      1. xxx xx xx xxxx xxxx xxxx xxxx
 *        1）若E<=23,将小数点向右移E位
 *                                     1  xxx  xx.xx xxxx xxxx xxxx xxxx
 *        为了把小数部分抹掉，直接将frac向右移动23-E位即可
 *        2）若E>=24,小数点向右移到0后面的位，相当于将frac向左移
 *                                    
 *        最后处理符号问题
 */
int floatFloat2Int(unsigned uf) {
  int sign = (uf >> 31) & 1;
  int exp = (uf >> 23) & 0xff;
  int frac = uf & 0x7fffff;

  int E = exp - 127;
  if (E < 0)  //小数
  {
    return 0; 
  }
  else if (E >= 31)  //out of range
  {
    return 0x80000000u;
  }

  frac = frac | (1 << 23);  //隐含的1
  
  if (E <= 23)
  {
    frac = frac >> (23 - E);
  }
  else
  {
    frac = frac << (E - 23);
  }
  
  if (sign) return -frac;
  else
    return frac;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 *   思路:
 *        v = (-1)^S * M * 2 ^ E
 *        2.0 ^ x
 *        normalized   S = 0  M = 1      E = x         exp = E + Bias  minE = 1-Bias = -126       maxE = 127 
 *        denormalized S = 0  M = 0.frac E = 1 - Bias  exp = 0         minE = -126-22 = -148      maxE = -126-1
 * 
 */
unsigned floatPower2(int x) {
    int E = x;
    
    if (E > 127)  //too large
      return (0xFF << 23);
    else if (E < -148)  //too small
      return 0;
    else if (E >= -126)  //normalized
    {
      int exp = E + 127;
      return (exp << 23);
    }
    else   //denormalized
    {
      int t = 148+E;
      return 1 << t;
    }
}
