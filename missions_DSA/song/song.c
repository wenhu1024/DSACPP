#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 55 // 设置字符串数组最大长度
/* 该课设使用双向链表数据结构 */

/* 定义双向链表中的每一个节点 */
typedef struct Node ListNode; // 使用typedef，用ListNode替换struct Node

struct Node
{
    char name[MAX_LENGTH];   // 歌曲名字
    char writer[MAX_LENGTH]; // 作者
    char singer[MAX_LENGTH]; // 演唱者
    char date[MAX_LENGTH];   // 发行年月
    int sales;               // 销量
    ListNode *pred;          // 前驱
    ListNode *succ;          // 后继
};

char _name[MAX_LENGTH], _writer[MAX_LENGTH], _singer[MAX_LENGTH], _date[MAX_LENGTH]; // 接受输入的数据(全局变量)
int _sales;                                                                          // 接受输入的数据(全局变量)
ListNode *header = NULL;                                                             // 头哨兵(全局变量)
ListNode *trailer = NULL;                                                            // 尾哨兵(全局变量)
int _size = 0;                                                                       // 数据规模(全局变量)

/* 定义双向链表中的每一个节点 */
typedef struct writerNode writerListNode; //    使用typedef，用WriterListNode替换struct WriterNode

struct writerNode
{
    char writer[MAX_LENGTH];
    writerListNode *pred;
    writerListNode *succ;
};

writerListNode *writerHeader = NULL;  // 头哨兵(全局变量)
writerListNode *writerTrailer = NULL; // 尾哨兵(全局变量)
int writerSize = 0;                   // 数据规模(全局变量)

void init();                                                                              // 全局变量头，尾哨兵初始化
int writerHasSame(ListNode *e);                                                           // 判断writer链表中是否含有相同的e->writer,含有则返回1，否则返回0
void writerInsertAsLast(ListNode *e);                                                     // e->writer不同则插入，插入至末端
void writerRemove(ListNode *e);                                                           // 把writerList中对应writer移除
int size() { return _size; }                                                              // 返回数据规模
int empty() { return size() == 0; }                                                       // 判断链表是否为空
ListNode *createNode(char *_name, char *_writer, char *_singer, char *_date, int _sales); // 创建一个节点，返回该节点指针
void insertAsFirst(ListNode *e);                                                          // e当作首节点插入
void insertAsLast(ListNode *e);                                                           // e当作末节点插入
void insertB(ListNode *p, ListNode *e);                                                   // 将e当作p的前驱插入
void insertA(ListNode *p, ListNode *e);                                                   // 将e当作p的后驱插入
ListNode Remove(ListNode *e);                                                             // 将e节点从链表移除出去,并返回备份
ListNode *searchByName(char *);                                                           // 按照歌曲名查询（默认歌曲信息中，歌曲名字不会重复）,返回匹配的节点指针
int searchByWriter(char *);                                                               // 按照作者查询,符合条件立即输出,返回符合条件的歌曲信息条数
int searchBySinger(char *);                                                               // 按照演唱者查询,符合条件立即输出,返回符合条件的歌曲信息条数
void forwardTraverse();                                                                   // 顺序遍历输出信息
void reverseTraverse();                                                                   // 逆序遍历输出信息
void printGroupByWriter();                                                                // 按照作者分组查看
void selectSort(ListNode *p, int n);                                                      // 选择排序,对起始于位置p的n个元素的排序
ListNode *selectMax(ListNode *p, int n);                                                  // 从起始位置p的n个元素中选出最大者,若有2个相同最大节点，返回靠后的一个节点
void read();                                                                              // 文件读取
void save();                                                                              // 文件保存

void init()
{
    header = (ListNode *)malloc(sizeof(ListNode));
    trailer = (ListNode *)malloc(sizeof(ListNode));
    header->pred = NULL;
    header->succ = trailer;
    trailer->pred = header;
    trailer->succ = NULL;

    writerHeader = (writerListNode *)malloc(sizeof(writerListNode));
    writerTrailer = (writerListNode *)malloc(sizeof(writerListNode));
    writerHeader->pred = NULL;
    writerHeader->succ = writerTrailer;
    writerTrailer->pred = writerHeader;
    writerTrailer->succ = NULL;
}

