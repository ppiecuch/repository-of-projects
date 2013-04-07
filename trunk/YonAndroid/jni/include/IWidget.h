#ifndef _YON_GUI_IWIDGET_H_
#define _YON_GUI_IWIDGET_H_

#include "IReferencable.h"
//#include "IEventReceiver.h"
#include "IHandler.h"
#include "yonString.h"
#include "position2d.h"
#include "rect.h"
#include "yonList.h"
#include "SRenderUnit.h"
#include "IGUISystem.h"
#include "IVideoDriver.h"


//refer to:http://www.php100.com/manual/css3_0

namespace yon{
namespace gui{

	class ITheme;

	class IWidget : public virtual core::IReferencable,public virtual IHandler{
	public:
		//const core::stringc& getTag() const{
			//TODO
		//}
	protected:

		typedef core::list<IWidget*> WidgetList;

		IGUISystem* m_pSysem;

		widget::ENUM_TYPE m_type;
		core::stringc m_id;
		WidgetList m_children;
		IWidget* m_parent;
		bool m_bVisible;
		bool m_bMessageReceivable;

		//相当于irrlicht中的SubElement
		//! is a part of a larger whole and should not be serialized?
		bool m_bPartial;

		bool m_bDirty;

		//! for calculating the difference when resizing parent
		core::recti m_lastParentRect;

		core::recti m_relativeRect;
		core::recti m_absoluteRect;
		core::recti m_absoluteClippingRect;
		//core::dimension2du m_maxSize, m_minSize;

		//! the rectangle the element would prefer to be,
		//! if it was not constrained by parent or max/min size
		core::recti m_desiredRect;

		//! tells the element how to act when its parent is resized
		widget::ENUM_ALIGN m_alignLeft, m_alignRight, m_alignTop, m_alignBottom;

		core::array<video::RenderEntry2D> m_renderEntries;

		void group(core::array<video::RenderEntry2D>& result)
		{
			if(&result!=&m_renderEntries)
			{
				for(u32 i=0;i<m_renderEntries.size();++i)
					result.push_back(m_renderEntries[i]);
			}
			WidgetList::Iterator it=m_children.begin();
			for(;it!=m_children.end();++it)
				(*it)->group(result);
		}


		void addChildToEnd(IWidget* child)
		{
			if (child)
			{
				child->grab(); // prevent destruction when removed
				child->remove(); // remove from old parent
				child->m_lastParentRect = getAbsoluteRectangle();
				child->m_parent = this;
				m_children.push_back(child);
			}
		}

