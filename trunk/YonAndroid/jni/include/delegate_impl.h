
#include "IReferencable.h"

#define _YON_I_DELEGATE				YON_COMBINE(IDelegate, _YON_DELEGATE_SUFFIX)
#define _YON_C_STATIC_DELEGATE      YON_COMBINE(CStaticDelegate, _YON_DELEGATE_SUFFIX)
#define _YON_C_METHOD_DELEGATE      YON_COMBINE(CMethodDelegate, _YON_DELEGATE_SUFFIX)
#define _YON_C_DELEGATE             YON_COMBINE(CDelegate, _YON_DELEGATE_SUFFIX)

template<class R _YON_DELEGATE_TEMPLATE_PARAMS>
class _YON_I_DELEGATE : public virtual core::IReferencable
{
public:
    virtual ~_YON_I_DELEGATE() {}
    virtual R invoke(_YON_DELEGATE_PARAMS) = 0;
    virtual bool equal(_YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* pDelegate) = 0;
};


template<class R _YON_DELEGATE_TEMPLATE_PARAMS>
class _YON_C_STATIC_DELEGATE : public _YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>
{
public:
    typedef R (*PFunc)(_YON_DELEGATE_PARAMS);
    _YON_C_STATIC_DELEGATE(PFunc pFunc) { m_pFunc = pFunc; }
    virtual R invoke(_YON_DELEGATE_PARAMS) { return m_pFunc(_YON_DELEGATE_ARGS); }
    virtual bool equal(_YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* pDelegate)
    {
        _YON_C_STATIC_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* pStaticDel =
            dynamic_cast<_YON_C_STATIC_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>*>(pDelegate);
        if(pStaticDel == NULL || pStaticDel->m_pFunc != m_pFunc)
            return false;

        return true;
    }

private:
    PFunc m_pFunc;
};


template<class TObj, class R _YON_DELEGATE_TEMPLATE_PARAMS>
class _YON_C_METHOD_DELEGATE : public _YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>
{
public:
    typedef R (TObj::*PMethod)(_YON_DELEGATE_PARAMS);
    _YON_C_METHOD_DELEGATE(TObj* pObj, PMethod pMethod)
    {
        m_pObj = pObj;
        m_pMethod = pMethod;
    }
    virtual R invoke(_YON_DELEGATE_PARAMS) { return (m_pObj->*m_pMethod)(_YON_DELEGATE_ARGS); }
    virtual bool equal(_YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* pDelegate)
    {
        _YON_C_METHOD_DELEGATE<TObj, R _YON_DELEGATE_TEMPLATE_ARGS>* pMethodDel =
            dynamic_cast<_YON_C_METHOD_DELEGATE<TObj, R _YON_DELEGATE_TEMPLATE_ARGS>* >(pDelegate);
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


template<class R _YON_DELEGATE_TEMPLATE_PARAMS>
_YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* createDelegate(R (*pFunc)(_YON_DELEGATE_PARAMS))
{
    return new _YON_C_STATIC_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>(pFunc);
}

template <class TObj, class R _YON_DELEGATE_TEMPLATE_PARAMS>
_YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>* createDelegate(TObj* pObj, R (TObj::*pMethod)(_YON_DELEGATE_PARAMS))
{
    return new _YON_C_METHOD_DELEGATE<TObj, R _YON_DELEGATE_TEMPLATE_ARGS> (pObj, pMethod);
}


template<class R _YON_DELEGATE_TEMPLATE_PARAMS>
class _YON_C_DELEGATE
{
public:
    typedef _YON_I_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS> IDelegate;
    typedef core::list<IDelegate*> DelegateList;

    _YON_C_DELEGATE(IDelegate* pDelegate = NULL) { add(pDelegate); }
    ~_YON_C_DELEGATE() { removeAll(); }
    bool empty() { return (m_DelegateList.empty()); }

    _YON_C_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>& operator=(IDelegate* pDelegate)
    {
        removeAll();
        add(pDelegate);
        return *this;
    }
    
    _YON_C_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>& operator+=(IDelegate* pDelegate)
    {
        add(pDelegate);
        return *this;
    }

    _YON_C_DELEGATE<R _YON_DELEGATE_TEMPLATE_ARGS>& operator-=(IDelegate* pDelegate)
    {
        remove(pDelegate);
        return *this;
    }

    R operator()(_YON_DELEGATE_PARAMS)
    {
        return invoke(_YON_DELEGATE_ARGS);
    }

private:
    void add(IDelegate* pDelegate)
    {
        if(pDelegate != NULL)
            m_DelegateList.push_back(pDelegate);
    }

    void remove(IDelegate* pDelegate)
    {
        typename DelegateList::Iterator it;
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
        typename DelegateList::Iterator it;
        for(it = m_DelegateList.begin(); it != m_DelegateList.end(); ++it)
            delete (*it);

        m_DelegateList.clear();
    }

    R invoke(_YON_DELEGATE_PARAMS)
    {
        typename DelegateList::ConstIterator it;
        for(it = m_DelegateList.begin(); it != m_DelegateList.end(); ++it)
		{
			if(it!=m_DelegateList.getLast())
				(*it)->invoke(_YON_DELEGATE_ARGS);
			else
				return (*it)->invoke(_YON_DELEGATE_ARGS);
		}
    }

private:
    DelegateList m_DelegateList;
};

#undef _YON_I_DELEGATE
#undef _YON_C_STATIC_DELEGATE
#undef _YON_C_METHOD_DELEGATE
#undef _YON_C_DELEGATE