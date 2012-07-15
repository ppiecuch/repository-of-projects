#ifndef _YON_PLATFORM_ICURSORCONTROL_H_
#define _YON_PLATFORM_ICURSORCONTROL_H_

#include "IReferencable.h"
#include "position2d.h"
#include "rect.h"

namespace yon{
namespace platform{

	class ICursorControl : public virtual core::IReferencable{
	public:
		virtual void setVisible(bool visible) = 0;
		virtual bool isVisible() const = 0;

		//! Sets the new position of the cursor.
		/** The position must be
		between (0.0f, 0.0f) and (1.0f, 1.0f), where (0.0f, 0.0f) is
		the top left corner and (1.0f, 1.0f) is the bottom right corner of the
		render window.
		\param pos New position of the cursor. */
		virtual void setPosition(const core::position2df &pos) = 0;
		virtual void setPosition(f32 x, f32 y) = 0;
		virtual core::position2df getRelativePosition() = 0;

		//! Sets the new position of the cursor.
		/** \param pos: New position of the cursor. The coordinates are pixel units. */
		virtual void setPosition(const core::position2di &pos) = 0;
		virtual void setPosition(s32 x, s32 y) = 0;
		virtual const core::position2di& getPosition() = 0;

		//! Sets an absolute reference rect for setting and retrieving the cursor position.
		/** If this rect is set, the cursor position is not being calculated relative to
		the rendering window but to this rect. You can set the rect pointer to 0 to disable
		this feature again. This feature is useful when rendering into parts of foreign windows
		for example in an editor.
		\param rect: A pointer to an reference rectangle or 0 to disable the reference rectangle.*/
		virtual void setReferenceRect(core::recti* rect=0) = 0;
	};
}
}
#endif