#ifndef _POINTEMITER_H_
#define _POINTEMITER_H_

#include "ParticleEmiter.h"
class CPointEmiter :
	public CParticleEmiter
{

public:
    int  getType();                                                         //�õ�����������
	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);       //�㷢�����Ĳ���
	void setZ(int Z);
	CPointEmiter(void);
	~CPointEmiter(void);
};

#endif