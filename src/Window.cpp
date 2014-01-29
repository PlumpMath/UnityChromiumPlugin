// Copyright (c) 2010 Jeroen Dierckx - Expertise Centre for Digital Media. All Rights reserved.
// Part of this source code is developed for the flemish (Belgian) OSMA project (http://osma.phl.be/)
//
// Contributors (Unity forum usernames): reissgrant, agentdm
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Includes

#include "global.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"

#include "Window.h"
#include "Handler.h"


SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  // Information used when creating the native window.
  CefWindowInfo window_info;

#if defined(OS_WIN)
  // On Windows we need to specify certain flags that will be passed to
  // CreateWindowEx().
  window_info.SetAsPopup(NULL, "cefsimple");
#endif

  // SimpleHandler implements browser-level callbacks.
  CefRefPtr<SimpleHandler> handler(new SimpleHandler());

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  std::string url;

  // Check if a "--url=" value was provided via the command-line. If so, use
  // that instead of the default URL.
  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();
  url = command_line->GetSwitchValue("url");
  if (url.empty())
    url = "http://www.google.com";

  // Create the first browser window.
  CefBrowserHost::CreateBrowserSync(window_info, handler.get(), url,
                                    browser_settings, NULL);
}




#if 0

/******************************
* Constructors and destructor *
******************************/

UnityChromiumWindow::UnityChromiumWindow(int uniqueID, float *buffer, bool transparency, int width, int height, const string &url)
: m_id(uniqueID), m_buffer(buffer), m_transparency(transparency), m_width(width), m_height(height), m_url(url)
, m_setPixelsFunc(0), m_applyTextureFunc(0), m_externalHostFunc(0)
{
	assert(m_buffer);
	assert(width > 0 && height > 0);

	Chromium::Context *pContext = Chromium::Context::create();
	m_pWindow = Chromium::Window::create(pContext);
	delete pContext;

	m_pWindow->setDelegate(this);
	m_pWindow->setTransparent(transparency);
	m_pWindow->resize(width, height);
	m_pWindow->navigateTo(url.data(), url.length());
}

UnityChromiumWindow::~UnityChromiumWindow()
{
	delete m_pWindow;
}


/******************
* Other functions *
******************/

void UnityChromiumWindow::navigateTo(const string &url)
{
	m_pWindow->navigateTo(url.data(), url.length());
}

void UnityChromiumWindow::setPaintFunctions(SetPixelsFunc setPixelsFunc, ApplyTextureFunc applyTextureFunc)
{
	m_setPixelsFunc = setPixelsFunc;
	m_applyTextureFunc = applyTextureFunc;
}

void UnityChromiumWindow::setExternalHostCallback(ExternalHostFunc callback)
{
	m_externalHostFunc = callback;
}


/**************************************
* Chromium::WindowDelegate functions *
**************************************/

