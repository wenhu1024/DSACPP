#include <iostream>
#include "Stack.h"

using namespace std;

int nCheck; //尝试总次数
int nSolu;  //解的数目
int N=4;   

class Queen{   //皇后类
public:
    int x,y;    //皇后在棋盘的位置坐标
    Queen(int xx=0,int yy=0):x(xx),y(yy){}

    bool operator==(const Queen &q) const{  //重载判等操作符，以检测不同皇后之间可能的冲突
        return x==q.x   //同一行
            || y==q.y   //同一列
            || x+y==q.x+q.y //正对角线冲突( / 型)
            || x-y==q.x-q.y; //反对角线冲突( \ 型)
    }

    bool  operator!=(const Queen &q) const{
        return !(*this==q);   //注意这里的已经重载了，不能改为 return *this!=q;
    } 
};

void displayRow(Queen &q){//打印当前皇后（放置于col列）所在行
   printf ( "%2d: ", q.x );
   int i = 0;
   while ( i++ < q.y ) printf ( "[]" );
   printf ( "█" );
   while ( i++ < N ) printf ( "[]" );
   printf ( "%2d\n", q.y );
}
void displayProgress(Stack<Queen> &S,int nQueen){//在棋盘上显示搜查的进展
    system("clear");
    N=nQueen;
    S.traverse(displayRow);
    if ( nQueen <= S.size() )
      printf("%d solution(s) found after %d check(s)\a", nSolu, nCheck);
    getchar();  
}

void placeQueens(int N){ //N皇后算法（迭代版）：采用试探/回溯的策略，借助栈记录查找的结果
    Stack<Queen> solu;  //存放解的栈
    Queen q(0,0);   //从原点位置开始

    do{ //循环试探，回溯
        if(solu.size() >= N || q.y >= N){   //回溯
            q = solu.pop(); ++q.y;  //回溯一行，继续试探下一列
        }
        else{//试探
            while( (q.y<N) && (0<=solu.find(q))){
                ++q.y;  ++nCheck;
            }
            if(q.y<N){  //没有冲突，y也不出界。对应一行找到可放的列
                solu.push(q);
                if(solu.size() >=N ){++nSolu;}
                ++q.x; q.y = 0; //转入下一行，从第0列开始，试探下一皇后
            }
        }displayProgress ( solu, N );
    }while(q.x > 0 || q.y < N); //结束条件： x==0 && y==N
}


int main(int argc,char **argv){
    if(argc!=2){
        printf("Usage: %s <N>\n", argv[0]);
        return 0;
    }
    N = atoi(argv[1]);
    placeQueens(N);
    printf("%d solution(s) found after %d checks\n", nSolu, nCheck);
    return 0;
}