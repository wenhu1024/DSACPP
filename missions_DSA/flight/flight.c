#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 55

/* 该课设使用双向链表,顺序表数据结构实现 */
typedef struct clientNode clientListNode;
typedef struct airNode airListNode;

char _airId[MAX_LENGTH], _city[MAX_LENGTH], _flightNum[MAX_LENGTH], _beginTime[MAX_LENGTH], _endTime[MAX_LENGTH];
double _price;
int _totalSit, _rest; // 全局变量，方便接受输入数据

char _name[MAX_LENGTH], _id[MAX_LENGTH], _choosedNum[MAX_LENGTH];
int _num; // 全局变量，方便接受输入数据

struct clientNode
{
    char name[MAX_LENGTH];       // 客户姓名
    char id[MAX_LENGTH];         // 证件号
    char choosedNum[MAX_LENGTH]; // 航班号
    int num;                     // 订票数
    clientListNode *pred;        // 前驱
    clientListNode *succ;        // 后继
};

struct airNode
{
    char airId[MAX_LENGTH];     // 飞机号
    char city[MAX_LENGTH];      // 飞机抵达的城市
    char flightNum[MAX_LENGTH]; // 航班号
    char beginTime[MAX_LENGTH]; // 起飞时间
    char endTime[MAX_LENGTH];   // 降落时间
    double price;               // 航班票价
    int totalSit;               // 总位置
    int rest;                   // 剩余位置
    airListNode *pred;          // 前驱
    airListNode *succ;          // 后继
};

clientListNode *clientHeader = NULL;  // 头哨兵(全局变量)
clientListNode *clientTrailer = NULL; // 尾哨兵(全局变量)
int clientSize = 0;                   // 客户数据规模(全局变量)

airListNode *airHeader = NULL;  // 头哨兵(全局变量)
airListNode *airTrailer = NULL; // 尾哨兵(全局变量)
int airSize = 0;                // 航班数据规模(全局变量)

void read();                                   // 文件读取
void save();                                   // 文件保存
void init();                                   // 头，尾哨兵分配动态内存
clientListNode *client_createNode();           // 动态分配一个client节点，初始化后返回节点指针
void client_insert(clientListNode *e);         // e插入链表尾部
int canBook(airListNode *p, int n);            // 查看p航班剩余票数，和订票数n比较，判断能否订票，可以订返回1，否则返回0
void subTicket(airListNode *p, int n);         // p节点对应航班，订票n张,rest-n
void addTicket(airListNode *p, int n);         // p航班退票(默认退掉客户所有已定的票),rest+n
void client_remove(clientListNode *e);         // 移除e节点
airListNode *air_searchByAirFlightNum(char *); // 按照航班号查询(唯一)，返回节点指针
int air_searchByCity(char *);                  // 按照飞机抵达城市查询,直接打印输出，并返回符合条件的航班个数
int air_searchByBeginTime(char *);             // 按照起飞时间查询，直接打印输出，并返回符合条件的航班个数
int air_searchByEndTime(char *);               // 按照降落时间查询，直接打印输出，并返回符合条件的航班个数
clientListNode *client_searchById(char *);     // 按照客户证件号查询(默认一个客户id只能定一个航班的票)
void client_print(clientListNode *p);          // 打印p节点客户信息
void air_print(airListNode *p);                // 打印p节点航班信息
void air_print_traverse();                     // 遍历输出所有航班信息

void init()
{
    clientHeader = (clientListNode *)malloc(sizeof(clientListNode));
    clientTrailer = (clientListNode *)malloc(sizeof(clientListNode));
    clientHeader->pred = NULL;
    clientHeader->succ = clientTrailer;
    clientTrailer->pred = clientHeader;
    clientTrailer->succ = NULL;

    airHeader = (airListNode *)malloc(sizeof(airListNode));
    airTrailer = (airListNode *)malloc(sizeof(airListNode));
    airHeader->pred = NULL;
    airHeader->succ = airTrailer;
    airTrailer->pred = airHeader;
    airTrailer->succ = NULL;
}

airListNode *air_createNode(char _airId[MAX_LENGTH], char _city[MAX_LENGTH], char _flightNum[MAX_LENGTH], char _beginTime[MAX_LENGTH], char _endTime[MAX_LENGTH], double _price, int _totalSit, int _rest)
{
    airListNode *newNode = (airListNode *)malloc(sizeof(airListNode));
    strcpy(newNode->airId, _airId);
    strcpy(newNode->city, _city);
    strcpy(newNode->flightNum, _flightNum);
    strcpy(newNode->beginTime, _beginTime);
    strcpy(newNode->endTime, _endTime);
    newNode->price = _price;
    newNode->totalSit = _totalSit;
    newNode->rest = _rest;
    newNode->pred = NULL;
    newNode->succ = NULL;
    return newNode;
}