ListNode *createNode(char *_name, char *_writer, char *_singer, char *_date, int _sales)
{
    ListNode *p = (ListNode *)malloc(sizeof(ListNode)); // 动态内存分配
    strcpy(p->name, _name);                             // 使用strcpy函数，字符串拷贝
    strcpy(p->writer, _writer);
    strcpy(p->singer, _singer);
    strcpy(p->date, _date);
    p->sales = _sales;
    return p;
}

void insertAsFirst(ListNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = header;
    e->succ = header->succ;
    header->succ->pred = e;
    header->succ = e;
}

void insertAsLast(ListNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = trailer->pred;
    e->succ = trailer;
    trailer->pred->succ = e;
    trailer->pred = e;
}

void insertB(ListNode *p, ListNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = p->pred;
    e->succ = p;
    p->pred->succ = e;
    p->pred = e;
}

void insertA(ListNode *p, ListNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = p;
    e->succ = p->succ;
    p->succ->pred = e;
    p->succ = e;
}

ListNode Remove(ListNode *e)
{
    ListNode copy = *e; // 备份待删除节点的数值
    e->pred->succ = e->succ;
    e->succ->pred = e->pred;
    free(e); // 释放动态内存
    _size--; // 数据规模减1
    return copy;
}

ListNode *searchByName(char *_name)
{
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        if (strcmp(p->name, _name) == 0)
        { // 找到
            return p;
        }
    }
    return NULL; // 否则，返回空指针，表示未找到
}

int searchByWriter(char *_writer)
{
    int sum = 0;
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        if (strcmp(p->writer, _writer) == 0)
        { // 找到
            printf("%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
            sum++;
        }
    }
    return sum; // 返回符合条件的数据规模大小
}

int searchBySinger(char *_singer)
{
    int sum = 0;
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        if (strcmp(p->singer, _singer) == 0)
        { // 找到
            printf("%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
            sum++;
        }
    }
    return sum; // 返回符合条件的数据规模大小
}

void forwardTraverse()
{
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        printf("%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
    }
}