		void recalculateAbsolutePosition(bool recursive)
		{
			core::recti parentAbsolute(0,0,0,0);
			core::recti parentAbsoluteClip;
			f32 fw=0.f, fh=0.f;

			if (m_parent)
			{
				parentAbsolute = m_parent->m_absoluteRect;

				/*if (NoClip)
				{
				IGUIElement* p=this;
				while (p && p->Parent)
				p = p->Parent;
				parentAbsoluteClip = p->AbsoluteClippingRect;
				}
				else
				parentAbsoluteClip = Parent->AbsoluteClippingRect;*/
			}

			const s32 diffx = parentAbsolute.getWidth() - m_lastParentRect.getWidth();
			const s32 diffy = parentAbsolute.getHeight() - m_lastParentRect.getHeight();

			//if (AlignLeft == EGUIA_SCALE || AlignRight == EGUIA_SCALE)
			//	fw = (f32)parentAbsolute.getWidth();

			//if (AlignTop == EGUIA_SCALE || AlignBottom == EGUIA_SCALE)
			//	fh = (f32)parentAbsolute.getHeight();

			switch (m_alignLeft)
			{
			case widget::UPPERLEFT:
				break;
			case widget::LOWERRIGHT:
				m_desiredRect.topLeft.x += diffx;
				break;
			case widget::CENTER:
				m_desiredRect.topLeft.x += diffx/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.UpperLeftCorner.X = core::round32(ScaleRect.UpperLeftCorner.X * fw);
				//	break;
			}

			switch (m_alignRight)
			{
			case widget::UPPERLEFT:
				break;
			case widget::LOWERRIGHT:
				m_desiredRect.bottomRight.x += diffx;
				break;
			case widget::CENTER:
				m_desiredRect.bottomRight.x += diffx/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.LowerRightCorner.X = core::round32(ScaleRect.LowerRightCorner.X * fw);
				//	break;
			}

			switch (m_alignTop)
			{
			case widget::UPPERLEFT:
				break;
			case widget::LOWERRIGHT:
				m_desiredRect.topLeft.y += diffy;
				break;
			case widget::CENTER:
				m_desiredRect.topLeft.y += diffy/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.UpperLeftCorner.Y = core::round32(ScaleRect.UpperLeftCorner.Y * fh);
				//	break;
			}

			switch (m_alignBottom)
			{
			case widget::UPPERLEFT:
				break;
			case widget::LOWERRIGHT:
				m_desiredRect.bottomRight.y += diffy;
				break;
			case widget::CENTER:
				m_desiredRect.bottomRight.y += diffy/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.LowerRightCorner.Y = core::round32(ScaleRect.LowerRightCorner.Y * fh);
				//	break;
			}

			m_relativeRect = m_desiredRect;

			const s32 w = m_relativeRect.getWidth();
			const s32 h = m_relativeRect.getHeight();

			// make sure the desired rectangle is allowed
			//if (w < (s32)MinSize.Width)
			//	RelativeRect.LowerRightCorner.X = RelativeRect.UpperLeftCorner.X + MinSize.Width;
			//if (h < (s32)MinSize.Height)
			//	RelativeRect.LowerRightCorner.Y = RelativeRect.UpperLeftCorner.Y + MinSize.Height;
			//if (MaxSize.Width && w > (s32)MaxSize.Width)
			//	RelativeRect.LowerRightCorner.X = RelativeRect.UpperLeftCorner.X + MaxSize.Width;
			//if (MaxSize.Height && h > (s32)MaxSize.Height)
			//	RelativeRect.LowerRightCorner.Y = RelativeRect.UpperLeftCorner.Y + MaxSize.Height;


			m_relativeRect.repair();

			m_absoluteRect = m_relativeRect + parentAbsolute.topLeft;

			if (!m_parent)
				parentAbsoluteClip = m_absoluteRect;

			m_absoluteClippingRect = m_absoluteRect;
			m_absoluteClippingRect.clipAgainst(parentAbsoluteClip);

			m_lastParentRect = parentAbsolute;

			if ( recursive )
			{
				// update all children
				WidgetList::Iterator it = m_children.begin();
				for (; it != m_children.end(); ++it)
				{
					(*it)->recalculateAbsolutePosition(recursive);
				}
			}
		}

	public:
		IWidget(widget::ENUM_TYPE type,IGUISystem* guiSystem,IWidget* parent,const core::stringc& id,const core::recti& rectangle)
			:m_type(type),m_parent(parent),m_id(id),m_bMessageReceivable(true),m_bPartial(false),m_pSysem(guiSystem),
			m_relativeRect(rectangle),m_absoluteRect(rectangle),m_desiredRect(rectangle),m_absoluteClippingRect(rectangle),
			m_bVisible(true),m_alignLeft(widget::UPPERLEFT), m_alignRight(widget::UPPERLEFT), m_alignTop(widget::UPPERLEFT), m_alignBottom(widget::UPPERLEFT)
		{
				if (parent)
				{
					parent->addChildToEnd(this);
					recalculateAbsolutePosition(true);
				}

		}

		virtual ~IWidget()
		{
			//TODO 是否直接调用VideoDriver？
			if(m_pSysem)
			{
				for(u32 i=0;i<m_renderEntries.size();++i)
					m_pSysem->getVideoDriver()->recycleRenderUnit2D(m_renderEntries[i].Unit);
				m_renderEntries.clear();
			}

			WidgetList::Iterator it=m_children.begin();
			for(;it!=m_children.end();++it)
				(*it)->drop();
			m_children.clear();
		}

		/**
		* @brief get the opacity of the widget
		* 
		* @return the opacity of the widget's Skin. Traditionally this is 1.0f, fully Opaque.
		*/
		//virtual f32 getOpacity() = 0;

		/**
		* @brief chekc if the widget's transparent area pickable
		*
		* @return true if the Widget is not detected by mouse cursor unless the cursor is within the widget's dimensions
		* and over a non-transparent pixel, false if the Widget is detected simply when the cursor enters widget bounds.
		*/
		//virtual bool isTransparencyPickable() = 0;

		IGUISystem* getGUISystem() const{
			return m_pSysem;
		}

		virtual widget::ENUM_TYPE getType() const{
			return m_type;
		}

		virtual widget::ENUM_STATE getState() const = 0;

