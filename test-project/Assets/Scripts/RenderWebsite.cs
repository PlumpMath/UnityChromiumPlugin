// Copyright (c) 2010 Jeroen Dierckx - Expertise Centre for Digital Media. All Rights reserved.
// Part of this source code is developed for the flemish (Belgian) OSMA project (http://osma.phl.be/)
//
// Contributors (Unity forum usernames): reissgrant, agentdm
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

using UnityEngine;
using System;
using System.Runtime.InteropServices;
using System.Collections;

public class RenderWebsite : MonoBehaviour
{
	public int width = 512;
	public int height = 512;
	public string url = "http://www.google.com";
	public bool interactive = true;
	public bool transparency = false;

	private Texture2D m_Texture;
	private Color[] m_Pixels;
	private GCHandle m_PixelsHandle;
	private int m_TextureID;

	private UnityChromium.SetPixelsFunc m_setPixelsFunc;
	private UnityChromium.ApplyTextureFunc m_applyTextureFunc;
	private UnityChromium.ExternalHostFunc m_externalHostFunc;

    void Start ()
	{
		// Initialize Chromium
		UnityChromium.init();

		// Create the texture that will represent the website (with optional transparency and without mipmaps)
		TextureFormat texFormat = transparency ? TextureFormat.ARGB32 : TextureFormat.RGB24;
		m_Texture = new Texture2D (width, height, texFormat, false);

		// Create the pixel array for the plugin to write into at startup
		m_Pixels = m_Texture.GetPixels (0);
		// "pin" the array in memory, so we can pass direct pointer to it's data to the plugin,
		// without costly marshaling of array of structures.
		m_PixelsHandle = GCHandle.Alloc(m_Pixels, GCHandleType.Pinned);

		// Save the texture ID
		m_TextureID = m_Texture.GetInstanceID();

		// Improve rendering at shallow angles
		m_Texture.filterMode = FilterMode.Trilinear;
		m_Texture.anisoLevel = 2;

		// Assign texture to the renderer
		if (renderer)
		{
			renderer.material.mainTexture = m_Texture;

			// Transparency?
			if(transparency)
				renderer.material.shader = Shader.Find("Transparent/Diffuse");
			else
				renderer.material.shader = Shader.Find("Diffuse");

			// The texture has to be flipped
			renderer.material.mainTextureScale = new Vector2(1,-1);
		}
		// or gui texture
		else if (GetComponent(typeof(GUITexture)))
		{
			GUITexture gui = GetComponent(typeof(GUITexture)) as GUITexture;
			gui.texture = m_Texture;
		}
		else
		{
			Debug.Log("Game object has no renderer or gui texture to assign the generated texture to!");
		}

		// Create new web window
		UnityChromium.Window.create(m_TextureID, m_PixelsHandle.AddrOfPinnedObject(), transparency, width,height, url);
		print("Created new web window: " + m_TextureID);

		// Paint callbacks
		m_setPixelsFunc = new UnityChromium.SetPixelsFunc(this.SetPixels);
		m_applyTextureFunc = new UnityChromium.ApplyTextureFunc(this.ApplyTexture);
		UnityChromium.Window.setPaintFunctions(m_TextureID, m_setPixelsFunc, m_applyTextureFunc);

		// Set the external host callback (for calling Unity functions from javascript)
		m_externalHostFunc = new UnityChromium.ExternalHostFunc(this.onExternalHost);
		UnityChromium.Window.setExternalHostCallback(m_TextureID, m_externalHostFunc);
    }

	void SetPixels(/*int left, int top, int width, int height*/)
	{
		UnityChromium.Rect rect = UnityChromium.Window.getLastDirtyRect(m_TextureID);
		//print("Painting rect: (" + rect.left + ", " + rect.top + ", " + rect.width + ", " + rect.height + ")");
		m_Texture.SetPixels(rect.left, rect.top, rect.width, rect.height, m_Pixels, 0);
	}

	void ApplyTexture()
	{
		//print("Applying texture");
		m_Texture.Apply();
	}

