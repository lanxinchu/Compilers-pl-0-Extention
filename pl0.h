///这个文件中的代码与课本的一致

//对于使用C++的编译器，这句话可以注释掉，但对于C语言不行
//typedef enum {FALSE,TRUE,} bool;

#define norw 16//关键字的个数
#define txmax 100//名字表的容量
#define nmax 14//数字的最大位数
#define al 10//符号的最大长度
#define amax 2047//地址上界
#define levmax 3//最大允许的嵌套声明层数
#define cxmax 200//最多的虚拟机代码数


enum symbol{
    //枚举类型：
    //nul=0，ident=1，number=2，plus=3，minus=4
    //times=5，依次类推，procsym=31
     nul,      ident,     number,
     //下面的表示的是系统的算数运算符以及逻辑运算符，知道意思就可以
    plus,     minus,    times,//相乘的意思
    slash,      oddsym,    eql,      neq,
    lss,      leq,       gtr,        geq,      lparen,//左括号
    rparen,   comma,     semicolon,  period,   becomes,//赋值的意思


  //下面的表示是系统的保留字
  beginsym, endsym,    ifsym,      thensym,  whilesym,
  writesym, readsym,   dosym,      callsym,  constsym,
  varsym,   procsym,   elsesym,    repeatsym,   untilsym,
  colon,
};
#define symnum  36
enum object{
    constant,
    variable,
    procedur,
    array,
};
enum fct{
          lit,          opr,           lod,
    sto,          cal,           inte,
    jmp,          jpc,	mov, sta, lda, arrcheck,
};
#define   fctnum 12
struct instruction
{
     enum  fct  f;//虚拟机代码指令
     int l;//引用层与声明层的层差
     int a;//根据f的不同而不同，参考课本
};
FILE*   fas;//输出名字表
FILE*   fa;//输出虚拟机代码
FILE*   fa1;//输出源文件及其各行对应的首地址
FILE*   fa2;//输出结果
bool listswitch;
bool tableswitch;
char ch;//获取字符的缓冲区,getch使用
enum symbol sym;
char id[al+1];//当前的id，多出的1个长度是字符串结束标志'\0'
int num;
int cc,ll;//cc表示当前字符的位置
int cx;//虚拟机代码指针
char line[81];//读取行缓冲区
int save_dx;        // 过程传参部分使用的辅助变量, 存放之前的dx值
int param_num = 0;   // 记录传参参数个数
char a[al+1];//临时符号，多出的一个字节用于表示字符串的结尾
struct instruction code [cxmax];//存放虚拟机代码的数组
char word[norw][al];//保留字
enum symbol wsym[norw];//保留字对应的符号值
enum symbol ssym[256];//单符号的符号值
char mnemonic [fctnum][5];//虚拟机代码指令的名称
bool declbegsys[symnum];//表示声明开始的符号集合
bool statbegsys[symnum];//表示语句开始的符号集合
bool facbegsys[symnum];//表示因子开始的符号集合
//名字表结构
struct tablestruct
{
  char name[al];//名字
  enum object kind;//类型：const,var,array,procedure
  int val;//数值，仅const使用
  int level;//所处层
  int adr;//地址
  int size;//需分配的数据空间,仅procedure使用
  int startid;//数组上界 
};
 struct tablestruct table[txmax];//名字表
 FILE* fin;
 FILE* fout;
 char fname[al];
 int err;//错误计数器，每出一次错误，其值加一
#define getsymdo                              if(-1==getsym())return -1
#define getchdo                               if(-1==getch())return -1
#define testdo(a,b,c)                         if(-1==test(a,b,c))return -1
#define gendo(a,b,c)                          if(-1==gen(a,b,c))return -1
#define expressiondo(a,b,c)                   if(-1==expression_r(a,b,c))return -1
#define factordo(a,b,c)                       if(-1==factor(a,b,c))return -1
#define termdo(a,b,c)                         if(-1==term(a,b,c))return -1
#define conditiondo(a,b,c)                    if(-1==condition(a,b,c))return -1
#define statementdo(a,b,c)                    if(-1==statement(a,b,c))return -1
#define constdeclarationdo(a,b,c)             if(-1==constdeclaration(a,b,c))return -1
#define vardeclarationdo(a,b,c)               if(-1==vardeclaration(a,b,c))return -1
void error(int n);
int getsym();
int getch();
void init();
int gen(enum fct x,int y,int z);
int test(bool*s1,bool*s2,int n);
int inset(int e,bool*s);
int addset(bool*sr,bool*s1,bool*s2,int n);
int subset(bool*sr,bool*s1,bool*s2,int n);
int mulset(bool*sr,bool*s1,bool*s2,int n);
int block(int lev,int tx,bool* fsys);
void interpret();
int factor(bool* fsys,int* ptx,int lev);
int term(bool*fsys,int*ptx,int lev);
int condition(bool*fsys,int*ptx,int lev);
int expression_r(bool*fsys,int*ptx,int lev);
int statement(bool*fsys,int*ptx,int lev);
void listcode(int cx0);
int vardeclaration(int* ptx,int lev, int* pdx);
int constdeclaration(int* ptx,int lev, int* pdx);
int position(char* idt,int tx);
void enter(enum object k,int* ptx,int lev,int* pdx);
int base(int l,int* s,int b);
