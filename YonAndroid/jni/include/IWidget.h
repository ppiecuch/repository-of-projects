#ifndef _YON_GUI_IWIDGET_H_
#define _YON_GUI_IWIDGET_H_

#include "IReferencable.h"
#include "IEventReceiver.h"
#include "yonString.h"
#include "position2d.h"
#include "rect.h"
#include "yonList.h"
#include "SRenderUnit.h"


//refer to:http://www.php100.com/manual/css3_0

namespace yon{
namespace gui{

	class ITheme;
	class IGUISystem;

	class IWidget : public virtual core::IReferencable,public virtual event::IEventReceiver{
	public:

		enum ENUM_TYPE{
			NONE = -1,
			BUTTON = 0,
			ENUM_TYPE_COUNT
		};

		//TODO 水平垂直分开
		enum ENUM_ALIGN{
			//! Aligned to parent's top or left side (default)
			UPPERLEFT=0,
			//! Aligned to parent's bottom or right side
			LOWERRIGHT,
			//! Aligned to the center of parent
			CENTER,
			ENUM_ALIGN_COUNT
		};

		enum MASK_STATE{
			DOWN	= 0x00000001,
			UP		= 0x00000002,
			OVER	= 0x00000004,
			OUT		= 0x00000008,
			//TODO 长按
			//HOLD
			ENUM_STATE_COUNT
		};

		//const core::stringc& getTag() const{
			//TODO
		//}
	protected:

		typedef core::list<IWidget*> WidgetList;

		IGUISystem* m_pSysem;

		IWidget::ENUM_TYPE m_type;
		core::stringc m_id;
		WidgetList m_children;
		IWidget* m_parent;
		bool m_bVisible;
		bool m_bEventReceiable;

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
		ENUM_ALIGN m_alignLeft, m_alignRight, m_alignTop, m_alignBottom;


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
			case UPPERLEFT:
				break;
			case LOWERRIGHT:
				m_desiredRect.topLeft.x += diffx;
				break;
			case CENTER:
				m_desiredRect.topLeft.x += diffx/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.UpperLeftCorner.X = core::round32(ScaleRect.UpperLeftCorner.X * fw);
				//	break;
			}

			switch (m_alignRight)
			{
			case UPPERLEFT:
				break;
			case LOWERRIGHT:
				m_desiredRect.bottomRight.x += diffx;
				break;
			case CENTER:
				m_desiredRect.bottomRight.x += diffx/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.LowerRightCorner.X = core::round32(ScaleRect.LowerRightCorner.X * fw);
				//	break;
			}

			switch (m_alignTop)
			{
			case UPPERLEFT:
				break;
			case LOWERRIGHT:
				m_desiredRect.topLeft.y += diffy;
				break;
			case CENTER:
				m_desiredRect.topLeft.y += diffy/2;
				break;
				//case EGUIA_SCALE:
				//	DesiredRect.UpperLeftCorner.Y = core::round32(ScaleRect.UpperLeftCorner.Y * fh);
				//	break;
			}

			switch (m_alignBottom)
			{
			case UPPERLEFT:
				break;
			case LOWERRIGHT:
				m_desiredRect.bottomRight.y += diffy;
				break;
			case CENTER:
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

		core::array<video::SRenderEntry2D> m_renderEntries;

	public:
		IWidget(IWidget::ENUM_TYPE type,IGUISystem* guiSystem,IWidget* parent,const core::stringc& id,const core::recti& rectangle)
			:m_type(type),m_parent(parent),m_id(id),m_bEventReceiable(true),
			m_relativeRect(rectangle),m_absoluteRect(rectangle),m_desiredRect(rectangle),m_absoluteClippingRect(rectangle),
			m_bVisible(true),m_alignLeft(UPPERLEFT), m_alignRight(UPPERLEFT), m_alignTop(UPPERLEFT), m_alignBottom(UPPERLEFT){

		}

		IGUISystem* getGUISystem() const{
			return m_pSysem;
		}

		virtual IWidget::ENUM_TYPE getType() const{
			return m_type;
		}

		const core::array<video::SRenderEntry2D>& getRenderEntries() const{
			return m_renderEntries;
		}

		void setDirty(bool on)
		{
			m_bDirty=on;
			if(m_bDirty&&m_parent)
				m_parent->setDirty(on);
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

		virtual void setEventReceivable(bool on){m_bEventReceiable=on;}
		virtual bool isEventReceivable() const{return m_bEventReceiable;}

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

		virtual ITheme* getTheme() const = 0;

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
	};

	const static c8* WIDGET_TYPE_NAMES[IWidget::ENUM_TYPE_COUNT] =
	{
		"button"
	};
	const static c8* WIDGET_ALIGN_NAMES[IWidget::ENUM_ALIGN_COUNT] =
	{
		"upperleft",
		"lowerright",
		"center"
	};
	const static c8* WIDGET_STATE_NAMES[IWidget::ENUM_STATE_COUNT]=
	{
		"down",
		"up",
		"over",
		"out"
	};
}
}
#endif