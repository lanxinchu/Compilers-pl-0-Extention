///����ļ��еĴ�����α���һ��

//����ʹ��C++�ı���������仰����ע�͵���������C���Բ���
//typedef enum {FALSE,TRUE,} bool;

#define norw 16//�ؼ��ֵĸ���
#define txmax 100//���ֱ������
#define nmax 14//���ֵ����λ��
#define al 10//���ŵ���󳤶�
#define amax 2047//��ַ�Ͻ�
#define levmax 3//��������Ƕ����������
#define cxmax 200//���������������


enum symbol{
    //ö�����ͣ�
    //nul=0��ident=1��number=2��plus=3��minus=4
    //times=5���������ƣ�procsym=31
     nul,      ident,     number,
     //����ı�ʾ����ϵͳ������������Լ��߼��������֪����˼�Ϳ���
    plus,     minus,    times,//��˵���˼
    slash,      oddsym,    eql,      neq,
    lss,      leq,       gtr,        geq,      lparen,//������
    rparen,   comma,     semicolon,  period,   becomes,//��ֵ����˼


  //����ı�ʾ��ϵͳ�ı�����
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
     enum  fct  f;//���������ָ��
     int l;//���ò���������Ĳ��
     int a;//����f�Ĳ�ͬ����ͬ���ο��α�
};
FILE*   fas;//������ֱ�
FILE*   fa;//������������
FILE*   fa1;//���Դ�ļ�������ж�Ӧ���׵�ַ
FILE*   fa2;//������
bool listswitch;
bool tableswitch;
char ch;//��ȡ�ַ��Ļ�����,getchʹ��
enum symbol sym;
char id[al+1];//��ǰ��id�������1���������ַ���������־'\0'
int num;
int cc,ll;//cc��ʾ��ǰ�ַ���λ��
int cx;//���������ָ��
char line[81];//��ȡ�л�����
int save_dx;        // ���̴��β���ʹ�õĸ�������, ���֮ǰ��dxֵ
int param_num = 0;   // ��¼���β�������
char a[al+1];//��ʱ���ţ������һ���ֽ����ڱ�ʾ�ַ����Ľ�β
struct instruction code [cxmax];//�����������������
char word[norw][al];//������
enum symbol wsym[norw];//�����ֶ�Ӧ�ķ���ֵ
enum symbol ssym[256];//�����ŵķ���ֵ
char mnemonic [fctnum][5];//���������ָ�������
bool declbegsys[symnum];//��ʾ������ʼ�ķ��ż���
bool statbegsys[symnum];//��ʾ��俪ʼ�ķ��ż���
bool facbegsys[symnum];//��ʾ���ӿ�ʼ�ķ��ż���
//���ֱ�ṹ
struct tablestruct
{
  char name[al];//����
  enum object kind;//���ͣ�const,var,array,procedure
  int val;//��ֵ����constʹ��
  int level;//������
  int adr;//��ַ
  int size;//���������ݿռ�,��procedureʹ��
  int startid;//�����Ͻ� 
};
 struct tablestruct table[txmax];//���ֱ�
 FILE* fin;
 FILE* fout;
 char fname[al];
 int err;//�����������ÿ��һ�δ�����ֵ��һ
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
