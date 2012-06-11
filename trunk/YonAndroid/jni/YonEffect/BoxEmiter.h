#pragma once
#include "particleemiter.h"

class CBoxEmiter :
	public CParticleEmiter
{
private:
	
	int length,width;        //方形发射器的长和宽
	int minLength,minWidth;  //最小发射尺寸
public:
	//重载读写函数，写入特有数据
   // void Save(irr::io::IXMLWriter * XmlWriter);
	//void Open(irr::io::IXMLReader * XmlReader);

	void  load(IReadStream *FileReader);
    int  getType();                                                         //得到发射器类型
	int emit(int now,int timeSinceLastCall,CSparticle*& outArray);
	void setLengthAndWidth(int length_,int width_);
    void setMinLengthAndWidth(int minLength_,int minWidth_);
	CBoxEmiter(void);
	~CBoxEmiter(void);
};
