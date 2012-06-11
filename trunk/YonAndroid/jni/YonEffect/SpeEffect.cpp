
#include "SpeEffect.h"

CSpeEffect::CSpeEffect(void)
{

	m_SpeEffect.p_BillBoard=NULL;
	m_SpeEffect.p_ParticleSystem=NULL;
    
	enable=true;
	startTime=0;
	endTime=100000;
	timeHasStart=0;
}

CSpeEffect::~CSpeEffect(void)
{
	 
	if(m_SpeEffect.p_BillBoard)
		delete m_SpeEffect.p_BillBoard;

	if(m_SpeEffect.p_ParticleSystem)
		delete m_SpeEffect.p_ParticleSystem;



}


void CSpeEffect::load(c8 * filePath)
{



	int fileType=getFileType(filePath);
	if(fileType==-1)return ;
	if(!driver)return;


	stringc filePath2="";
	if(fileType==1)
		filePath2="exportBillBoard/";
	if(fileType==2)
		filePath2="exportParticle/";

	stringc fileName=filePath;
	filePath2=filePath2+fileName;
	
	IReadStream * p_FileReader=driver->getFileSystem()->createAndOpenReadFileStream(filePath2);

	if(fileType==1)
	{
		speType=BILLBOARD;

		if(!m_SpeEffect.p_BillBoard)
			m_SpeEffect.p_BillBoard=new CBillBoardAmt();

		m_SpeEffect.p_BillBoard->setDriver(driver);

		m_SpeEffect.p_BillBoard->load(p_FileReader);


	}

	if (fileType==2)
	{

		speType=PARTICLE;

		if(!m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem=new CParticleSystem();

		//���û�������
		m_SpeEffect.p_ParticleSystem->setDriver(driver);

		m_SpeEffect.p_ParticleSystem->load(p_FileReader);


	}

	//fileReader.close();

	p_FileReader->drop();



}

void CSpeEffect::load( IReadStream *FileReader )
{

	position2df pos;
	pos.x=FileReader->readFloat();
	pos.y=FileReader->readFloat();

	startTime=FileReader->readInt();
	endTime=FileReader->readInt();

	bool playAlways=FileReader->readBool();


	//��������·��
	char * chs=NULL;
	int length=0;
	length=FileReader->readInt();
	if(length!=0)
	{

		length=1000;
		chs=new char[length+1];
		for (int i=0;i<length+1;i++)
		{
			chs[i]=FileReader->readByte();
			if(chs[i]=='\0')
				break;


		}

		
	
	}

	if(chs!=NULL)
		load(chs);



	setPos(position2di(pos.x,pos.y));
	setPlayAlways(playAlways);


	if(chs)
	delete []chs;



}


void CSpeEffect:: update(int timediff)
{

    //�������ֹ��ֱ�ӷ���
    if(!enable)return;
    
	//����Ч�ѱ�����
	//if(timeHasStart>endTime)return;


	//����
	timeHasStart+=timediff;


	//����Ѿ�������ʼʱ��,��ʼ��������

	if(timeHasStart<startTime)return;

	if(speType==AROLA)
	{

            
	}


	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		    m_SpeEffect.p_BillBoard->update(timediff);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->update(timediff);
	
	}


    
}




void CSpeEffect::draw()
{

	//�������ֹ��ֱ�ӷ���
    if(!enable)return;
    

	//��û��ʼ��

	if(timeHasStart<startTime)return;

	//����Ч�ѱ�����
	//if(timeHasStart>endTime)return;




    if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		    m_SpeEffect.p_BillBoard->draw();

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->draw();
	
	}




}



//���û�������
void CSpeEffect::setDriver(IVideoDriver *driver)
{
   
		this->driver=driver;

}





//����λ��
void CSpeEffect ::setPos(position2di pos)
{

	
   //
	if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		    m_SpeEffect.p_BillBoard->setPos(pos);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->setPos(position2df(pos.x,pos.y));
	
	}




}

position2df  CSpeEffect ::getPos()
{

	if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		    return position2df(m_SpeEffect.p_BillBoard->getPos().x,m_SpeEffect.p_BillBoard->getPos().y);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
	    return  m_SpeEffect.p_ParticleSystem->getPos();
	
	}


	return position2df(0,0);

}



//���ùر�����
void CSpeEffect::setDisable(bool enable,bool reset)
{
     
	this->enable=enable;

	if(reset)
		reSet();

}



void CSpeEffect::reSet()
{


  timeHasStart=0;

  //������ӵĵ�����
  if(speType==PARTICLE)
	  if(m_SpeEffect.p_ParticleSystem)
		  m_SpeEffect.p_ParticleSystem->clear();


  if(speType==BILLBOARD)
	  if(m_SpeEffect.p_BillBoard)
		  if(m_SpeEffect.p_BillBoard->draw_model!=-1)
		  {
			  m_SpeEffect.p_BillBoard->time_since_start=0;
			  m_SpeEffect.p_BillBoard->draw_model=1;
		  }	  


}



//������ʼʱ��
void CSpeEffect::setStartTime(int time)
{

    
	startTime=time;
	

	//���ø������ݵ�����

	setLife(endTime-startTime);
	

}


//������ֹʱ��

void CSpeEffect ::setEndTime(int time)
{
 
	endTime=time;

	//���ø������ݵ�����
 
	setLife(endTime-startTime);



}


//��������
void   CSpeEffect ::setLife(int life)
{




	if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		    m_SpeEffect.p_BillBoard->setTimeLength(life);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->setEmiteLife(life);
	
	}





}



int   CSpeEffect ::getLife()
{


  if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
		return    m_SpeEffect.p_BillBoard->time_length;

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			return m_SpeEffect.p_ParticleSystem->getEmiteLife();
	
	}


	return 3000;








}


//�����Ƿ�ѭ������
void CSpeEffect::setPlayAlways(bool playAlways)
{



  if(speType==AROLA)
	{

            

	}


	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
			if(playAlways)
		    m_SpeEffect.p_BillBoard->draw_model=-1;
			else
				m_SpeEffect.p_BillBoard->draw_model=1;

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->setEmiteAlways(playAlways);
	
	}




}

//�����Ƿ�һֱ����
bool  CSpeEffect::getPlyaAlways()
{


	if(speType==AROLA)
	{

            

	}



	if(speType==BILLBOARD)
	{
	 
		if(m_SpeEffect.p_BillBoard)
            if(m_SpeEffect.p_BillBoard->draw_model==-1)
				return true;
			else
				return false;
			

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			return m_SpeEffect.p_ParticleSystem->getEmiteAlways();
	
	}





}





int CSpeEffect::getFileType( c8 * filePath )
{

	if(core::hasFileExtension(filePath,"bil"))
		return 1;
	if(core::hasFileExtension(filePath,"par"))
		return  2;

	return -1;

}

void CSpeEffect::setZ( int Z )
{


	if(speType==AROLA)
	{



	}



	if(speType==BILLBOARD)
	{

		if(m_SpeEffect.p_BillBoard)
			m_SpeEffect.p_BillBoard->setZ(Z);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->setZ(Z);

	}







}

void CSpeEffect::setAngle( f32 angle )
{



	if(speType==AROLA)
	{



	}



	if(speType==BILLBOARD)
	{

		if(m_SpeEffect.p_BillBoard)
			m_SpeEffect.p_BillBoard->setOrgAngel(angle);

	}



	if(speType==PARTICLE)
	{

		if(m_SpeEffect.p_ParticleSystem)
			m_SpeEffect.p_ParticleSystem->setAngle(angle*180/PI);

		

	}





}

