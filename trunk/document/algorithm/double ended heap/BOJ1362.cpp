/*双端堆(Deap, double-ended heap) 
双端堆是支持双端优先队列的插入、删除最小元素和最大元素等操作的堆。 
双端堆可以在对数时间内完成上述所有的操作。 
 
双端堆是一颗完全二叉树，该完全二叉树要么为空，要么同时满足下列性质： 
1.根结点不包含元素 
2.左子树是一个最小堆 
3.右子树是一个最大堆 
4.如果右子树不空，则令i是左子树中任意一个结点，是i在右子树中的对应结点。如果i在右子树中的对应结点j不存在，则令j为i的父亲结点在右子树中的对应结点。对于结点i和j，结点i的关键字值小于等于结点j的关键字值。 
 
例如： 
          [            ] 
         /              \ 
      [ 5 ]             [45] 
     /     \           /    \ 
  [10]      [8]       [25] [40] 
 /    \    /   \     / 
[15] [19] [9] [30] [20] 
 
双端堆是实现双端优先队列的最好方法之一。 
1.它是完全二叉树，所以空间O(n)而且无须记录孩子的指针，通过位置可直接判断逻辑关系。 
2.它是完全二叉树，所以树高是严格的log(2,n)，操作的时间复杂度也是严格的log(2,n) 
  而一个具有n个结点的平衡二叉树的高度h为： 
  log(2,(N+1)) <= h <= log(a,(sqrt(5)*(N+1)))-2 
  其中a=(1＋sqrt(5))/2 
所以双端堆在时间和空间上都是双端优先队列的严格下限，效率也会比较出众。 
 
下面是BOJ1362的G++代码。 
BOJ1362 执行时间136MS 使用空间288K 
已实现为容器模板类。供大家参考。 
 
由于快速位扫描函数使用了AT&T内联汇编，只能使用G++进行编译，不能使用C++。 
如果希望使用C++编译，请如下替换unsigned int bits(unsigned int i)这个方法。 
unsigned int bits(unsigned int i) 
{ 
  __asm 
  { 
    BSR EAX , i 
    MOV i , EAX 
  } 
  return i; 
} 
*/
#include<iostream>
using namespace std;

template <class TYPE>
class DHeap
{
  private:
    TYPE *heap;
    unsigned int max;
    unsigned int cur;
    void insL(const TYPE& e,unsigned int i)
    {
      unsigned int j=i>>1;
      while(j>1&&heap[j]>e)
      {
        heap[i]=heap[j];
        i=j;
        j>>=1;
      }
      heap[i]=e;
    }
    void insR(const TYPE& e,unsigned int i)
    {
      unsigned int j=i>>1;
      while(j>1&&e>heap[j])
      {
        heap[i]=heap[j];
        i=j;
        j>>=1;
      }
      heap[i]=e;
    }
    unsigned int bits(unsigned int i)
    {
      __asm("bsrl %1, %0"
           :"=r"(i)
           :"r"(i)
           );
      return i;
    }
    unsigned int partner(unsigned int i)
    {
      return i^(1<<(bits(i)-1));
    }
    unsigned int inR(unsigned int i)
    {
      return i&(1<<(bits(i)-1));
    }
  public:
    DHeap(unsigned int num)
    {
      max=num;
      heap=new TYPE[max+2];
      cur=1;
    }
    DHeap(){DHeap(128);}
    ~DHeap(){delete [] heap;}
    const TYPE& operator[] (unsigned int i)
    {return i>cur?heap[0]:heap[i];}
    void Clear(){cur=1;}
    void SetZero(const TYPE& e){heap[0]=e;}
    void SetInf(const TYPE& e){heap[1]=e;}
    unsigned int Len(){return cur;}
    void Double()
    {
      unsigned int i;
      TYPE *temp;
      max<<=1;
      temp=new TYPE[max+2];
      for(i=0;i<=cur;++i)
        temp[i]=heap[i];
      delete [] heap;
      heap=temp;
    }
    void Push(const TYPE& e)
    {
      unsigned int i,j;
      if(cur+1>max) Double();
      ++cur;
      if(cur==2)
        heap[2]=e;
      else
      {
        if(inR(cur))
        {
          i=partner(cur);
          if(e>heap[i])
            insR(e,cur);
          else
          {
            heap[cur]=heap[i];
            insL(e,i);
          }
        }
        else
        {
          i=partner(cur)>>1;
          if(heap[i]>e)
            insL(e,cur);
          else
          {
            heap[cur]=heap[i];
            insR(e,i);
          }
        }
      }
    }
    TYPE Pop_min()
    {
      if(cur<2) return heap[0];
      TYPE a=heap[2];
      TYPE& b=heap[cur--];
      unsigned int i=2,j=4;
      while(j<=cur)
      {
        if(j<cur&&heap[j]>heap[j+1])
          ++j;
        heap[i]=heap[j];
        i=j;
        j<<=1;
      }
      j=partner(i);
      if(j>cur&&cur>2)j>>=1;
      if(b>heap[j])
      {
        heap[i]=heap[j];
        insR(b,j);
      }
      else
        insL(b,i);
      return a;
    }
    TYPE Pop_max()
    {
      if(cur<2) return heap[1];
      if(cur<4) return heap[cur--];
      TYPE a=heap[3];
      TYPE& b=heap[cur--];
      unsigned int i=3,j=6;
      while(j<=cur)
      {
        if(j<cur&&heap[j+1]>heap[j])
          ++j;
        heap[i]=heap[j];
        i=j;
        j<<=1;
      }
      j=partner(i);
      if((j<<1)<=cur)
      {
        j<<=1;
        if(j<cur&&heap[j+1]>heap[j])
          ++j; 
      }
      if(b>heap[j])
        insR(b,i);
      else
      {
        heap[i]=heap[j];
        insL(b,j);
      }
      return a;
    }
    void ShowTree()
    {
      for(unsigned int i=1;i<=cur;i++)
      {
        if(!(i&(i+1))) cout<<heap[i]<<endl;
        else cout<<heap[i]<<",";
      }
      if(cur&(cur+1))cout<<endl;
    }
};

struct PPP
{
  int k,p;
  bool operator> (const PPP& x) const
  {return p>x.p;}
  friend ostream& operator<< (ostream& o,PPP& x)
  {
    o<<"["<<x.k<<","<<x.p<<"]";
    return o;
  }
}X;


main()
{
  int i,j,k,p,n;
  DHeap<PPP> h(8192);
  X.k=X.p=0;
  h.SetZero(X);
  h.SetInf(X);
  while(scanf("%d",&n)&&n)
  {
    if(n==1)
    {
      scanf("%d%d",&k,&p);
      X.k=k;X.p=p;
      h.Push(X);
    }
    else if(n==2)
      printf("%d\n",h.Pop_max().k);
    else if(n==3)
      printf("%d\n",h.Pop_min().k);
  }
}

