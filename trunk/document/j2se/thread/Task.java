/**
* Task Entry
* ����ʵ��
* Mode:Template Mode
* ģʽ:ģ��ģʽ
* @author Micheal Hong
*
*/
public abstract class Task {
protected String projectId;

public Task(String projectId)
{
   this.projectId=projectId;
}

public String getProjectId() {
   return projectId;
}

/**
* task performance.It is an interface supplied for implemented by caller.
* �����ִ�С��ɵ���������ʵ�֡�
* @see TaskThread
*/
public abstract void perform();

}

