# Data Lab: Manipulating Bits

## 1. Overview

本实验的目的是考察位操作符的使用以及整数和单精度浮点数的位级表示。本实验包括一系列编程题目，通过完成这些题目可以加深大家对计算机中数据的理解。

## 2. Programming Task

我们的编程任务是修改bits.c，根据要求完成其中的13个函数。注意：每个函数的注释给出了完成该函数所允许使用的操作符类型和操作符最大数目。**禁止使用超过8bits长度的常数**。

实验提供了自动评分程序——btest和dlc。btest可以检查bits.c中函数的正确性，可以通过以下两条命令来编译和使用它。

```shell
$ make
$ ./btest
```

注意：每次更改bits.c文件后都要重新编译btest。如果需要检查单个函数的正确性，可以使用-f标志：

```shell
$ ./btest -f bitXor
```

dlc程序可以检查是否大家遵循了每个函数的要求(操作符类型、数量等)，可以通过以下命令来使用它。如果程序没有输出，说明我们的程序没有问题。

```shell
$ ./dlc bits.c
```

## 3. The Puzzles

### 3.1. bitXor

题目描述：

> bitXor - x^y using only ~ and &
>
>  Example: bitXor(4, 5) = 1
>
>  Legal ops: ~ &
>
>  Max ops: 14
>
>  Rating: 1

解题思路：

数字电路中常见的题目，可以**列出真值表**，观察求解。

|  x   |  y   | x ^ y | x & y | ~x & ~y | ~(x & y) & ~(~x & ~y) |
| :--: | :--: | :---: | :---: | :-----: | :-------------------: |
|  0   |  0   |   0   |   0   |    1    |           0           |
|  0   |  1   |   1   |   0   |    0    |           1           |
|  1   |  0   |   1   |   0   |    0    |           1           |
|  1   |  1   |   0   |   1   |    0    |           0           |

代码：

```c
int bitXor(int x, int y) {
  return ~(x & y) & ~(~x & ~y);
}
```

### 3.2. tmin

题目描述：

 >tmin - return minimum two's complement integer 
 >
 >Legal ops: ! ~ & ^ | + << >>
 >
 >Max ops: 4
 >
 >Rating: 1

解题思路：

tmin : 0x80000000

代码：

```c
int tmin(void) {
  return 1 << 31;
}
```

### 3.3. isTmax

题目描述：

>isTmax - returns 1 if x is the maximum, two's complement number, and 0 otherwise 
>
>Legal ops: ! ~ & ^ | +
>
>Max ops: 10
>
>Rating: 1

解题思路：

观察真值表(以4bits为例)

| x = Tmax | x + 1 | x ^ (x + 1) | ~(x ^ (x + 1)) | !(~(x ^ (x + 1))) |
| :------: | :---: | :---------: | :------------: | :---------------: |
|   0111   | 1000  |    1111     |      0000      |       0001        |

|  x   | x + 1 | x ^ (x + 1) |
| :--: | :---: | :---------: |
| 1111 | 0000  |    1111     |

需要区分x = Tmax和x = -1(全1)的情况。区分方法：当x + 1 = 1000时，!!(x+1) = 1，而当x + 1 = 0000时，!!(x + 1) = 0。

代码：

```c
int isTmax(int x) {
  int m = !(~((x + 1) ^ x));
  int n = !!(x+1);
  return m & n; 
}
```

### 3.4. allOddBits

题目描述：

>allOddBits - return 1 if all odd-numbered bits in word set to 1 where bits are numbered from 0 (least significant) to 31 (most significant)
>
>Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 12
>
>Rating: 2

解题思路：

检查某个位是否为1，可以用异或(^)。需要构造出0xAAAAAAAA。

|  x   | x & 0101 | (x & 0101) ^ (0101) |
| :--: | :------: | :-----------------: |
| x1x1 |   0101   |        0000         |
| x1x0 |   0100   |        0001         |

代码：

```c
int allOddBits(int x) {
  int a = 0xAA << 8;         //0xAA00
  int b = a | 0xAA;          //0xAAAA 
  int c = b << 16 | b;       //0xAAAA AAAA
  return ! (c ^ (x & c));
}
```

### 3.5. negate

题目描述：

