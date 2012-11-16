#include "CProfile.h"

namespace yon{
namespace debug{

	CProfile::CProfile()
	{
		reset();
	}

	CProfile::~CProfile(){
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Iterator it=apiMap.getIterator();
		for(;!it.atEnd();++it)
		{
			SProfileReport::SAPIReport* report=it->getValue();
			delete report;
		}
		apiMap.clear();
	}

	void CProfile::reset(){
		m_bNeedUpdate=true;
		m_uStartTime=0;
		m_uEndTime=0;
		m_uAllCallConsume=0;
		m_report.FrameCount=0;
		m_report.TimeDiff=0;
		m_report.ApiInfos.clear();
	}

	void CProfile::reset(SProfileReport::SAPIReport* report){
		report->_calltime=0;
		report->CallCount=0;
		report->CallCountAvg=0;
		report->Name="";
		report->TimeConsume=0;
		report->TimeConsumeAvg=0;
		report->TimeConsumeMax=0;
		report->TimeConsumeMin=LONG_MAX;
		report->TimeConsumePct=0.f;
	}

	void CProfile::update(){
		if(m_bNeedUpdate)
		{
			m_report.TimeDiff=m_uEndTime-m_uStartTime;
			APIMap& apiMap=m_report.ApiInfos;
			APIMap::Iterator it=apiMap.getIterator();
			for(;!it.atEnd();++it)
			{
				SProfileReport::SAPIReport* report=it->getValue();
				report->CallCountAvg=report->CallCount/m_report.FrameCount;
				report->TimeConsumeAvg=report->TimeConsume/report->CallCount;
				report->TimeConsumePct=(f32)((f64)report->TimeConsume*100.f/(f64)m_uAllCallConsume);
			}
		}
		m_bNeedUpdate=false;
	}

	void CProfile::registerFrame(){
		if(m_uStartTime==0)
			m_uStartTime=getNanoSecond();
		m_uEndTime=getNanoSecond();
		++m_report.FrameCount;
		m_bNeedUpdate=true;
	}
	void CProfile::updateCallStart(SProfileReport::SAPIReport* report){
		report->_calltime=getNanoSecond();
	}
	void CProfile::updateCallEnd(SProfileReport::SAPIReport* report){
		++report->CallCount;
		u64 diff=getNanoSecond()-report->_calltime;
		report->TimeConsume+=diff;
		m_uAllCallConsume+=diff;
		if(report->TimeConsumeMin>diff)
			report->TimeConsumeMin=diff;
		if(report->TimeConsumeMax<diff)
			report->TimeConsumeMax=diff;
	}
	void CProfile::startCall(void* fun,const char* name){
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Node* node=apiMap.find(fun);
		if(node==NULL)
		{
			SProfileReport::SAPIReport* report=new SProfileReport::SAPIReport();
			reset(report);
			report->Name=name;
			apiMap[fun]=report;
			updateCallStart(report);
		}
		else
		{
			SProfileReport::SAPIReport* report=node->getValue();
			updateCallStart(report);
		}
		m_bNeedUpdate=true;
	}
	void CProfile::endCall(void* fun){
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Node* node=apiMap.find(fun);
		YON_DEBUG_BREAK_IF(node==NULL);
		SProfileReport::SAPIReport* report=node->getValue();
		updateCallEnd(report);
		m_bNeedUpdate=true;
	}

	void CProfile::report(){
		update();
		printf("FrameCount:%u\tTimeDiff:%u\r\n",m_report.FrameCount,m_report.TimeDiff);
		printf("All info of apis:\r\n");
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Iterator it=apiMap.getIterator();
		printf("CallCount\tCallCountAvg\tTimeConsume\tTimeConsumeMin\tTimeConsumeMax\tTimeConsumeAvg\tTimeConsumePct\r\n");
		for(;!it.atEnd();++it)
		{
			SProfileReport::SAPIReport* report=it->getValue();
			printf("%u\t\t%u\t\t%llu\t%llu\t%llu\t%llu\t%.2f%%\r\n",report->CallCount,report->CallCountAvg,report->TimeConsume,report->TimeConsumeMin,report->TimeConsumeMax,report->TimeConsumeAvg,report->TimeConsumePct);
		}
	}

}
}