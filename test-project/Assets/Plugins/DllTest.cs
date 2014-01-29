using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class DllTest : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
		Debug.Log("--- It is " + mrDllTest());
		Debug.Log("--- It is from " + getRemoteNum());
	
//		init();
//		Debug.Log("I called init!");
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	[DllImport("DllTest", EntryPoint="mrDllTest")]
	public static extern int mrDllTest();
	
	[DllImport("DllTest", EntryPoint="getRemoteNum")]
	public static extern int getRemoteNum();
	
//	[DllImport("UnityChromiumPlugin", EntryPoint="Chromium_init")]
//	public static extern void init();
}
