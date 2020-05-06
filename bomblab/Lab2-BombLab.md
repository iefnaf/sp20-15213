# BombLab:Defusing a Binary Bomb  

## 1. OverView

邪恶博士在我们的机器上安装了一个"二进制炸弹"。二进制炸弹是一个包含一系列阶段的可执行程序，每个阶段需要你在`stdin`上输入一个特定的字符串。如果你输入的字符串正确，那么这个阶段就被成功拆除，炸弹进入下一阶段。否则，炸弹会爆炸，并输出"BOOM!!!",然后终止。当所有的阶段都被成功拆除后炸弹就被拆除。

### step1：获取炸弹

这里有一个[Linux/x86-64 binary bomb](http://csapp.cs.cmu.edu/3e/bomb.tar) 你可以下载并使用它。

下载之后可以使用以下命令来解压：

```shel
$ tar -xvf bomb.tar
```

该命令将会创建一个名为bomb的目录，目录内包含以下文件：

* README
* bomb：可执行二进制炸弹
* bomb.c

### step2：拆炸弹

炸弹分为6个阶段，前4个阶段每个10分，第5和第6阶段有点难，所以每个15分。在拆除炸弹的过程中，炸弹每爆炸一次将从你的总分里扣除0.5分，最多扣除20分。所以为了避免意外引爆炸弹，**你需要学会单步执行汇编代码以及设置断点**。通过做这个实验，你将学会如何使用一个debugger，这在你今后的职业生涯中都将是一项非常重要的技能。

## 2. Hints

拆炸弹的姿势有很多种。你可以不去运行程序而去仔细阅读汇编代码，搞懂每行代码的作用，这是一个有用的本领，但是这样做比较困难。拆炸弹最快的方法是在一个debugger里运行它，逐步观察发生了什么，根据这些信息去拆除它。

这里有一系列你可能会用到的工具：

* gdb

  GNU debugger，一个可以在所有平台上运行的命令行debugger工具。你可以使用它来逐行跟踪程序的执行、观察内存和寄存器的值、查看源代码和汇编代码、设置断点等。

  这里有一个[gdb命令手册](http://csapp.cs.cmu.edu/2e/docs/gdbnotes-x86-64.pdf)以供查阅。

* objdump -t

  输出炸弹程序的符号表。符号表里包含了所有函数和全局变量的名字。

* objdump -d

  可以使用这条命令来反汇编炸弹程序。

个人建议：

* 进入gdb程序后首先break explode_bomb，防止炸弹意外爆炸。如果不幸执行到explode_bomb，程序会停下，此时kill掉重新执行即可。
* layout asm 调出汇编代码窗口
* layout reg   调出寄存器窗口
* 对于一些函数，根据函数名猜测作用即可，不用看它是如何实现的。

## 3. The Phases

首先观察一下main函数，我们可以看到这里有6个阶段，每个阶段是一个函数，在函数正常结束之后运行`phase_defused`拆除这个阶段，然后进入下一阶段。

```assembly
  400e19:	e8 84 05 00 00       	callq  4013a2 <initialize_bomb>
  400e1e:	bf 38 23 40 00       	mov    $0x402338,%edi
  400e23:	e8 e8 fc ff ff       	callq  400b10 <puts@plt>
  400e28:	bf 78 23 40 00       	mov    $0x402378,%edi
  400e2d:	e8 de fc ff ff       	callq  400b10 <puts@plt>
  400e32:	e8 67 06 00 00       	callq  40149e <read_line>
  400e37:	48 89 c7             	mov    %rax,%rdi
  400e3a:	e8 a1 00 00 00       	callq  400ee0 <phase_1>
  400e3f:	e8 80 07 00 00       	callq  4015c4 <phase_defused>
  400e44:	bf a8 23 40 00       	mov    $0x4023a8,%edi
  400e49:	e8 c2 fc ff ff       	callq  400b10 <puts@plt>
  400e4e:	e8 4b 06 00 00       	callq  40149e <read_line>
  400e53:	48 89 c7             	mov    %rax,%rdi
  400e56:	e8 a1 00 00 00       	callq  400efc <phase_2>
  400e5b:	e8 64 07 00 00       	callq  4015c4 <phase_defused>
  400e60:	bf ed 22 40 00       	mov    $0x4022ed,%edi
  400e65:	e8 a6 fc ff ff       	callq  400b10 <puts@plt>
  400e6a:	e8 2f 06 00 00       	callq  40149e <read_line>
  400e6f:	48 89 c7             	mov    %rax,%rdi
  400e72:	e8 cc 00 00 00       	callq  400f43 <phase_3>
  400e77:	e8 48 07 00 00       	callq  4015c4 <phase_defused>
  400e7c:	bf 0b 23 40 00       	mov    $0x40230b,%edi
  400e81:	e8 8a fc ff ff       	callq  400b10 <puts@plt>
  400e86:	e8 13 06 00 00       	callq  40149e <read_line>
  400e8b:	48 89 c7             	mov    %rax,%rdi
  400e8e:	e8 79 01 00 00       	callq  40100c <phase_4>
  400e93:	e8 2c 07 00 00       	callq  4015c4 <phase_defused>
  400e98:	bf d8 23 40 00       	mov    $0x4023d8,%edi
  400e9d:	e8 6e fc ff ff       	callq  400b10 <puts@plt>
  400ea2:	e8 f7 05 00 00       	callq  40149e <read_line>
  400ea7:	48 89 c7             	mov    %rax,%rdi
  400eaa:	e8 b3 01 00 00       	callq  401062 <phase_5>
  400eaf:	e8 10 07 00 00       	callq  4015c4 <phase_defused>
  400eb4:	bf 1a 23 40 00       	mov    $0x40231a,%edi
  400eb9:	e8 52 fc ff ff       	callq  400b10 <puts@plt>
  400ebe:	e8 db 05 00 00       	callq  40149e <read_line>
  400ec3:	48 89 c7             	mov    %rax,%rdi
  400ec6:	e8 29 02 00 00       	callq  4010f4 <phase_6>
  400ecb:	e8 f4 06 00 00       	callq  4015c4 <phase_defused>
```



### 3.1. Phase1

我们可以看到这里有一个`explode_bomb`函数，从名字上就可以知道这是引爆炸弹的程序，所以为了防止炸弹引爆，我们需要在这里设置断点，同时在phase_1设置断点，让程序在此停下。

```shell
(gdb) break explode_bomb
(gdb) break phase_1
```

这里有一个`strings_not_equal`函数，我们可以根据名字知道这里是比较两个字符串是否相等，然后测试函数的返回值，如果不为0则调用`explode_bomb`程序引爆炸弹。

那么与我们输入的字符串比较的那个字符串放在哪里呢？可以看到第二行把`0x402400`放入`%esi`，这个地址就存放着目标字符串。我们可以使用以下指令来查看：

```shell
(gdb) print (char*) 0x402400
```

![YAAXfH.png](https://s1.ax1x.com/2020/05/06/YAAXfH.png)

```assembly
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq   
```

### 3.2. Phase2

`phase_2`首先调用了一个`read_six_number`函数，可以推测阶段二让我们输入的字符串是6个数。这6个数存放在`%rsp`指向的位置，我们可以用`x/6wd $rsp`来验证这里是不是我们输入的6个数。 

首先判断第一个数是否为1，不为1则引爆炸弹。然后是一个循环，判断后一个数是否为前一个数的2倍，如果不是则引爆炸弹。所以这6个数为1 2 4 8 16 32。 

```assembly
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)	   ;第一个数为1
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax ;%eax = before
  400f1a:	01 c0                	add    %eax,%eax       ;before = 2 * before
  400f1c:	39 03                	cmp    %eax,(%rbx)     ;2 * before == after
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq 
```

### 3.3. Phase3

首先调用了sscanf函数，sscanf需要一个格式化字符串，而根据第四行，该格式化字符串的地址就是`0x4025cf`。

我们可以用`print (char*) 0x4025cf`来查看这个格式化字符串。  可以看到阶段3让我们输入的是两个数字。

```shell
(gdb) print (char*) 0x4025cf
$1 = 0x4025cf "%d %d"
```

```assembly
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
```

接下来首先判断x1是否大于7，如果大于7，则引爆炸弹。

然后是一条间接跳转指令，这条指令是本题的关键所在。根据这个jmp指令的格式，可以知道这是一条switch语句，根据跳转表内容进行跳转。跳转表存储在`0x402470`处。

```shell
(gdb) x/8gd 0x402470
0x402470:       4198268 4198329
0x402480:       4198275 4198282
0x402490:       4198289 4198296
0x4024a0:       4198303 4198310
```

这里先假设我们输入的第一个数为2，查阅跳转表，可知目的地址为4198275(0x400f83)。在0x400f83处的指令将0x2c3(707)放入%eax寄存器，然后跳转到400fbe。在400fbe处的指令是比较x2和%eax中的值，不等则引爆炸弹。  

```assembly
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)        ;if x1 > 7 
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a> ;explode_bomb
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8)  ;switch语句的jump table
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
```

所以这段汇编的等效c代码为：

```c
if (x1 > 7)
    explode_bomb();
switch(x1)
{
    case 0:
        if (x2 != anumber)
            explode_bomb();
       	break;
    case 1:
        if (x2 != anumber)
            explode_bomb();
        break;
    ...
}
```

### 3.4. Phase4

phase_4首先调用了sscanf，通过查看存储在`0x4025cf`处的格式化字符串可知让我们输入的是两个整数。然后比较x1与0xe，如果x1大于0xe则引爆炸弹，否则调用另一个函数`func4`。如果func4的返回值不为0，则引爆炸弹。所以我们需要设置x1,x2，使得func4的返回值为0。

```assembly
000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax
  401024:	e8 c7 fb ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  401029:	83 f8 02             	cmp    $0x2,%eax
  40102c:	75 07                	jne    401035 <phase_4+0x29>
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)
  401033:	76 05                	jbe    40103a <phase_4+0x2e>
  401035:	e8 00 04 00 00       	callq  40143a <explode_bomb>
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx
  40103f:	be 00 00 00 00       	mov    $0x0,%esi
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi
  401048:	e8 81 ff ff ff       	callq  400fce <func4>
  40104d:	85 c0                	test   %eax,%eax
  40104f:	75 07                	jne    401058 <phase_4+0x4c>
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp)
  401056:	74 05                	je     40105d <phase_4+0x51>
  401058:	e8 dd 03 00 00       	callq  40143a <explode_bomb>
  40105d:	48 83 c4 18          	add    $0x18,%rsp
  401061:	c3                   	retq  
```

这里使用的寄存器较多，**最好的方法不是分析读代码，而是单步调试**，查看各个寄存器中的值。我的方法是单步执行到`cmp %edi, %ecx`，然后跳转到0x400ff2，接着执行`cmp %edi, %ecx`，保证%ecx >= %edi，然后跳转到401007。

```assembly
0000000000400fce <func4>:
  400fce:	48 83 ec 08          	sub    $0x8,%rsp
  400fd2:	89 d0                	mov    %edx,%eax
  400fd4:	29 f0                	sub    %esi,%eax
  400fd6:	89 c1                	mov    %eax,%ecx
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx
  400fdb:	01 c8                	add    %ecx,%eax
  400fdd:	d1 f8                	sar    %eax
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx
  400fe2:	39 f9                	cmp    %edi,%ecx
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24>
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx
  400fe9:	e8 e0 ff ff ff       	callq  400fce <func4>
  400fee:	01 c0                	add    %eax,%eax
  400ff0:	eb 15                	jmp    401007 <func4+0x39>
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax
  400ff7:	39 f9                	cmp    %edi,%ecx
  400ff9:	7d 0c                	jge    401007 <func4+0x39>
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi
  400ffe:	e8 cb ff ff ff       	callq  400fce <func4>
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  401007:	48 83 c4 08          	add    $0x8,%rsp
  40100b:	c3                   	retq 
```

### 3.5. Phase5

首先是判断读入字符串的长度，如果不等于6则引爆炸弹。

```assembly
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp
  401067:	48 89 fb             	mov    %rdi,%rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401071:	00 00 
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  40107a:	e8 9c 02 00 00       	callq  40131b <string_length>
  40107f:	83 f8 06             	cmp    $0x6,%eax
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
  401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
```

然后是一个循环，将输入的6个字符分别&0xf，所得结果作为下标，取0x4024b0处的一个字符，依次存入%rsp + 0x10处。

```shel
(gdb) print (char*) $rbx
$1 = 0x6038c0 <input_strings+320> "ionefg"    <-----输入的6个字符
(gdb) print (char*) 0x4024b0
$2 = 0x4024b0 <array> "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"   <----- 字符数组
```

```assembly
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx
  40108f:	88 0c 24             	mov    %cl,(%rsp)
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>
```

最后是一个字符串比较，待比较字符串存放在`0x40245e`。

```shell
(gdb) print (char*) 0x40245e
$3 = 0x40245e "flyers"
```

```assembly
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	callq  401338 <strings_not_equal>
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
  4010c6:	e8 6f 03 00 00       	callq  40143a <explode_bomb>
```

所以我们的任务是：输入6个字符，把每个字符 & 0xf，所得结果作为下标，从一个字符数组中取字符，最终组成"flyers"。

### 3.6. Phase6

首先在`explode_bomb`和`phase_6`处设置断点，然后开始运行程序。

```
(gdb) b explode_bomb
(gdb) b phase_6
```

然后浏览一下`phase_6`的代码。

```shell
(gdb) layout asm
(gdb) disas phase_6
```

可以看到有一个`read_six_numbers`函数，说明`phase_6`需要我们输入6个数字。  

`read_six_numbers`函数将用户输入的6个数字存放在`$rsp`开始的位置，每个数占一个字(4个字节)。我们可以通过`x/6wd $rsp`来查看我们输入的这6个数字。

```assembly
  4010f4:	41 56                	push   %r14
  4010f6:	41 55                	push   %r13
  4010f8:	41 54                	push   %r12
  4010fa:	55                   	push   %rbp
  4010fb:	53                   	push   %rbx
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp
  401100:	49 89 e5             	mov    %rsp,%r13
  401103:	48 89 e6             	mov    %rsp,%rsi
  401106:	e8 51 03 00 00       	callq  40145c <read_six_numbers>
```

下面开始分析phase_6的代码(以下代码为objdump -d bomb的输出结果，和gdb disas略有不同)。   

下面这段汇编最重要的两条语句是`401121:76 05 jbe 401128 <phase_6+0x34>`和`40113e:75 05 jne 401145 <phase_6+0x51>`，分别保证了输入的6个数小于等于6和6个数各不相同。

```assembly
  40110b:	49 89 e6             	mov    %rsp,%r14
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d
  401114:	4c 89 ed             	mov    %r13,%rbp
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax
  40111b:	83 e8 01             	sub    $0x1,%eax
  40111e:	83 f8 05             	cmp    $0x5,%eax
  401121:	76 05                	jbe    401128 <phase_6+0x34>
  401123:	e8 12 03 00 00       	callq  40143a <explode_bomb>
  401128:	41 83 c4 01          	add    $0x1,%r12d
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d
  401130:	74 21                	je     401153 <phase_6+0x5f>
  401132:	44 89 e3             	mov    %r12d,%ebx
  401135:	48 63 c3             	movslq %ebx,%rax
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp)
  40113e:	75 05                	jne    401145 <phase_6+0x51>
  401140:	e8 f5 02 00 00       	callq  40143a <explode_bomb>
  401145:	83 c3 01             	add    $0x1,%ebx
  401148:	83 fb 05             	cmp    $0x5,%ebx
  40114b:	7e e8                	jle    401135 <phase_6+0x41>
  40114d:	49 83 c5 04          	add    $0x4,%r13
  401151:	eb c1                	jmp    401114 <phase_6+0x20>
```

这段汇编代码是一个二重循环，等效于以下c代码。其中`%r12d`相当于`i`，`%ebx`相当于`j`。这段代码的作用是保证用户输入的6个数小于等于6，且各不相同。

```c
for (int i = 0; i < 6; i++)
{
    if (arr[0] > 6)
        explode_bomb();
    for (int j = i; j <= 5; j++)
        if (arr[i] == arr[j])
            explode_bomb();
```

下面这段代码的作用是将栈内存储的6个数变为7-x。

```assembly
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi
  401158:	4c 89 f0             	mov    %r14,%rax
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx
  401160:	89 ca                	mov    %ecx,%edx
  401162:	2b 10                	sub    (%rax),%edx
  401164:	89 10                	mov    %edx,(%rax)
  401166:	48 83 c0 04          	add    $0x4,%rax
  40116a:	48 39 f0             	cmp    %rsi,%rax
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>
```

下面这段代码是最难理解的部分。我们可以看到这里有一个常数`$0x6032d0`，这是本题的关键所在。尝试查看该地址的内容(这里大概率不会一次成功，我尝试了x/gd和x/wd等等多次之后才成功)。可以看到有有6个节点，每个节点有一个值，还有一个指向下一节点的指针(下一节点的地址)，所以这是一个**链表**。

![YF89P0.png](https://s1.ax1x.com/2020/05/05/YF89P0.png)



这段代码的作用是依次取出栈中存放的6个数(已经被7减过)，根据每个数x，从前向后遍历链表到链表的第x个节点，然后将节点的地址依次存入`$rsp+0x20`处的6个位置。

```assembly
  40116f:	be 00 00 00 00       	mov    $0x0,%esi				
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>	
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx	;rdx保存节点的地址
  40117a:	83 c0 01             	add    $0x1,%eax	;顺序遍历链表，找第$ecx(7-Xi)个节点
  40117d:	39 c8                	cmp    %ecx,%eax	;
  40117f:	75 f5                	jne    401176 <phase_6+0x82>	;
  401181:	eb 05                	jmp    401188 <phase_6+0x94>
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2) ;将节点地址存入$rsp+0x20处
  40118d:	48 83 c6 04          	add    $0x4,%rsi
  401191:	48 83 fe 18          	cmp    $0x18,%rsi
  401195:	74 14                	je     4011ab <phase_6+0xb7>
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx		;顺序取栈中的6个数(7-Xi)
  40119a:	83 f9 01             	cmp    $0x1,%ecx
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>
```

执行完这段代码后可以使用`x/6g $rsp+0x20`来查看存入的6个节点地址。

![YFYmIs.png](https://s1.ax1x.com/2020/05/05/YFYmIs.png)

最后一段代码的作用是通过`$rsp+0x20`处存入的6个节点的地址取节点值，保证前一个数<后一个数。

```assembly
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax
  4011e3:	8b 00                	mov    (%rax),%eax
  4011e5:	39 03                	cmp    %eax,(%rbx)
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa> ;前一个数>=后一个数
  4011e9:	e8 4c 02 00 00       	callq  40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>
```

由6个节点值分别为332,168,924,691,477,443，可知每个节点按从大到小的顺序为3,4,5,6,1,2，因为栈中的每个数都被7减去，所以原来存入的6个数为4,3,2,1,6,5。

## 4. Summary

第一次听说拆炸弹是在大三的计算机网络课上，当时老师说我这里有一个拆炸弹的游戏，不知道有没有同学有兴趣来玩一下，班里大概有两三个人报名。第二次接触到拆炸弹就是大四毕业的那个暑假，在字节跳动实习的时候，晚上自学了cmu15213的前几章，当时做了拆炸弹这个实验，解决了phase1~phase_5，phase_6最终没做出来，看了别人写的博客才发现是一个链表。

![第一次拆炸弹现场](https://s1.ax1x.com/2020/05/06/YADiQJ.jpg)

第一次拆炸弹的时候看这么多汇编代码难免头皮发麻，不过最好边跑代码边寄存器的变化。 拆完炸弹的过程不知不觉的就学会了gdb的使用。

总的来说，拆炸弹是一个非常有趣又很有挑战的实验。Hope you enjoy it.