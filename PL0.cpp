
#include<stdio.h>
#include"pl0.h"
#include"string.h"

//����궨�����˼��ʾ������ʱ������ջ��С���������interpret�����г���
#define stacksize 500


/*********************************************************************************
    **  �ڿ�Դ����֮ǰ����ʼ�ռ�ס                              **
    **  sym��ʾ���ǵ�ǰ�ַ��������ͣ�                           **
    **  num��ʾ�����֣�                                          **
    **  id�д�ŵ����ַ�����                                     **
    **  ��ס����ڿ�Դ�����ʱ��ǳ���Ҫ��                      **
    **  ���⣬��Ҫ��ס����nxtlev�����д�ŵ��Ǻ�̷��ż�        **
**********************************************************************************/
int main()
{
    bool nxtlev[symnum];//��������ʾ���ǵ�ǰ���ĺ�̷��ż��ϣ�һ����32��
    printf("Input pl/0 file?");//�����ļ���·��
    scanf("%s",fname);
    fin=fopen(fname,"r");//��ֻ�����ļ���fin�ļ�ָ��
    if(fin)
    {
        printf("List object code? (Y/N)");
        scanf("%s",fname);
        listswitch=(fname[0]=='y'||fname[0]=='Y');//listswitch�����оٴ���
        printf("List symbol table? (Y/N)");
        scanf("s%",fname);
        tableswitch=(fname[0]=='y'||fname[0]=='Y');//tableswitch��ʾ�Ƿ����table��
                                                   //�ñ�������22ҳ�ı�

        fa1=fopen("fa1.tmp","w");//��ֻ����ʽ���ļ������ڱ�������ʵ���½�һ���ļ�
        //fa1�ļ��д洢����Դ��������һ�Σ���������ļ���������
        fprintf(fa1,"Input pl/0 file?");//��"Input pl/0 file?"�����ļ���
        fprintf(fa1,"%s\n",fname);//ͬ��
        init();//��ʼ��
        err=0;
        cc=cx=ll=0;
        ch=' ';
        if(-1!=getsym())
        {
            //�������ʼ�ǶԵģ�������ִ��
            //fa��ŵ���ģ�����Ĵ���
            //fas��ŵ���table���е�����
            //����һ�δ��뿴һ���ļ�������
            fa=fopen("fa.tmp","w");//��ֻ����ʽ��
            fas=fopen("fas.tmp","w");//ͬ��
            //����ԭ�ͣ�int addset(bool*sr,bool*s1,bool*s2,int n);
            //bool nxtlev[32];
            /*
                declbegsys[constsym]=true;
                declbegsys[varsym]=true;
                declbegsys[procsym]=true;

                statbegsys[beginsym]=true;
                statbegsys[callsym]=true;
                statbegsys[ifsym]=true;
                statbegsys[whilesym]=true;
                ִ�иú����Ľ��Ϊ��Ӧ���±���nxtlev������ֵ��Ϊtrue��
            */
            //�ڷֳ���ʼ��ʱ�����̷��ſ�����
            //const,var,procedure,begin,call,if,while
            //������nxtlev�н�������Щ��Ϊtrue
            //��Ȼ�������������ֻ��һ��'.'���������ǿյ�Ҳ���ԣ����Ի���nxtlev[period]=true;
            addset(nxtlev,declbegsys,statbegsys,symnum);//symnum=32��
            nxtlev[period]=true;
            //block�Ǳ��������������Ƿֳ���ķ���������̣�����������˴󲿷ֳ���
            //���Ŀ�
            //���ס��block����ʼ��ʱ��ֻ��ȡ��Դ�����һ����䵥λ
            if(-1==block(0,0,nxtlev))
            {
                //����������ʱ����ر������ļ������˳�����
                fclose(fa);
                fclose(fa1);
                fclose(fas);
                fclose(fin);
                printf("\n");
                return 0;
            }
            fclose(fa);
            fclose(fa1);
            fclose(fas);
            //�����Դ����Ľ�βû��'.'���򱨴�
            if(sym!=period)
            {
                error(9);
            }
            //err=0��ʾԴ����û�д���
            if(err==0)
            {
                //��Դ����û�д����ǰ���£���fa2�ļ�������ִ���������䣬���Դ������
                fa2=fopen("fa2.tmp","w");

                //ģ������ִ��Դ����
                interpret();

                fclose(fa2);
            }
            else
            {
                //���Դ�����д�Ļ���������
                printf("Errors in pl/0 program");
            }
        }
        fclose(fin);
    }
    else
    {
        printf("Can't open file! \n");
    }
    printf("\n");
    return 0;
}
void init()
{
    int i;

    for (i=0;i<=255;i++)
    {
        ssym[i]=nul;//nulΪ0����ssym��ʼ��Ϊ0
    }
    //ssym��СΪ256�����Ա�ʾ���е�ASCII��
    ssym['+']=plus;
    ssym['-']=minus;
    ssym['*']=times;
    ssym['/']=slash;
    ssym['(']=lparen;
    ssym[')']=rparen;
    ssym['=']=eql;
    ssym[',']=comma;
    ssym['.']=period;
    ssym['#']=neq;
    ssym[';']=semicolon;

    //������������word�����У�Ϊ��ʹ�ö��ֲ��ң��������˳���ǰ�����ĸ��
    strcpy(&(word[0][0]),"begin");
    strcpy(&(word[1][0]),"call");
    strcpy(&(word[2][0]),"const");
    strcpy(&(word[3][0]),"do");
    strcpy(&(word[4][0]),"else");
    strcpy(&(word[5][0]),"end");
    strcpy(&(word[6][0]),"if");
    strcpy(&(word[7][0]),"odd");
    strcpy(&(word[8][0]),"procedure");
    strcpy(&(word[9][0]),"read");
    strcpy(&(word[10][0]),"repeat");
    strcpy(&(word[11][0]),"then");
    strcpy(&(word[12][0]),"until");
    strcpy(&(word[13][0]),"var");
    strcpy(&(word[14][0]),"while");
    strcpy(&(word[15][0]),"write");

    //wsym�ڷ��б���������Ӧ��ö�ٱ�����ֵ
    wsym[0]=beginsym;
    wsym[1]=callsym;
    wsym[2]=constsym;
    wsym[3]=dosym;
    wsym[4]=elsesym;
    wsym[5]=endsym;
    wsym[6]=ifsym;
    wsym[7]=oddsym;
    wsym[8]=procsym;
    wsym[9]=readsym;
    wsym[10]=repeatsym;
    wsym[11]=thensym;
    wsym[12]=untilsym;
    wsym[13]=varsym;
    wsym[14]=whilesym;
    wsym[15]=writesym;

    //mnemonic�д�ŵ���ģ�����������
    //��С��8������Ϊֻ��8�������
    strcpy(&(mnemonic[lit][0]),"lit");
    strcpy(&(mnemonic[opr][0]),"opr");
    strcpy(&(mnemonic[lod][0]),"lod");
    strcpy(&(mnemonic[sto][0]),"sto");
    strcpy(&(mnemonic[cal][0]),"cal");
    strcpy(&(mnemonic[inte][0]),"int");
    strcpy(&(mnemonic[jmp][0]),"jmp");
    strcpy(&(mnemonic[jpc][0]),"jpc");
    strcpy(&(mnemonic[mov][0]),"mov");
    strcpy(&(mnemonic[sta][0]), "sta");
	strcpy(&(mnemonic[lda][0]), "lda");
	strcpy(&(mnemonic[arrcheck][0]), "arrcheck");

    //��ʼ����Щ���飬ȫ��Ϊfalse
    for(i=0;i<symnum;i++)
    {
        declbegsys[i]=false;
        statbegsys[i]=false;
        facbegsys[i]=false;
    }

    //��������������ݵĹ����ں����Ľ����л��У�
    //�ֱ���������ʼ���ż�����俪ʼ���ż������ӿ�ʼ���ż�
    declbegsys[constsym]=true;
    declbegsys[varsym]=true;
    declbegsys[procsym]=true;

    statbegsys[beginsym]=true;
    statbegsys[callsym]=true;
    statbegsys[ifsym]=true;
    statbegsys[whilesym]=true;

    facbegsys[ident]=true;
    facbegsys[number]=true;
    facbegsys[lparen]=true;
}

