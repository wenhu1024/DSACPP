#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_stu 2000 // 设置学生最大人数
#define MAX 100      // 设置字符串最大长度
#define MAX_score 4  // 设置分数数组的数目

char _name[MAX];
int _id;
float _score[MAX_score]; // 全局变量

/* 本课设使用结构体数组实现 */

struct stu
{
    char name[MAX];         // 姓名
    int id;                 // 学号
    float score[MAX_score]; // 下标0-3，分别表示学科1成绩，学科2成绩，学科3成绩，总成绩
} stu[MAX_stu];             // 创建保存学生成绩的结构体数组,全局变量

int _size = 0; // 记录数据规模(全局变量)

void Insert(int n);      // 增加n位学生成绩信息
void Remove(int index);  // 删除下标index对应的学生成绩信息
void Modify(int i);      // 修改坐标为i的学生成绩信息
int searchById(int _id); // 按照id查找学生，返回数组对应下标,找不到返回-1
void Sort(int n);        // 根据n，按照多种方式对学生成绩信息排序
void print(int index);   // 根据坐标打印对应学生成绩信息
void menu();             // 菜单
void traverse();         // 遍历打印
float getAve(int n);     // 根据n，求本班某一科平均分或总分的平均分
int getPeople(int n,float _min,float _max); //分数段查询，n表示查询方式，_min,_max 分别表示查询区间

void Insert(int n)
{
    int beginIndex = _size; // 插入的起始下标
    printf("请输入学生信息%d条（姓名 学号 学科1成绩 学科2成绩 学科3成绩），用空格分隔：\n",n);

    for (int i = 0; i < n; ++i)
    {
        scanf("%s %d %f %f %f", stu[i].name, &stu[i].id, &stu[i].score[0], &stu[i].score[1], &stu[i].score[2]);
        stu[i].score[3] = stu[i].score[0] + stu[i].score[1] + stu[i].score[2];
    }
    _size += n; // 更新数据规模
    printf("添加成功!\n");
}

void Remove(int index)
{
    for (int i = index; i < _size - 1; ++i)
    { // 数组从后往前移动
        stu[i].id = stu[i + 1].id;
        strcpy(stu[i].name, stu[i + 1].name);
        stu[i].score[0] = stu[i + 1].score[0];
        stu[i].score[1] = stu[i + 1].score[1];
        stu[i].score[2] = stu[i + 1].score[2];
        stu[i].score[3] = stu[i + 1].score[3];
    }
    _size--; // 数据规模减1
}

int searchById(int _id)
{
    int found = 0; // 标记是否找到
    for (int i = 0; i < _id; ++i)
    {
        if (stu[i].id == _id)
        {
            found = 1; // 找到
            return i;  // 返回下标
        }
    }
    if (found == 0)
    {
        printf("不存在该学生信息!\n");
        return -1;
    }
}

void Modify(int i)
{
    printf("请输入学生信息（姓名 学号 学科1成绩 学科2成绩 学科3成绩），用空格分隔：\n");
    scanf("%s %d %f %f %f", stu[i].name, &stu[i].id, &stu[i].score[0], &stu[i].score[1], &stu[i].score[2]);
    stu[i].score[3] = stu[i].score[0] + stu[i].score[1] + stu[i].score[2];
}

void menu()
{
    printf("\t\t\t\t***************************************************\n");
    printf("\t\t\t\t*\t\t学生成绩管理系统                   *\n");
    printf("\t\t\t\t***************************************************\n");
    printf("\t\t\t\t*1.添加学生信息   2.查找学生信息  3.删除学生信息  *\n");
    printf("\t\t\t\t*4.修改学生信息   5.排序         6.显示所有信息 *\n");
    printf("\t\t\t\t*7.统计平均分     8.分数段人数查询 0.退出系统    *\n");
    printf("\t\t\t\t***************************************************\n");
}

void print(int i)
{
    printf("%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\n", stu[i].name, stu[i].id, stu[i].score[0], stu[i].score[1], stu[i].score[2], stu[i].score[3]);
}

