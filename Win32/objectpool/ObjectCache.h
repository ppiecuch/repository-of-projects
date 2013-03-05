/************************************************************************/
/* @Desc	:       */
/* @Anthor	: Guo Huafeng												*/
/* @Date	: 2013-2													*/
/************************************************************************/
#ifndef __OBJECT_CACHE_H__
#define __OBJECT_CACHE_H__

#include <vector>
#include <map>

const int kPoolDefaultSize = 10;

template <typename TObject>
class CObjectCache 
{
public:
	CObjectCache(int cacheSize = kPoolDefaultSize)
	{
		init(cacheSize);
	}
	~CObjectCache()
	{
		mIdList.clear();
		mHitCountList.clear();
		
		for (int i = 0; i < mMaxSize; i++) 
			destoryObject(mObjectList[i]);

		mObjectList.clear();
	}

public:
	typedef std::vector<TObject*>	VecObjectPtr;
	typedef std::vector<int>		VecInt;

	void init(int maxSize)
	{
		mMaxSize = maxSize;
		mObjectList.resize(maxSize);
		mIdList.resize(maxSize);
		mHitCountList.resize(maxSize);
	}

	TObject* getObject(int id)
	{
		VecInt::iterator it = std::find(mIdList.begin(), mIdList.end(), id);
		if (it != mIdList.end())
		{
			mHitCountList[it - mIdList.begin()]++;
			return mObjectList[it - mIdList.begin()];
		}
		else
		{
			int index = getLRUObject();
			destoryObject(mObjectList[index]);
			TObject* obj = createObject(id);
			if (obj != 0)
				cacheObject(index, obj, id);
			
			return obj;
		}
	}

protected:
	// 子类需要重写创建和销毁对象的方法
	virtual TObject* createObject(int id)		= 0;
	virtual void destoryObject(TObject* obj)	{}

private:
	// Get "Least Recently Used" Object
	int getLRUObject()			
	{
		int minHit = mHitCountList[0];
		int index = 0;
		for (int i = 1; i < mMaxSize; i++)
		{
			if (mHitCountList[i] < minHit)
			{	index = i;	minHit = mHitCountList[i];	}

			if (minHit <= 0)	break;
		}

		return index;
	}

	void cacheObject(int index, TObject* obj, int id)
	{
		mObjectList[index] = obj;
		mIdList[index] = id;
		mHitCountList[index] = 1;
	}


private:
	
	VecObjectPtr		mObjectList;
	VecInt				mIdList;
	VecInt				mHitCountList;

	int					mMaxSize;

};


#endif	//__OBJECT_CACHE_H__