void air_insert(airListNode *e)
{
    airSize++; // 链表数据规模加1
    e->pred = airTrailer->pred;
    e->succ = airTrailer;
    airTrailer->pred->succ = e;
    airTrailer->pred = e;
}

clientListNode *client_createNode(char *_name, char *_id, char *_choosedNum, int _num)
{
    clientListNode *newNode = (clientListNode *)malloc(sizeof(clientListNode));
    strcpy(newNode->name, _name);
    strcpy(newNode->id, _id);
    strcpy(newNode->choosedNum, _choosedNum);
    newNode->num = _num;
    newNode->pred = NULL;
    newNode->succ = NULL;

    return newNode;
}

void client_insert(clientListNode *e)
{
    clientSize++; // 链表数据规模加1
    e->pred = clientTrailer->pred;
    e->succ = clientTrailer;
    clientTrailer->pred->succ = e;
    clientTrailer->pred = e;
}

void air_print(airListNode *p)
{
    printf("飞机号：%s\t", p->airId);
    printf("飞机抵达的城市：%s\t", p->city);
    printf("航班号：%s\t", p->flightNum);
    printf("起飞时间：%s\t", p->beginTime);
    printf("降落时间：%s\t", p->endTime);
    printf("航班票价：%lf\t", p->price);
    printf("总位置：%d\t", p->totalSit);
    printf("剩余位置：%d\t", p->rest);
    printf("\n");
}

int air_searchByCity(char *_city)
{
    int n = 0;
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        if (strcmp(p->city, _city) == 0)
        {
            n++;
            air_print(p);
        }
    }
    return n;
}

int air_searchByBeginTime(char *_beginTime)
{
    int n = 0;
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        if (strcmp(p->beginTime, _beginTime) == 0)
        {
            n++;
            air_print(p);
        }
    }
    return n;
}

int air_searchByEndTime(char *_endTime)
{
    int n = 0;
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        if (strcmp(p->endTime, _endTime) == 0)
        {
            n++;
            air_print(p);
        }
    }
    return n;
}

airListNode *air_searchByAirFlightNum(char *_flightNum)
{
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        if (strcmp(p->flightNum, _flightNum) == 0)
        {
            return p;
        }
    }
    return NULL;
}

void air_print_traverse()
{
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        air_print(p);
    }
}

void client_print(clientListNode *p)
{
    printf("客户姓名：%s\t", p->name);
    printf("证件号：%s\t", p->id);
    printf("航班号：%s\t", p->choosedNum);
    printf("订票数：%d\t", p->num);
    printf("\n");
}

clientListNode *client_searchById(char *_id)
{
    for (clientListNode *p = clientHeader->succ; p != clientTrailer; p = p->succ)
    {
        if (strcmp(p->id, _id) == 0)
        {
            return p;
        }
    }
    return NULL;
}

void client_remove(clientListNode *e)
{
    e->pred->succ = e->succ;
    e->succ->pred = e->pred;
    free(e);
    clientSize--; // 数据规模减1
}

int canBook(airListNode *p, int n)
{
    if (p->rest >= n)
    {
        return 1;
    }
    return 0;
}

void subTicket(airListNode *p, int n)
{
    p->rest -= n;
}

void addTicket(airListNode *p, int n)
{
    p->rest += n;
}

void read()
{
    int num = 0;
    FILE *fp = fopen("flight.txt", "r");
    if (!fp)
    {
        printf("文件读取失败！");
        return;
    }

    fscanf(fp, "数据条数为%d", &num); // 获得文件数据总数
    for (int i = 0; i < num; ++i)
    {
        fscanf(fp, "%s %s %s %s %s %lf %d %d\n", _airId, _city, _flightNum, _beginTime, _endTime, &_price, &_totalSit, &_rest);
        airListNode *p = air_createNode(_airId, _city, _flightNum, _beginTime, _endTime, _price, _totalSit, _rest);
        air_insert(p);
    }
    fclose(fp);

    fp = fopen("client.txt", "r");
    if (!fp)
    {
        printf("文件读取失败！");
        return;
    }

    fscanf(fp, "数据条数为%d", &num); // 获得文件数据总数
    for (int i = 0; i < num; ++i)
    {
        fscanf(fp, "%s %s %s %d", _name, _id, _choosedNum, &_num);
        clientListNode *p = client_createNode(_name, _id, _choosedNum, _num);
        client_insert(p);
    }
    fclose(fp);
}

