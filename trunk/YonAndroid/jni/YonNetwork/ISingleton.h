
#ifndef __I_SINGLETON__H_
#define __I_SINGLETON__H_


template<class T> 
class CSingleton
{
public:
	static T* GetInstance()	
	{	
		return ManagerInstance();
	}
	static void	ReleaseInstance()
	{
		ManagerInstance( true );
	}

protected:

	static T* ManagerInstance( bool destroy = false )
	{
		static T* m_sInstance; //

		if( destroy )
		{
			delete m_sInstance;
		}
		else
		{
			if( !m_sInstance )
			{
				m_sInstance = new T();
			}
		}

		return m_sInstance;
	}

	virtual ~CSingleton() {};
	CSingleton() {};
};

#endif