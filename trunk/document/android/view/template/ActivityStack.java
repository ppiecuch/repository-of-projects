package com.pixcoo.common;

import java.util.Stack;

import android.app.Activity;

/**
 * �Զ���Activityջ
 *
 */
public class ActivityStack
{
    private static Stack<Activity> stack;
    private static ActivityStack instance;
    
    public static ActivityStack getActivityStack()
    {
        if (instance == null)
        {
            instance = new ActivityStack();
            stack = new Stack<Activity>();
        }
        return instance;
    }
    
    /**
     * ��ջ����µ�Activity
     * @param activity Ҫ����ջ�е�Activity
     */
    public synchronized void push(Activity activity)
    {
        stack.add(activity);
    }
    
    /**
     * ����ջ�е����һ��Activity����finish��
     */
    public synchronized void pop()
    {
        if (!stack.isEmpty())
        {
            Activity activity = stack.lastElement();
            if (activity != null)
            {
                activity.finish();
                stack.remove(activity);
                activity = null;
            }
        }
    }
    
    /**
     * ���ջ����finish��ջ�����е�Activity
     */
    public synchronized void clear()
    {
        int size = stack.size();
        for (int i = 0; i < size; i++)
        {
            Activity activity = stack.elementAt(i);
            if (activity != null)
            {
                activity.finish();
                activity = null;
            }
        }
        stack.removeAllElements();
    }
    
    public synchronized int size()
    {
        return stack.size();
    }
}
