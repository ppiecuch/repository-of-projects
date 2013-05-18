#ifndef _IPROCEDURE_H_
#define _IPROCEDURE_H_

#include "yon.h"
using namespace yon;
class IProcedure : public core::IReferencable{
public:
	//�Ƿ���Ҫ����
	virtual bool needProcess(void* data=NULL){return true;}
	//Ԥ����
	virtual void preProcess(void* data=NULL){}
	//���ش�����
	virtual bool process(void* data) = 0;
	//����
	virtual void postProcess(void* data=NULL){}
	//�ɷ񴫵ݵ���һ��
	virtual bool canPass(void* data=NULL){return true;}
};
#endif