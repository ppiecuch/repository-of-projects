#pragma once
#include "ParticleEmiter.h"
class CPointEmiter :
	public CParticleEmiter
{

public:
    int  getType();                                                         //得到发射器类型
	int  emit(int now,int timeSinceLastCall,CSparticle*& outArray);       //点发射器的操作
	void setZ(int Z);
	CPointEmiter(void);
	~CPointEmiter(void);
};
