package yon.autoupdate.core;

public class StateManager {
	
	private State lastState;
	private State currentState;
	
	public StateManager() {
		this.lastState=null;
		this.currentState=null;
	}
	
	public void setState(State s){
		if(lastState!=null)
			lastState.onShutdown();
		currentState=s;
		currentState.onSetup();
	}
	
	public State getCurrentState(){
		return currentState;
	}
	
	private static StateManager instance=null;
	
	public static StateManager getInstance(){
		if(instance==null)
			instance=new StateManager();
		return instance;
	}
	
}
