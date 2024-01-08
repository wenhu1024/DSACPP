#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 60 // 设置字符串最大长度

int _id;
char _name[MAX];
float _chinese, _maths, _english;

typedef struct node studentNode;

struct node
{
    int id;            // 学号
    char name[MAX];    // 姓名
    float chinese;     // 语文成绩
    float maths;       // 数学成绩
    float english;     // 英语成绩
    float total;       // 总成绩
    float ave;         // 平均成绩
    int totalRank;     // 总分排名
    int chineseRank;   // 语文排名
    int mathsRank;     // 数学排名
    int englishRank;   // 英语排名
    studentNode *pred; // 前驱
    studentNode *succ; // 后继
};

studentNode *header = NULL;  // 头哨兵
studentNode *trailer = NULL; // 尾哨兵
int _size;                   // 记录数据规模

void read();                                                   // 从文件读取数据
void save();                                                   // 保存数据至文件
void initList();                                               // 创建空链表
studentNode *createNode(int, char *, float, float, float);     // 动态分配节点
void insertB(studentNode *p, studentNode *e);                  // e作为p的前驱插入
void insertAsLast(studentNode *p);                             // 添加节点p到链表末端
void display(studentNode *p);                                  // 显示p节点信息
void modify(studentNode *p, int, char *, float, float, float); // 修改p节点信息
studentNode Remove(studentNode *p);                            // 移除p节点，返回备份
studentNode *searchById(int);                                  // 按照id查找节点，返回节点指针
studentNode *id_selexMAX(studentNode *p, int n);               // 从起始位置p的n个节点中选出id最大者,若有2个相同最大节点，返回靠后的一个节点
studentNode *total_selexMAX(studentNode *p, int n);            // 从起始位置p的n个节点中选出总分最大者,若有2个相同最大节点，返回靠后的一个节点
studentNode *chinese_selexMAX(studentNode *p, int n);          // 从起始位置p的n个节点中选出语文分数最大者,若有2个相同最大节点，返回靠后的一个节点
studentNode *maths_selectMAX(studentNode *p, int n);           // 从起始位置p的n个节点中选出数学分数最大者,若有2个相同最大节点，返回靠后的一个节点
studentNode *english_selectMAX(studentNode *p, int n);         // 从起始位置p的n个节点中选出英语分数最大者,若有2个相同最大节点，返回靠后的一个节点
void id_selectSort(studentNode *p, int n);                     // 选择排序,对起始于位置p的n个节点的id升序排序
void total_selexSort(studentNode *p, int n);                   // 选择排序,对起始于位置p的n个节点的总分升序排序
void chinese_selexSort(studentNode *p, int n);                 // 选择排序,对起始于位置p的n个节点的语文分数升序排序
void english_selexSort(studentNode *p, int n);                 // 选择排序,对起始于位置p的n个节点的英语分数升序排序
void maths_selectSort(studentNode *p, int n);                  // 选择排序,对起始于位置p的n个节点的数学分数升序排序
void setTotalRank();                                           // 为每一节点设置总分排名
void setChineseRank();                                         // 为每一节点设置语文排名
void setMathsRank();                                           // 为每一节点设置数学排名
void setEnglishRank();                                         // 为每一节点设置英语排名
void freshAllRank();                                           // 刷新每一个节点的总分排名，语文排名，数学排名，英语排名
void freshTotalAndAve();                                       // 刷新每一个节点的总分，平均分
void read();                                                   // 读取文件
void save();                                                   // 保存文件
void initList()
{
    header = (studentNode *)malloc(sizeof(studentNode));
    trailer = (studentNode *)malloc(sizeof(studentNode));
    header->pred = NULL;
    header->succ = trailer;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0;
}

studentNode *createNode(int _id, char *_name, float _chinese, float _maths, float _english)
{
    studentNode *newNode = (studentNode *)malloc(sizeof(studentNode));
    newNode->id = _id;
    strcpy(newNode->name, _name);
    newNode->chinese = _chinese;
    newNode->maths = _maths;
    newNode->english = _english;
    newNode->pred = NULL;
    newNode->succ = NULL;
    return newNode;
}

void insertB(studentNode *p, studentNode *e)
{
    _size++; // 链表数据规模加1
    e->pred = p->pred;
    e->succ = p;
    p->pred->succ = e;
    p->pred = e;
}

