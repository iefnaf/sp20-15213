# C Programming Lab : Assessing Your C Programming Skills

## 1. Overview

本次实验主要是复习一下C语言，为以后的实验打下基础，其中涉及到的内容包括：

* 显式内存管理
* 创建和操作基于指针的数据结构
* 使用string
* 通过在数据结构中存储其余相关信息来提高关键操作的性能
* 编写健壮的代码

本次实验主要涉及两个文件：queue.h和queue.c，主要任务是实现一个string的单链表。

![GfCHld.png](https://s1.ax1x.com/2020/04/08/GfCHld.png)

## 2. Programming Task

我们的任务是修改queue.h和queue.c，实现以下函数

* q_new：创建一个空队列
* q_free：释放队列使用的所有内存
* q_insert_head：在队列头部插入一个新元素
* q_insert_tail：在队列尾部插入一个新元素
* q_remove_head：删除队头元素
* q_size：计算队列中元素的个数
* q_reverse：逆置队列。这个函数不能申请或者释放任何队列元素

## 3. Implementation

### 3.1 扩充数据结构

由于需要高效计算队列中元素个数和在队列尾部插入元素，所以对queue_t进行补充，增加两个成员：

* int _size：当前队列中元素个数
* list_ele_t *tail：队尾指针

```c
/* Linked list element (You shouldn't need to change this) */
typedef struct ELE {
    /* Pointer to array holding string.
       This array needs to be explicitly allocated and freed */
    char *value;
    struct ELE *next;
} list_ele_t;

/* Queue structure */
typedef struct {
    list_ele_t *head;  /* Linked list of elements */
    /*
      You will need to add more fields to this structure
      to efficiently implement q_size and q_insert_tail
    */
    list_ele_t *tail;
    int _size;
} queue_t;
```

![Gfuc1P.jpg](https://s1.ax1x.com/2020/04/08/Gfuc1P.jpg)

### 3.2 相关函数实现

#### 3.2.1 q_new

创建一个空队列，如果申请内存失败则返回NULL。

```c
queue_t *q_new()
{
    queue_t *q =  (queue_t*)malloc(sizeof(queue_t));
    if (q == NULL) //内存申请失败
    {
      return NULL;
    }
    //队列元素初始化
    q->head = NULL;
    q->tail = NULL;
    q->_size = 0;
    return q;
}
```

#### 3.2.2 q_free

释放队列使用的内存。注意：为了程序的健壮性，**在释放内存之前需要先检查指针是否为NULL**，此处释放的内存分为三部分：

* 结构体queue_t占用的内存
* 结构体list_ele_t占用的内存
* 结构体list_ele_t中value指针指向的字节数组占用的内存

```c
void q_free(queue_t *q)
{
    if (q == NULL)
    {
      return;
    }
    for (int i = 0; i < q->_size; i++)
    {
      list_ele_t *p = q->head;
      q->head = q->head->next;
      if (p->value != NULL)
      {
        free(p->value);
      }
      free(p);
    }
    free(q);
}
```

#### 3.2.3 q_insert_head

在队首插入元素，如果插入成功，返回true；如果q为NULL或者申请内存失败，返回false。参数s是指向需要存储string的指针。此函数需要显式申请内存，并将string拷贝到申请的内存中。

```c
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
    {
      return false;
    }

    list_ele_t *newh = (list_ele_t*)malloc(sizeof(list_ele_t));
    if (newh == NULL)
    {
      return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    newh->value = (char*)malloc(strlen(s) + 1);
    
    if (newh->value == NULL)
    {
      free(newh); //don't forget to free
      return false;
    }

    strcpy(newh->value, s);
    
    //当前队列为空
    if (q->_size == 0)
    {
      q->head = newh;
      q->tail = newh;
      newh->next = NULL;
    }
    else
    {
      newh->next = q->head;
      q->head = newh;
    }
    q->_size++;
    return true;
}
```

1. 首先需要判断队列指针p是否为NULL，如果为NULL直接返回false
2. 为新队列头元素申请内存空间，如果申请失败返回false
3. 为字节数组申请空间，注意这里**申请的内存空间大小为strlen(s) + 1**，+1是为了存储末尾的0。如果申请失败，需要**释放队头元素所占内存空间**，然后返回false
4. 调用strcpy函数将s处的内容copy到新队头的value处
5. 如果当前队列为空，需要设置head和tail，并将newh的next指针置为NULL
6. 如果当前队列非空，使用**头插法**插入新队头
7. **改变队列元素个数**

#### 3.2.4 q_insert_tail

在队列尾部插入元素，实现与q_insert_head类似，为list_ele_t和字节数组申请空间，copy字符串，最后将list_ele_t插入尾部。

```c
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
      return false;
    }

    list_ele_t *newt = (list_ele_t*)malloc(sizeof(list_ele_t));
    if (newt == NULL)
    {
      return false;
    }
    newt->value = (char*)malloc(strlen(s) + 1);
    if (newt->value == NULL)
    {
      free(newt);
      return false;
    }

    strcpy(newt->value, s);

    if (q->tail == NULL)//当前队列中元素为空
    {
      q->head = newt;
      q->tail = newt;
    }
    else
    {
      q->tail->next = newt;
      q->tail = newt;
    }
    newt->next = NULL;
    q->_size++;
    return true;
}
```

#### 3.2.5 q_remove_head

删除队首元素，并释放所占内存，如果参数sp != NULL，需要将队首字符串复制到sp处，**最多复制bufsize-1个字节**。

```c
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->_size == 0)
    {
      return false;
    }
	//将队首元素从队列中摘下
    list_ele_t *oldh = q->head;
    q->head = q->head->next;
    oldh->next = NULL;

    if (sp != NULL)
    {
      strncpy(sp, oldh->value, bufsize - 1);
      sp[bufsize-1] = '\0'; 
    }
    
    if (oldh->value != NULL)
    {
      free(oldh->value);
    }
    
    free(oldh);
    q->_size--;
    return true;
}
```

如果sp!=NULL，使用strncpy函数，复制**至多bufsize-1个字节**，然后将sp[bufsize-1]处手动填入0。最后释放字节数组和原队首元素占用的内存空间。

#### 3.2.6 q_size

返回当前队列中元素的个数。

```c
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
    {
      return 0;
    }
    return q->_size;
}
```

#### 3.2.7 q_reverse

单链表的就地逆置。采用头插法，将[head~tail-1]处的元素插到tail处。

```c
void q_reverse(queue_t *q)
{
    if (q == NULL || q->_size == 0)
    {
      return;
    }

    list_ele_t *oldh = q->head;
    list_ele_t *oldt = q->tail;
    for (int i = 0; i < q->_size - 1; i++)
    {
      list_ele_t *h = q->head;
      q->head = q->head->next;
      h->next = q->tail->next;
      q->tail->next = h;
    }
    q->head = oldt;
    q->tail = oldh;
}
```

## 4. Make & Make test

在代码编写完成之后需要测试代码的正确性，首先需要编译代码，Makefile已经为我们写好，我们只需make即可

```shell
$ make
```

这里可能会出现一个错误：

```shell
/usr/local/depot/llvm-7.0/bin/clang -O0 -g -Wall -Werror -c queue.c 
make: /usr/local/depot/llvm-7.0/bin/clang: Command not found
Makefile:8: recipe for target 'queue.o' failed
make: *** [queue.o] Error 127
```

解决方法：

1.安装clang

```shell
$ sudo apt-get install clang
```

2.修改Makefile第一行

```makefile
CC = /usr/bin/clang
```

编译成功之后测试

```shell
$ make test
```

## 5. Summary

通过本次实验，复习了C语言中的内存管理(malloc和free)、链表的相关操作、字符串的相关函数。同时，实验强调了编写健壮代码的重要性，比如在malloc后需要检查是否申请成功，在释放内存的时候需要把所有申请的空间都释放掉，以免造成内存泄漏。