//�ú����Ĺ����Ƿ��ط���e�Ƿ��ڷ��ż�s��
//���򷵻�true�����򷵻�false
int inset(int e,bool* s)
{
    return s[e];
}

//�����±�i�����s1��s2��true����srҲ��true
//���������ʵ�ǳ�ʼ����̷��ż�����nxtlev����
int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for (i=0;i<n;i++)
    {
        sr[i]=s1[i]||s2[i];
    }
    return 0;
}

//�����±�i�����s1Ϊ�沢��s2�Ǽ٣���sr��true�����������Ϊ��
int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        sr[i]=s1[i]&&(! s2[i]);
    }
    return 0;
}
//�����±�i�����s1Ϊ�沢��s2Ϊ�棬��sr��true�����������Ϊ��

int mulset(bool * sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        sr[i]=s1[i]&&s2[i];
    }
    return 0;
}

//���������ʱ�򣬽���������󣬲����˳�����
//���ڳ����룬���Բο��α�27ҳ
void error (int n)
{
    char space[81];
    memset(space,32,81);//��spaceȫ����Ϊ�ո������32��ASCII��Ϊ32�Ŀո�
    space[cc-1]=0;//����ʱ��ǰ�����Ѿ����꣬������CC-1
    printf("****%s!%d\n",space,n);
    fprintf(fa1,"****%s!%d\n",space,n);
    err++;
}