void reverseTraverse()
{
    for (ListNode *p = trailer->pred; p != header; p = p->pred)
    {
        printf("%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
    }
}

ListNode *selectMax(ListNode *p, int n)
{
    ListNode *_max = p; // 暂定销量最大的对应节点为p
    for (ListNode *cur = p; 1 < n; n--)
    {
        if ((cur = cur->succ)->sales >= _max->sales)
        {
            _max = cur;
        }
    }
    return _max;
}

void selectSort(ListNode *p, int n)
{
    ListNode *head = p->pred;
    ListNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {                                                // 待排序区间节点还剩余的数目大于1个，则继续排序
        ListNode *_max = selectMax(header->succ, n); // 找出区间最大者
        ListNode p = Remove(_max);
        ListNode *select = createNode(p.name, p.writer, p.singer, p.date, p.sales); // 被选中的最大节点
        insertB(tail, select);                                                      // 被选中的最大节点作为tail前驱插入
        tail = tail->pred;                                                          // tail前移
        n--;                                                                        // 待排序区间减一
    }
}

int writerHasSame(ListNode *e)
{
    for (writerListNode *p = writerHeader->succ; p != writerTrailer; p = p->succ)
    {
        if (strcmp(p->writer, e->writer) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void writerInsertAsLast(ListNode *e)
{
    writerListNode *p = (writerListNode *)malloc(sizeof(writerListNode));
    strcpy(p->writer, e->writer);
    p->pred = writerTrailer->pred;
    p->succ = writerTrailer;
    writerTrailer->pred->succ = p;
    writerTrailer->pred = p;
    writerSize++; // 数据规模加1
}

void writerRemove(ListNode *e)
{
    for (writerListNode *p = writerHeader->succ; p != writerTrailer; p = p->succ)
    {
        if (strcmp(p->writer, e->writer) == 0)
        {
            p->pred->succ = p->succ;
            p->succ->pred = p->pred;
            free(p);
            writerSize--; // 数据规模减1
            break;
        }
    }
}

void printGroupByWriter()
{
    for (writerListNode *ptr = writerHeader->succ; ptr != writerTrailer; ptr = ptr->succ)
    {
        for (ListNode *p = header->succ; p != trailer; p = p->succ)
        {
            if (strcmp(p->writer, ptr->writer) == 0)
            {
                printf("%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
            }
        }
        printf("________________________________\n");
    }
}

void read()
{
    int num = 0;
    FILE *fp = fopen("information.txt", "r"); // r；读模式
    if (!fp)
    {
        printf("文件读取失败！");
        return;
    }

    fscanf(fp, "数据条数为%d", &num); // 获得文件数据总数
    for (int i = 0; i < num; ++i)
    {
        fscanf(fp, "%s %s %s %s %d\n", _name, _writer, _singer, _date, &_sales); // 读取信息
        ListNode *p = createNode(_name, _writer, _singer, _date, _sales);        // 建立节点
        insertAsLast(p);                                                         // 将节点加至链表尾端
    }
    fclose(fp);
}

void save()
{
    FILE *fp = fopen("information.txt", "w"); // w:写模式
    if (!fp)
    {
        printf("文件保存失败！");
        return;
    }

    fprintf(fp, "数据条数为%d\n", size()); // 将数据条数保存至文件
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        fprintf(fp, "%s %s %s %s %d\n", p->name, p->writer, p->singer, p->date, p->sales);
    }
    fclose(fp);
}

int main()
{
    init();

    read();
    for (ListNode *p = header->succ; p != trailer; p = p->succ)
    {
        if (!writerHasSame(p))
        {
            writerInsertAsLast(p);
        }
    }
    ListNode *ptr = NULL;
    while (1)
    {
        printf("***********欢迎使用歌曲管理系统**************\n");
        printf("*********功能如下所示**********************\n");
        printf("**** [1] 首端插入一条歌曲信息***************\n");
        printf("**** [2] 尾端插入一条歌曲信息***************\n");
        printf("**** [3] 将一条歌曲信息插入到某一特定歌曲信息前面**\n");
        printf("**** [4] 将一条歌曲信息插入到某一特定歌曲信息后面**\n");
        printf("**** [5] 按照歌曲名查找歌曲信息****************\n");
        printf("**** [6] 按照作者查找歌曲信息*****************\n");
        printf("**** [7] 按照演唱者查找歌曲信息***************\n");
        printf("**** [8] 按照作者分组显示歌曲信息***************\n");
        printf("**** [9] 删除某一条特定歌曲信息***************\n");
        printf("**** [10] 顺序显示歌曲信息**********************\n");
        printf("**** [11] 逆序显示歌曲信息*********************\n");
        printf("**** [12] 以销量升序排序显示歌曲信息************\n");
        printf("**** [0] 退出系统*****************************\n");
        printf("**********************************************\n");
        printf("输入数字0-12,选择功能\n");
        int opt;
        if (scanf("%d", &opt) != 1)
        {
            printf("输入错误！自动退出系统\n"); 
            fflush(stdin);
        }
        switch (opt)
        {
        case 1:
            ptr = NULL;
            printf("请分别输入 歌曲名字 作者 演唱者 发行日期 销量\n");
            scanf("%s %s %s %s %d", _name, _writer, _singer, _date, &_sales);
            ptr = createNode(_name, _writer, _singer, _date, _sales);
            if (ptr)
            {
                printf("插入成功！\n");
            }
            else
            {
                printf("插入失败！\n");
            }
            if (!writerHasSame(ptr))
            {
                writerInsertAsLast(ptr);
            }
            insertAsFirst(ptr);
            save();
            break;
        case 2:
            ptr = NULL;
            printf("请分别输入 歌曲名字 作者 演唱者 发行日期 销量\n");
            scanf("%s %s %s %s %d", _name, _writer, _singer, _date, &_sales);
            ptr = createNode(_name, _writer, _singer, _date, _sales);
            if (ptr)
            {
                printf("插入成功！\n");
            }
            else
            {
                printf("插入失败！\n");
            }
            if (!writerHasSame(ptr))
            {
                writerInsertAsLast(ptr);
            }
            insertAsLast(ptr);
            save();
            break;
        case 3:
            ptr = NULL;
            printf("请输入该特定歌曲的歌曲名字（默认歌曲信息中，歌曲名字不会重复）:\n");
            scanf("%s", _name);
            fflush(stdin); // 清除输入缓冲区
            ptr = searchByName(_name);
            if (ptr != NULL)
            {
                printf("请分别输入 歌曲名字 作者 演唱者 发行日期 销量\n");
                scanf("%s %s %s %s %d", _name, _writer, _singer, _date, &_sales);
                ListNode *e = createNode(_name, _writer, _singer, _date, _sales);
                if (!writerHasSame(ptr))
                {
                    writerInsertAsLast(e);
                }
                insertB(ptr, e);
                save();
                printf("插入成功！\n");
            }
            else
            {
                printf("该歌曲不存在！\n");
            }
            break;
        case 4:
            ptr = NULL;
            printf("请输入该特定歌曲的歌曲名字（默认歌曲信息中，歌曲名字不会重复）:\n");
            scanf("%s", _name);
            fflush(stdin); // 清除输入缓冲区
            ptr = searchByName(_name);
            if (ptr != NULL)
            {
                printf("请分别输入 歌曲名字 作者 演唱者 发行日期 销量\n");
                scanf("%s %s %s %s %d", _name, _writer, _singer, _date, &_sales);
                ListNode *e = createNode(_name, _writer, _singer, _date, _sales);
                if (!writerHasSame(ptr))
                {
                    writerInsertAsLast(e);
                }
                insertA(ptr, e);
                save();
                printf("插入成功！\n");
            }
            else
            {
                printf("该歌曲不存在！\n");
            }
            break;
        case 5:
            ptr = NULL;
            printf("请输入歌曲名字：\n");
            scanf("%s", _name);
            ptr = searchByName(_name);
            if (ptr != NULL)
            {
                printf("%s %s %s %s %d\n", ptr->name, ptr->writer, ptr->singer, ptr->date, ptr->sales);
            }
            else
            {
                printf("该歌曲不存在！\n");
            }
            break;
        case 6:
            printf("请输入歌曲作者：\n");
            scanf("%s", _writer);
            searchByWriter(_writer);
            break;
        case 7:
            printf("请输入歌曲演唱者：\n");
            scanf("%s", _singer);
            searchBySinger(_singer);
            break;
        case 8:
            printGroupByWriter();
            break;
        case 9:
            ptr = NULL;
            printf("请输入歌曲名字：\n");
            scanf("%s", _name);
            ptr = searchByName(_name);
            if (ptr != NULL)
            {
                Remove(ptr);
                save();
                printf("删除成功！\n");
                if (searchByName(_name) == NULL)
                {
                    writerRemove(ptr);
                }
            }
            else
            {
                printf("该歌曲不存在！\n");
            }
            break;
        case 10:
            if (!empty())
            {
                forwardTraverse();
            }
            else
            {
                printf("数据为空！\n");
            }
            break;
        case 11:
            if (!empty())
            {
                reverseTraverse();
            }
            else
            {
                printf("数据为空！\n");
            }
            break;
        case 12:
            selectSort(header->succ, size());
            save();
            forwardTraverse();
            break;
        case 0:
            printf("成功退出系统！\n");
            return 0;
        default:
            printf("输入错误！\n");
            break;
        }
    }
    return 0;
}