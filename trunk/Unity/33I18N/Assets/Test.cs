using UnityEngine;
using System.Collections;

public class Test : MonoBehaviour {

    void Awake(){
        I18NManager.getInstance().setLanguage(SystemLanguage.English);
        I18NManager.getInstance().reset();
    }

	// Use this for initialization
	void Start () {
        print("LG:" + Application.systemLanguage);
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetMouseButtonDown(0))
        {
            I18NManager.getInstance().setLanguage(SystemLanguage.Chinese);
            I18NManager.getInstance().reset();
        }
        if (Input.GetMouseButtonUp(0))
        {
            I18NManager.getInstance().setLanguage(SystemLanguage.English);
            I18NManager.getInstance().reset();
        }
	}
}
