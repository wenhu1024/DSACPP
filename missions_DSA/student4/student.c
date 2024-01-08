#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node node;

typedef struct scores
{
    int math;
    int shuju;
    int c;
    int sum;
} scores;

typedef struct students
{
    char id[15];
    char name[10];
    char sex[5];
    char birth[20];
    char email[20];
    char phone[20];
    scores score;
} students;

struct node 
{
    students student;   // 数据域
    struct node *next;  // 指针，指向下一个链表节点
};

struct node *head; //头结点，不保存学生信息(全局变量)，记录链表起始位置 
 
void menu();  //菜单
void add();     //录入信息，添加节点至链表末尾
void search();  //根据id查找
void Delete();  //根据id删除一个节点
void Insert();  //选择插入位置，并添加
void show_all(); //查看所有信息

void menu()
{
    printf("学生信息管理系统\n");
    printf("1、输入信息\n");
    printf("2、查找信息\n");
    printf("3、删除信息\n");
    printf("4、插入信息\n");
    printf("5、查看所有学生信息\n");
    printf("6、总分排序\n");
    printf("7、保存信息\n");
    printf("0、退出系统\n");
}

void add()
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node)); //动态内存分配，分配一个节点
    new_node->next = NULL;
    struct node *i = head;
    printf("请输入学生学号:\n");
    scanf("%s", new_node->student.id);
    printf("请输入学生姓名:\n");
    scanf("%s", new_node->student.name);
    while (i != NULL)
    {
        if (strcmp(i->student.id, new_node->student.id) == 0)
        {
            printf("该学生已存在!\n");
            return;
        }
        i = i->next;
    }
    printf("请输入学生性别:\n");
    scanf("%s", new_node->student.sex);
    printf("请输入学生出生日期:\n");
    scanf("%s", new_node->student.birth);
    printf("请输入学生邮件:\n");
    scanf("%s", new_node->student.email);
    printf("请输入学生电话:\n");
    scanf("%s", new_node->student.phone);
    printf("请输入学生成绩:\n");
    printf("高数:");
    scanf("%d", &new_node->student.score.math);
    printf("shuju:");
    scanf("%d", &new_node->student.score.shuju);
    printf("c语言:");
    scanf("%d", &new_node->student.score.c);
    new_node->student.score.sum =
        new_node->student.score.math +
        new_node->student.score.shuju +
        new_node->student.score.c;

    struct node *t = head;
    if (head->next == NULL)
    {   //链表为空时
        head->next = new_node;
        new_node->next = NULL;
    }
    else
    {
        while (t->next != NULL && t->next->student.score.sum > new_node->student.score.sum)
        {
            t = t->next;
        }
        if (t->next == NULL)
        {
            t->next = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = t->next;
            t->next = new_node;
        }
    }
    printf("添加成功!\n");
}

void search()
{
    struct node *t = head->next;
    printf("请输入你要查找的学生学号:\n");
    char s_id[15];
    scanf("%s", s_id);
    while (t != NULL)
    {
        if (strcmp(t->student.id, s_id) == 0) //字符串相等，返回0，不等返回1
        {
            printf("学号:%s\n", t->student.id);
            printf("姓名:%s\n", t->student.name);
            printf("性别:%s\n", t->student.sex);
            printf("出生日期:%s\n", t->student.birth);
            printf("邮件:%s\n", t->student.email);
            printf("电话:%s\n", t->student.phone);
            printf("成绩:\n");
            printf("高数:%d\n", t->student.score.math);
            printf("数据结构:%d\n", t->student.score.shuju);
            printf("c语言:%d\n", t->student.score.c);
            printf("总分:%d\n", t->student.score.sum);
            return;
        }
        t = t->next; //往后查找
    }
    printf("该学生不存在!\n");
}


void Delete()
{
    struct node *t = head, *f;
    char d_id[15];
    printf("请输入你想要删除的学生学号:\n");
    scanf("%s", d_id);
    while (t->next != NULL)
    {
        if (strcmp(t->next->student.id, d_id) == 0) //字符串相等，返回0，不等返回1
        {
            if (t->next->next == NULL)//找到在链表末尾
            {
                free(t->next);   //释放动态内存
                t->next = NULL; 
            }
            else
            {//找到在链表中间
                f = t->next;
                t->next = f->next;
                free(f);
            }
            printf("删除成功!\n");
            return;
        }
        t = t->next;
    }

    if (t->next == NULL)
    {
        printf("该学生不存在!删除失败！\n");
    }
}

