/**
* Task Entry
* 任务实体
* Mode:Template Mode
* 模式:模板模式
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
* 任务的执行。由调用者自行实现。
* @see TaskThread
*/
public abstract void perform();

}

