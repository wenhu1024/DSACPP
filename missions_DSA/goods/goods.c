#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 60 // 设置数组最大长度

char _id[MAX], _name[MAX], _date[MAX], _manufacturer[MAX];
int _num;
double _price; // 全局变量，接受输入数据

/* 本课设使用双向链表实现 */

typedef struct node GoodsNode;

struct node
{
    char id[MAX];           // 商品id
    char name[MAX];         // 商品名
    char date[MAX];         // 生产日期
    char manufacturer[MAX]; // 生产商
    int num;                // 商品数目
    double price;           // 单价
    GoodsNode *pred;        // 前驱
    GoodsNode *succ;        // 后继
};

int _size;                 // 全局变量，记录数据规模
GoodsNode *header = NULL;  // 双向链表头哨兵(全局变量)
GoodsNode *trailer = NULL; // 双向链表尾哨兵(全局变量)

GoodsNode *createNode(char *, char *, char *, char *, int, double); // 动态分配节点内存并初始化，返回节点指针
void insertAsLast(GoodsNode *e);                                    // 将e节点插入链表尾部
GoodsNode *searchById(char *);                                      // 按照id查找，找到返回节点指针，否则返回空指针
void addNum(GoodsNode *p, int _num);                                // 增加商品p数目num
int canSub(GoodsNode *p, int _num);                                 // 判断商品p能否减少数目num，可以返回1，否则返回0
void subNum(GoodsNode *p, int _num);                                // 减少商品p数目num
int isEmpty(GoodsNode *p);                                          // 检验商品数目是否为0，是的话返回1，否则返回0
void Remove(GoodsNode *p);                                          // 移除节点p
void print(GoodsNode *p);                                           // 打印商品信息
void forwardTraverse();                                             // 顺序打印商品信息
void reverseTraverse();                                             // 逆序打印商品信息
void save();                                                        // 文件保存
void read();                                                        // 文件读取
void initialize();                                                  // 空链表初始化

void initialize()
{
    header = (GoodsNode *)malloc(sizeof(GoodsNode));
    trailer = (GoodsNode *)malloc(sizeof(GoodsNode));
    header->pred = NULL;
    header->succ = trailer;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0;
}

GoodsNode *createNode(char *_id, char *_name, char *_date, char *_manufacturer, int _num, double _price)
{
    GoodsNode *newNode = (GoodsNode *)malloc(sizeof(GoodsNode));
    strcpy(newNode->id, _id);
    strcpy(newNode->name, _name);
    strcpy(newNode->date, _date);
    strcpy(newNode->manufacturer, _manufacturer);
    newNode->num = _num;
    newNode->price = _price;
    newNode->pred = NULL;
    newNode->succ = NULL;
    return newNode;
}

void insertAsLast(GoodsNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = trailer->pred;
    e->succ = trailer;
    trailer->pred->succ = e;
    trailer->pred = e;
}

void Remove(GoodsNode *p)
{
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    free(p);
    _size--; // 数据规模减1
}

GoodsNode *searchById(char *_id)
{
    for (GoodsNode *p = header->succ; p != trailer; p = p->succ)
    {
        if (strcmp(p->id, _id) == 0)
        {
            return p;
        }
    }
    return NULL;
}
void print(GoodsNode *p)
{
    printf("ID: %s\t", p->id);
    printf("名称: %s\t", p->name);
    printf("日期: %s\t", p->date);
    printf("生产商: %s\t", p->manufacturer);
    printf("数量: %d\t", p->num);
    printf("价格: %lf\t", p->price);
    printf("\n");
}

void forwardTraverse()
{
    for (GoodsNode *p = header->succ; p != trailer; p = p->succ)
    {
        print(p);
    }
}

void reverseTraverse()
{
    for (GoodsNode *p = trailer->pred; p != header; p = p->pred)
    {
        print(p);
    }
}

void addNum(GoodsNode *p, int _num)
{
    p->num += _num;
}

void subNum(GoodsNode *p, int _num)
{
    p->num -= _num;
}

int canSub(GoodsNode *p, int _num)
{
    if (p->num >= _num)
    {
        return 1;
    }
    return 0;
}

int isEmpty(GoodsNode *p)
{
    if (p->num == 0)
    {
        return 1;
    }
    return 0;
}