void sort(int n)
{
    // n=1,表示按学科1降序排序
    // n=2,表示按学科2降序排序
    // n=3,表示按学科3降序排序
    // n=4,表示按总分降序排序
    int sum=_size;
    int sorted = 0; // 标记是否有序，有序为1，无序为0
    while (!sorted)
    {               // 不断扫描数组，直到数组整体有序
        sorted = 1; // 假定已经排序
        for (int i = 1; i < sum; ++i)
        {
            if (stu[i - 1].score[n - 1] < stu[i].score[n - 1])
            {
                struct stu tmp=stu[i-1];
                stu[i-1]=stu[i];
                stu[i]=tmp; // 交换
                sorted = 0; // 整体有序仍无法保证，重新设置为无序
            }
        }
        sum--; // 待排序列减1
    }
}

void traverse()
{
    printf("姓名\t学号\t学科1成绩\t学科2成绩\t学科3成绩\t总成绩\n");
    for (int i = 0; i < _size; ++i)
    {
        print(i);
    }
}

float getAve(int n)
{
    // n=1,表示求学科1班级平均分
    // n=2,表示求学科2班级平均分
    // n=3,表示求学科3班级平均分
    // n=4,表示求学科4班级平均分
    float sum = 0.0;
    for (int i = 0; i < _size; i++)
    {
        sum += stu[i].score[n - 1];
    }
    return sum / _size;
}

int getPeople(int n,float _min,float _max){
    // n=1,表示求学科1班级平均分
    // n=2,表示求学科2班级平均分
    // n=3,表示求学科3班级平均分
    // n=4,表示求学科4班级平均分
    int count=0;
    for(int i=0;i<_size;++i){
        if(stu[i].score[n-1]>=_min && stu[i].score[n-1]<=_max){
            count++;
        }
    }
    return count;
}

int main()
{
    int choice;

    while (1)
    {
        menu();
        printf("输入0-8，选择功能！\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("请输入你要添加的成绩条数！\n");
            int num;
            scanf("%d", &num);
            Insert(num);
            break;
        case 2:
            printf("请输入你要查找的学生学号！\n");
            scanf("%d", &_id);
            int res2 = searchById(_id);
            if (res2 != -1)
            {
                printf("姓名\t学号\t学科1成绩\t学科2成绩\t学科3成绩\n");
                print(res2);
            }
            else
            {
                printf("该同学成绩不存在！\n");
            }
            break;
        case 3:
            printf("请输入你要删除的学生学号！\n");
            scanf("%d", &_id);
            int res3 = searchById(_id);
            if (res3 != -1)
            {
                Remove(res3);
                printf("删除成功！\n");
            }
            else
            {
                printf("该同学成绩不存在！\n");
            }
            break;
        case 4:
            printf("请输入你要修改的学生学号！\n");
            scanf("%d", &_id);
            int res4 = searchById(_id);
            if (res4 != -1)
            {
                Modify(res4);
                printf("修改成功！\n");
            }
            else
            {
                printf("该同学成绩不存在！\n");
            }
            break;
        case 5:
            printf("输入1-4，选择排序方式！\n");
            printf("1->按学科1降序排序\n");
            printf("2->按学科2降序排序\n");
            printf("3->按学科3降序排序\n");
            printf("4->按总分降序排序\n");
            int t;
            scanf("%d", &t);
            if (t > 4 || t < 1)
            {
                printf("输入错误！\n");
                break;
            }
            sort(t);
            traverse();
            break;
        case 6:
            if (_size != 0)
            {
                traverse();
            }
            else
            {
                printf("成绩信息为空！\n");
            }
            break;
        case 7:
            printf("输入1-4，选择求班级平均分方式！\n");
            printf("1->求学科1平均分\n");
            printf("2->求学科2平均分\n");
            printf("3->求学科3平均分\n");
            printf("4->求总分平均分\n");
            int opt;
            scanf("%d", &opt);
            if (opt > 4 || opt < 1)
            {
                printf("输入错误！\n");
                break;
            }
            printf("%.2f\n", getAve(opt));
            break;
        case 8:
            printf("输入1-4，选择分数段查询方式！\n");
            printf("1->学科1\n");
            printf("2->学科2\n");
            printf("3->学科3\n");
            printf("4->总分\n");
            int t1;
            scanf("%d",&t1);
            if (t1 > 4 || t1 < 1)
            {
                printf("输入错误！\n");
                break;
            }
            float s1,s2;
            printf("请分别输入 左区间(min) 有区间(max)!\n");
            scanf("%f %f",&s1,&s2);
            printf("人数：%d\n",getPeople(t1,s1,s2));
            break;
        case 0:
            printf("已退出系统！\n");
            return 0;
        default:
            printf("无效选择！\n");
            break;
        }
    }
    return 0;
}
