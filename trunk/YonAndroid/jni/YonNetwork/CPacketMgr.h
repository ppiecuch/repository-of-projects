#ifndef _CPACKETMGR_H_
#define _CPACKETMGR_H_

#include "MyGUI_UString.h"
#include "yon.h"
using namespace yon;
#include "ISingleton.h"



class CGameNetwork;
class CCmdPacket;



class CPacketMgr : public CSingleton<CPacketMgr>
{
private:
	friend class CCmdPacket;
	CGameNetwork* m_pNetwork;
public:
	CPacketMgr();
	~CPacketMgr();

	void Init(){}

	void init(CGameNetwork* network){
		this->m_pNetwork=network;
	}

	//登录
	void sendC2SLoginPacket(const char* account, const char* password, const char* lianMeng, const char* ua);
	//注册
	void sendC2SRegisterPacket(const char* account, const char* password,int type);
	//请求角色列表
	void sendC2SPlayerListReqPacket(char* serverKey);

	//选择角色进入游戏
	void sendC2SEnterGamePacket(char* charKey);

	//创建角色
	void sendC2SCreateRolePacket(const char* name, int jobId, int sex,bool isVisitor);

	//删除角色
	void sendC2SDeleteRolePacket(char* charKey);

	//技能树升级
	void sendC2SUpSkillLevel(int skillID);

	//使用技能
	void sendC2SAttack(int uuid, int skillID);

	//使用技能
	void sendC2SPreAttack(int uuid, int skillID);

	//坐标同步
	void sendC2SLocation(int posX, int posY);

	//坐标改变(摇杆)
	void sendC2SLocationList(core::array<core::vector2df>& pos);

	//坐标改变(寻路)
	void sendC2SMoveTo(core::array<core::vector2df>& pos);

	//npc对话
	void sendC2STalk2NPC(int uuid, int contextType, int taskId);

	//采集
	void sendC2SCollect(int type, int uuid);

	//地图加载完成
	void sendC2SMapLoadComplete();

	//技能树保存
	void sendC2SSkillSave(int which);

	//技能树载入
	void sendC2SSkillLoad(int which);

	//技能树重置
	void sendC2SSkillReset();

	//改变杀戮/工会/和平模式
	void sendC2SChangeAttackType(int attackType);

	//
	// cmd命令协议
	//
	// strCmd - 必须是utf-8编码
	void sendC2SCmd(const char* strCmd);
	void sendC2SCmd(std::string& strCmd);
	// strCmd - UString对象
	void sendC2SCmd(const MyGUI::UString& strCmd);

	void sendC2SItemInfo(int uuid, int itemServerId);

	//邮件操作
	void sendEmailInfoC2S(const MyGUI::UString &s_name,unsigned int s_uuid,const MyGUI::UString &s_title,const MyGUI::UString &s_info,int s_money,
		unsigned int s_things_one,unsigned char s_number_one,	
		unsigned int s_things_two,unsigned char s_number_two,
		unsigned int s_things_three,unsigned char s_number_three);
	//聊天
	void sendC2SSendChatMsg(const MyGUI::UString& srolename, int chattype, const MyGUI::UString& chatontent);

	void sendC2SSendSomeMessage(void);

	void sendC2SHonourUpdate(short id);

	//请求地图怪物列表
	void sendC2SMapActor(int mapID);

	//请求地图队友列表
	void sendC2STeamMates(int mapID);

	//请求寻路数据
	void sendC2SPathFinding(int mapDestID,int npcID,int DestX, int DestY);

	bool IsSenderThreadExit();
};

#define GetPacketMgr()		CPacketMgr::GetInstance()

#endif