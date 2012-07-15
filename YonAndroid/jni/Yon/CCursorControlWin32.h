#ifndef _YON_PLATFORM_CCURSORCONTROLWIN32_H_
#define _YON_PLATFORM_CCURSORCONTROLWIN32_H_

#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#include <Windowsx.h>
#include "dimension2d.h"
#include "ICursorControl.h"

namespace yon{
namespace platform{

	class CCursorControlWin32 : public ICursorControl{
	private:
		HWND m_hwnd;
		bool m_bUseReferenceRect;
		bool m_bVisible;
		core::position2di m_CursorPos;
		core::dimension2du m_WindowSize;
		core::dimension2df m_InvWindowSize;
		core::recti m_ReferenceRect;
		s32 m_iBorderX, m_iBorderY;

		//! Updates the internal cursor position
		void updateInternalCursorPosition()
		{
			POINT p;
			if (!GetCursorPos(&p))
			{
				DWORD xy = GetMessagePos();
				p.x = GET_X_LPARAM(xy);
				p.y = GET_Y_LPARAM(xy);
			}

			if (m_bUseReferenceRect)
			{
				m_CursorPos.x = p.x - m_ReferenceRect.topLeft.x;
				m_CursorPos.y = p.y - m_ReferenceRect.topLeft.y;
			}
			else
			{
				RECT rect;
				//该函数返回指定窗口的边框矩形的尺寸。该尺寸以相对于屏幕坐标左上角的屏幕坐标给出。
				//如果函数成功，返回值为非零：如果函数失败，返回值为零。
				if (GetWindowRect(m_hwnd, &rect))
				{
					m_CursorPos.x = p.x-rect.left-m_iBorderX;
					m_CursorPos.y = p.y-rect.top-m_iBorderY;
				}
				else
				{
					// window seems not to be existent, so set cursor to
					// a negative value
					m_CursorPos.x = -1;
					m_CursorPos.y = -1;
				}
			}
		}
	public:
		CCursorControlWin32(const core::dimension2du& wsize, HWND hwnd, bool fullscreen)
			: m_WindowSize(wsize), m_InvWindowSize(0.0f, 0.0f),
			m_hwnd(hwnd), m_iBorderX(0), m_iBorderY(0),
			m_bUseReferenceRect(false), m_bVisible(true)
		{
			if (m_WindowSize.w!=0)
				m_InvWindowSize.w = 1.0f / m_WindowSize.w;

			if (m_WindowSize.h!=0)
				m_InvWindowSize.h = 1.0f / m_WindowSize.h;

			updateBorderSize(fullscreen, false);
		}

		//! Changes the visible state of the mouse cursor.
		virtual void setVisible(bool visible)
		{
			CURSORINFO info;
			info.cbSize = sizeof(CURSORINFO);
			BOOL gotCursorInfo = GetCursorInfo(&info);
			while ( gotCursorInfo )
			{
				if ( (visible && info.flags == CURSOR_SHOWING) 	// visible
					|| (!visible && info.flags == 0 ) )			// hidden
				{
					break;
				}
				int showResult = ShowCursor(visible);   // this only increases an internal display counter in windows, so it might have to be called some more
				if ( showResult < 0 )
				{
					break;
				}
				info.cbSize = sizeof(CURSORINFO);	// yes, it really must be set each time
				gotCursorInfo = GetCursorInfo(&info);
			}
			m_bVisible = visible;
		}

		//! Returns if the cursor is currently visible.
		virtual bool isVisible() const
		{
			return m_bVisible;
		}

		//! Sets the new position of the cursor.
		virtual void setPosition(const core::position2df &pos)
		{
			setPosition(pos.x, pos.y);
		}

		//! Sets the new position of the cursor.
		virtual void setPosition(f32 x, f32 y)
		{
			if (!m_bUseReferenceRect)
				setPosition(core::round32(x*m_WindowSize.w), core::round32(y*m_WindowSize.h));
			else
				setPosition(core::round32(x*m_ReferenceRect.getWidth()), core::round32(y*m_ReferenceRect.getHeight()));
		}

		//! Sets the new position of the cursor.
		virtual void setPosition(const core::position2di &pos)
		{
			setPosition(pos.x, pos.y);
		}

		//! Sets the new position of the cursor.
		virtual void setPosition(s32 x, s32 y)
		{
			if (m_bUseReferenceRect)
			{
				SetCursorPos(m_ReferenceRect.topLeft.x + x,m_ReferenceRect.topLeft.y + y);
			}
			else
			{
				RECT rect;
				if (GetWindowRect(m_hwnd, &rect))
					SetCursorPos(x + rect.left + m_iBorderX, y + rect.top + m_iBorderY);
			}

			m_CursorPos.x = x;
			m_CursorPos.y = y;
		}

		//! Returns the current position of the mouse cursor.
		virtual const core::position2di& getPosition()
		{
			updateInternalCursorPosition();
			return m_CursorPos;
		}

		//! Returns the current position of the mouse cursor.
		virtual core::position2df getRelativePosition()
		{
			updateInternalCursorPosition();

			if (!m_bUseReferenceRect)
			{
				return core::position2df(m_CursorPos.x * m_InvWindowSize.w,m_CursorPos.y * m_InvWindowSize.h);
			}

			return core::position2df(m_CursorPos.x / (f32)m_ReferenceRect.getWidth(),m_CursorPos.y / (f32)m_ReferenceRect.getHeight());
		}

		//! Sets an absolute reference rect for calculating the cursor position.
		virtual void setReferenceRect(core::recti* rect=0)
		{
			if (rect)
			{
				m_ReferenceRect = *rect;
				m_bUseReferenceRect = true;

				// prevent division through zero and uneven sizes

				if (!m_ReferenceRect.getHeight() || m_ReferenceRect.getHeight()%2)
					m_ReferenceRect.bottomRight.y += 1;

				if (!m_ReferenceRect.getWidth() || m_ReferenceRect.getWidth()%2)
					m_ReferenceRect.bottomRight.x += 1;
			}
			else
				m_bUseReferenceRect = false;
		}


		/** Used to notify the cursor that the window was resized. */
		virtual void onResize(const core::dimension2du& size)
		{
			m_WindowSize = size;
			if (size.w!=0)
				m_InvWindowSize.w = 1.0f / size.w;
			else
				m_InvWindowSize.w = 0.f;

			if (size.h!=0)
				m_InvWindowSize.h = 1.0f / size.h;
			else
				m_InvWindowSize.h = 0.f;
		}

		/** Used to notify the cursor that the window resizable settings changed. */
		void updateBorderSize(bool fullscreen, bool resizable)
		{
			if (!fullscreen)
			{
				if (resizable)
				{
					//用于得到被定义的系统数据或者系统配置信息.
					//SM_CXSIZEFRAME:围绕可改变大小的窗口的边框的厚度
					//SM_CYCAPTION:以像素计算的普通窗口标题的高度
					//SM_CYSMCAPTION:以像素计算的窗口小标题栏的高度
					//SM_CYDLGFRAME:等同与SM_CXFIXEDFRAME and SM_CYFIXEDFRAME
					//SM_CYFIXEDFRAME:围绕具有标题但无法改变尺寸的窗口（通常是一些对话框）的边框的厚度
					m_iBorderX = GetSystemMetrics(SM_CXSIZEFRAME);
					m_iBorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME);
				}
				else
				{
					m_iBorderX = GetSystemMetrics(SM_CXDLGFRAME);
					m_iBorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
				}
			}
			else
			{
				m_iBorderX = m_iBorderY = 0;
			}
		}
	};
}
}
#endif
#endif