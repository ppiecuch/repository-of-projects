#include"stdio.h"
#include"string.h"
#include"malloc.h"
#include"stdlib.h"
#define TRUE 1
#define FALSE 0
#define OVERFLOW 0
#define OK 1
#define ERROR 0

#define STACK_INIT_SIZE 10//存储空间初始化分配量
#define STACK_INCREMENT 2//存储空间分配增量

typedef struct page{
	int pagebig;
	char data[100];
	int count;
	int front;
	int rear;
	struct page *next;
}pagequeue;


typedef char  SElemType;  
typedef struct  { 
	//栈定义：LRU
	SElemType *base; 
	SElemType *top;  
	int stacksize;  
}SqStack;



int num;//物理块个数
void Init()
{
	printf("输入物理块个数: ");
	scanf("%d",&num);
	if(num<=0){
		printf("输入错误!\n");
		return;
	}
}
void InitQueue(pagequeue *q)
{
	//初始化队列
/*	printf("输入物理块个数: ");
	scanf("%d",&num);
	if(num<=0){
		printf("输入错误!\n");
		return;
	}
*/	
	q->pagebig=num;
	q->count=0;
	q->front=0;
	q->rear=0;
}

int QueueEmpty(pagequeue *q)
{
	if(q->count==0)
		return 1;
	else
		return 0;
}

int QueueFull(pagequeue *q)
{
	if(q->count==num)
		return 1;
	else
		return 0;
}

void EnQueue(pagequeue *q,char x)
{
	//入队
	if(QueueFull(q))
	{
		printf("队列已满!\n");
		return;
	}
	q->count++;
	q->data[q->rear]=x;
	q->rear=(q->rear+1)%num;
}

char DeQueue(pagequeue *q)
{
	//出队
	char x;
	if(QueueEmpty(q))
	{
		printf("队列为空!\n");
		exit(-1);
	}
	x=q->data[q->front];
	q->front=(q->front+1)%num;
	q->count--;
	return x;
}

void InitStack(SqStack &S)
{  //构造空栈
	S.base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!S.base)
		exit(OVERFLOW);

	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
void DestoryStack(SqStack &S){  
	free(S.base);//释放栈
	S.top=S.base=NULL;
	S.stacksize=0;
}

void Push(SqStack &S,SElemType e){ 
	if(S.top-S.base==S.stacksize) //栈满
	{  
		S.base=(SElemType*)realloc(S.base,(S.stacksize+STACK_INCREMENT)*sizeof(SElemType));
		if(!S.base) exit(OVERFLOW);
		S.top=S.base+S.stacksize;//修改栈底指针
		S.stacksize+=STACK_INCREMENT;
	}
	*(S.top)++=e;//将e入栈，成为新的栈顶元素
}

char Pop(SqStack &S){
	if(S.top==S.base) 
		return ERROR;
	char e=*--S.top;//将栈底元素赋给e，栈顶指针下移
	return e;
}

void StackTraverse(SqStack S,void(*visit)(SElemType)){ 
	while(S.top>S.base) 
		visit(*S.base++);
}
void print(SElemType e)
{ printf(" %c",e);
}

int compare(int a[ ],int m)
{
	int j;
	int max,temp=0;
	max=a[0];
	for(j=1;j<=m;j++){
		if(max<a[j]){
			max=a[j];
			temp=j;
			
		}
	}
	
	return temp;
}


void FIFO(pagequeue *q,char page[],int length)
{
	//中心算法
	int i=1,c=0,flag,j,t,n;
	char buffer[20];
	
	for(i=0;i<num;i++)
	{
		//前几个入队
		EnQueue(q,page[i]);
		printf("			内存装入情况：");
		for(int m=0;m<i+1;m++)
			printf("%c",page[m]);
		printf("\n");
		//printf("%c ",page[i]);打印队列中的最好
	}
	
	
	for(i=num;i<length;i++)
	{
		//后几个开始判断找出要淘汰的页面
		flag=0;
		j=0;
		while(!QueueEmpty(q))
		{
			//直到队列为空
			buffer[j]=DeQueue(q);//出队放入到buffer数组中去
			j++;//计数器
		}
		n=j;
		
		
		for(t=0;t<n;t++)
			if(page[i]==buffer[t])
			{
				//如果要访问的页面已经存在，不需要置换
				flag=1;
				printf(" 无需置换！\n");
				for(t=0;t<n;t++){
					EnQueue(q,buffer[t]);//从buffer中取出，再入队
				}
				break;
			}
			
			if(flag==0)
			{
				//要访问的页面不存在，需要置换
				for(t=0;t<n;t++)
					EnQueue(q,buffer[t]);//从buffer中取出，再入队
				printf(" 要置换的是：%c",DeQueue(q));
				EnQueue(q,page[i]);
				printf("  入队的是：%c\n",page[i]);
				c++;
			}			
			
			printf("			内存装入情况：");
			j=0;
			while(!QueueEmpty(q)){
				buffer[j]=DeQueue(q);
				printf(" %c",buffer[j]);//出队，打印出每个页面号--------只是为了打印。。
				
				j++;
			}
			printf("\n");
			
			for(t=0;t<n;t++) {
				EnQueue(q,buffer[t]);//从buffer中取出(不是真实的取出)，再入队
				//	printf("是：%d,%c;",t,buffer[t]);
				
			}
			
	}

}

