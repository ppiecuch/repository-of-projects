#include "convertorTestCase.h"
using namespace yon;

CPPUNIT_TEST_SUITE_REGISTRATION( convertorTestCase );

void convertorTestCase::setUp()
{
}
void convertorTestCase::same()
{
	void* src1=this;
	void* dest1=NULL;
	core::convertor<pvoid,pvoid>::convert(src1,dest1);
	CPPUNIT_ASSERT(dest1==this);

	s32 src2=523;
	s32 dest2=0;
	core::convertor<s32,s32>::convert(src2,dest2);
	CPPUNIT_ASSERT(dest2==523);

	u32 src3=523;
	u32 dest3=0;
	core::convertor<u32,u32>::convert(src3,dest3);
	CPPUNIT_ASSERT(dest3==523);

	f32 src4=5.2185f;
	f32 dest4=0;
	core::convertor<f32,f32>::convert(src4,dest4);
	CPPUNIT_ASSERT(core::equals(dest4,5.2185f));

	bool src5=true;
	bool dest5=false;
	core::convertor<bool,bool>::convert(src5,dest5);
	CPPUNIT_ASSERT(dest5==true);

	c8 src6=-52;
	c8 dest6=0;
	core::convertor<c8,c8>::convert(src6,dest6);
	CPPUNIT_ASSERT(dest6==-52);

	u16 src7=9854;
	u16 dest7=0;
	core::convertor<u16,u16>::convert(src7,dest7);
	CPPUNIT_ASSERT(dest7==9854);

	core::vector2df src8=core::IDENTITY_VECTOR2DF;
	core::vector2df dest8=core::ORIGIN_VECTOR2DF;
	core::convertor<core::vector2df,core::vector2df>::convert(src8,dest8);
	CPPUNIT_ASSERT(dest8==core::IDENTITY_VECTOR2DF);

	video::SColorf src9=video::SColorf(video::COLOR_PURPLE);
	video::SColorf dest9;
	core::convertor<video::SColorf,video::SColorf>::convert(src9,dest9);
	CPPUNIT_ASSERT(dest9==src9);

	video::SColor src10=video::COLOR_YELLOW;
	video::SColor dest10;
	core::convertor<video::SColor,video::SColor>::convert(src10,dest10);
	CPPUNIT_ASSERT(dest10==video::COLOR_YELLOW);

	core::stringc src11="china0123465798";
	core::stringc dest11;
	core::convertor<core::stringc,core::stringc>::convert(src11,dest11);
	CPPUNIT_ASSERT(dest11==src11);
}
void convertorTestCase::basic()
{
	bool src1=true;
	s32 dest1=0;
	core::convertor<bool,s32>::convert(src1,dest1);
	CPPUNIT_ASSERT(dest1==1);

	bool src2=true;
	u32 dest2=0;
	core::convertor<bool,u32>::convert(src2,dest2);
	CPPUNIT_ASSERT(dest2==1);

	bool src3=true;
	f32 dest3=0;
	core::convertor<bool,f32>::convert(src3,dest3);
	CPPUNIT_ASSERT(core::equals(dest3,1.0f));

	c8 src4=25;
	u16 dest4=0;
	core::convertor<c8,u16>::convert(src4,dest4);
	CPPUNIT_ASSERT(dest4==25);

	s32 src5=586;
	u32 dest5=0;
	core::convertor<s32,u32>::convert(src5,dest5);
	CPPUNIT_ASSERT(dest5==586);

	s32 src6=523;
	bool dest6=false;
	core::convertor<s32,bool>::convert(src6,dest6);
	CPPUNIT_ASSERT(dest6==true);

	s32 src7=523;
	f32 dest7=0;
	core::convertor<s32,f32>::convert(src7,dest7);
	CPPUNIT_ASSERT(core::equals(dest7,523.f));

	f32 src8=523;
	bool dest8=false;
	core::convertor<f32,bool>::convert(src8,dest8);
	CPPUNIT_ASSERT(dest8==true);

	f32 src9=523;
	s32 dest9=0;
	core::convertor<f32,s32>::convert(src9,dest9);
	CPPUNIT_ASSERT(dest9==523);

	f32 src10=523;
	u32 dest10=0;
	core::convertor<f32,u32>::convert(src10,dest10);
	CPPUNIT_ASSERT(dest10==523);
}
void convertorTestCase::string2other()
{
	core::stringc src1("123456.12");
	f32 dest1=0;
	core::convertor<core::stringc,f32>::convert(src1,dest1);
	CPPUNIT_ASSERT(core::equals(dest1,123456.12f));

	core::stringc src2("123456.12");
	f64 dest2=0;
	core::convertor<core::stringc,f64>::convert(src2,dest2);
	CPPUNIT_ASSERT(core::equals(dest2,123456.12,0.01));

	core::stringc src3("-123456");
	s32 dest3=0;
	core::convertor<core::stringc,s32>::convert(src3,dest3);
	CPPUNIT_ASSERT(dest3==-123456);

	core::stringc src4("123456");
	u32 dest4=0;
	core::convertor<core::stringc,u32>::convert(src4,dest4);
	CPPUNIT_ASSERT(dest4==123456);

	core::stringc src5("125");
	u8 dest5=0;
	core::convertor<core::stringc,u8>::convert(src5,dest5);
	CPPUNIT_ASSERT(dest5==125);

	core::stringc src6("true");
	bool dest6=0;
	core::convertor<core::stringc,bool>::convert(src6,dest6);
	CPPUNIT_ASSERT(dest6==true);

	core::stringc src7("false");
	bool dest7=1;
	core::convertor<core::stringc,bool>::convert(src7,dest7);
	CPPUNIT_ASSERT(dest7==false);

	core::stringc src8("124.12 526.52");
	core::vector2df dest8;
	core::convertor<core::stringc,core::vector2df>::convert(src8,dest8);
	CPPUNIT_ASSERT(dest8==core::vector2df(124.12f,526.52f));

	core::stringc src9("#125FF634");
	video::SColor dest9;
	core::convertor<core::stringc,video::SColor>::convert(src9,dest9);
	CPPUNIT_ASSERT(dest9==video::SColor(0x125FF634));

	core::stringc src10("#125FF634");
	video::SColorf dest10;
	core::convertor<core::stringc,video::SColorf>::convert(src10,dest10);
	CPPUNIT_ASSERT(dest10==video::SColorf(video::SColor(0x125FF634)));
}
void convertorTestCase::other2string()
{
	f32 src1=1256.23f;
	core::stringc dest1;
	core::convertor<f32,core::stringc>::convert(src1,dest1);
	CPPUNIT_ASSERT(dest1.find("1256.23")==0);

	f64 src2=1123132256.23;
	core::stringc dest2;
	core::convertor<f64,core::stringc>::convert(src2,dest2);
	CPPUNIT_ASSERT(dest2.find("1123132256.23")==0);

	s32 src3=-1256;
	core::stringc dest3;
	core::convertor<s32,core::stringc>::convert(src3,dest3);
	CPPUNIT_ASSERT(dest3=="-1256");

	u32 src4=45641;
	core::stringc dest4;
	core::convertor<u32,core::stringc>::convert(src4,dest4);
	CPPUNIT_ASSERT(dest4=="45641");

	u8 src5=255;
	core::stringc dest5;
	core::convertor<u8,core::stringc>::convert(src5,dest5);
	CPPUNIT_ASSERT(dest5=="255");

	bool src6=true;
	core::stringc dest6;
	core::convertor<bool,core::stringc>::convert(src6,dest6);
	CPPUNIT_ASSERT(dest6=="true");

	c8* src7="I am hzb";
	core::stringc dest7;
	core::convertor<c8*,core::stringc>::convert(src7,dest7);
	CPPUNIT_ASSERT(dest7=="I am hzb");

	core::vector2df src8=core::IDENTITY_VECTOR2DF;
	core::stringc dest8;
	core::convertor<core::vector2df,core::stringc>::convert(src8,dest8);
	CPPUNIT_ASSERT(dest8=="1.0000 1.0000");

	video::SColor src9=video::COLOR_DEFAULT;
	core::stringc dest9;
	core::convertor<video::SColor,core::stringc>::convert(src9,dest9);
	CPPUNIT_ASSERT(dest9=="#FF472E13");

	video::SColorf src10=video::SColorf(video::COLOR_DEFAULT);
	core::stringc dest10;
	core::convertor<video::SColorf,core::stringc>::convert(src10,dest10);
	CPPUNIT_ASSERT(dest10=="#FF472E13");
}