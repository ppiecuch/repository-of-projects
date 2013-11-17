using UnityEngine;
using System.Collections;

public class I18NHelper : MonoBehaviour,IObserver {

    private UILabel label = null;
    private int fontSize = 12;

	// Use this for initialization
	void Start () {
        label = GetComponent<UILabel>();
        if (label == null)
            throw new System.Exception("Not found!");
        if (label.font == null)
            throw new System.Exception("Not found font!");
        fontSize = label.font.size;

        I18NManager.getInstance().addObserver(this);
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnDestroy(){
        print("OnDestroy");
        I18NManager.getInstance().deleteObserver(this);
    }

    public void update(Observable o, params object[] args)
    {
        I18NManager mgr = o as I18NManager;
        label.font=mgr.getFont(fontSize);
        label.MakePixelPerfect();
    }
}
