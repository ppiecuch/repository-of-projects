#ifndef _YON_SCENE_ANIMATOR_CANIMATORALPHA_H_
#define _YON_SCENE_ANIMATOR_CANIMATORALPHA_H_

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorAlpha : public IAnimator{
	private:
		u32 m_uUnitIndex;
		u8 m_uMinValue;
		u8 m_uMaxValue;
		s32 m_uIncrement;
	public:
		CAnimatorAlpha(u32 unitIndex,u8 minValue,u8 maxValue,s32 inc):m_uUnitIndex(unitIndex),m_uIncrement(inc),m_uMinValue(minValue),m_uMaxValue(maxValue){}

		virtual void animateNode(IModel* model, u32 timeMs){
			IUnit* unit=model->getEntity()->getUnit(m_uUnitIndex);
			//TODO
			//疑问：不是说是ABGR吗，怎么这里是RGBA(应该是跟大小端有关)
			u32 offset=(u32)getVertexColorOffset(unit->getVertexType())+3;
			u32 size=unit->getShap()->getVertexSize();
			u8* pointer=(u8*)unit->getShap()->getVertices();
			u8 color;
			for(u32 i=0;i<unit->getShap()->getVertexCount();++i)
			{
				color=*(pointer+offset+size*i);
				if(color<m_uMaxValue)
				{
					color+=m_uIncrement;
					color=core::clamp(color,m_uMinValue,m_uMaxValue);
				}
				else
				{
					color=m_uMinValue;
				}
				*(pointer+offset+size*i)=color;
			}
			//TODO changedID
		}
		virtual ENUM_ANIMATOR_TYPE getType() const{
			return ENUM_ANIMATOR_TYPE_ALPHA;
		}
	};
}
}
}
#endif