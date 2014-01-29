using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class CTest : MonoBehaviour {

	// Use this for initialization
	void Start () {
		try {
			init();
			Debug.Log("I did it!");
		} catch (System.DllNotFoundException ex) {
			Debug.LogError("Not found! " + ex);
			System.Exception v = ex;
			int c = 5;
			while (v != null && c > 0) {
				Debug.LogError("Not found more: " + v);
				v = v.InnerException;
				c--;
			}
		}
	
	}

	// Update is called once per frame
	void Update () {
	
	}
	
	[DllImport("UnityChromiumPlugin", EntryPoint="Chromium_init")]
	public static extern void init();
	

}