#if 0
void UnityChromiumWindow::onPaint(Chromium::Window *pWindow, const unsigned char *sourceBuffer, const Chromium::Rect &rect, int dx, int dy, const Chromium::Rect &scrollRect)
{
#ifdef DEBUG
	cerr << "[UnityChromiumWindow] onPaint called (window: " << pWindow << ")" << endl;
	cerr << "  rect: (left=" << rect.left() << ", width=" << rect.width() << ", top=" << rect.top() << ", height=" << rect.height() << ")" << endl;
	cerr << "  sourceBuffer: " << (void *) sourceBuffer << endl;
#endif

	// Scrolling
	if(dx != 0 || dy != 0)
	{
#if 0
		// scroll_rect contains the Rect we need to move
		// First we figure out where the the data is moved to by translating it
		//Chromium::Rect scrolled_rect = scrollRect.translate(-dx, -dy);
		Chromium::Rect scrolled_rect = scrollRect;
		scrolled_rect.mLeft -= dx;
		scrolled_rect.mTop -= dy;

		// Next we figure out where they intersect, giving the scrolled
		// region
		Chromium::Rect scrolled_shared_rect = scrollRect.intersect(scrolled_rect);

		// Only do scrolling if they have non-zero intersection
		if(scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0)
		{
			// And the scroll is performed by moving shared_rect by (dx,dy)
			//Chromium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);
			Chromium::Rect shared_rect = scrolled_shared_rect;
			shared_rect.mLeft += dx;
			shared_rect.mTop += dy;

			for(int y = scrolled_shared_rect.top(); y < shared_rect.bottom(); ++y)
			{
				::memcpy(
					m_buffer + y * m_width * 4 /*+ scrolled_shared_rect.left() * 4*/,
					m_buffer + ((scrolled_shared_rect.top() + y) * m_width /*+ scrolled_shared_rect.left()*/) * 4,
					scrolled_shared_rect.width() * 4
				);
			}

			// Copy the data out of the texture
			/*size_t scrollBuffSize = scrolled_shared_rect.width() * scrolled_shared_rect.height() * 4;
			unsigned char *scrollBuffer = new unsigned char[scrollBuffSize];

			for(int line = 0; line < scrolled_shared_rect.height(); ++line)
			{
				::memcpy(
					scrollBuffer + line * scrolled_shared_rect.width() * 4,

			}*/
#if 0
			glGetTexImage(
				GL_TEXTURE_2D, 0,
				GL_BGRA, GL_UNSIGNED_BYTE,
				scroll_buffer
				);

			// Annoyingly, OpenGL doesn't provide convenient primitives, so
			// we manually copy out the region to the beginning of the
			// buffer
			int wid = scrolled_shared_rect.width();
			int hig = scrolled_shared_rect.height();
			for(int jj = 0; jj < hig; jj++) {
				memcpy(
					scroll_buffer + (jj*wid * 4),
					scroll_buffer + ((scrolled_shared_rect.top()+jj)*dest_texture_width + scrolled_shared_rect.left()) * 4,
					wid*4
					);
			}

			// And finally, we push it back into the texture in the right
			// location
			glTexSubImage2D(GL_TEXTURE_2D, 0,
				shared_rect.left(), shared_rect.top(),
				shared_rect.width(), shared_rect.height(),
				GL_BGRA, GL_UNSIGNED_BYTE, scroll_buffer
				);
#endif
		}
#endif
	}

	// Apply the dirty rectangle
	convertColors(rect, sourceBuffer);

	// Call the paint callback
	m_lastDirtyRect = rect;
	m_paintFunc(/*rect.left(), rect.top(), rect.width(), rect.height()*/);
}
#endif

void UnityChromiumWindow::onAddressBarChanged(Chromium::Window *win, Chromium::URLString newURL)
{
	cerr << "[UnityChromiumWindow] onAddressBarChanged called (window: " << win << ")" << endl;
	cerr << "  newURL: " << newURL << endl;
}

void UnityChromiumWindow::onStartLoading(Chromium::Window *win, Chromium::URLString newURL)
{
	cerr << "[UnityChromiumWindow] onStartLoading called (window: " << win << ")" << endl;
	cerr << "  newURL: " << newURL << endl;
}

