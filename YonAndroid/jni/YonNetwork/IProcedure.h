#ifndef _IPROCEDURE_H_
#define _IPROCEDURE_H_

#include "yon.h"
using namespace yon;
class IProcedure : public core::IReferencable{
public:
	//是否需要处理
	virtual bool needProcess(void* data=NULL){return true;}
	//预处理
	virtual void preProcess(void* data=NULL){}
	//返回处理结果
	virtual bool process(void* data) = 0;
	//后处理
	virtual void postProcess(void* data=NULL){}
	//可否传递到下一步
	virtual bool canPass(void* data=NULL){return true;}
};
#endif