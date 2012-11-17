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
		m_bCalling=false;
		m_between.StartCycle=0;
		m_between.EndCycle=0;
		m_between.StartTime.time=0;
		m_between.StartTime.millitm=0;
		m_between.EndTime.time=0;
		m_between.EndTime.millitm=0;
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
			u64 cyclediff=getCycleDiff();
			m_report.TimeDiff=getTimeDiff();
			u64 CPUFreq=cyclediff*1000/m_report.TimeDiff;
			APIMap& apiMap=m_report.ApiInfos;
			APIMap::Iterator it=apiMap.getIterator();
			for(;!it.atEnd();++it)
			{
				SProfileReport::SAPIReport* report=it->getValue();
				report->CallCountAvg=(f32)report->CallCount/(f32)m_report.FrameCount;
				report->TimeConsumePct=(f32)((f64)report->TimeConsume*100.f/(f64)m_uAllCallConsume);
				report->TimeConsume=report->TimeConsume*1000/CPUFreq;
				report->TimeConsumeMin=report->TimeConsumeMin*1000/CPUFreq;
				report->TimeConsumeMax=report->TimeConsumeMax*1000/CPUFreq;
				report->TimeConsumeAvg=report->TimeConsume/report->CallCount;
			}
		}
		m_bNeedUpdate=false;
	}

	void CProfile::registerFrame(){
		if(m_between.StartCycle==0)
		{
			m_between.StartCycle=getNanoSecond();
			updateTime(m_between.StartTime);
		}
		m_between.EndCycle=getNanoSecond();
		updateTime(m_between.EndTime);
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
	void CProfile::startCall(u32 id,const char* name){
		YON_DEBUG_BREAK_IF(m_bCalling);
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Node* node=apiMap.find(id);
		if(node==NULL)
		{
			SProfileReport::SAPIReport* report=new SProfileReport::SAPIReport();
			reset(report);
			report->Name=name;
			apiMap[id]=report;
			updateCallStart(report);
		}
		else
		{
			SProfileReport::SAPIReport* report=node->getValue();
			updateCallStart(report);
		}
		m_bNeedUpdate=true;
		m_bCalling=true;
	}
	void CProfile::endCall(u32 id){
		YON_DEBUG_BREAK_IF(!m_bCalling);
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Node* node=apiMap.find(id);
		YON_DEBUG_BREAK_IF(node==NULL);
		SProfileReport::SAPIReport* report=node->getValue();
		updateCallEnd(report);
		m_bNeedUpdate=true;
		m_bCalling=false;
	}

	void CProfile::report(){
		update();
		printf("FrameCount:%u\tTimeDiff:%u\r\n",m_report.FrameCount,m_report.TimeDiff);
		printf("All info of apis:\r\n");
		APIMap& apiMap=m_report.ApiInfos;
		APIMap::Iterator it=apiMap.getIterator();
		printf("Name\t\tCallCount\tCallCountAvg\tTimeConsume\tTimeConsumeMin\tTimeConsumeMax\tTimeConsumeAvg\tTimeConsumePct\r\n");
		for(;!it.atEnd();++it)
		{
			SProfileReport::SAPIReport* report=it->getValue();
			printf("%-15.15s\t%u\t\t%.2f\t\t%llu\t%llu\t%llu\t%llu\t%.2f%%\r\n",report->Name.c_str(),report->CallCount,report->CallCountAvg,report->TimeConsume,report->TimeConsumeMin,report->TimeConsumeMax,report->TimeConsumeAvg,report->TimeConsumePct);
		}
	}

}
}