void UnityChromiumWindow::onLoad(Chromium::Window *win)
{
	cerr << "[UnityChromiumWindow] onLoad called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onCrashedWorker(Chromium::Window *win)
{
	cerr << "[UnityChromiumWindow] onCrashedWorker called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onCrashedPlugin(Chromium::Window *win, Chromium::WideString pluginName)
{
	cerr << "[UnityChromiumWindow] onCrashedPlugin called (window: " << win << ")" << endl;
	wcerr << "  pluginName: " << pluginName << endl;
}

void UnityChromiumWindow::onProvisionalLoadError(Chromium::Window *win, Chromium::URLString url, int errorCode, bool isMainFrame)
{
	cerr << "[UnityChromiumWindow] onProvisionalLoadError called (window: " << win << ")" << endl;
	cerr << "  url: " << url << endl;
	cerr << "  errorCode: " << errorCode << endl;
	cerr << "  isMainFrame: " << (isMainFrame ? "yes" : "no") << endl;
}

void UnityChromiumWindow::onConsoleMessage(Chromium::Window *win, Chromium::WideString message, Chromium::WideString sourceId, int line_no)
{
	cerr << "[UnityChromiumWindow] onConsoleMessage called (window: " << win << ")" << endl;
	wcerr << "  message: " << message << endl;
	wcerr << "  sourceId: " << sourceId << endl;
	cerr << "  line_no: " << line_no << endl;
}

void UnityChromiumWindow::onScriptAlert(Chromium::Window *win, Chromium::WideString message, Chromium::WideString defaultValue, Chromium::URLString url, int flags, bool &success, Chromium::WideString &value)
{
	cerr << "[UnityChromiumWindow] onScriptAlert called (window: " << win << ")" << endl;
	wcerr << "  message: " << message << endl;
	wcerr << "  defaultValue: " << defaultValue << endl;
	cerr << "  url: " << url << endl;
	cerr << "  flags: " << flags << endl;
}

void UnityChromiumWindow::freeLastScriptAlert(Chromium::WideString lastValue)
{
	cerr << "[UnityChromiumWindow] freeLastScriptAlert called (lastValue: " << lastValue << ")" << endl;
	wcerr << "  lastValue: " << lastValue << endl;
}

void UnityChromiumWindow::onNavigationRequested(Chromium::Window *win, Chromium::URLString newUrl, Chromium::URLString referrer, bool isNewWindow, bool &cancelDefaultAction)
{
	cerr << "[UnityChromiumWindow] onNavigationRequested called (window: " << win << ")" << endl;
	cerr << "  newUrl: " << newUrl << endl;
	cerr << "  referrer: " << referrer << endl;
	cerr << "  isNewWindow: " << (isNewWindow ? "yes" : "no") << endl;
}

void UnityChromiumWindow::onLoadingStateChanged(Chromium::Window *win, bool isLoading)
{
	cerr << "[UnityChromiumWindow] onLoadingStateChanged called (window: " << win << ")" << endl;
	cerr << "  isLoading: " << (isLoading? "yes" : "no") << endl;
}

void UnityChromiumWindow::onTitleChanged(Chromium::Window *win, Chromium::WideString title)
{
	cerr << "[UnityChromiumWindow] onTitleChanged called (window: " << win << ")" << endl;
	wcerr << "  title: " << title << endl;
}

void UnityChromiumWindow::onTooltipChanged(Chromium::Window *win, Chromium::WideString text)
{
	cerr << "[UnityChromiumWindow] onTooltipChanged called (window: " << win << ")" << endl;
	wcerr << "  tetx: " << text << endl;
}

void UnityChromiumWindow::onCrashed(Chromium::Window *win)
{
	cerr << "[UnityChromiumWindow] onCrashed called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onUnresponsive(Chromium::Window *win)
{
	cerr << "[UnityChromiumWindow] onUnresponsive called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onResponsive(Chromium::Window *win)
{
	cerr << "[UnityChromiumWindow] onResponsive called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onExternalHost(Chromium::Window *win, Chromium::WideString message, Chromium::URLString origin, Chromium::URLString target)
{
	cerr << "[UnityChromiumWindow] onExternalHost called (window: " << win << ")" << endl;
	wcerr << "  message: " << message << endl;
	cerr << "  origin: " << origin << endl;
	cerr << "  target: " << target << endl;

	if(m_externalHostFunc)
	{
		m_lastExternalHostMessage = std::wstring(message.data(), message.length());
		m_externalHostFunc(/*message.data()*/);
	}
}

void UnityChromiumWindow::onCreatedWindow(Chromium::Window *win, Chromium::Window *newWindow, const Chromium::Rect &initialRect)
{
	cerr << "[UnityChromiumWindow] onCreatedWindow called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onPaint(Chromium::Window *win, const unsigned char *sourceBuffer, const Chromium::Rect &sourceBufferRect, size_t numCopyRects, const Chromium::Rect *copyRects, int dx, int dy, const Chromium::Rect &scrollRect)
{
#ifdef DEBUG
	cerr << "[UnityChromiumWindow] onPaint called (window: " << win << ")" << endl;
	cerr << "  sourceBuffer: " << (void *) sourceBuffer << endl;
	cerr << "  sourceBufferRect: (left=" << sourceBufferRect.left() << ", width=" << sourceBufferRect.width() << ", top=" << sourceBufferRect.top() << ", height=" << sourceBufferRect.height() << ")" << endl;
	cerr << "  num dirty rects: " << numCopyRects << endl;
	for(size_t i = 0; i < numCopyRects; ++i)
		cerr << "  rect " << i << ": (left=" << copyRects[i].left() << ", width=" << copyRects[i].width() << ", top=" << copyRects[i].top() << ", height=" << copyRects[i].height() << ")" << endl;
#endif

	//! @todo Handle Scrolling
	if(dx != 0 || dy != 0)
	{
	}

	// Apply the dirty rectangles
	for(size_t i = 0; i < numCopyRects; ++i)
	{
		convertColors(copyRects[i], sourceBuffer);
		m_lastDirtyRect = copyRects[i];

		if(m_setPixelsFunc)
			m_setPixelsFunc(/*rect.left(), rect.top(), rect.width(), rect.height()*/);
	}

	// Call the paint callback
	if(m_applyTextureFunc)
		m_applyTextureFunc();
}

void UnityChromiumWindow::onWidgetCreated(Chromium::Window *win, Chromium::Widget *newWidget, int zIndex)
{
	cerr << "[UnityChromiumWindow] onWidgetCreated called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onWidgetDestroyed(Chromium::Window *win, Chromium::Widget *wid)
{
	cerr << "[UnityChromiumWindow] onWidgetDestroyed called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onWidgetResize(Chromium::Window *win, Chromium::Widget *wid, int newWidth, int newHeight)
{
	cerr << "[UnityChromiumWindow] onWidgetResize called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onWidgetMove(Chromium::Window *win, Chromium::Widget *wid, int newX, int newY)
{
	cerr << "[UnityChromiumWindow] onWidgetMove called (window: " << win << ")" << endl;
}

void UnityChromiumWindow::onWidgetPaint(Chromium::Window *win, Chromium::Widget *wid, const unsigned char *sourceBuffer, const Chromium::Rect &sourceBufferRect, size_t numCopyRects, const Chromium::Rect *copyRects, int dx, int dy, const Chromium::Rect &scrollRect)
{
#ifdef DEBUG
	cerr << "[UnityChromiumWindow] onWidgetPaint called (window: " << win << ")" << endl;
#endif
}

void UnityChromiumWindow::onCursorUpdated(Chromium::Window *win, const Chromium::Cursor& newCursor)
{
#ifdef DEBUG
	cerr << "[UnityChromiumWindow] onCursorUpdated called (window: " << win << ")" << endl;
#endif
}

void UnityChromiumWindow::onShowContextMenu(Chromium::Window *win, const Chromium::ContextMenuEventArgs& args)
{
	cerr << "[UnityChromiumWindow] onShowContextMenu called (window: " << win << ")" << endl;
}


/**********************
* Protected functions *
**********************/

void UnityChromiumWindow::convertColors(const Chromium::Rect &rect, const unsigned char *sourceBuffer)
{
	// Note: we convert from BGRA bytes to RGBA floats. RGB24 textures in Unity are still 32bit in memory.
	for(int x = rect.left(); x < rect.right(); ++x)
	{
		for(int y = rect.top(); y < rect.bottom(); ++y)
		{
			// We copy to the beginning of the buffer
			// (because we can't change the start address of the buffer we provide the Unity .SetPixels function
			int idx = (y - rect.top()) * rect.width() + (x - rect.left());
			//int idx = y * rect.width() + x;

			m_buffer[idx * 4 + 0] = sourceBuffer[idx * 4 + 2] / 255.0f; // R
			m_buffer[idx * 4 + 1] = sourceBuffer[idx * 4 + 1] / 255.0f; // G
			m_buffer[idx * 4 + 2] = sourceBuffer[idx * 4 + 0] / 255.0f; // B

			if(m_transparency)
				m_buffer[idx * 4 + 3] = sourceBuffer[idx * 4 + 3] / 255.0f; // A
		}
	}
}

#endif