void read()
{
    int n;
    FILE *fp = fopen("goods.txt", "r");
    fscanf(fp, "数据条数为%d\n", &n);

    for (int i = 0; i < n; ++i)
    {
        fscanf(fp, "%s %s %s %s %d %lf\n", _id, _name, _date, _manufacturer, &_num, &_price);
        GoodsNode *p = createNode(_id, _name, _date, _manufacturer, _num, _price);
        insertAsLast(p);
    }
    fclose(fp);
}

void save()
{
    int n = _size;

    FILE *fp = fopen("goods.txt", "w");
    fprintf(fp, "数据条数为%d\n", n);
    for (GoodsNode *p = header->succ; p != trailer; p = p->succ)
    {
        fprintf(fp, "%s %s %s %s %d %lf\n", p->id, p->name, p->date, p->manufacturer, p->num, p->price);
    }
    fclose(fp);
}

int main()
{
    initialize();
    read();

    GoodsNode *p = NULL;

    while (1)
    {
        printf("************欢迎使用商品信息管理系统******************* \n");
        printf("************功能如下所示****************************** \n");
        printf("*** [1]顺序查询所有商品信息 *************************** \n");
        printf("*** [2]逆序查询所有商品信息**************************** \n");
        printf("*** [3]根据商品id查询商品信息 ************************* \n");
        printf("*** [4]商品表插入商品信息 ***************************** \n");
        printf("*** [5]增加商品表中某一商品数目(入库) ****************** \n");
        printf("*** [6]减少商品表中某一商品数目(出库) ****************** \n");
        printf("*** [7]商品表删除商品信息 ***************************** \n");
        printf("*** [0]退出系统 ************************************** \n");
        printf("***************************************************** \n");

        printf("输入数字0-7,选择功能\n");
        int t;
        scanf("%d", &t);
        switch (t)
        {
        case 0:
            printf("退出系统成功！\n");
            return 0;
        case 1:
            if (_size == 0)
            {
                printf("商品信息为空！\n");
                break;
            }
            forwardTraverse();
            break;
        case 2:
            if (_size == 0)
            {
                printf("商品信息为空！\n");
                break;
            }

            reverseTraverse();
            break;
        case 3:
            p = NULL;
            printf("请输入商品id！\n");
            scanf("%s", _id);
            p = searchById(_id);
            if (p)
            {
                print(p);
            }
            else
            {
                printf("商品不存在！\n");
            }
            break;
        case 4:
            p = NULL;
            printf("请分别输入 商品id 商品名 生产日期 生产商 商品数目 单价\n");
            scanf("%s %s %s %s %d %lf", _id, _name, _date, _manufacturer, &_num, &_price);
            p = searchById(_id);
            if (p)
            {
                printf("商品表已存在相同商品id，插入商品信息失败！\n");
            }
            else
            {
                p = createNode(_id, _name, _date, _manufacturer, _num, _price);
                insertAsLast(p);
                save();
                printf("插入商品信息成功！\n");
            }
            break;
        case 5:
            p = NULL;
            printf("请分别输入 商品id 增加数目！\n");
            scanf("%s %d", _id, &_num);
            p = searchById(_id);
            if (p)
            {
                addNum(p, _num);
                printf("入库成功！\n");
                save();
            }
            else
            {
                printf("商品不存在,入库失败！\n");
            }
            break;
        case 6:
            p = NULL;
            printf("请分别输入 商品id 减少数目！\n");
            scanf("%s %d", _id, &_num);
            p = searchById(_id);
            if (p)
            {
                if (canSub(p, _num))
                {
                    subNum(p, _num);
                    printf("出库成功！\n");

                    if (isEmpty(p))
                    {
                        Remove(p);
                    }
                    save();
                }
                else
                {
                    printf("出库数目超过商品现有数目，出库失败！\n");
                }
            }
            else
            {
                printf("商品不存在,出库失败！\n");
            }
            break;
        case 7:
            p = NULL;
            printf("请输入商品id！\n");
            scanf("%s", _id);
            p = searchById(_id);
            if (p)
            {
                Remove(p);
                printf("删除商品信息成功！\n");
                save();
            }
            else
            {
                printf("商品不存在！\n");
            }
            break;
        default:
            printf("无效的选择!\n");
            break;
        }
    }
    return 0;
}
