#include "Stack.h"
#include <cstring>
#include <cmath>
#include <cctype>
#include <cstdio>

#define N_OPTR 9 //运算符总数
typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    FAC,
    L_P,
    R_P,
    EOE,
} Operator; // 运算符集合

//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符
const char pri[N_OPTR][N_OPTR] =
{ //运算符优先等级 [栈顶] [当前]
   /*              |-------------------- 当 前 运 算 符 --------------------| */
   /*              +      -      *      /      ^      !      (      )      \0 */
    /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
    /* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
    /* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
    /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
    /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
};
// 对优先级表的解读：
// '(' 作为操作符号栈顶，除了当前操作符号为')',总是可以允许当前操作符号入栈;
// '(' 作为当前操作符号，总是可以入栈

// ')' 作为当前操作符号，除了栈顶操作符号为'(',总是可以允许执行栈顶操作符号
// 首'\0' 相当于 (   末'\0' 相当于 )

void readNumber(char *&p, Stack<double> &s){ //注意这里p是指针引用
//操作数入栈
    s.push(static_cast<double>(*p - '0'));

    double t = 0.0; // 临时保存操作数的变量
    while (isdigit(*(++p)))
    {
        t=s.pop();
        s.push(10 * t + static_cast<double>((*p - '0')));
    } // 可能不是个位数

    if (*p != '.')
    {
        return;
    } // 不是小数,操作数入栈完成

    // 否则,继续
    double fraction = 1.0; // 小数部分的权值

    while (isdigit(*(++p)))
    {
        t = s.pop();
        s.push(t + (*p - '0') * (fraction /= 10));
    }
}

Operator getOprRank(char op)
{
    switch (op)
    {
    case '+':
        return ADD;
    case '-':
        return SUB;
    case '*':
        return MUL;
    case '/':
        return DIV;
    case '^':
        return POW;
    case '!':
        return FAC;
    case '(':
        return L_P;
    case ')':
        return R_P;
    case '\0':
        return EOE;
    default:
        exit(-1); // 操作符未知
    }
}


char orderBetween(char op1,char op2){
    //得到操作符优先级比较值
    return pri[getOprRank(op1)][getOprRank(op2)];
}

char *removeSpace(char *s){
    //去除字符串空格
    int i=0,j=0;
    
    while (s[i] != '\0'){
        if(! isspace(s[i])){
            s[j++]=s[i];
        }
        ++i;
    }
    s[j]='\0';
    return s;
}

void append(char* &rpn,double opnd){
//将操作数移至RPN末尾
    char buf[64];
    if(0.0 < opnd - (int) opnd){
        sprintf(buf,"%f \0",opnd);  //浮点格式
    }
    else{
        sprintf(buf,"%d \0",(int)opnd); //整数格式
    }
    rpn=(char*)realloc(rpn,sizeof(char)*(strlen(rpn)+strlen(buf)+1));    // 有'\0', +1
    strcat(rpn,buf);
}

void append(char *&rpn,char optr){
//将操作符移至RNP末尾
    int n=strlen(rpn);
    rpn=(char *)realloc(rpn,sizeof(char)*(n+3)); //3: optr,' ','\0'
    sprintf(rpn+n,"%c ",optr);
    rpn[n+2]='\0';
}

int fac(int n){
//阶乘运算
    int sum=1;
    int hi=n+1;
    for(int i=1;i<hi;++i){
        sum*=i;
    }
    return sum;
}

double calcu(char op,double b){
//执行一元运算
    switch(op){
        case '!':
            return static_cast<double>(fac(static_cast<int>(b)));
        default:
            exit(-1);
    }
}

double calcu(char a,char op,char b){
//执行二元运算
    switch(op){
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            if(b==0) exit(-1);
            else return a/b;
        case '^':
            return pow(a,b);
        default:
            exit(-1);   
    }    
}

double evaluate(char *S,char *&RPN){
//前提：中缀表达式S已经剔除空格
//对表达式S求值，并由中缀表达式转化为逆波兰（又称后缀）表达式RPN
    Stack<double> opnd; //操作数栈（operand）
    Stack<char>   optr; //操作符栈（operator）

    optr.push('\0');    //这里'\0'也作为头哨兵首先入操作符栈

    while(!optr.empty()){
        if(isdigit(*S)){
            //是数字
            readNumber(S,opnd);
            append(RPN,opnd.top()); //读入操作数，并放至RPN末尾
        }
        else{
            switch(orderBetween(optr.top(),*S)){
                case '<':   //栈顶运算符优先级更低时
                {
                    optr.push(*S);
                    ++S;
                    break;
                    //栈顶计算推迟，当前操作符入栈，看S的下一个字符   
                }
                case '=' :   //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
                {
                    optr.pop();
                    ++S;
                    break;
                    //丢栈顶的'(' 或者 '\0'
                }
                case '>' :  //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入操作数栈
                {
                    char op=optr.pop();
                    append(RPN,op);

                    if(op=='!'){
                        double pOpnd=opnd.pop();
                        opnd.push(calcu(op,pOpnd));
                    }                 
                    else{
                        double pOpnd2=opnd.pop();
                        double pOpnd1=opnd.pop();   //注意顺序
                        opnd.push(calcu(pOpnd1,op,pOpnd2));
                    }   
                    break;
                }
                default:
                {
                    exit(-1);
                }
            }
        }
    }
    return opnd.pop();

}

double evaluate_rpn(char* r){
//得到逆波兰表达式后，求值
    Stack<double> opnd;

    while(*r!='\0'){
        if(isdigit(*r)){
            readNumber(r,opnd);
        }else if(*r==' '){
            ++r;
        }
        else{
            char op=*r;
            if (op == '!')
            {
                double pOpnd = opnd.pop();
                opnd.push(calcu(op, pOpnd));
            }
            else
            {
                double pOpnd2 = opnd.pop();
                double pOpnd1 = opnd.pop(); // 注意顺序
                opnd.push(calcu(pOpnd1, op, pOpnd2));
            }
            ++r;
        }
    }
    return opnd.pop();
}

int main()
{
    char* rpn = (char*)malloc(sizeof(char) * 1);
    rpn[0] = '\0'; //逆波兰表达式
    double value = evaluate(const_cast<char*>("(1+2^3!-4)*(5!-(6-(7-(89-0!))))"), rpn); //求值
    printf("%lf\n", value);
    printf("the rpn is: ");
    for(int i = 0; rpn[i]!='\0'; i++)
        printf("%c", rpn[i]);
    printf("\n");
    
    char *r=rpn;
    printf("%f\n",evaluate_rpn(r));
    
    free(rpn);
    rpn = nullptr;
    return 0;
}