>negate - return -x 
>
>Example: negate(1) = -1.
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 5
>
>Rating: 2

解题思路：

-x = ~x + 1

代码：

```c
int negate(int x) {
  return ~x+1;
}
```

### 3.6. isAsciiDigit

题目描述：

>isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
>
>Example: 
>
>​    isAsciiDigit(0x35) = 1.
>
>​    isAsciiDigit(0x3a) = 0.
>
>​    isAsciiDigit(0x05) = 0.
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 15
>
>Rating: 3

解题思路：

考虑0x30 ~ 0x39的位级表示，0x30 ~ 0x37 的位级表示为 0011 0xxx，舍弃低3位，再判断低3位是否为110即可，0x38和0x39分别处理。

代码

```c
int isAsciiDigit(int x) {
  int a = !((x >> 3) ^ 0x06); //0x30 <= x <= 0x37
  int b = !(x ^ 0x38); //x == 0x38
  int c = !(x ^ 0x39); //x == 0x39

  return a | b | c;
}
```

### 3.7. conditional

题目表述：

>conditional - same as x ? y : z 
>
>Example: conditional(2,4,5) = 4
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 16
>
>Rating: 3

解题思路：

寻找一种转换，使得当x非0时转变为0xFFFFFFFF，当x为0时不变，然后返回 (x & y) | (~x & z)。

|                   |   x != 0   | x == 0 |
| :---------------: | :--------: | :----: |
|        !x         |     0      |   1    |
|        !!x        |     1      |   0    |
| (!!x << 31) >> 31 | 0xFFFFFFFF |   0    |

代码：

```c
int conditional(int x, int y, int z) {
  int mask = ((!!x) << 31) >> 31; 
  return ((mask & y) | (~mask & z)) ;
}
```

### 3.8. isLessOrEqual

题目描述：

>isLessOrEqual - if x <= y then return 1, else return 0 
>
>Example: isLessOrEqual(4,5) = 1.
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 24
>
>Rating: 3

解题思路：

x <= y 时 y-x >=0，y-x的符号位为0。但是需要注意，**y-x可能会发生溢出**。但是当x > 0, y < 0时可直接判定x > y，同样当x < 0，y > 0时可直接判定x < y。

|   x   |   y   |    x - y     |
| :---: | :---: | :----------: |
| x > 0 | y > 0 |     正确     |
| x > 0 | y < 0 | 可能发生上溢 |
| x < 0 | y > 0 | 可能发生下溢 |
| x < 0 | y < 0 |     正确     |

