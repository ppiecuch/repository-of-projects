#include "heapsortTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( heapsortTestCase );

void heapsortTestCase::setUp()
{
}
void heapsortTestCase::sort1()
{
	u32 arr[8]={49,38,65,97,76,13,27,49};
	heapsort<u32>(arr,8);
	printf("\n");
	for(u32 i=0;i<8;++i){
		printf("%d,",arr[i]);
	}
	printf("\n");
	CPPUNIT_ASSERT(arr[0]==13&&arr[1]==27&&arr[2]==38&&arr[3]==49&&arr[4]==49&&arr[5]==65&&arr[6]==76&&arr[7]==97);
}
void heapsortTestCase::sort2()
{
	u32 arr[18]={22,12,13,8,9,20,33,42,44,38,24,28,60,58,74,49,86,53};
	heapsort<u32>(arr,18);
	printf("\n");
	for(u32 i=0;i<18;++i){
		printf("%d,",arr[i]);
	}
	printf("\n");
	CPPUNIT_ASSERT(arr[0]==8&&arr[1]==9&&arr[2]==12&&arr[3]==13&&arr[4]==20&&arr[5]==22&&arr[6]==24&&arr[7]==28&&arr[8]==33
		&&arr[9]==38&&arr[10]==42&&arr[11]==44&&arr[12]==49&&arr[13]==53&&arr[14]==58&&arr[15]==60&&arr[16]==74&&arr[17]==86);
}
void heapsortTestCase::sort3()
{
	u32 arr[2]={49,38};
	heapsort<u32>(arr,2);
	printf("\n");
	for(u32 i=0;i<2;++i){
		printf("%d,",arr[i]);
	}
	printf("\n");
	CPPUNIT_ASSERT(arr[0]==38&&arr[1]==49);
}