void insertAsLast(studentNode *p)
{
    _size++; // 链表数据规模加1
    p->pred = trailer->pred;
    p->succ = trailer;
    trailer->pred->succ = p;
    trailer->pred = p;
}

void modify(studentNode *p, int _id, char *_name, float _chinese, float _maths, float _english)
{
    p->id = _id;
    strcpy(p->name, _name);
    p->chinese = _chinese;
    p->maths = _maths;
    p->english = _english;
}

studentNode *searchById(int targetId)
{
    for (studentNode *currentNode = header->succ; currentNode != trailer; currentNode = currentNode->succ)
    {
        if (currentNode->id == targetId)
        {
            return currentNode; // 找到匹配的id，返回节点指针
        }
    }
    return NULL; // 遍历完整个链表都没有找到匹配的id，返回空指针
}

studentNode *id_selectMax(studentNode *p, int n)
{
    studentNode *maxNode = p; // 暂定id最大的对应节点为p
    for (studentNode *cur = p; n > 1; n--)
    {
        if ((cur = cur->succ)->id > maxNode->id)
        {
            maxNode = cur;
        }
    }
    return maxNode;
}

studentNode *total_selectMax(studentNode *p, int n)
{
    studentNode *maxNode = p; // 暂定总分最大的对应节点为p
    for (studentNode *cur = p; n > 1; n--)
    {
        if ((cur = cur->succ)->total > maxNode->total)
        {
            maxNode = cur;
        }
    }
    return maxNode;
}
studentNode *chinese_selexMAX(studentNode *p, int n)
{
    studentNode *maxNode = p; // 暂定总分最大的对应节点为p
    for (studentNode *cur = p; n > 1; n--)
    {
        if ((cur = cur->succ)->chinese > maxNode->chinese)
        {
            maxNode = cur;
        }
    }
    return maxNode;
}
studentNode *english_selectMAX(studentNode *p, int n)
{
    studentNode *maxNode = p; // 暂定总分最大的对应节点为p
    for (studentNode *cur = p; n > 1; n--)
    {
        if ((cur = cur->succ)->english > maxNode->english)
        {
            maxNode = cur;
        }
    }
    return maxNode;
}

studentNode *maths_selectMAX(studentNode *p, int n)
{
    studentNode *maxNode = p; // 暂定数学分数最大的对应节点为p

    for (studentNode *cur = p; n > 1; n--)
    {
        if ((cur = cur->succ)->maths > maxNode->maths)
        {
            maxNode = p;
        }
    }
    return maxNode;
}

studentNode Remove(studentNode *p)
{
    studentNode backup = *p; // 备份待删除节点的数值

    p->pred->succ = p->succ;
    p->succ->pred = p->pred;

    free(p); // 释放动态内存
    _size--; // 数据规模减1

    return backup;
}

void id_selectSort(studentNode *p, int n)
{
    studentNode *head = p->pred;
    studentNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {
        studentNode *maxNode = id_selectMax(head->succ, n);                                           // 找出区间中id最大者
        studentNode copy = Remove(maxNode);                                                           // 备份待删除节点的数值
        studentNode *select = createNode(copy.id, copy.name, copy.chinese, copy.maths, copy.english); // 被选中的最大节点
        insertB(tail, select);                                                                        // 被选中的最大节点作为tail的前驱插入
        tail = tail->pred;                                                                            // tail前移
        n--;                                                                                          // 待排序区间减一
    }
}

void total_selectSort(studentNode *p, int n)
{
    studentNode *head = p->pred;
    studentNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {
        studentNode *maxNode = total_selectMax(head->succ, n);                                        // 找出区间中总分最大者
        studentNode copy = Remove(maxNode);                                                           // 备份待删除节点的数值
        studentNode *select = createNode(copy.id, copy.name, copy.chinese, copy.maths, copy.english); // 被选中的最大节点
        insertB(tail, select);                                                                        // 被选中的最大节点作为tail的前驱插入
        tail = tail->pred;                                                                            // tail前移
        n--;                                                                                          // 待排序区间减一
    }
}

void chinese_selexSort(studentNode *p, int n)
{
    studentNode *head = p->pred;
    studentNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {
        studentNode *maxNode = chinese_selexMAX(head->succ, n);                                       // 找出区间中数学分数最大者
        studentNode copy = Remove(maxNode);                                                           // 备份待删除节点的数值
        studentNode *select = createNode(copy.id, copy.name, copy.chinese, copy.maths, copy.english); // 被选中的最大节点
        insertB(tail, select);                                                                        // 被选中的最大节点作为tail的前驱插入
        tail = tail->pred;                                                                            // tail前移
        n--;                                                                                          // 待排序区间减一
    }
}