	void onExternalHost(/*string message*/)
	{
		string message = Marshal.PtrToStringUni(UnityChromium.Window.getLastExternalHostMessage(m_TextureID));
		print("Message from javascript: " + message);

		// Broadcast the message
		SendMessage("OnExternalHost", message, SendMessageOptions.DontRequireReceiver);

		// Parse the JSON object
		object parsedObject = JSON.JsonDecode(message);
		if(parsedObject is Hashtable)
		{
			Hashtable table = (Hashtable) parsedObject;

			string func = (string) table["func"];
			Hashtable args = (Hashtable) table["args"];

			print("  function: " + func);
			print("  #arguments: " + args.Count);

			IDictionaryEnumerator enumerator = args.GetEnumerator();
			while(enumerator.MoveNext())
				print("  " + enumerator.Key.ToString() + " = " + enumerator.Value.ToString());

			// Broadcast the function
			SendMessage(func, args, SendMessageOptions.DontRequireReceiver);
		}
	}

    void OnDisable() {
		// Destroy the web window
		UnityChromium.Window.destroy(m_TextureID);

        // Free the pinned array handle.
        m_PixelsHandle.Free();
    }

	void OnApplicationQuit()
	{
		// Destroy Chromium
		//UnityChromium.destroy();
		//print("Destroyed Chromium");
	}

    void Update ()
	{
		// Update Chromium
		// TODO This only has to be done once in stead of per object
		UnityChromium.update();
    }

	void OnMouseEnter()
	{
	}

	void OnMouseOver()
	{
		// Only when interactive is enabled
		if(!interactive)
			return;

		RaycastHit hit;
		if (Physics.Raycast (Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
		{
			int x = /*width -*/ (int) (hit.textureCoord.x * width);
			int y = height - (int) (hit.textureCoord.y * height);

			UnityChromium.Window.mouseMove(m_TextureID, x, y);
		}
	}

	void OnMouseExit()
	{
	}

	void OnMouseDown()
	{
		// Only when interactive is enabled
		if(!interactive)
			return;

		RaycastHit hit;
		if (Physics.Raycast (Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
		{
			int x = /*width -*/ (int) (hit.textureCoord.x * width);
			int y = height - (int) (hit.textureCoord.y * height);

			// Focus the window
			UnityChromium.Window.focus(m_TextureID);

			UnityChromium.Window.mouseMove(m_TextureID, x, y);
			UnityChromium.Window.mouseDown(m_TextureID, 0);
		}
	}

	void OnMouseUp()
	{
		// Only when interactive is enabled
		if(!interactive)
			return;

		RaycastHit hit;
		if (Physics.Raycast (Camera.main.ScreenPointToRay(Input.mousePosition), out hit))
		{
			int x = /*width -*/ (int) (hit.textureCoord.x * width);
			int y = height - (int) (hit.textureCoord.y * height);

			UnityChromium.Window.mouseMove(m_TextureID, x, y);
			UnityChromium.Window.mouseUp(m_TextureID, 0);
		}
	}

	void OnGUI()
	{
		// Inject input into the page when the GUI doesn't have focus
		if(Event.current.isKey && GUIUtility.keyboardControl == 0)
		{
			// Insert character
			UnityChromium.Window.textEvent(m_TextureID, Event.current.character);

			KeyCode key = Event.current.keyCode;
			bool pressed = (Event.current.type == EventType.KeyDown);

			// Special case for backspace
			if(key == KeyCode.Backspace)
				UnityChromium.Window.keyEvent(m_TextureID, pressed, 0, 08, 0);
			// Special case for enter
			else if(key == KeyCode.Return)
				UnityChromium.Window.keyEvent(m_TextureID, pressed, 0, 13, 0);

			// TODO Handle all keys
			/*int mods = 0;
			int vk_code = UnityChromium.convertKeyCode(Event.current.keyCode);
			int scancode = 0;
			UnityChromium.Window.keyEvent(m_TextureID, pressed, mods, vk_code, scancode);
			print("Key event: " + pressed + ", " + Event.current.keyCode);*/
		}
	}

	public void navigateTo(string url)
	{
		print("Changing url to " + url);
		UnityChromium.Window.navigateTo(m_TextureID, url);
	}

	public void executeJavascript(string javascript)
	{
		print("Executing Javascript: " + javascript);
		UnityChromium.Window.executeJavascript(m_TextureID, javascript);
	}
}
