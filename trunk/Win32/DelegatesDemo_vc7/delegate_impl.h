#define COMBINE(a,b)           COMBINE1(a,b)
#define COMBINE1(a,b)          a##b

#define _YON_I_DELEGATE				COMBINE(IDelegate, SUFFIX)
#define _YON_C_STATIC_DELEGATE      COMBINE(CStaticDelegate, SUFFIX)
#define _YON_C_METHOD_DELEGATE      COMBINE(CMethodDelegate, SUFFIX)
#define _YON_C_DELEGATE             COMBINE(CDelegate, SUFFIX)

template<class R TEMPLATE_PARAMS>
class _YON_I_DELEGATE
{
public:
    virtual ~_YON_I_DELEGATE() {}
    virtual R invoke(PARAMS) = 0;
    virtual bool equal(_YON_I_DELEGATE<R TEMPLATE_ARGS>* pDelegate) = 0;
};


template<class R TEMPLATE_PARAMS>
class _YON_C_STATIC_DELEGATE : public _YON_I_DELEGATE<R TEMPLATE_ARGS>
{
public:
    typedef R (*PFunc)(PARAMS);
    _YON_C_STATIC_DELEGATE(PFunc pFunc) { m_pFunc = pFunc; }
    virtual R invoke(PARAMS) { return m_pFunc(ARGS); }
    virtual bool equal(_YON_I_DELEGATE<R TEMPLATE_ARGS>* pDelegate)
    {
        _YON_C_STATIC_DELEGATE<R TEMPLATE_ARGS>* pStaticDel =
            dynamic_cast<_YON_C_STATIC_DELEGATE<R TEMPLATE_ARGS>*>(pDelegate);
        if(pStaticDel == NULL || pStaticDel->m_pFunc != m_pFunc)
            return false;

        return true;
    }

private:
    PFunc m_pFunc;
};


template<class TObj, class R TEMPLATE_PARAMS>
class _YON_C_METHOD_DELEGATE : public _YON_I_DELEGATE<R TEMPLATE_ARGS>
{
public:
    typedef R (TObj::*PMethod)(PARAMS);
    _YON_C_METHOD_DELEGATE(TObj* pObj, PMethod pMethod)
    {
        m_pObj = pObj;
        m_pMethod = pMethod;
    }
    virtual R invoke(PARAMS) { return (m_pObj->*m_pMethod)(ARGS); }
    virtual bool equal(_YON_I_DELEGATE<R TEMPLATE_ARGS>* pDelegate)
    {
        _YON_C_METHOD_DELEGATE<TObj, R TEMPLATE_ARGS>* pMethodDel =
            dynamic_cast<_YON_C_METHOD_DELEGATE<TObj, R TEMPLATE_ARGS>* >(pDelegate);
        if
        (
            pMethodDel == NULL ||
            pMethodDel->m_pObj != m_pObj ||
            pMethodDel->m_pMethod != m_pMethod
        )
        {
            return false;
        }

        return true;
    }

private:
    TObj *m_pObj;
    PMethod m_pMethod;
};


template<class R TEMPLATE_PARAMS>
_YON_I_DELEGATE<R TEMPLATE_ARGS>* createDelegate(R (*pFunc)(PARAMS))
{
    return new _YON_C_STATIC_DELEGATE<R TEMPLATE_ARGS>(pFunc);
}

template <class TObj, class R TEMPLATE_PARAMS>
_YON_I_DELEGATE<R TEMPLATE_ARGS>* createDelegate(TObj* pObj, R (TObj::*pMethod)(PARAMS))
{
    return new _YON_C_METHOD_DELEGATE<TObj, R TEMPLATE_ARGS> (pObj, pMethod);
}


template<class R TEMPLATE_PARAMS>
class _YON_C_DELEGATE
{
public:
    typedef _YON_I_DELEGATE<R TEMPLATE_ARGS> IDelegate;
    typedef std::list<IDelegate*> DelegateList;

    _YON_C_DELEGATE(IDelegate* pDelegate = NULL) { add(pDelegate); }
    ~_YON_C_DELEGATE() { removeAll(); }
    bool empty() { return (m_DelegateList.empty()); }

    _YON_C_DELEGATE<R TEMPLATE_ARGS>& operator=(IDelegate* pDelegate)
    {
        removeAll();
        add(pDelegate);
        return *this;
    }
    
    _YON_C_DELEGATE<R TEMPLATE_ARGS>& operator+=(IDelegate* pDelegate)
    {
        add(pDelegate);
        return *this;
    }

    _YON_C_DELEGATE<R TEMPLATE_ARGS>& operator-=(IDelegate* pDelegate)
    {
        remove(pDelegate);
        return *this;
    }

    R operator()(PARAMS)
    {
        return invoke(ARGS);
    }

private:
    void add(IDelegate* pDelegate)
    {
        if(pDelegate != NULL)
            m_DelegateList.push_back(pDelegate);
    }

    void remove(IDelegate* pDelegate)
    {
        DelegateList::iterator it;
        for(it = m_DelegateList.begin(); it != m_DelegateList.end(); ++it)
        {
            if((*it)->equal(pDelegate))
            {
                delete (*it);
                m_DelegateList.erase(it);
                break;
            }
        }
    }

    void removeAll()
    {
        DelegateList::iterator it;
        for(it = m_DelegateList.begin(); it != m_DelegateList.end(); ++it)
            delete (*it);

        m_DelegateList.clear();
    }

    R invoke(PARAMS)
    {
        DelegateList::const_iterator it;
        for(it = m_DelegateList.begin(); it != --m_DelegateList.end(); ++it)
		{
			 (*it)->invoke(ARGS);
			 printf("%08X,invoke:%08X\r\n",this,*it);
		}

		 printf("%08X,invoke:%08X\r\n",this,*it);
        return m_DelegateList.back()->invoke(ARGS);
    }

private:
    DelegateList m_DelegateList;
};

#undef COMBINE
#undef COMBINE1

#undef _YON_I_DELEGATE
#undef _YON_C_STATIC_DELEGATE
#undef _YON_C_METHOD_DELEGATE
#undef _YON_C_DELEGATE