void english_selexSort(studentNode *p, int n)
{
    studentNode *head = p->pred;
    studentNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {
        studentNode *maxNode = english_selectMAX(head->succ, n);                                      // 找出区间中数学分数最大者
        studentNode copy = Remove(maxNode);                                                           // 备份待删除节点的数值
        studentNode *select = createNode(copy.id, copy.name, copy.chinese, copy.maths, copy.english); // 被选中的最大节点
        insertB(tail, select);                                                                        // 被选中的最大节点作为tail的前驱插入
        tail = tail->pred;                                                                            // tail前移
        n--;                                                                                          // 待排序区间减一
    }
}

void maths_selectSort(studentNode *p, int n)
{
    studentNode *head = p->pred;
    studentNode *tail = p;
    for (int i = 0; i < n; ++i)
        tail = tail->succ; // 待排序区间(head,tail)
    while (n > 1)
    {
        studentNode *maxNode = maths_selectMAX(head->succ, n);                                        // 找出区间中数学分数最大者
        studentNode copy = Remove(maxNode);                                                           // 备份待删除节点的数值
        studentNode *select = createNode(copy.id, copy.name, copy.chinese, copy.maths, copy.english); // 被选中的最大节点
        insertB(tail, select);                                                                        // 被选中的最大节点作为tail的前驱插入
        tail = tail->pred;                                                                            // tail前移
        n--;                                                                                          // 待排序区间减一
    }
}

void setTotalRank()
{ // 前提:节点已经按照总分升序排序
    int i = _size;
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        p->totalRank = i--;
    }
}

void setMathsRank()
{ // 前提:节点已经按照数学分升序排序
    int i = _size;
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        p->mathsRank = i--;
    }
}

void setChineseRank()
{ // 前提:节点已经按照语文分升序排序
    int i = _size;
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        p->chineseRank = i--;
    }
}

void setEnglishRank()
{ // 前提:节点已经按照英语分升序排序
    int i = _size;
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        p->englishRank = i--;
    }
}

void display(studentNode *p)
{
    printf("学号：%d\t姓名：%s\t语文成绩：%.2f\t数学成绩：%.2f\t英语成绩：%.2f\t总成绩：%.2f\t平均成绩：%.2f\t总分排名：%d\t语文排名：%d\t数学排名：%d\t英语排名：%d\n",
           p->id, p->name, p->chinese, p->maths, p->english, p->total, p->ave, p->totalRank, p->chineseRank, p->mathsRank, p->englishRank);
}

void freshTotalAndAve()
{
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        p->total = p->chinese + p->maths + p->english;
        p->ave = p->total / 3.0;
    }
}

void freshAllRank()
{
    total_selectSort(header->succ, _size);
    setTotalRank();

    chinese_selexSort(header->succ, _size);
    setChineseRank();

    maths_selectSort(header->succ, _size);
    setMathsRank();

    english_selexSort(header->succ, _size);
    setEnglishRank();
}

void read()
{
    int num = 0;
    FILE *fp = fopen("information.txt", "r"); // 打开文件以读取数据
    if (!fp)
    {
        printf("文件读取失败！");
        return;
    }

    fscanf(fp, "数据条数为%d", &num); // 获取数据总数
    for (int i = 0; i < num; ++i)
    {
        // 读取信息
        int id;
        char name[MAX];
        float chinese, maths, english;
        float total, ave;
        int totalRank, chineseRank, mathsRank, englishRank;
        fscanf(fp, "%d %s %f %f %f\n", &_id, _name, &_chinese, &_maths, &_english);

        // 创建节点
        studentNode *p = createNode(_id, _name, _chinese, _maths, _english);

        // 将节点加至链表尾端
        insertAsLast(p);
    }
    fclose(fp);
}

void save()
{
    FILE *fp1 = fopen("information.txt", "w"); // w:写模式
    FILE *fp2 = fopen("result.txt", "w");      // w:写模式

    fprintf(fp1, "数据条数为%d\n", _size);                                                      // 将数据条数保存至文件
    fprintf(fp2, "数据条数为%d\n", _size);                                                      // 将数据条数保存至文件
    fprintf(fp2, "学号 姓名 语文 数学 英语 总分 平均分 总分排名 语文排名 数学排名 英语排名\n"); // 表头信息
    for (studentNode *p = header->succ; p != trailer; p = p->succ)
    {
        fprintf(fp1, "%d %s %f %f %f\n", p->id, p->name, p->chinese, p->maths, p->english);
        fprintf(fp2, "%d %s %f %f %f %f %f %d %d %d %d\n", p->id, p->name, p->chinese, p->maths, p->english, p->total, p->ave, p->totalRank, p->chineseRank, p->mathsRank, p->englishRank);
    }
    fclose(fp1);
    fclose(fp2);
}

