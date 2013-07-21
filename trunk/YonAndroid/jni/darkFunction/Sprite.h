#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "yonString.h"
#include "ITexture.h"
#include "yonArray.h"
#include "rect.h"
#include "SVertex.h"

using namespace yon;

struct Sprite{
	core::stringc Path;
	core::stringc Name;
	core::stringc FullName;
	//coordinate Coord;
	core::rectf Rect;
	core::dimension2di Size;
	scene::SVertex Vertices[4];

	static u16 Indices[6];

	//0 1
	//3 2

	void setPosition(f32 x,f32 y,f32 z){
		Vertices[0].set(x,y,z,Rect.topLeft.x,Rect.topLeft.y,video::COLOR_WHITE);
		Vertices[1].set(x+Size.w,y,z,Rect.bottomRight.x,Rect.topLeft.y,video::COLOR_WHITE);
		Vertices[2].set(x+Size.w,y+Size.h,z,Rect.bottomRight.x,Rect.bottomRight.y,video::COLOR_WHITE);
		Vertices[3].set(x,y+Size.h,z,Rect.topLeft.x,Rect.bottomRight.y,video::COLOR_WHITE);
	}

	void setCenterPosition(f32 x,f32 y,f32 z){
		s32 hw=Size.w>>1;
		s32 hh=Size.h>>1;
		Vertices[0].set(x-hw,y-hh,z,Rect.topLeft.x,Rect.topLeft.y,video::COLOR_WHITE);
		Vertices[1].set(x+Size.w-hw,y-hh,z,Rect.bottomRight.x,Rect.topLeft.y,video::COLOR_WHITE);
		Vertices[2].set(x+Size.w-hw,y+Size.h-hh,z,Rect.bottomRight.x,Rect.bottomRight.y,video::COLOR_WHITE);
		Vertices[3].set(x-hw,y+Size.h-hh,z,Rect.topLeft.x,Rect.bottomRight.y,video::COLOR_WHITE);
	}
};

struct Spritesheet : public core::IReferencable{
	video::ITexture* Texture;
	core::array<Sprite> Sprites;
};

struct Animation{
	struct SpriteExt{
		core::stringc Name;
		s32 X,Y;
		s32 ZOrder;
		s32 Angle;
		bool FlipH,FlipV;
		Sprite* Sprite;

		SpriteExt():Sprite(NULL){}
	};
	struct Cell{
		s32 Index;
		s32 Delay;
		core::array<SpriteExt> Sprites;
	};
	core::stringc Name;
	s32 Loop;
	core::array<Cell> Cells;
};

struct AnimationGroup : public core::IReferencable{
	core::array<Animation> Animiations;
	Spritesheet* Sheet;
	core::stringc SpriteSheet;

	AnimationGroup():Sheet(NULL){}
};
#endif