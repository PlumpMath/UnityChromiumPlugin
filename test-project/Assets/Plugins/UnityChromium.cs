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

public class UnityChromium
{
	// A rectangle (same structure as the Chromium Rect)
	public struct Rect
	{
		public int left, top;
		public int width, height;
	}

	// The delegate that is called when there is a dirty rectangle
	// Note: Unity crashes when using a struct or more than two parameters
	//delegate void SetPixelsFunc(Rect rect);
	public delegate void SetPixelsFunc(/*int left, int top, int width, int height*/);

	// The delgate that is called when all dirty rectangles can be applied
	public delegate void ApplyTextureFunc();

	// The delegate that is called when javascript calls to Unity
	public delegate void ExternalHostFunc(/*string message*/);

	// Imported functions
	[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_init")]
	public static extern void init();

	[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_destroy")]
	public static extern void destroy();

	[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_update")]
	public static extern void update();

	public class Window
	{
		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_create")]
		public static extern int create(int windowID, IntPtr colors, bool transparency, int width, int height, string url);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_destroy")]
		public static extern void destroy(int windowID);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_navigateTo")]
		public static extern void navigateTo(int windowID, string url);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_setPaintFunctions")]
		public static extern void setPaintFunctions(int windowID, SetPixelsFunc setPixelsFunc, ApplyTextureFunc applyTextureFunc);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_getLastDirtyRect")]
		public static extern Rect getLastDirtyRect(int windowID);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_focus")]
		public static extern void focus(int windowID);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_unfocus")]
		public static extern void unfocus(int windowID);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_mouseDown")]
		public static extern void mouseDown(int windowID, int button);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_mouseUp")]
		public static extern void mouseUp(int windowID, int button);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_mouseMove")]
		public static extern void mouseMove(int windowID, int x, int y);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_textEvent")]
		public static extern void textEvent(int windowID, char c);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_keyEvent")]
		public static extern void keyEvent(int windowID, bool pressed, int mods, int vk_code, int scancode);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_executeJavascript")]
		public static extern void executeJavascript(int windowID, string javascript);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_setExternalHostCallback")]
		public static extern void setExternalHostCallback(int windowID, ExternalHostFunc callback);

		[DllImport ("UnityChromiumPlugin", EntryPoint="Chromium_Window_getLastExternalHostMessage")]
		public static extern IntPtr getLastExternalHostMessage(int windowID);
	}

	public int convertKeyCode(KeyCode keyCode)
	{
		return (int) keyCode;
	}
}
