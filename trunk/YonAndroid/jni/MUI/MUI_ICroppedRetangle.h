#ifndef _MUI_ICROPPEDRETANGLE_H_
#define _MUI_ICROPPEDRETANGLE_H_

#include "MUI_Prepare.h"

namespace mui{

	class ICroppedRectangle{
	protected:
		//ICroppedRectangle* mCroppedParent;
		ICroppedRectangle* m_pParent;
		//bool mIsMargin;
		bool m_bCroppedByParent;

		core::margini m_margin;
		core::recti m_coordinate;				//�����Ͻ�Ϊ��������ڸ��ڵ�ƫ����
		core::position2di m_absolutePosition;

		/** True if rectangle is cropped by parent rectangle */
		//bool _isMargin() const
		bool isCroppedByParent() const
		{
			return m_bCroppedByParent;
		}
		// Get cropped by parent rectangle coordinates
		//int _getViewLeft() const
		s32 getCroppedLeft() const
		{
			return m_coordinate.topLeft.x + m_margin.Left;
		}
		//int _getViewRight() const
		s32 getCroppedRight() const
		{
			return m_coordinate.bottomRight.x - m_margin.Right;
		}
		//int _getViewTop() const
		s32 getCroppedTop() const
		{
			return m_coordinate.topLeft.y + m_margin.Top;
		}
		//int _getViewBottom() const
		s32 getCroppedBottom() const
		{
			return m_coordinate.bottomRight.y - m_margin.Bottom;
		}
		//int _getViewWidth() const
		s32 getCroppedWidth() const
		{
			return m_coordinate.getWidth() - m_margin.Left - m_margin.Right;
		}
		//int _getViewHeight() const
		s32 getCroppedHeight() const
		{
			return m_coordinate.getHeight() - m_margin.Top - m_margin.Bottom;
		}

		void setCroppedParent(ICroppedRectangle* parent)
		{
			m_pParent = parent;
		}

		const core::margini& getMargin() const
		{
			return m_margin;
		}

		//parent.Left
		//��
		//������������������
		//��  ��������������
		//��  �� margin ����
		//������----��  ����
		//������    ��  ����
		//�����������੤����
		//������������    ��
		//������������������
		//  ����
		//  ����
		//  ����
		//  ��parent.margin.Left
		//  ��
		//  this.Left
		//��Ȼ��LTRB������Գ���
		//����coordinate���ԣ�LTRB���Ǳ�Retangle��parent retangle�����Ͻ�Ϊ����ĳ���
		//����margin���ԣ�LTRBҲ����parentΪ���գ���ͬ����margin.Left�������retangle��Left��
		//margin.Right�������retangle��Right���Դ����ƣ�ע����ȡֵ��Χ��>=0����>0ʱ����ʾ��ǰretangle���ü��ˣ�
		//������ü�������true�����򷵻�false
		bool updateMargin()
		{
			bool result = false;
			if (getLeft() < m_pParent->m_margin.Left)
			{
				m_margin.Left = m_pParent->m_margin.Left - getLeft();
				result = true;
			}
			else
			{
				m_margin.Left = 0;
			}

			if (getRight() > m_pParent->getWidth() - m_pParent->m_margin.Right)
			{
				m_margin.Right = getRight() - (m_pParent->getWidth() - m_pParent->m_margin.Right);
				result = true;
			}
			else
			{
				m_margin.Right = 0;
			}

			if (getTop() < m_pParent->m_margin.Top)
			{
				m_margin.Top = m_pParent->m_margin.Top - getTop();
				result = true;
			}
			else
			{
				m_margin.Top = 0;
			}

			if (getBottom() > m_pParent->getHeight() - m_pParent->m_margin.Bottom)
			{
				m_margin.Bottom = getBottom() - (m_pParent->getHeight() - m_pParent->m_margin.Bottom);
				result = true;
			}
			else
			{
				m_margin.Bottom = 0;
			}

			return result;
		}

		//bool _checkOutside() const 
		bool isFullCropped() const
		{
			return ( (getRight() < m_pParent->m_margin.Left ) ||
				(getLeft() > m_pParent->getWidth() - m_pParent->m_margin.Right ) ||
				(getBottom() < m_pParent->m_margin.Top  ) ||
				(getTop() > m_pParent->getHeight() - m_pParent->m_margin.Bottom ) );
		}
	public:
		ICroppedRectangle() :
		  m_bCroppedByParent(false),
			  m_pParent(NULL)
		{}
		virtual ~ICroppedRectangle(){}

		ICroppedRectangle* getCroppedParent()
		{
			return m_pParent;
		}

		virtual void setPosition(const core::position2di& value)
		{
			m_coordinate.topLeft = value;
		}

		virtual void setSize(const core::dimension2di& value)
		{
			m_coordinate.bottomRight.x = value.w+m_coordinate.topLeft.x;
			m_coordinate.bottomRight.y = value.h+m_coordinate.topLeft.y;
		}

		virtual void setCoord(const core::recti& value)
		{
			m_coordinate = value;
		}

		const core::position2di& getPosition() const
		{
			return m_coordinate.topLeft;
		}

		core::dimension2di getSize() const
		{
			return m_coordinate.getSize();
		}

		const core::recti& getCoordinate() const
		{
			return m_coordinate;
		}

		//Get position in screen coordinates
		const core::position2di& getAbsolutePosition() const
		{
			return m_absolutePosition;
		}

		//Get rectangle in screen coordinates 
		core::recti getAbsoluteRect() const
		{
			return core::recti(m_absolutePosition.x, m_absolutePosition.y, m_absolutePosition.x + m_coordinate.getWidth(), m_absolutePosition.y + m_coordinate.getHeight());
		}
		//��getAbsoluteRect������ͬ
		//Get coordinate in screen coordinates
		//core::recti getAbsoluteCoordinate() const
		//{
			//return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height);
		//	return core::recti(m_absolutePosition.x, m_absolutePosition.y, m_absolutePosition.x + m_coordinate.getWidth(), m_absolutePosition.y + m_coordinate.getHeight());
		//}

		//Get X in screen coordinates
		int getAbsoluteLeft() const
		{
			return m_absolutePosition.x;
		}

		//Get Y in screen coordinates
		int getAbsoluteTop() const
		{
			return m_absolutePosition.y;
		}

		s32 getLeft() const
		{
			return m_coordinate.topLeft.x;
		}
		s32 getRight() const
		{
			return m_coordinate.bottomRight.x;
		}
		s32 getTop() const
		{
			return m_coordinate.topLeft.y;
		}
		s32 getBottom() const
		{
			return m_coordinate.bottomRight.y;
		}
		s32 getWidth() const
		{
			return m_coordinate.getWidth();
		}
		s32 getHeight() const
		{
			return m_coordinate.getHeight();
		}
	};
}
#endif