int main()
{
    initList();
    read();
    freshTotalAndAve();
    freshAllRank();

    studentNode *p = NULL;
    while (1)
    {
        printf("***********欢迎使用学生成绩管理系统*********\n");
        printf("*********功能如下所示**********************\n");
        printf("**** [1] 增加一位学生成绩信息***************\n");
        printf("**** [2] 删除一位学生成绩信息***************\n");
        printf("**** [3] 修改一位学生成绩信息***************\n");
        printf("**** [4] 查找一位学生成绩信息***************\n");
        printf("**** [5] 统计学生语文排名*******************\n");
        printf("**** [6] 统计学生数学排名*******************\n");
        printf("**** [7] 统计学生英语排名*******************\n");
        printf("**** [8] 统计学生总分排名*******************\n");
        printf("**** [9] 按学号升序输出所有学生成绩信息******\n");
        printf("**** [0] 退出系统**************************\n");
        printf("*******************************************\n");
        printf("输入数字0-9,选择功能\n");
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
        case 0:
            printf("成功退出系统\n");
            return 0;
        case 1:
            p = NULL;
            printf("请分别输入 学号 姓名 语文成绩 数学成绩 英语成绩！\n");
            scanf("%d %s %f %f %f", &_id, _name, &_chinese, &_maths, &_english);
            if (searchById(_id))
            {
                printf("添加失败！已存在该学号！\n");
                break;
            }
            else
            {
                p = createNode(_id, _name, _chinese, _maths, _english);
                insertAsLast(p);
                printf("添加成功！\n");
                freshTotalAndAve();
                freshAllRank();
                save();
                break;
            }
        case 2:
            p = NULL;
            printf("请输入要删除的学生学号！\n");
            scanf("%d", &_id);
            p = searchById(_id);
            if (p)
            {
                Remove(p);
                printf("删除成功！\n");
                freshTotalAndAve();
                freshAllRank();
                save();
            }
            else
            {
                printf("不存在该学生信息！删除失败！\n");
            }
            break;
        case 3:
            p = NULL;
            printf("请输入要修改的学生学号！\n");
            scanf("%d", &_id);
            p = searchById(_id);
            if (p)
            {
                printf("请分别输入修改后的 学生学号 姓名 语文成绩 数学成绩 英语成绩！\n");
                scanf("%d %s %f %f %f", &_id, _name, &_chinese, &_maths, &_english);
                modify(p, _id, _name, _chinese, _maths, _english);
                printf("修改成功！\n");
                freshTotalAndAve();
                freshAllRank();
                save();
            }
            else
            {
                printf("不存在该学生信息！修改失败！\n");
            }
            break;
        case 4:
            p = NULL;
            printf("请输入要查询的学生学号！\n");
            scanf("%d", &_id);
            p = searchById(_id);
            if (p)
            {
                display(p);
            }
            else
            {
                printf("不存在该学生信息！\n");
            }
            break;
        case 5:
            chinese_selexSort(header->succ, _size);
            for (studentNode *p = trailer->pred; p != header; p = p->pred)
            {
                display(p);
            }
            save();
            break;
        case 6:
            maths_selectSort(header->succ, _size);
            for (studentNode *p = trailer->pred; p != header; p = p->pred)
            {
                display(p);
            }
            save();
            break;
        case 7:
            english_selexSort(header->succ, _size);
            for (studentNode *p = trailer->pred; p != header; p = p->pred)
            {
                display(p);
            }
            save();
            break;
        case 8:
            total_selectSort(header->succ, _size);
            for (studentNode *p = trailer->pred; p != header; p = p->pred)
            {
                display(p);
            }
            save();
            break;
        case 9:
            id_selectSort(header->succ, _size);
            for (studentNode *p = header->succ; p != trailer; p = p->succ)
            {
                display(p);
            }
            save();
            break;
        default:
            printf("无效的选择!\n");
            break;
        }
    }
    return 0;
}