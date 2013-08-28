#ifndef _List_H_
#define _List_H_
template <class T,int size=1,int increment=1>
class List
{
	int Size;
	int ElementNumber;
public:
	T*  (*Index);
	T* operator [] (int index)
	{
		return this->Index[index];
	}
	List()
	{
		this->Index=new T*[size];
		this->Size =size;
		this->ElementNumber =0;
		for(int i=0;i<this->Size-1 ;i++)
		{
			this->Index [i]=0;
		}
		
	}
	~List()
	{
		delete[] this->Index ;
	}
	int  GetSize()
	{
		return this->Size;
	}
	unsigned int  GetElementNumber()
	{
		return this->ElementNumber;
	}
	void AddElement(T* t)    
	{
		if((this->ElementNumber+1) >this->Size )
		{
			T* (*temp)=new T*[Size+increment];
			for (int i=0;i!=this->ElementNumber;i++)
			{
				temp[i]=this->Index [i];
			}
			delete[] Index;
			this->Index=temp;
			this->Index [this->ElementNumber]=t;
			this->ElementNumber +=1;
			this->Size +=increment;
		}
		else
		{
			this->Index [this->ElementNumber]=t;
			this->ElementNumber +=1;
		}
	}
	void InsertElement(T *t,int index)
	{
		if(index>=this->Size ) return;
		int tempSize;
		if((this->ElementNumber +1)>this->Size)
		{
			tempSize=this->Size +increment;
		}
		else
		{
			tempSize =this->Size;
		}
			T* (*temp)=new T*[tempSize];
			for(int i=0;i!=index ;i++)
			{
				temp[i]=this->Index [i];
			}
			temp[index]=t;
			for(int i=index;i!=this->Size ;i++)
			{
				temp[i+1]=this->Index [i];
			}
		delete[] Index;
		this->Index =temp;
		this->ElementNumber +=1;
		this->Size =tempSize ;
		
	}
	void DeleteElement(int index)
	{
		if(index >=this->ElementNumber||index<0 ) return;
		for(int i=index;i!=this->Size;i++)
		{
			this->Index [i]=this->Index [i+1];
		}
		this->ElementNumber -=1;
		this->Index [this->ElementNumber]=0;
		if(this->ElementNumber/this->Size>3)
		{
			T* (*temp)=new T*[2*this->ElementNumber];
			for(int i=0;i!=this->ElementNumber;i++)
			{
				temp[i]=this->Index[i];
			}
			delete[] this->Index;
			this->Index=temp;
			this->Size=this->ElementNumber*2;
		}
	}
};
#endif