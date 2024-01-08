#include <cstdio>
#include <cstdlib>
#include <memory.h>

class Bitmap{
private:
    char *M;//存放比特图的空间M[]
    int N;  //N为字节数，比特数为8*N
protected:
    void init(int n){M=new char[N=(n+7)/8]; memset(M,0,N);}
public:
    Bitmap(int n=8){init(n);}
    Bitmap(char *file,int n=8)  //从指定文件读取比特图
    {init(n); FILE *fp=fopen(file,"r"); fread(M,sizeof(char),N,fp);fclose(fp);}
    ~Bitmap(){delete [] M;M=nullptr;}
    
    
    void set(int k) {   expand(k); M[k>>3]          |= (0x80 >> (k & 0x07));}
    void clear(int k) { expand(k); M[k>>3]          &= ~(0x80 >> (k& 0x07));}
    int test(int k) {   expand(k); return M[k>>3]   & (0x80 >> (k& 0x07));}

    void dump(char *file)   //将位图整体导出至指定的文件
    {FILE *fp=fopen(file,"w");fwrite(M,sizeof(char),N,fp);fclose(fp);}
    

    char *bit2string(int n){ //将前n位转换为字符串
        expand(n-1);    //最高访问bitmap[n-1]
        char *s=new char[n+1];
        s[n]='\0';
        for(int i=0;i<n;++i){
            s[i]=test(i)? '1' : '0';
        }
        return s;   //返回字符串位置
    }
    void expand(int k){ //如果访问的bitmap[k]已经出界，则需要扩容
        if(k<8*N)   return;
        int oldN=N;
        char *oldM=M;
        init(k<<1); //加倍策略
        memcpy(M,oldM,oldN);
        delete [] oldM;
    }
};

