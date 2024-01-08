#include <iostream>
#include <string>

using namespace std;

static void print(char *s);
static void print(const char *s);
static void print(string s);

static void print ( char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理
static void print ( const char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理


static void print ( string e) {
   const char* c = e.c_str();
   while(*c){
      putchar(*c);
      c++;
   }
   putchar('\n');
}


//还需要完善