void Opt(char block[],char page[],int length){
	//OPt算法
	int i=1,c=0,flag,t,j;

	int times[20];//存放出现次数的数组


	for(i=0;i<num;i++)
	{
		//前几个入数组中
		block[i]=page[i];
		times[i]=20;
		printf("			内存装入情况：");
		for(int n=0;n<=num;n++)
			printf(" %c",block[n]);
		printf("\n");
	}
	

	
	flag=0;
	for(i=num;i<length;i++)
	{
		
		for(j=0;j<num;j++){
			if(block[j]==page[i])
				flag=2;
		}

		if(flag!=2){
			for(j=0;j<num;j++){
				for(int k=num+i-2;k<length;k++){
					if(block[j]==page[k]){
						times[j]=k;//记录出现的下标
						flag=1;
						break;
					}
				}
			}
		}

		if(flag==1){
		

			t=compare(times,num);//最大值的下标
			printf(" 要被替换的是：%c",block[t]);
			block[t]=page[i];//替换掉
			printf(" 新进入的是：%c\n",page[i]);
			
		}
		else{
			printf(" 无需置换!\n");
		}

	
			
		flag=0;
		for(int k=0;k<num;k++){
			times[k]=20;
		}

		printf("			内存装入情况：");
		for(k=0;k<num;k++)
			printf(" %c",block[k]);
		printf("\n");
		printf("\n");

	}
}

void LRU(SqStack *S,char page[],int length){
	//LRU算法
	int i=1,c=0,flag,n,j;
	SqStack buffer;
	InitStack(buffer);//临时存放数据的栈
	char temp,e='?';
	
	for(i=0;i<num;i++)
	{
		//前几个入zhan
		Push(*S,page[i]);
		printf("			内存装入情况：");
		StackTraverse(*S,print);
		printf("\n");
	}
	
	
	for(i=num;i<length;i++)
	{

		printf("\n 待比较的是%c\n",page[i]);
		//后几个开始判断找出要淘汰的页面
		flag=0;
		j=0;
		while(S->top!=S->base)
		{
			
			temp=Pop(*S);//出zhan
			//printf("   出栈的是%c\n",temp);
			if(temp==page[i]){
				//要访问的存在
				e=temp;
				flag=1;
			}
			else {
				
				Push(buffer,temp);
				//flag=1;
				//printf("要置换的是：%c\n",temp);
			}
			j++;
		}
		n=j;
		

		if(flag==1){
			while(buffer.top!=buffer.base)
			{
			
				//要访问的存在于内存中,将它放在栈顶
				temp=Pop(buffer);
				Push(*S,temp);
				//printf(" 要出栈的是：%c\n",e);
				
			}
			Push(*S,e);
				printf(" 存在，现在栈顶是：%c\n",e);
		}
		else{
			Pop(buffer);
			while(buffer.top!=buffer.base)
			{
				//要访问的不存在，淘汰出栈底的，将要访问的放在栈顶
				temp=Pop(buffer);
				Push(*S,temp);
			}
			Push(*S,page[i]);
			printf(" 不存在，新入栈的是：%c\n",page[i]);
		}
			
			
		
		printf("			内存装入情况：");
		StackTraverse(*S,print);
		printf("\n");
	}
}

void main()
{
    FILE *fp;
	


	
	char page[20],c;
	int i=0,t;
	



	printf("从文件中读入页面走向:\n");
	gets(page);
    if((fp=fopen("D:\\page.txt","r+"))==NULL)
	{
		printf("文件page.txt无法打开!\n");
		exit(-1);
	}

	i=0;
	while(!feof(fp))
	{
		fscanf(fp,"%c",&c);
		if(c>='0'&&c<='9')
		{
			page[i]=c;
			putchar(c);
			printf(" ");
			i++;
		}
		
	}
	printf("\n");


	t=1;
	while(t!=0){

		printf("**************************页面置换算法模拟**************************\n");
		printf("请选择算法： 1、最佳置换算法  2、FIFO页面置换算法  3、LRU置换算法  0、退出 \n");
		scanf("%d", &t);
		if(t!=0)
			Init();//初始化

		switch(t)
		{
		case 1:	
			{
				char block[50];//物理块
						
				for(i=0;i<50;i++){
					block[i]='\0';
				}
				Opt(block,page,20);
				break;
			}
		case 2: 
			{
				pagequeue q;
				InitQueue(&q);
				FIFO(&q,page,20);
				break;
			}
		case 3:
			{
				SqStack S;
				InitStack(S);
				LRU(&S,page,20);
				break;
			}
		default:
			break;
		}

	}
	printf("您已退出，谢谢使用！");
	fclose(fp);

}
