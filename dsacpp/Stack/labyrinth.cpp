#include "Stack.h"
#include <iostream>
#include <string>

typedef enum
{
    AVAILABLE,   // 原始可用的
    ROUTE,       // 在当前路径的
    BACKTRACKED, // 所有方向均尝试失败后回溯过的
    WALL         // 不可使用的(墙)
} Status;        // 迷宫单元状态

typedef enum
{
    UNKNOWN, // 未定的
    EAST,    // 东
    SOUTH,   // 南
    WEST,    // 西
    NORTH,   // 北
    NO_WAY   // 无路可通
} ESWN;      // 单元的相对邻接方向

inline ESWN nextESWN(ESWN eswn)
{
    return ESWN(eswn + 1);
}

struct Cell
{                            // 迷宫格点
    int x, y;                // x坐标, y坐标
    Status status;           // 类型
    ESWN incoming, outgoing; // 进入，走出方向
};

const int LABY_MAX = 24;  // 最大迷宫尺寸
struct Cell laby[LABY_MAX][LABY_MAX]; // 迷宫

int labySize;
Cell* startCell;
Cell* goalCell;

/*    [ 西 ] 
[ 北 ][cell][ 南 ]        //单纯定义问题，逆时针东南西北  :)
      [ 东 ]
*/

inline Cell *neighbor(Cell *cell)
{ // 查询当前位置的相邻格点
    switch (cell->outgoing)
    {
    case EAST:
        return cell + LABY_MAX; // 向东
    case SOUTH:
        return cell + 1; // 向南
    case WEST:
        return cell - LABY_MAX; // 向西
    case NORTH:
        return cell - 1; // 向北
    default:
        exit(-1);
    }
}

inline Cell *advance(Cell *cell)
{
    Cell *next = nullptr;
    switch (cell->outgoing)
    {
    case EAST:
        next = cell + LABY_MAX;
        next->incoming = WEST;
        break;
        // 向东
    case SOUTH:
        next = cell + 1;
        next->incoming = NORTH;
        break;
        // 向南
    case WEST:
        next = cell - LABY_MAX;
        next->incoming = EAST;
        break;
        // 向西
    case NORTH:
        next = cell - 1;
        next->incoming = SOUTH;
        break;
        // 向北
    default:
        exit(-1);
    }
    return next;
}

void readLaby(char *labyFile)
{
    //  读取迷宫文件
    FILE *fp;
    if (!(fp = fopen(labyFile, "r")))
    {
        std::cout << "can't open " << labyFile << std::endl;
        exit(-1);
    }

    fscanf(fp, "Laby Size = %d\n", &labySize);

    if (LABY_MAX < labySize)
    {
        std::cout << "Laby size " << labySize << " > " << LABY_MAX << std::endl;
        exit(-1);
    }

    int startX, startY;
    fscanf(fp, "Start = (%d, %d)\n", &startX, &startY);
    startCell = &laby[startX][startY];

    int goalX, goalY;
    fscanf(fp, "Goal = (%d, %d)\n", &goalX, &goalY);
    goalCell = &laby[goalX][goalY];

    for (int j = 0; j < labySize; j++)
        for (int i = 0; i < labySize; i++)
        {
            laby[i][j].x = i;
            laby[i][j].y = j;
            int type;
            fscanf(fp, "%d", &type);
            switch (type)
            {
            case 1:
                laby[i][j].status = WALL;
                break;
            case 0:
                laby[i][j].status = AVAILABLE;
                break;
            default:
                exit(-1);
            }
            laby[i][j].incoming =
                laby[i][j].outgoing = UNKNOWN;
        }
    fclose(fp);
}

void printLabyCell(Cell *elem)
{ // 打印某一个迷宫格信息
    printf("%d -> (%d, %d) -> %d\n",
           ((Cell *)elem)->incoming,
           ((Cell *)elem)->x,
           ((Cell *)elem)->y,
           ((Cell *)elem)->outgoing);
}

void displayLaby() { //┘└┐┌│─
   static const char*   pattern[5][5] = {
      "┼", "┼", "┼", "┼", "┼",
      "┼", "  ", "┌", "─", "└",
      "┼", "┌", "  ", "┐", "│",
      "┼", "─", "┐", "  ", "┘",
      "┼", "└", "│", "┘", "  "
   };
   system ( "clear" );
   printf ( " " );
   for ( int j = 0; j < labySize; j++ )
      ( j < 10 ) ? printf ( "%X", j ) : printf ( "%c", 'A' - 10 + j );
   printf ( "\n" );
   for ( int j = 0; j < labySize; j++ ) {
      ( j < 10 ) ? printf ( "%X", j ) : printf ( "%c", 'A' - 10 + j );
      for ( int i = 0; i < labySize; i++ )
         if ( goalCell == &laby[i][j] )
            printf ( "$" );
         else
            switch ( laby[i][j].status ) {
               case WALL:  printf ( "█" );   break;
               case BACKTRACKED: printf ( "○" );   break;
               case AVAILABLE: printf ( " " );   break;
               default   : printf ( "%s", pattern[laby[i][j].outgoing][laby[i][j].incoming] );   break;
            }
      printf ( "\n" );
   }
}

bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX], Cell *s, Cell *t)
{
    if ((s->status != AVAILABLE) || (t->status != AVAILABLE))
        return false;   // 退化情况
    Stack<Cell *> path; // 用栈记录通路
    s->incoming = UNKNOWN;
    s->status = ROUTE;
    path.push(s);

    do
    {
        displayLaby(); 
        getchar();
        //  path.traverse(printLabyCell); 
        //  printLabyCell(path.top());
        Cell *c = path.top();
        if (c == t)
            return true; // 若已抵达终点，则找到了一条通路；否则，沿尚未试探的方向继续试探
        while ( (c->outgoing = nextESWN(c->outgoing)) < NO_WAY )
        {
            if (neighbor(c)->status == AVAILABLE)
                break; // 试图找到尚未试探的方向
        }
        if (c->outgoing >= NO_WAY)
        { // 若所有方向都已尝试
            c->status = BACKTRACKED;
            c = path.pop(); // 则向后回溯一步
        }
        else
        { // 否则，向前试探一步
            path.push(c = advance(c));
            c->outgoing = UNKNOWN;
            c->status = ROUTE;
        }
    } while (!path.empty());
    return false;
}

int main(){
   readLaby (const_cast<char*>("laby/laby_00.txt")) ; //使用指定迷宫文件，或随机生成
   labyrinth ( laby, startCell, goalCell ) ? //启动算法
   printf ( "\nRoute found\a\n" ) :
   printf ( "\nNo route found\a\n" );
   getchar();
   return 0;
}