		const core::array<video::RenderEntry2D>& getRenderEntries() const{
			return m_renderEntries;
		}

		void setDirty(bool on)
		{
			m_bDirty=on;
			if(m_bDirty)
			{
				if(m_parent)
					m_parent->setDirty(true);
			}
			else
			{
				WidgetList::Iterator it=m_children.begin();
				for(;it!=m_children.end();++it)
					(*it)->setDirty(false);
			}
		}

		bool isDirty() const{return m_bDirty;}

		virtual bool isVisible() const{
			return m_bVisible;
		}

		virtual IWidget* getParent() const{
			return m_parent;
		}

		virtual const core::recti& getAbsoluteRectangle() const
		{
			return m_absoluteRect;
		}

		virtual void setMessageReceivable(bool on){m_bMessageReceivable=on;}
		virtual bool isMessageReceivable() const{return m_bMessageReceivable;}

		/**
		 * @brief Returns the visible area of the element.
		 *
		 * @return the area
		 */
		const core::recti& getAbsoluteClippingRect() const
		{
			return m_absoluteClippingRect;
		}

		/**
		 * @brief Returns true if a point is within this element.
		 *
		 * Note: Elements with a shape other than a rectangle should override this method
		 * @param point : checking point
		 * @return true if the point is inside the widget, otherwise return false.
		 */
		virtual bool isPointInside(const core::position2di& point) const
		{
			return m_absoluteClippingRect.isPointInside(point);
		}

		virtual ITheme* getTheme() const{
			if(m_pSysem)
				return m_pSysem->getBindedTheme();
			return NULL;
		}

		virtual void addChild(IWidget* child)
		{
			addChildToEnd(child);
			if (child)
			{
				child->updateAbsolutePosition();
			}
		}

		virtual void remove()
		{
			if (m_parent)
				m_parent->removeChild(this);
		}

		virtual void removeChild(IWidget* child)
		{
			//TODO 链表遍历？
			WidgetList::Iterator it = m_children.begin();
			for (; it != m_children.end(); ++it)
			{
				if ((*it) == child)
				{
					(*it)->m_parent = NULL;
					(*it)->drop();
					m_children.erase(it);
					return;
				}
			}
		}

		virtual void updateAbsolutePosition()
		{
			recalculateAbsolutePosition(false);

			// update all children
			WidgetList::Iterator it = m_children.begin();
			for (; it != m_children.end(); ++it)
			{
				(*it)->updateAbsolutePosition();
			}
		}

		//TODO 主要用于ScrollBar
		// @brief animate the element and its children.
		virtual void onPostRender(u32 timeMs)
		{
			if ( isVisible() )
			{
				WidgetList::Iterator it = m_children.begin();
				for (; it != m_children.end(); ++it)
					(*it)->onPostRender( timeMs );
			}
		}

		void setPartial(bool on)
		{
			m_bPartial=on;
		}

		// @returns true if this element was created as part of its parent control
		bool isPartial() const
		{
			return m_bPartial;
		}

		/**
		* @brief get the real widget of the widget
		*
		* Because some widget is partial of a widget(real),
		* if the widget is partial, we will get it's unpartial ancestor
		*
		* @return the real widget
		*/
		IWidget* getRealWidget()
		{
			IWidget* temp = this;
			while( temp && temp->isPartial() )
			{
				temp = temp->getParent();
			}
			return temp;
		}

		/** @brief Get the topmost GUI widget at the specific position.
		*
		* This will check this GUI widget and all of its descendants, so it
		* may return this GUI widget.  To check all GUI widgets, call this
		* function on the root widget of the gui system. Note
		* that the root widget is the size of the screen, so doing so (with
		* an on-screen point) will always return the root widget if no other
		* element is above it at that point.
		*
		* @param point: The point at which to find a GUI element.
		* @return the topmost GUI widget at that point, or NULL if there are
		* no candidate elements at this point.
		*/
		IWidget* getWidgetFromPoint(const core::position2di& point)
		{
			IWidget* target = NULL;

			// we have to search from back to front, because later children
			// might be drawn over the top of earlier ones.

			WidgetList::Iterator it = m_children.getLast();

			if (isVisible())
			{
				while(it != m_children.end())
				{
					target = (*it)->getWidgetFromPoint(point);
					if (target)
						return target;

					--it;
				}
			}

			if (isVisible() && isPointInside(point))
				target = this;

			return target;
		}
	};
}
}
#endif