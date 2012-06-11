#pragma once
#include "particleemiter.h"

class CBoxEmiter :
	public CParticleEmiter
{
private:
	
	int length,width;        //���η������ĳ��Ϳ�
	int minLength,minWidth;  //��С����ߴ�
public:
	//���ض�д������д����������
   // void Save(irr::io::IXMLWriter * XmlWriter);
	//void Open(irr::io::IXMLReader * XmlReader);

	void  load(IReadStream *FileReader);
    int  getType();                                                         //�õ�����������
	int emit(int now,int timeSinceLastCall,CSparticle*& outArray);
	void setLengthAndWidth(int length_,int width_);
    void setMinLengthAndWidth(int minLength_,int minWidth_);
	CBoxEmiter(void);
	~CBoxEmiter(void);
};
