
#include "SpeEffectSet.h"


CSpeEffectSet::CSpeEffectSet(void)
{
}

CSpeEffectSet::~CSpeEffectSet(void)
{
	clear();

}




//更新
void CSpeEffectSet::update( int timediff )
{

	for(int i=0;i<SpeArray.size();i++)
	{      
		if(SpeArray[i])
			SpeArray[i]->update(timediff);       

	}



}

void CSpeEffectSet::draw()
{
	
	for(int i=0;i<SpeArray.size();i++)
	{

		if(SpeArray[i])
			SpeArray[i]->draw();       


	}

}



// void CSpeEffectSet::save( IXMLWriter *XmlWriter )
// {
// 
// 	XmlWriter->writeXMLHeader();
// 
// 	XmlWriter->writeElement(L"SpeEffect");
// 
// 	for (int i=0;i<SpeArray.size();i++)
// 	{
// 		if(SpeArray[i]&&SpeArray[i]->enable)
// 		SpeArray[i]->save(XmlWriter);
// 	}
// 
// 	XmlWriter->writeClosingTag(L"SpeEffect");
// 	
// 
// }
// 
// void CSpeEffectSet::open( IXMLReader *XmlReader )
// { 
// 	
// 	SpeArray.clear();
// 	const stringw SpeEffect=L"SpeEffect";
// 	const stringw SpeEffectFactor=L"SpeEffectFactor";
//          while(XmlReader->read())
// 		 {
// 
// 			 if(io::EXN_ELEMENT==XmlReader->getNodeType())
// 			 {
// 
// 				 if(SpeEffectFactor==XmlReader->getNodeName())
// 				 {
// 					 
// 					CSpeEffect * NewSpeEffect=new CSpeEffect();
// 					
// 					position2df pos;
// 					pos.X=XmlReader->getAttributeValueAsFloat(L"posX");
// 					pos.Y=XmlReader->getAttributeValueAsFloat(L"posY");
// 					NewSpeEffect->tempPos=pos;
// 
// 					NewSpeEffect->startTime=XmlReader->getAttributeValueAsInt(L"startTime");
// 					NewSpeEffect->endTime=XmlReader->getAttributeValueAsInt(L"endTime");
// 
// 					//NewSpeEffect->setPlayAlways();
// 					
// 					  NewSpeEffect->tempPlayAlways=(bool)XmlReader->getAttributeValueAsInt(L"playAways");
// 
// 					NewSpeEffect->file=XmlReader->getAttributeValue(L"filePath");
// 
// 					SpeArray.push_back(NewSpeEffect);
// 
// 					 
// 
// 				 }
// 			 
// 			 
// 			 
// 			 }
// 		 		 
// 		}
// 		 
// 
// }
//清除



void CSpeEffectSet::clear()
{

	//删除所有特效
	for(int i=0;i<SpeArray.size();i++)
	{
		if(SpeArray[i])
			delete SpeArray[i];
		SpeArray[i]=NULL;
	}

}

void CSpeEffectSet::load( IReadStream *FileReader )
{
	
	
	clear();
	pos.set(0,0);
	int size=0;

	size=FileReader->readInt();

	for(int i=0;i<size;i++)
	{

		CSpeEffect  *NewSpeEffect=new CSpeEffect();

		NewSpeEffect->setDriver(driver);
		NewSpeEffect->setGraphicsAdapter(gfAdapter);

		NewSpeEffect->load(FileReader);

		//做一次数据转换
		position2df tempPos=NewSpeEffect->getPos();
        NewSpeEffect->setPos(position2di(tempPos.x,1000-tempPos.y));

		SpeArray.push_back(NewSpeEffect);



	}

	


	reCalculatePos();




}

void CSpeEffectSet::setPos( position2di pos )
{
	position2di offsetPos=pos-this->pos;
	for(int i=0;i<SpeArray.size();i++)
	{
		if(SpeArray[i])
		{
			position2df  spePos=SpeArray[i]->getPos();

			SpeArray[i]->setPos(position2di(spePos.x+offsetPos.x,spePos.y+offsetPos.y));

		}
	}
	this->pos=pos;
}

core::position2di CSpeEffectSet::getPos()
{

	return pos;
}

void CSpeEffectSet::reCalculatePos()
{
	
	pos.set(0,0);
	int  accessC=0;;
	for(int i=0;i<SpeArray.size();i++)
		if(SpeArray[i])
		{
			position2df spePos=SpeArray[i]->getPos();


			pos.x+=spePos.x;
			pos.y+=spePos.y;

			accessC++;

		}


		if(accessC!=0)
		{

			pos.x/=accessC;

			pos.y/=accessC;


		}

	


}

void CSpeEffectSet::setZ( int Z )
{

	for (int i=0;i<SpeArray.size();i++)
	{
		if(SpeArray[i])
		SpeArray[i]->setZ(Z);
	}

}


//