void save()
{
    FILE *fp = fopen("flight.txt", "w"); // w:写模式
    if (!fp)
    {
        printf("文件保存失败！");
        return;
    }

    fprintf(fp, "数据条数为%d\n", airSize); // 将数据条数保存至文件
    for (airListNode *p = airHeader->succ; p != airTrailer; p = p->succ)
    {
        fprintf(fp, "%s %s %s %s %s %lf %d %d\n", p->airId, p->city, p->flightNum, p->beginTime, p->endTime, p->price, p->totalSit, p->rest);
    }
    fclose(fp);

    fp = fopen("client.txt", "w"); // w:写模式
    if (!fp)
    {
        printf("文件保存失败！");
        return;
    }

    fprintf(fp, "数据条数为%d\n", clientSize); // 将数据条数保存至文件
    for (clientListNode *p = clientHeader->succ; p != clientTrailer; p = p->succ)
    {
        fprintf(fp, "%s %s %s %d\n", p->name, p->id, p->choosedNum, p->num);
    }
    fclose(fp);
}
int main()
{
    init();
    read();

    airListNode *airP = NULL;
    clientListNode *clientP = NULL;

    while (1)
    {
        printf("************欢迎使用航空订票系统******************* \n");
        printf("************功能清单如下**************** \n");
        printf("*** [1]查询所有航班信息 **************** \n");
        printf("*** [2]根据航班号查询航班信息 *********** \n");
        printf("*** [3]根据抵达城市查询航班信息 ********* \n");
        printf("*** [4]根据起飞时间查询航班信息 ********* \n");
        printf("*** [5]根据降落时间查询航班信息 ********* \n");
        printf("*** [6]客户订票 *********************** \n");
        printf("*** [7]客户退票 *********************** \n");
        printf("*** [8]客户根据客户证件号查询预定信息 *** \n");
        printf("*** [0]退出学生系统 ******************* \n");
        printf("************************************************** \n");

        printf("输入数字0-8,选择功能\n");
        int opt;
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            if (airSize == 0)
            {
                printf("航班信息为空!\n");
            }
            else
            {
                air_print_traverse();
            }
            break;
        case 2:
            airP = NULL;
            printf("请输入航班号！\n");
            scanf("%s", _flightNum);
            fflush(stdin); //  清除输入缓冲区
            airP = air_searchByAirFlightNum(_flightNum);
            if (!airP)
            {
                printf("航班不存在！\n");
            }
            else
            {
                air_print(airP);
            }
            break;
        case 3:
            printf("请输入抵达城市！\n");
            scanf("%s", _city);
            fflush(stdin); //  清除输入缓冲区
            if (air_searchByCity(_city) == 0)
            {
                printf("航班不存在！\n");
            }
            break;
        case 4:
            printf("请输入起飞时间(格式举例 08:02)！\n");
            scanf("%s", _beginTime);
            fflush(stdin); //  清除输入缓冲区
            if (air_searchByBeginTime(_beginTime) == 0)
            {
                printf("航班不存在！\n");
            }
            break;
        case 5:
            printf("请输入降落时间(格式举例 08:02)！\n");
            scanf("%s", _endTime);
            fflush(stdin); //  清除输入缓冲区
            if (air_searchByEndTime(_endTime) == 0)
            {
                printf("航班不存在！\n");
            }
            break;
        case 6:
            airP = NULL;
            printf("请输入航班号！\n");
            scanf("%s", _flightNum);
            fflush(stdin); //  清除输入缓冲区
            airP = air_searchByAirFlightNum(_flightNum);
            if (!airP)
            {
                printf("航班不存在！\n");
            }
            else
            {
                printf("请分别输入 姓名 证件号 订票数\n");
                scanf("%s %s %d", _name, _id, &_num);
                if (canBook(airP, _num))
                {
                    strcpy(_choosedNum, _flightNum);
                    client_insert(client_createNode(_name, _id, _choosedNum, _num));
                    subTicket(airP, _num);
                    printf("订票成功！\n");
                    save();
                }
                else
                {
                    printf("余票不足，订票失败！\n");
                }
            }
            break;
        case 7:
            airP = NULL;
            clientP = NULL;
            printf("请输入航班号！\n");
            scanf("%s", _flightNum);
            fflush(stdin); //  清除输入缓冲区
            airP = air_searchByAirFlightNum(_flightNum);
            if (!airP)
            {
                printf("航班不存在！\n");
            }
            else
            {
                printf("请输入证件号\n");
                scanf("%s", _id);
                clientP = client_searchById(_id);
                if ((!clientP) || (strcmp(clientP->choosedNum, _flightNum)))
                {
                    printf("无此购票记录，退票失败！\n");
                }
                else
                {
                    addTicket(airP, clientP->num);
                    client_remove(clientP);
                    printf("退票成功!\n");
                    save();
                }
            }
            break;
        case 8:
            clientP = NULL;
            printf("请输入证件号\n");
            scanf("%s", _id);
            clientP = client_searchById(_id);
            if (!clientP)
            {
                printf("不存在该客户！\n");
            }
            else
            {
                client_print(clientP);
            }
            break;
        case 0:
            printf("成功退出系统!\n");
            return 0;
        default:
            printf("无效的输入！\n");
            break;
        }
    }
    return 0;
}