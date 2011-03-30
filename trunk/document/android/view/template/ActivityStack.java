package com.pixcoo.common;

import java.util.Stack;

import android.app.Activity;

/**
 * 自定义Activity栈
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
     * 向栈添加新的Activity
     * @param activity 要放入栈中的Activity
     */
    public synchronized void push(Activity activity)
    {
        stack.add(activity);
    }
    
    /**
     * 弹出栈中的最后一个Activity，并finish掉
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
     * 清空栈，并finish掉栈中所有的Activity
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