void Insert()
{
    int pos = 0;
    struct node *n = NULL;
    struct node *cur = NULL;
    int i = 1;
    int found = 0; //标记是否找到，找到的话设置为1，否则为0

    printf("请输入插入位置:\n");
    scanf("%d", &pos);
    n = head->next;
    i = 1;
    found = 0;
    cur = n;
    while (NULL != n)   //防止插入的位置大于链表长度
    {
        cur = n;
        if (pos == i)
        { /* 找到位置 */
            found = 1;
            break;
        }

        i++;
        n = n->next;
    }

    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->next = NULL;
    printf("请输入学生学号:\n");
    scanf("%s", new_node->student.id);
    printf("请输入学生姓名:\n");
    scanf("%s", new_node->student.name);
    printf("请输入学生性别:\n");
    scanf("%s", new_node->student.sex);
    printf("请输入学生出生日期:\n");
    scanf("%s", new_node->student.birth);
    printf("请输入学生邮件:\n");
    scanf("%s", new_node->student.email);
    printf("请输入学生电话:\n");
    scanf("%s", new_node->student.phone);
    printf("请输入学生成绩:\n");
    printf("高数:");
    scanf("%d", &new_node->student.score.math);
    printf("shuju:");
    scanf("%d", &new_node->student.score.shuju);
    printf("c语言:");
    scanf("%d", &new_node->student.score.c);
    new_node->student.score.sum =
        new_node->student.score.math +
        new_node->student.score.shuju +
        new_node->student.score.c;

    if (1 == found)
    {
        new_node->next = cur->next;
        cur->next = new_node;
    }
    else    //插入的位置大于链表长度，则把节点放置到链表第一个位置
    {
        new_node->next = head->next;
        head->next = new_node;
    }
}


void show_all()
{
    struct node *t = head->next;

    if (head->next == NULL)
    {
        printf("暂无学生信息!\n");
        return;
    }
    printf("所有学生信息:\n\n");
    while (t != NULL)
    {
        printf("姓名:%s\n", t->student.name);
        printf("学号:%s\n", t->student.id);
        printf("性别:%s\n", t->student.sex);
        printf("出生日期:%s\n", t->student.birth);
        printf("成绩:\n");
        printf("高数:%d\n", t->student.score.math);
        printf("数据结构:%d\n", t->student.score.shuju);
        printf("c语言:%d\n", t->student.score.c);
        printf("总分:%d\n", t->student.score.sum);
        printf("\n");
        t = t->next;
    }
}

// 总分从大到小
int cmp_small_sum(node e1, node e2)
{
    return e1.student.score.sum > e2.student.score.sum;
}

void Sort()
{
    system("cls");
    printf("按照总分从大到小排序\n");

    int flag = 0; //标记是否交换，1表示交换，0表示不交换
    for (node *p = head->next; p != NULL; p = p->next)
    {
        node *q = p->next;
        for (; q != NULL; q = q->next)
        {

            if (!cmp_small_sum(*p, *q)) //指针解引用
            {
                flag = 1;
            }
            break;
        }
        if (flag == 1)
        {
            // 交换数据域
            node t = *p;
            *p = *q;
            *q = t;
            // 处理指针域
            t.next = p->next;
            p->next = q->next;
            q->next = t.next;
            flag = 0;
        }
    }
}

void save()
{
    FILE *f;    
    f = fopen("D:\\students system.txt", "w"); // 数据保存到d盘文件students system.txt  //打开文件
    if (f == NULL)
    {
        printf("保存失败!\n");
        return;
    }
    struct node *t = head->next;

    if (head->next == NULL)
        printf("暂无学生信息!\n");

    else
    {
        while (t != NULL)
        {
            fprintf(f, "姓名:%s\n", t->student.name);
            fprintf(f, "学号:%s\n", t->student.id);
            fprintf(f, "性别:%s\n", t->student.sex);
            fprintf(f, "出生日期:%s\n", t->student.birth);
            fputs("成绩:\n", f);
            fprintf(f, "高数:%d\n", t->student.score.math);
            fprintf(f, "数据结构:%d\n", t->student.score.shuju);
            fprintf(f, "c语言:%d\n", t->student.score.c);
            fprintf(f, "总分:%d\n", t->student.score.sum);
            fputs("\n", f);
            t = t->next;
        }
        printf("保存成功!\n");
    }
    fclose(f);   //关闭文件
}
void Input()
{
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 0:
        exit(0);
        break;
    case 1:
        system("cls");  //清屏
        printf("输入学生信息:\n");
        add();
        system("pause");//暂停
        system("cls");
        break;
    case 2:
        system("cls");
        printf("查找学生信息:\n");
        search();
        system("pause");
        system("cls");
        break;
    case 3:
        system("cls");
        printf("删除学生信息:\n");
        Delete();
        system("pause");
        system("cls");
        break;
    case 4:
        system("cls");
        Insert();
        system("pause");
        system("cls");
        break;
    case 5:
        system("cls");
        show_all();
        system("pause");
        system("cls");
        break;
    case 6:
        system("cls");
        Sort();
        show_all();
        system("pause");
        system("cls");
        break;
    case 7:
        system("cls");
        save();
        system("pause");
        system("cls");
        break;
    default:
        system("cls");
        break;
    }
}


int main()
{
    head = (struct node *)malloc(sizeof(struct node)); // 头节点
    head->next = NULL;
    while (1)
    {
        menu();
        Input();
    }
    return 0;
}
