using UnityEngine;
using System.Collections;

public class TransparentRayer : MonoBehaviour {

    public GameObject target = null;
    public GameObject buildingContainer = null;
    private Transparentable latestTransparentable = null;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        
        if (target!=null)
        {
            RaycastHit hit;
            Debug.DrawLine(transform.position, target.transform.position);
            if (Physics.Linecast(transform.position, target.transform.position, out hit) && hit.collider.gameObject !=target)
            {
                GameObject thiz = hit.collider.gameObject;
                while (thiz.transform.parent!=null && thiz.transform.parent.gameObject != buildingContainer)
                {
                    thiz = thiz.transform.parent.gameObject;
                }

                if (thiz == null)
                    return;

                Transparentable temp = thiz.GetComponent<Transparentable>();
                if(temp!=latestTransparentable)
                {
                    if (latestTransparentable != null)
                        latestTransparentable.setTransparent(false);
                    if (temp != null)
                        temp.setTransparent(true);
                    latestTransparentable = temp;
                }
            }
        }
	}
}