int getch()
{
	////�ȶ���һ�зŵ�line�������С�����ֵcc==ll��
	//�Ժ�ÿ����һ��getch��ָ��������һ���ַ���
	//�����굱ǰ������ʱ���ٵ���getch�������cc==ll
    if(cc==ll)
    {
        if (feof(fin))
        {
            printf("program incomplete");
            return -1;//����-1��ʾ�ļ�����������
        }
        ll=0;
        cc=0;
        printf("%d",cx);
        fprintf(fa1,"%d",cx);
        ch=' ';
        //�����ѭ���Ĺ����ǽ�Դ�ļ���һ�д��������������fa1�ļ���
        while(ch!=10)//10Ϊ���з�
        {
            //fscanf(fin,"%c",&ch)
            if (EOF==fscanf(fin,"%c",&ch))//fscanf(fin,"%c",&ch)��finָ������ݷ���ch��
            {
                line[ll]=0;
                break;
            }
            printf("%c",ch);//�������
            fprintf(fa1,"%c",ch);//����������fa1�ļ���
            line[ll]=ch;//���������line�����У�ll��ʾ���볤��
            ll++;
        }
        printf("\n");
        fprintf(fa1,"\n");
    }
    ch=line[cc];
    cc++;
    return 0;
}
//�ú����Ĺ��ܾ���ȡһ�����ݵ�λ����һ�����ʷ��ţ�ͨ�����øú�������һ��ȡ��һ��
//���ֻ��ַ���������������������getch�����Ҿ���Ӧ���ǵ�������غ�������
//ÿ����һ�θú������ú����ͻ�ȥ����getch����
//sym��¼������䵥λ�����ͣ�num�д�ŵ��Ǿ������ֵ
int getsym()
{
    int i,j,k;
    //���Կո񣬻��з���10����'\n'���Ʊ��ASCIIΪ9����'\t'
    while (ch==' '||ch==10||ch==9)
    {
        //�൱��ִ��if(-1==getch())
        //          return -1
        //��ȡһ�����ŵ�λ
        getchdo;
    }
    //������ȡһ�����ʷ���
    if(ch>='a'&&ch<='z')
    {
        k=0;
        do
        {
            if (k<al)//al=10
            {
                a[k]=ch;//��һ�����ʷ��ŷ���a[]�����У���al���Կ�����pl0����Ҫ�����󵥴�
                        //���Ų��ܳ���10��
                k++;
            }
            getchdo;
        }while (ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
        a[k]=0;//���ʷ�����0��β
        strcpy(id,a);//���õ��ʷ��Ÿ��Ƶ�id[]������
        i=0;
        j=norw-1;//norw=13
        //ִ�ж��ֲ��ң������õ��ʷ����Ƿ��Ǳ�����
        do{
            k=(i+j)/2;
            if (strcmp(id,word[k])<=0)
            {
                j=k-1;
            }
            if (strcmp(id,word[k])>=0)
            {
                i=k+1;
            }
        }while(i<=j);
        if(i-1>j)
        {
            sym=wsym[k];//����Ǳ���������Ϊ�����ֵ�����
        }
        else
        {
            sym=ident;//������Ǳ����֣�����symΪ1
        }
    }
    //ȡһ�����ֵ�λ
    else
    {
        if (ch>='0'&&ch<='9')
        {
            k=0;
            num=0;
            sym=number;//��sym��ʾ����һ������
            do{
                num=10*num+ch-'0';
                k++;
                getchdo;
            }while(ch>='0'&&ch<='9');
            k--;//??���ࣿ
            if(k>nmax)//Ҫ������λ�����ܳ���15λ��nmax=14�������Ҿ������д���
            {
               error(30);//���������
            }
        }
        //����Ǹ�ֵ������Ļ���ִ�����
        else
        {
            if(ch==':')
            {
                getchdo;
                if(ch=='=')
                {
                    sym=becomes;//����Ǹ�ֵ�Ļ�������sym=19
                    getchdo;
                }
                else
                {
                    sym=colon; 
                }
            }
            else
            {
                if(ch=='<')
                {
                    getchdo;
                    if(ch=='=')
                    {
                        sym=leq;
                        getchdo;
                    }
                    else
                    {
                        sym=lss;
                    }
                 }
                 else
                 {
                    if(ch=='>')
                    {
                        getchdo;
                        if(ch=='=')
                        {
                            sym=geq;
                            getchdo;
                        }
                        else
                        {
                            sym=gtr;
                        }
                    }
                    else
                    {
                        sym=ssym[ch];//��������������ʱȫ�����յ��ַ����Ŵ���
                        if(sym!=period)
                        {
                            getchdo;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int gen(enum fct  x,int y,int z)
{
    if(cx>=cxmax)//cxmax=200
    {
        printf("Program too long");
        return -1;
    }
    code[cx].f=x;
    code[cx].l=y;
    code[cx].a=z;
    cx++ ;
    return 0;
}
int test(bool* s1,bool * s2,int n)
{
    if(!inset(sym,s1))
    {
        error(n);
        //����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ������������Ҫ�ļ��ϻ򲹾ȼ���
        while((!inset(sym,s1))&&(!inset(sym,s2)))
        {
            getsymdo;
        }
    }
    return 0;
}
//cc=cx=ll=0;cx��ʾԴ�����д���ĵڼ���
//block(0,0,nxtlev)����������
//nxtlev�����в���ֵΪ1����СΪ32
int block(int lev,int tx,bool* fsys)//lev��ʾ�ֳ������ڲ㣬tx��ʾ���ű�ǰָ�룬fsys��ʾ��ǰģ�������ż�
{
    int i;
    int dx;
    int tx0;
    int cx0;
    //�½�һ��
    bool nxtlev[symnum];

    dx=3;//Ĭ��Ϊ3��ԭ��������
    tx0=tx;//��¼�������ֵĳ�ʼλ��
    table[tx].adr=cx;//table���tx������ʾ�Ľṹ���adr���ֵ�ǵ�ǰ��cx�������棨jmp,0,0�����
    gendo(jmp,0,0);
    if(lev>levmax)//levmax=3�����Ըñ����������Ƕ��Ϊ3��
    {
        error(32);
    }
    do{
        //�����ǰ�ַ���constant�����ַ�����table��
        if(sym==constsym)
        {
            //��ȡconst��ĳ�������
            getsymdo;
            do{
                //#define constdeclarationdo(a,b,c)
                //if(-1==constdeclaration(a,b,c))
                //  return -1
                constdeclarationdo(&tx,lev,&dx);//tx=0,lev=0,dx=3;
                while(sym==comma)//����Ƕ��ţ����ʾ��һ�����ʴ�Ϊ�����������е�
                {                //��ͬһ�еĳ�����������table����
                    getsymdo;
                    constdeclarationdo(&tx,lev,&dx);
                }
                if(sym==semicolon)//Ϊ�ֺŵ�ʱ�򣬱�ʾ�����Ķ�ȡ��������ȡ��һ�����Ŵ�
                {
                    getsymdo;
                }
                else
                {
                    error(5);//ÿ�б����ԷֺŽ�����������ǣ��򱨴�
                }
            }while(sym==ident);
        }

        //�����ǰ�ַ��Ǳ��������ַ�����table�У���ʽͬ�������룬������治�������������Ĵ���
        if(sym==varsym)
        {
            getsymdo;
            do{
                vardeclarationdo(&tx,lev,&dx);
                while(sym==comma)
                {
                    getsymdo;
                    vardeclarationdo(&tx,lev,&dx);
                }
                if(sym==semicolon)
                {
                    getsymdo;
                }
                else
                {
                    error(5);
                }
            }while(sym==ident);
        }

        //�����ǰ�ַ����ӹ��̣����ַ�����table��
        while(sym==procsym)
        {
            getsymdo;
            if(sym==ident)
            {
                enter(procedur,&tx,lev,&dx);
                getsymdo;
            }
            else
            {
                error(4);
            }
            // �ݴ�dx
            save_dx = dx;
            // dx���3, ʹ�ô��α������ӹ�����dx=3��λ�ÿ�ʼ��¼
            dx = 3;
            int tx_cp = tx+1;
            if (sym==lparen){
            	param_num = 0;
				do{
            		getsymdo;
            		if(sym==ident){
            			vardeclarationdo(&tx, lev+1, &dx); 
            			param_num++;
            		}
            	}while(sym == comma);

            	if (sym==rparen)
            	{
            		getsymdo;
            	}else{
            		error(55);		// ���̴����﷨����
            	}
				// ��¼���̲���������val
				table[tx_cp-1].val = param_num;
            }
            // ��ֵ����
            dx = save_dx;

            //�ӹ��̵�����������һ���ֺţ��������
            if(sym==semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);
            }
            memcpy(nxtlev,fsys,sizeof(bool) *symnum);//��fsys��ָ���ڴ��׵�ַ����СΪ32�����ݷ���nxtlev��
                                                     //��ʵ���ǰѳ�ʼ���õ�nxtlev�����ݸ��Ƶ���������ڴ���ȥ
            nxtlev[semicolon]=true;//�ֳ���ĳ��������Ϊ��
            if(-1==block(lev+1,tx,nxtlev))//�ݹ���ã������ӹ��̣���ʱlev+1��
            {
                return -1;
            }
            tx = tx_cp-1;
            if (sym==semicolon)
            {
                getsymdo;
                memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
                nxtlev[ident]=true;
                nxtlev[procsym]=true;
                //#define testdo(a,b,c)
                //if(-1==test(a,b,c))
                //return -1
                testdo(nxtlev,fsys,6);//���Ժ���������������Ƿ�Ϸ�
            }
            else
            {
                error(5);
            }
        }

        //nxtlev��ʾ��俪ʼ�ķ��ż�
        memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
        nxtlev[ident]=true;
        nxtlev[period]=true;
        testdo(nxtlev,declbegsys,7);//���Ե�ǰ�����Ƿ��Ǻ�̷��ż��е�Ԫ��
                                    //����������ҵ�һ���ǵ�λ��
                                    //����һ���ֳ���Ŀ�ʼ�޷�����俪ʼ���ż���������ʼ���ż�
    }while(inset(sym,declbegsys));//���û�����������ŵ��ַ������˳�ѭ��
    //jmp 0 CX��ʾ��ת��CX      ��λ��ִ�д��룬���������ģ�����ĵ�һ��ָ��
    code[table[tx0].adr].a=cx;//���ǰ���̴���ĵ�ַ��ǰ���(jmp,0,0)3����

    table[tx0].adr=cx;
    table[tx0].size=dx;
    cx0=cx;
    //ģ����������ռ���䣬��ʾ������Ĵ洢�ռ䣬���ڴ洢����
    gendo(inte,0,dx);
    if(tableswitch)//�Ƿ��г�table���е�����
    {
        printf("TABLE:\n");
        if(tx0+1>tx)//��table��Ϊ��ʱ��ֻ���NULL
        {
            printf("NULL\n");
        }
        for(i=tx0+1;i<=tx;i++)
        {
            switch(table[i].kind)
            {
                case constant:
                printf("%d const %s",i,table[i].name);
                printf(" val=%d\n",table[i].val);
                fprintf(fas,"%d const %s",i,table[i].name);
                fprintf(fas," val=%d\n",table[i].val);
                break;
                case variable:
                printf("%d var %s",i,table[i].name);
                printf(" lev=%d addr=%d\n",table[i].level,table[i].adr);
                fprintf(fas,"%d var %s",i,table[i].name);
                fprintf(fas," lev=%d addr=%d\n",table[i].level,table[i].adr);
                break;
                case procedur:
                printf("%d proc %s",i,table[i].name);
                printf(" lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
                fprintf(fas,"%d proc %s",i,table[i].name);
                fprintf(fas," lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
                break;
                
				case array:
				printf("%d arr %s", i, table[i].name);
				printf("lev=%d addr=%d size=%d startid=%d\n", table[i].level, table[i].adr, table[i].size, table[i].startid);
				fprintf(fas, "%d arr %s", i, table[i].name);
				fprintf(fas, "lev=%d addr=%d size=%d startid=%d\n", table[i].level, table[i].adr, table[i].size, table[i].startid);
				break;
            }
        }
        printf("\n");
    }
    memcpy(nxtlev,fsys,sizeof(bool) *symnum);//������ż��ϰ����ϲ������ż���
    nxtlev[semicolon]=true;
    nxtlev[endsym]=true;
    statementdo(nxtlev,&tx,lev);//������䴦�����lev��ʾ�㣬nxtlev��ʾ��̷��ż�
    gendo(opr,0,0);
    memset(nxtlev,0,sizeof (bool) *symnum);//���������޲��ȼ���
    testdo(fsys,nxtlev,8);//�������������ȷ�ԣ�8�����������ĺ�����Ų���ȷ
    listcode(cx0);//�о�Ŀ�����
    return 0;
}

//�ú����Ĺ����ǽ�Դ�����еĳ������������ֳ�����Ŵ����뵽table����
//��table���Ǵ��±�1��ʼ�ģ�0�ſռ�����������;
void enter(enum object k,int* ptx ,int lev ,int * pdx)
{
    (*ptx)++;
    strcpy(table[(*ptx)].name,id);
    table[(*ptx)].kind=k;
    switch (k)
    {
        case constant:
        if(num>amax)//amax=2047,��const�����Ϊ2047
        {
            error(31);
            num=0;
        }
        table[(*ptx)].val=num;
        break;
        case variable:
        table[(*ptx)].level=lev;
        table[(*ptx)].adr=(*pdx);
        (*pdx)++;
        break;
        case  procedur:
        table[(*ptx)].level=lev;
        break;       
		
		case array:
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		break;
		
    }
}
//��table���в���idt��λ�ã�������������λ��
int position(char * idt,int tx)
{
    int i;
    strcpy(table[0].name,idt);
    i=tx;
    while(strcmp(table[i].name,idt)!=0)
    {
        i--;
    }
    return i;
}


int constdeclaration(int * ptx,int lev,int * pdx)
{
    //�����ǰ�ķ�����һ�������������ִ��
    if(sym==ident)
    {
        getsymdo;
        if(sym==eql||sym==becomes)
        {
            if(sym==becomes)
            error(1);
            getsymdo;
            if(sym==number)
            {
                enter(constant,ptx,lev,pdx);
                getsymdo;
            }
            else
            error(2);
        }
        else
        error(3);
    }
    else
    error(4);
    return 0;
}

int vardeclaration(int * ptx,int lev ,int * pdx)
{
    if (sym == ident)
	{
		// ��д���ֱ��ı��ջ֡������
		int startid = 0, endid = 0;
		enter(variable, ptx, lev, pdx);//��д���ֱ�
		(*pdx)++;
		getsymdo;
		if (sym == lparen) {
			getsymdo;
			switch (sym) {
			case ident:
				startid = table[position(id, *ptx)].val;
				break;
			case number:
				startid = num;
				break;
			case plus:
				getsymdo;
				startid = num;
				break;
			case minus:
				getsymdo;
				startid = -num;
				break;
			}
			table[(*ptx)].adr = table[(*ptx)].adr - startid;
			table[(*ptx)].startid = startid;
			getsymdo;
			if (sym != colon) {
				error(30);
			}
			else {
				getsymdo;
				switch (sym) {
				case ident:
					endid = table[position(id, *ptx)].val;
					table[(*ptx)].size = endid - startid + 1;
					break;
				case number:
					endid = num + 1;
					table[(*ptx)].size = endid - startid + 1;
					break;
				case plus:
					getsymdo;
					endid = num + 1;
					table[(*ptx)].size = endid - startid + 1;
					break;
				case minus:
					getsymdo;
					endid = -num + 1;
					table[(*ptx)].size = endid - startid + 1;
					break;
				}
				table[(*ptx)].kind = array;
				(*pdx) = (*pdx) + endid - startid + 1;	//���������еĸ���dx
				getsymdo;
				getsymdo;
			}
		}
	}
	else
	{
		error(4);
	}
	return 0;
}

//�о�ģ����򣬼�Ŀ�����
void listcode(int cx0)
{
    int i;
    if(listswitch)
    {
        for(i=cx0;i<cx;i++)
        {
            printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
            fprintf(fa,"%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
        }
    }
}
//�ݹ��ȡ����ÿһ����䣬�ú����Ὣ��������Ĵ󲿷��ӳ�����ϵ����
//�ó����д󲿷��ǵݹ���ã���˵����ܶ࣬����������һ����ʽ�����Զ�������������
//�����˵���Ƚϼ�
//���ڴ���ʵ���ϲ��Ĳ��֣���û���������ע��
int statement(bool* fsys,int * ptx,int lev)
{
    int i,cx1,cx2;
    bool nxtlev[symnum];
    //�����ǰ�ַ�����һ������������������һ�����ʽ
    if (sym == ident)   /* ׼�����ո�ֵ��䴦�� */
	{
		i = position(id, *ptx);
		if (i == 0)
		{
			error(11);  /* ����δ�ҵ� */
		}
		else
		{
			if(table[i].kind != variable)
			{
				error(12);  /* ��ֵ����ʽ���� */
				i = 0;
			}
			else
			{
				getsymdo;
				if(sym == becomes)
				{
					getsymdo;
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					expressiondo(nxtlev, ptx, lev); /* ����ֵ�����Ҳ���ʽ */
					if (i != 0)
					{
						/* expression��ִ��һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ */
						gendo(sto, lev - table[i].level, table[i].adr);
					}
				}
				else if (sym == lparen) {		// ������...
					getsymdo;
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					expressiondo(nxtlev, ptx, lev);
					// parseExpression������һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ
					//��expression����Ѿ��������:nextSym();	//һ��)
					getsymdo;	//һ��:=
					//�����var��ֵ��ͬ,����������ɵ����
					getsymdo;
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					expressiondo(nxtlev, ptx, lev);
					// parseExpression������һϵ��ָ������ս�����ᱣ����ջ����ִ��sto������ɸ�ֵ
					gendo(sta, lev - table[i].level, table[i].adr);
				}
				else
				{
					error(13);  /* û�м�⵽��ֵ���� */
				}
					
			}
		}//if (i == 0)
	}
    else
    {
        if(sym==readsym)
        {
            getsymdo;
            //read���������������ţ���ʽ��read(X,y);
            if(sym!=lparen)
            {
                error(34);
            }
            else
            {
                do {
					getsymdo;
					if (sym == ident)
					{
						i = position(id, *ptx); /* ����Ҫ���ı��� */
					}
					else
					{
						i=0;
					}
					if (i == 0)
					{
						error(35);  /* read()��Ӧ���������ı����� */
					}
					else
					{
						if (table[i].kind == constant || table[i].kind == procedur) {
							error(32);		// read()�еı�ʶ�����Ǳ���
						}
						else {
							getsymdo;
							if (sym != lparen) {			//������,������
								gendo(opr, 0, 16);
								gen(sto, lev - table[i].level, table[i].adr);
							}
							else {		//TODO ����
								getsymdo;
								memcpy(nxtlev, fsys, sizeof(bool) * symnum);
								nxtlev[rparen] = true;
								expressiondo(nxtlev, ptx, lev);
								int ltmp = lev - table[i].level;
								int adrtmp = table[i].adr;
								gendo(arrcheck, table[i].startid, table[i].size);
								gendo(jpc, 0, 0);
								gendo(opr, 0, 16);
								gendo(sta, ltmp, adrtmp);
								getsymdo;
							}
						}
					}
				 
				} while (sym == comma);
            }
            //����read�����Ƕ���ʱ��������������ţ���ʾ��ǰ����Ľ���
            if(sym!=rparen)
            {
                error(33);
                while(!inset(sym,fsys))
                {
                    getsymdo;
                }
            }
            else
            {
                getsymdo;
            }
        }
        else//����read
        {
            if(sym==writesym)//write(exp,exp)
            {
                getsymdo;
                if(sym==lparen)
                {
                    do
                    {
                        getsymdo;
                        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                        nxtlev[rparen]=true;
                        nxtlev[comma]=true;
                        //��ʾ������ʽ��ֵ
                        expressiondo(nxtlev,ptx,lev);
                        gendo(opr,0,14);//��ջ����ֵ�������Ļ
                    }while(sym==comma);
                    if(sym!=rparen)
                    {
                        error(33);
                    }
                    else
                    {
                        getsymdo;
                    }
                }
                gendo(opr,0,15);//write����Ҫ��Ļ������С�
            }
            else
            {
                //����call����������һ������ident
                if(sym==callsym)
                {
                    getsymdo;
                    if(sym!=ident)
                    {
                        error(14);
                    }
                    else
                    {
                        i=position(id,*ptx);
                        if(i==0)
                        {
                            error(11);
                        }
                        else
                        {
                            if(table[i].kind==procedur)
                            {
                            	getsymdo;
								if(sym == lparen){
									// ��¼��������
									param_num = 0;
									do{
										getsymdo;
										memcpy(nxtlev, fsys, sizeof(bool)*symnum);
										expressiondo(nxtlev,ptx,lev);
										param_num++;
									}while(sym == comma);

									if(sym ==rparen){
										getsymdo;
									}else{
										error(55);		// ���̴����﷨����
									}
								}else{
									error(55);		// ���̴����﷨����
								}
								if (param_num != table[i].val){
									error(56);		// ���̵��ò�����һ��
								}
								// ��ջ��A��Ԫ��������3��λ�ã�Ϊ���̵��������ڳ�λ�ã�t-A
								gendo(mov, 0, param_num);
                                gendo(cal,lev-table[i].level,table[i].adr);
                            }
                            else
                            {
                                error(15);
                            }
                        }
                        getsymdo;
                    }
                }
                else
                {
                    if(sym==ifsym)
                    {
                        getsymdo;
                        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                        nxtlev[thensym]=true;
                        nxtlev[dosym]=true;
                        //if�Ӿ��к����������ʽ������Ҫ���������ӹ���
                        conditiondo(nxtlev,ptx,lev);
                        //�������ʽ�󣬱����then��䣬��ʾ��������ʱ��Ҫִ�еĴ���
                        if(sym==thensym)
                        {
                            getsymdo;
                        }
                        else
                        {
                            error(16);
                        }
                        cx1=cx;
                        gendo(jpc,0,0);//��תת�ƣ�ջ��Ϊ��ʱ����
                        //then�������һ�����
                        statementdo(fsys,ptx,lev);
                        if(sym == elsesym){
                        	getsymdo;
                        	cx2=cx;
                        	gendo(jmp,0,0);
                        	code[cx1].a=cx;
                        	statementdo(fsys,ptx,lev);
                        	code[cx2].a=cx;
						}else{
							code[cx1].a=cx;//�������Ϊ�٣�����ת��cx��λ��
						}
                    }
                    else
                    {
                        if(sym==beginsym)
                        {
                            getsymdo;
                            memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                            nxtlev[semicolon]=true;
                            nxtlev[endsym]=true;

                            statementdo(nxtlev,ptx,lev);
                            //����Ĺ��̱�����˼����
                            //ÿһ���������󶼸�һ���ֺţ�
                            //����������һ���ֺţ������������һ�����
                            //�����һ������ֱ�Ӹ�����һ������򱨴����������
                            while(inset(sym,statbegsys)||sym==semicolon)
                            {
                                if(sym==semicolon)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(10);
                                }
                                statementdo(nxtlev,ptx,lev);
                            }
                            if(sym==endsym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(17);
                            }
                        }
                        else
                        {
                            if(sym==whilesym)
                            {
                                cx1=cx;
                                getsymdo;
                                memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                                nxtlev[dosym]=true;
                                conditiondo(nxtlev,ptx,lev);
                                cx2=cx;
                                gendo(jpc,0,0);
                                if(sym==dosym)
                                {
                                   getsymdo;
                                }
                                else
                                {
                                    error(18);
                                }
                                statementdo(fsys,ptx,lev);
                                gendo(jmp,0,cx1);
                                code[cx2].a=cx;//��������ѭ���ĵ�ַ
                            }
                            else
                            {
                                if(sym == repeatsym){
                                	getsymdo;
                                	cx1=cx;
                                	memcpy(nxtlev,fsys,sizeof(bool) * symnum);
                                	nxtlev[untilsym] = true;
                                	statementdo(fsys,ptx,lev);
                                	
                                	if(sym == untilsym){
                                		getsymdo;
                                		conditiondo(nxtlev,ptx,lev);
                                		gendo(jpc,0,cx1);
									}else{
										memset(nxtlev,0,sizeof(bool)*symnum);
                                		testdo(fsys,nxtlev,19);//19������ķ��Ų���ȷ
									}
								}
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int expression_r(bool * fsys,int * ptx,int lev)
{
    enum symbol addop;//���ڼ�¼��ǰ������+����-
    bool nxtlev[symnum];
    //���ʽ�ĵ�һ�������ǡ�+�������ǡ�-��
    if(sym==plus||sym==minus)
    {
        addop=sym;
        getsymdo;
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[plus]=true;
        nxtlev[minus]=true;
        termdo(nxtlev,ptx,lev);//�������Ĺ���
        if(addop==minus)
        {
            gendo(opr,0,1);//ջ��ֵȡ��
        }
    }
    else//���������+��-��ʼ�ģ���ֱָ�ӽ�����Ĵ������
    {
        memcpy(nxtlev,fsys,sizeof(bool) * symnum);
        nxtlev[plus]=true;
        nxtlev[minus]=true;
        termdo(nxtlev,ptx,lev);
    }
    //ѭ��������Ĵ�����̣�֪�������е������ϣ��˳�ѭ��
    while(sym==plus||sym==minus)
    {
        addop=sym;
        getsymdo;
        memcpy(nxtlev,fsys,sizeof(bool) * symnum);
        nxtlev[plus]=true;
        nxtlev[minus]=true;
        termdo(nxtlev,ptx,lev);
        if(addop==plus)
        {
            gendo(opr,0,2);
        }
        else
        {
            gendo(opr,0,3);
        }
    }
    return 0;
}

//����ÿһ��Ĵ���
int term(bool* fsys,int* ptx,int lev)
{
    enum symbol mulop;
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)* symnum);
    nxtlev[times]=true;
    nxtlev[slash]=true;
    factordo(nxtlev,ptx,lev);//�������ӵĴ������
    //ѭ���������ӵĴ�����̣�֪�������е����Ӵ�����ϣ��˳�ѭ��
    while(sym==times||sym==slash)
    {
        mulop=sym;
        getsymdo;
        factordo(nxtlev,ptx,lev);
        if(mulop==times)//��ǰ����ǳ˺ţ���opr 0 4
        {
            gendo(opr,0,4);
        }
        else            //�����ǳ���
        {
            gendo(opr,0,5);
        }
    }
    return 0;
}

int factor (bool* fsys,int *ptx,int lev)
{
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);//�жϵ�ǰ�Ƿ������ӵ��﷨��ʼ���ţ�������ǣ����ҵ�һ�����ʵ�
                              //��ʼ���ż���ִ��
    while(inset(sym,facbegsys))//�����ǰ�������ӵĿ�ʼ���ţ����˳��ú���
    {
        //�����ǰ�ַ���һ�������Ļ�
        if(sym==ident)
        {
            i=position(id,*ptx);
            if(i==0)
            {
                error(11);
            }
            else
            {
                switch(table[i].kind)
                {
                    case  constant:
                    gendo(lit,0,table[i].val);//����ǰ��������ջ��
                    break;
                    case  variable:
                    gendo(lod,lev-table[i].level,table[i].adr);
                    break;
                    case   procedur://�����в��ܺ��й��̵Ķ���
                    error(21);
                    break;
					case array:
					getsymdo;
					if (sym == lparen)
					{					//������
						int ltmp = lev - table[i].level;
						int adrtmp = table[i].adr;
						getsymdo;
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						nxtlev[rparen] = true;
						expressiondo(nxtlev, ptx, lev);
						gendo(lda, ltmp, adrtmp);
						//getsymdo;							
					}
					if (sym == rparen)
					{
						//getsymdo;
					}
					break;
                }
            }
            getsymdo;
        }
        else
        {
            if(sym==number)
            {
                if(num>amax)//������ִ�������ܱ�ʾ�����֣��򱨴�amax=2047
                {
                    error(31);
                    num=0;
                }
                gendo(lit,0,num);
                getsymdo;
            }
            else
            {
                //�����ǰ��һ�������ţ����ʾ����һ�����ʽ�Ĵ������
                if(sym==lparen)
                {
                    getsymdo;
                    memcpy(nxtlev,fsys,sizeof(bool)*symnum);
                    nxtlev[rparen]=true;
                    //��������������һ�����ʽ
                    expressiondo(nxtlev,ptx,lev);
                    if(sym==rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);
                    }
                }
                testdo(fsys,facbegsys,23);//��ǰ����Ҫô�����ӵĿ�ʼ���ţ�Ҫô�������Ŀ�ʼ����
            }
        }
    }
    return 0;
}



//��������в����ж���ż�ԣ������жϱ��ʽ֮��Ĵ�С��ϵ
int condition(bool*fsys,int * ptx,int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];
    //���������ǡ�odd���Ļ�����ʾ�ж��Ƿ�������
    if(sym==oddsym)
    {
        getsymdo;
        expressiondo(fsys,ptx,lev);
        gendo(opr,0,6);
    }
    else
    {
        memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[eql]=true;
        nxtlev[neq]=true;
        nxtlev[lss]=true;
        nxtlev[leq]=true;
        nxtlev[gtr]=true;
        nxtlev[geq]=true;
        //����Ǳ��ʽ�Ļ�
        expressiondo(nxtlev,ptx,lev);
        //���ʽ����ʽ֮������ӷ���=��#��<��<=��>��>=
        //������ǵĻ��������
        if(sym!=eql&&sym!=neq&&sym!=lss&&sym!=leq&&sym!=gtr&&sym!=geq)
        {
            error(20);
        }
        else
        {
            relop=sym;
            getsymdo;
            //��һ�����ʽ
            expressiondo(fsys,ptx,lev);
            switch(relop)
            {
                case eql:
                gendo(opr,0,8);
                break;
                case neq:
                gendo(opr,0,9);
                break;
                case lss:
                gendo(opr,0,10);
                break;
                case geq:
                gendo(opr,0,11);
                break;
                case gtr:
                gendo(opr,0,12);
                break;
                case leq:
                gendo(opr,0,13);
                break;
            }
        }
    }
    return 0;
}


void interpret()
{
    int p,b,t,iter;
    struct instruction i;//��ŵ�ǰָ��
    //��������ʱ����ջS����α�29ҳ��ͼ���ƣ�ջ��СΪ500
    int s[stacksize];
    //��ʾ��ʼִ��Դ����
    printf("start pl0\n");
    t=0;//ջ��ָ��
    b=0;//ָ���ַ
    p=0;//��ʾcode�е�ָ��
    //ͷ����Ϊ0����ʾ����SL��DL��RA��Ϊ0
    s[0]=s[1]=s[2]=0;
    do{
        i=code[p];//code�д�ŵľ���Դ�����ģ�����ȡ��ÿһ��ָ��
        p++;//ָ��ָ����һ��ָ��

        //����Ĵ������ģ����������
        //�ڿ�����Ĵ���֮ǰ�����ȿ�23ҳ��ָ��˵��
        switch(i.f)
        {
            case lit:
            s[t]=i.a;
            t++;
            break;
            case opr:
            switch(i.a)
            {
                case 0:
                t=b;
                p=s[t+2];
                b=s[t+1];
                break;
                case 1:
                s[t-1]-=s[t-1];
                break;
                case 2:
                t--;
                s[t-1]=s[t-1]+s[t];
                break;
                case 3:
                t--;
                s[t-1]=s[t-1]-s[t];
                break;
                case 4:
                t--;
                s[t-1]=s[t-1]*s[t];
                break;
                case 5:
                t--;
                s[t-1]=s[t-1]/s[t];
                break;
                case 6:
                s[t-1]=s[t-1]%2;
                break;
                case 8:
                t--;
                s[t-1]=(s[t-1]==s[t]);
                break;
                case 9:
                t--;
                s[t-1]=(s[t-1]!=s[t]);
                break;
                case 10:
                t--;
                s[t-1]=(s[t-1]<s[t]);
                break;
                case 11:
                t--;
                s[t-1]=(s[t-1]>=s[t]);
                break;
                case 12:
                t--;
                s[t-1]=(s[t-1]>s[t]);
                break;
                case 13:
                t--;
                s[t-1]=(s[t-1]<=s[t]);
                break;
                case 14:
                printf("%d",s[t-1]);
                fprintf(fa2,"%d",s[t-1]);
                t--;
                break;
                case 15:
                printf("\n");
                fprintf(fa2,"n");
                break;
                case 16:
                printf("?");
                fprintf(fa2,"?");
                scanf("%d",&(s[t]));
                fprintf(fa2,"%d\n",s[t]);
                t++;
                break;
            }
            break;
            case lod://ȡ��Ե�ǰ���̵����ݻ�ַΪa���ڴ��ֵ��ջ��
            s[t]=s[base(i.l,s,b)+i.a];
            t++;
            break;
            case sto://ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪa���ڴ�
            t--;
            s[base(i.l,s,b)+i.a]=s[t];
            break;
            case cal://�����ӹ���
            s[t]=base(i.l,s,b);//�������̻���ַ��ջ
            s[t+1]=b;//�������̻���ַ��ջ
            s[t+2]=p;//����ǰָ��ָ����ջ
            b=t;//�ı����ַָ��ֵΪ�¹��̵Ļ���ַ
            p=i.a;//��ת
            break;
            case inte://�����ڴ�
            t+=i.a;
            break;
            case jmp://ֱ����ת
            p=i.a;
            break;
            case jpc://������ת
            t--;
            if(s[t]==0)
            {
                p=i.a;
            }
            break;           
			case sta:                        // ջ����ֵ�浽�����±�Ϊa���ڴ�
			t--;
			s[base(i.l, s, b) + i.a + s[t - 1]] = s[t];
			t--;
			break;
			case lda:
			s[t - 1] = s[base(i.l, s, b) + i.a + s[t - 1]];       //������Ķ�ȡ �ڶ���S[T]�Ƿ���ƫ�Ƶ�ַ��				
			break;
			case arrcheck:
			s[t] = i.a;
			if ((s[t - 1] < i.l) || (s[t - 1] > s[t] + i.l - 1))
			{
				printf("Error:Out of the array's size!\n");
				fprintf(fa2, "Error:Out of the array's size!\n");
				s[t] = 0;
			}
			else {
				s[t] = 1;
			}
			t++;
			break;
			case mov:           // ��ջ��A��Ԫ��������3��λ�ã�Ϊ���̵��������ڳ�λ�ã�t-A
            for(iter=i.a ; iter>0 ; iter--){
                s[t+2] = s[t-1];
                t--;
            }
            break;

        }
    }while(p!=0);
}
//�ú����Ĺ�����ͨ������ַ����һ���̵Ļ�ַ
int base(int l,int * s,int b)
{
    int b1;
    b1=b;
    while(l>0)
    {
        b1=s[b1];
        l--;
    }
    return b1;
}
