// Copyright (c) 2010 Jeroen Dierckx - Expertise Centre for Digital Media. All Rights reserved.
// Part of this source code is developed for the flemish (Belgian) OSMA project (http://osma.phl.be/)
//
// Contributors (Unity forum usernames): reissgrant, agentdm
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UNITYCHROMIUMWINDOW_H
#define UNITYCHROMIUMWINDOW_H

// Includes
#include "global.h"

#pragma warning (disable: 4100)
#include "include/cef_app.h"
#pragma warning (default: 4100)


class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler {
 public:
  SimpleApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
};




#if 0
#include <berkelium/Chromium.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>
#include <vector>

/** Class Description */
class UnityChromiumWindow: Chromium::WindowDelegate
{
public:
	// The callback function that is called when a paint event occurs
	// Note: Unity crashes when providing a struct or more than two standard parameters
	//typedef void (*SetPixelsFunc)(Chromium::Rect);
	typedef void (*SetPixelsFunc)(/*int left, int top, int width, int height*/);
	typedef void (*ApplyTextureFunc)();

	// The callback function that is called from a javascript externalHost call
	typedef void (*ExternalHostFunc)(/*const wchar_t *message*/);

	// Constructors and destructor
	UnityChromiumWindow(int uniqueID, float *buffer, bool transparency, int width, int height, const string &url);
	virtual ~UnityChromiumWindow();

	// Information
	Chromium::Window *getChromiumWindow() const { return m_pWindow; }

	// Navigation
	void navigateTo(const string &url);

	// Callbacks
	void setPaintFunctions(SetPixelsFunc setPixelsFunc, ApplyTextureFunc applyTextureFunc);
	void setExternalHostCallback(ExternalHostFunc callback);

	// Current paint info
	const Chromium::Rect &getLastDirtyRect() const { return m_lastDirtyRect; }

	// Last external host message
	const std::wstring &getLastExternalHostMessage() const { return m_lastExternalHostMessage; }

protected:
	// Chromium::WindowDelegate functions
	virtual void onAddressBarChanged(Chromium::Window *win, Chromium::URLString newURL);
	virtual void onStartLoading(Chromium::Window *win, Chromium::URLString newURL);
	virtual void onLoad(Chromium::Window *win);
	virtual void onCrashedWorker(Chromium::Window *win);
	virtual void onCrashedPlugin(Chromium::Window *win, Chromium::WideString pluginName);
	virtual void onProvisionalLoadError(Chromium::Window *win, Chromium::URLString url, int errorCode, bool isMainFrame);
	virtual void onConsoleMessage(Chromium::Window *win, Chromium::WideString message, Chromium::WideString sourceId, int line_no);
	virtual void onScriptAlert(Chromium::Window *win, Chromium::WideString message, Chromium::WideString defaultValue, Chromium::URLString url, int flags, bool &success, Chromium::WideString &value);
	virtual void freeLastScriptAlert(Chromium::WideString lastValue);
	virtual void onNavigationRequested(Chromium::Window *win, Chromium::URLString newUrl, Chromium::URLString referrer, bool isNewWindow, bool &cancelDefaultAction);
	virtual void onLoadingStateChanged(Chromium::Window *win, bool isLoading);
	virtual void onTitleChanged(Chromium::Window *win, Chromium::WideString title);
	virtual void onTooltipChanged(Chromium::Window *win, Chromium::WideString text);
	virtual void onCrashed(Chromium::Window *win);
	virtual void onUnresponsive(Chromium::Window *win);
	virtual void onResponsive(Chromium::Window *win);
	virtual void onExternalHost(Chromium::Window *win, Chromium::WideString message, Chromium::URLString origin, Chromium::URLString target);
	virtual void onCreatedWindow(Chromium::Window *win, Chromium::Window *newWindow, const Chromium::Rect &initialRect);
	virtual void onPaint(Chromium::Window *win, const unsigned char *sourceBuffer, const Chromium::Rect &sourceBufferRect, size_t numCopyRects, const Chromium::Rect *copyRects, int dx, int dy, const Chromium::Rect &scrollRect);
	virtual void onWidgetCreated(Chromium::Window *win, Chromium::Widget *newWidget, int zIndex);
	virtual void onWidgetDestroyed(Chromium::Window *win, Chromium::Widget *wid);
	virtual void onWidgetResize(Chromium::Window *win, Chromium::Widget *wid, int newWidth, int newHeight);
	virtual void onWidgetMove(Chromium::Window *win, Chromium::Widget *wid, int newX, int newY);
	virtual void onWidgetPaint(Chromium::Window *win, Chromium::Widget *wid, const unsigned char *sourceBuffer, const Chromium::Rect &sourceBufferRect, size_t numCopyRects, const Chromium::Rect *copyRects, int dx, int dy, const Chromium::Rect &scrollRect);
	virtual void onCursorUpdated(Chromium::Window *win, const Chromium::Cursor& newCursor);
	virtual void onShowContextMenu(Chromium::Window *win, const Chromium::ContextMenuEventArgs& args);

	// Protected functions
	void convertColors(const Chromium::Rect &rect, const unsigned char *sourceBuffer);

	// Member variables
	Chromium::Window *m_pWindow;
	int m_id;
	float *m_buffer;
	bool m_transparency;
	int m_width, m_height;
	string m_url;

	SetPixelsFunc m_setPixelsFunc;
	ApplyTextureFunc m_applyTextureFunc;
	Chromium::Rect m_lastDirtyRect;

	ExternalHostFunc m_externalHostFunc;
	std::wstring m_lastExternalHostMessage;
};

#endif

#endif // UNITYCHROMIUMWINDOW_H