[![JTRPpQ.md.png](https://s1.ax1x.com/2020/04/29/JTRPpQ.md.png)](https://imgchr.com/i/JTRPpQ)

代码：

```c
int isLessOrEqual(int x, int y) {
  int sx = (x >> 31) & 0x1;
  int sy = (y >> 31) & 0x1;

  int c1 = (~sy & sx);  //y正x负，直接可以判定y>x
  int c2 = (~sx & sy); //y负x正，直接可以判定y<x
  
  int y_x = y + (~x + 1);
  int sign = y_x >> 31;

  return c1 | (!c2 & !sign);
}
```

### 3.9. LogicalNeg

题目描述：

>logicalNeg - implement the ! operator, using all of the legal operators except !
>
>Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
>
>Legal ops: ~ & ^ | + << >>
>
>Max ops: 12
>
>Rating: 4 

解题思路：

0的相反数还是0，二者符号位都为0，而其余数与其相反数的符号位不同。

代码：

```c
int logicalNeg(int x) {
  int sx = (x >> 31) & 1;
  int y = ~x + 1;
  int sy = (y >> 31) & 1;

  return (~(sx | sy) + 2);
}
```

### 3.10. howMangBits

题目描述：

>howManyBits - return the minimum number of bits required to represent x in two's complement
>
>Examples: 
>
>​		howManyBits(12) = 5
>
>​		howManyBits(298) = 10
>
>​		howManyBits(-5) = 4
>
>​		howManyBits(0) = 1
>
>​		howManyBits(-1) = 1
>
>​		howManyBits(0x80000000) = 32
>
>Legal ops: ! ~ & ^ | + << >>
>
>Max ops: 90
>
>Rating: 4

解题思路：

对于正数而言，比如000000110101001，前面有很多0，我们**需要找到最左边的1**所在的位，然后再加一个符号位0就是表示该正数的最少位数。对于负数，将其按位取反后按照正数处理，结果不变。

找最左边1所用的方法是二分查找

```c
int binSearch(int left, int right)
{
    if (left == right)
        return left;
        
    int mid = (left + right) >> 1;
    if ([left, mid] 包含1) //最左边1在左半侧
        return binSearch(left, mid);
    else
        return binSearch(mid+1, right);
}
```

但是本实验只能采用顺序编程，所以我们需要手动实现binSearch的每一步。



代码：

```c
int howManyBits(int x) {
  int b16,b8,b4,b2,b1,b0;
  int mask = x >> 31;
  x = (mask & ~x) | (~mask & x); //如果为正数，保持不变；如果为负数，按位取反

  //step1:判断高16为是否有1
  b16 = !!(x >> 16) << 4; //如果高16为有1,则b16 = 16，否则为0
  x >>= b16; //如果高16为有1,x右移16位舍弃低16位,在新的低16位继续查找；否则保持不变
  //step2:判断高8位是否有1
  b8 = !!(x >> 8) << 3;
  x >>= b8;
  //step3:高4位
  b4 = !!(x >> 4) << 2;
  x >>= b4;
  //step4:高2位
  b2 = !!(x >> 2) << 1;
  x >>= b2;
  //step5:高1位
  b1 = !!(x >> 1);
  x >>= b1;
  //step6:低1位
  b0 = x;

  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
```

下图给出了执行的一个实例便于理解。

![JTqcE4.png](https://s1.ax1x.com/2020/04/29/JTqcE4.png)

### 3.11. floatScale2

题目描述：

>floatScale2 - Return bit-level equivalent of expression 2\*f for floating point argument f.
>
>Both the argument and result are passed as unsigned int's, but
>
>they are to be interpreted as the bit-level representation of
>
>single-precision floating point values.
>
>When argument is NaN, return argument
>
>Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
>
>Max ops: 30
>
>Rating: 4

解题思路：

分类讨论

[![J7pnUK.md.png](https://s1.ax1x.com/2020/04/29/J7pnUK.md.png)](https://imgchr.com/i/J7pnUK)

* denormalized

  exp = 0，M = 1 - Bias = -126，M = 0.frac，将frac左移1位。  

  这里有一种特殊情况，如果frac最高位为1，假设frac = 1100，M=0.frac = 0.1100，左移1位将最高位移到exp中，此时E = exp - Bias = 1 - Bias = -126保持不变，M = 1.frac = 1.1000，这种情况同样正确。

* special

  exp = 0xFF，当frac = 0时，代表无穷大，无穷大的2倍同样是无穷大，此时直接返回；当frac != 0时，代表NaN，直接返回。所以当exp = 0xFF时直接返回函数参数。

* normalized
  M = exp - Bias, M = 1.frac，将exp+1。

代码：

```c
unsigned floatScale2(unsigned uf) {
  unsigned sign = (uf >> 31) & 0x1;
  unsigned exp = (uf >> 23) & 0xFF;
  unsigned frac = uf & 0x7FFFFF;
  unsigned res;

  if (exp == 0xFF) //special
  {
    return uf;
  }
  else if (exp == 0) //denormalized
  {
    frac <<= 1;
    res = (sign << 31) | (exp << 23) | frac; 
  }
  else //normalized
  {
    exp++;
    res = (sign << 31) | (exp << 23) | frac;
  }
  return res;
}
```

### 3.12. floatFloat2Int

题目描述：

>floatFloat2Int - Return bit-level equivalent of expression (int) f
>
>for floating point argument f.
>
>Argument is passed as unsigned int, but
>
>it is to be interpreted as the bit-level representation of a
>
>single-precision floating point value.
>
> Anything out of range (including NaN and infinity) should return 0x80000000u.
>
> Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
>
> Max ops: 30
>
> Rating: 4

解题思路：

浮点数在数轴上的分布如下图所示。

![J7uFnf.png](https://s1.ax1x.com/2020/04/29/J7uFnf.png)

![J7usED.png](https://s1.ax1x.com/2020/04/29/J7usED.png)

分三种情况：

* denormalized，表示非常接近0的数，转换为int值后为0
* normalized，数的分布从接近0到无穷越来越稀疏，当f不超过int型表示的范围时，转换为int；当超过int型表示的范围时返回0x80000000u
* special，返回0x8000000u

在将normalized float转换为int型数时，M = 1.frac

* 如果E >= 31，小数点右移31位，此时隐含的1和frac占32位，另外还需要一个符号位，超出了int型范围
* 如果E < 0，小数点左移1位后为0.1frac，转换为int后为0
* 如果0 < E < 23, 小数点左移E为后需要舍弃frac中部分位，此时直接将frac右移23-E位，抹去小数部分
* 如果23 <= E < 31，此时小数点右移后frac全部移到小数点以左，将frac左移E-23位，在后面补零

代码：

```c
int floatFloat2Int(unsigned uf) {
  int sign = (uf >> 31) & 1;
  int exp = (uf >> 23) & 0xff;
  int frac = uf & 0x7fffff;

  int E = exp - 127;

  if (E < 0) //小数
  {
    return 0;
  }
  else if (E >= 31) // 超出int范围
  {
    return 0x80000000u;
  }
  else
  {
    frac = frac | (1 << 23);  //加上隐含的1

    if (E < 23)		//舍去部分小数
    {
      frac >>= (23 - E);
    }
    else		//不需要舍去小数
    {
      frac <<= (E - 23);
    }

    if (sign)
      return -frac;
    else
      return frac;
  }
}
```

### 3.13. floatPower2

题目描述：

> floatPower2 - Return bit-level equivalent of the expression 2.0^x
>
> (2.0 raised to the power x) for any 32-bit integer x.
>
> 
>
> The unsigned value that is returned should have the identical bit
>
> representation as the single-precision floating-point number 2.0^x.
>
> If the result is too small to be represented as a denorm, return 0.
>
> If too large, return +INF.
>
>  
>
> Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
>
> Max ops: 30 
>
> Rating: 4

解题思路：

根据浮点数求值公式**v = (-1)^s \* M \* 2 ^ E** = 2 ^ x

1.normalized

​	令M = 1(frac = 0)，x = E =  exp - Bias, exp = x + Bias

2.denormalized

​	exp = 0，在frac中令某一位为1，从而可以使x更小

|              | exp  |  frac  |   M    | maxE | minE |
| :----------: | :--: | :----: | :----: | :--: | :--: |
| denormalized |  0   | 0\*10* | 0.frac | -127 | -148 |
|  normalized  | 非0  |   0    |  1.0   | 127  | -126 |

下面是对边界情况的分析。

1.denormalized

* 当frac = 100 0000 0000 0000 0000 0000时，M = 0.1b = 0.5, E = 1- Bias = -126，此时v = 0.5 * 2.0 ^ -126 = 2.0 ^ -127
* 当frac = 000 0000 0000 0000 0000 0001时，M = 0.000 0000 0000 0000 0000 0001 = 2.0 ^ -22, E = -126，此时v = 2.0 ^ -22 * 2 ^ -126 = 2.0 ^ -148

2.normalized

* exp = 0xFF时，E = exp - Bias = 127
* exp = 1时，E = exp - Bias = -126

代码：

```c
unsigned floatPower2(int x) {
  if (x > 127) //too large, return +INF
  {
    return (0xFF << 23);
  }
  else if (x < -148) //too small, return 0
  {
    return 0;
  }
  else if (x >= -126) //norm，计算exp
  {
    int exp = x + 127;
    return (exp << 23);
  }
  else //denorm，令frac中某一位为1
  {
    int t = 148 + x;
    return (1 << t);
  }
  
}
```

## 4. Make & Test

在make时可能会出现一个错误：

>Error message : fatal error: bits/libc-header-start.h: No such file or directory

错误原因：Makefile第5行CFLAGS中-m32告诉gcc产生32位代码，但是在64位机器上gcc通常使用64位的库。

解决方法：将CFLAGS中-m32删掉，直接编译产生64位代码

## 5. Summary

1. 计算机中的数是用有限位来表示的，所以有一定的范围，在进行运算的时候可能会产生溢出(上溢、下溢)。
2.  整形数在数轴上的分布是均匀的，浮点型数的规格数在数轴上分布是不均匀的，越靠近0越稠密。

