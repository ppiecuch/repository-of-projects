#include "CPacketMgr.h"
#include "cmdpacket.h"
#include "gamenetworkprotocol.h"
#include "gamenetwork.h"

CPacketMgr::CPacketMgr():m_pNetwork(NULL)
{
}

CPacketMgr::~CPacketMgr()
{
}

bool CPacketMgr::IsSenderThreadExit()	{ return m_pNetwork?m_pNetwork->isConnected():true; }

void CPacketMgr::sendC2SLoginPacket(const char* account, const char* password, const char* lianMeng, const char* ua)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_ACCOUNT_C2S::CMD_C2SLoginReq);
	//数据
	p.WriteString(account);
	p.WriteString(password);
	p.WriteString(lianMeng);
	p.WriteString(ua);
	m_pNetwork->SendPacket(p.getBuffer());

}



void CPacketMgr::sendC2SPlayerListReqPacket(char* serverKey)
{
	CCmdPacket p;
	//重置封包 - 协议码
	p.BeginWrite();
	p.WriteByte(MSG_ROLE_SELECT_C2S::CMD_C2SPlayerListReq);

	//数据
	p.WriteString(serverKey);

	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SEnterGamePacket(char* charKey)
{
	CCmdPacket p;
	//重置封包 - 协议码
	p.BeginWrite();
	p.WriteByte(MSG_ROLE_SELECT_C2S::CMD_C2SGameLoginReq); // 协议码

	//数据
	p.WriteString(charKey);

	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SCreateRolePacket(const char* name, int jobId, int sex,bool isVisitor)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_ROLE_SELECT_C2S::CMD_C2SCreatePlayerReq); // 协议码

	p.WriteString(name);
	p.WriteByte(jobId);
	p.WriteByte(sex);
	p.WriteByte(isVisitor);

	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SDeleteRolePacket(char* charKey)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_ROLE_SELECT_C2S::CMD_C2SDelPlayerReq); // 协议码

	p.WriteString(charKey);

	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SAttack(int uuid, int skillID)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SAttack); // 协议码
	p.WriteInt32(uuid);
	p.WriteShort((short)skillID);
	// 	writeByte(0);
	m_pNetwork->SendPacket(p.getBuffer());
	// 	XC_DEBUG("sendC2SAttack skillID:%d->%d\r\n", skillID, uuid);
}

void CPacketMgr::sendC2SPreAttack(int uuid, int skillID)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SPreAttack); // 协议码
	p.WriteInt32(uuid);
	p.WriteShort((short)skillID);
	p.WriteByte(0);
	m_pNetwork->SendPacket(p.getBuffer());
	// 	XC_DEBUG("sendC2SPreAttack skillID:%d->%d\r\n", skillID, uuid);
}

void CPacketMgr::sendC2SLocation(int posX, int posY)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_MAP_C2S::CMD_C2SLocation);
	p.WriteShort((short)posX);
	p.WriteShort((short)posY);
	m_pNetwork->SendPacket(p.getBuffer());
	// 	xc::debug::Logger->info("sendC2SLocation*********************\r\n");
}

void CPacketMgr::sendC2SLocationList(core::array<core::vector2df>& pos)
{
	CCmdPacket p;
	// 	XC_DEBUG("\r\n*****************sendC2SLocationList*******************\r\n");
	int size = pos.size();
	p.BeginWrite();
	p.WriteByte(MSG_MAP_C2S::CMD_C2SLocationList);
	p.WriteByte(size);
	for(int i = 0; i < size; ++i)
	{
		p.WriteShort((short)pos[i].x);
		p.WriteShort((short)pos[i].y);
		// 		XC_DEBUG("X:%d,Y:%d\r\n",(short)pos[i].x,(short)pos[i].y);
	}
	m_pNetwork->SendPacket(p.getBuffer());
	// 	XC_DEBUG("\r\n");
}

void CPacketMgr::sendC2SMoveTo(core::array<core::vector2df>& pos)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_MAP_C2S::CMD_C2SLocationTarget);
	p.WriteShort((short)pos[0].x);
	p.WriteShort((short)pos[0].y);
	p.WriteShort((short)pos[1].x);
	p.WriteShort((short)pos[1].y);
	m_pNetwork->SendPacket(p.getBuffer());
	//   	XC_DEBUG("\r\n**********************sendC2SMoveTo*********************\r\n");
	// 	XC_DEBUG("X:%d,Y:%d,aimX:%d,aimY:%d\r\n", (short)pos[0].x, (short)pos[0].y, (short)pos[1].x, (short)pos[1].y);
	// 	XC_DEBUG("\r\n");
}

void CPacketMgr::sendC2STalk2NPC(int uuid, int contextType, int taskId)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_MISC_C2S::CMD_C2STalk2NPC);
	p.WriteInt32(uuid);
	p.WriteByte(contextType);
	p.WriteInt32(taskId);
	// 	if( 1 == contextType )
	// 	{
	// 		writeInt32(taskId);
	// 	}else 
	// 	{
	// 		writeInt32(-1);
	// 	}

	m_pNetwork->SendPacket(p.getBuffer());

	//GetNPCDialogMgr()->setContextType(contextType);

}

void CPacketMgr::sendC2SCollect(int type, int uuid)//type:0-start, 1-cancel, 2-finish
{
	std::stringstream ss;
	ss<<"collect:";

	switch(type)
	{
	case 0:
		ss<<"start:";
		break;
	case 1:
		ss<<"cancel:";
		break;
	case 2:
		ss<<"finish:";
		break;
	}

	ss<<uuid;
	sendC2SCmd(const_cast<char*>(ss.str().c_str()));
}

void CPacketMgr::sendC2SMapLoadComplete()
{
	std::stringstream ss;
	ss<<"LoadFinshed";
	sendC2SCmd(const_cast<char*>(ss.str().c_str()));
}

void CPacketMgr::sendC2SUpSkillLevel( int skillID )
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SSkillUpLevel);
	p.WriteShort(skillID);
	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SSendChatMsg(const MyGUI::UString& srolename, int chattype, const MyGUI::UString& chatcontent)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_MISC_C2S::CMD_SendChatMsg);
	p.WriteString(srolename.asUTF8_c_str());
	p.WriteByte(chattype);
	p.WriteString(chatcontent.asUTF8_c_str());
	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SSkillSave( int which )
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SSkillSave);
	p.WriteByte(which);
	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SSkillLoad( int which )
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SSkillLoad);
	p.WriteByte(which);
	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SSkillReset()
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_BATTLE_C2S::CMD_C2SSkillReset);
	m_pNetwork->SendPacket(p.getBuffer());
}

void CPacketMgr::sendC2SChangeAttackType(int attackType)
{
	CCmdPacket p;
	std::stringstream ss;
	ss<<"changeKill:";
	ss<<attackType;
	sendC2SCmd(const_cast<char*>(ss.str().c_str()));
}
void CPacketMgr::sendC2SCmd(const char* strCmd)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_MISC_C2S::CMD_C2SCmd); // 协议码
	p.WriteString(strCmd);
	m_pNetwork->SendPacket(p.getBuffer());
	// 	XC_DEBUG("<Attetion> client send cmd info :%s \n;", strCmd);
}

void CPacketMgr::sendC2SCmd(std::string& strCmd)
{

	if (strCmd.length() > 0)
		sendC2SCmd(strCmd.c_str());

}

void CPacketMgr::sendC2SCmd(const MyGUI::UString& strCmd)
{
	if(strCmd.capacity() == 0)
		return;
	sendC2SCmd(strCmd.asUTF8_c_str());
}

void CPacketMgr::sendC2SItemInfo(int uuid, int itemServerId)
{
	CCmdPacket p;
	p.BeginWrite();
	p.WriteByte(MSG_ITEM_C2S::CMD_C2SItemInfo);
	p.WriteInt32(uuid);
	p.WriteInt32(itemServerId);
	m_pNetwork->SendPacket(p.getBuffer());
}
void CPacketMgr::sendC2SRegisterPacket(const char* account, const char* password ,int type)
{
	CCmdPacket p;
	//MyGUI::UString invalid("0");
	p.BeginWrite();
	p.WriteByte(MSG_ACCOUNT_C2S::CMD_C2SRegisterReq);
	//p.WriteString(core::stringc("%d",CGame::getInstance()->pUnion->getUnionId()).c_str());	//联盟ID
	//p.WriteString( CGame::getInstance()->model.c_str());	//手机平台信息
	p.WriteByte(110); //客户端版本	Byte
	p.WriteByte(110);//Sdk	Byte
	p.WriteByte(type);//注册类型	Byte
	if (type == 1)
	{
		p.WriteString(account);
		p.WriteString(password);
	}

	m_pNetwork->SendPacket(p.getBuffer());
}

//邮件发送
void CPacketMgr::sendEmailInfoC2S(const MyGUI::UString &s_name,unsigned int s_uuid,const MyGUI::UString &s_title,const MyGUI::UString &s_info,int s_money,
								  unsigned int s_things_one,unsigned char s_number_one,	
								  unsigned int s_things_two,unsigned char s_number_two,
								  unsigned int s_things_three,unsigned char s_number_three)
{
	CCmdPacket p;
	p.BeginWrite();

	p.WriteByte(MSG_MISC_C2S::CMD_WriteMail);

	p.WriteString(s_name.asUTF8_c_str());

	p.WriteInt32(s_uuid);

	p.WriteString(s_title.asUTF8_c_str());

	p.WriteString(s_info.asUTF8_c_str());

	p.WriteInt32(s_money);

	p.WriteInt32(s_things_one);

	p.WriteByte(s_number_one);

	p.WriteInt32(s_things_two);

	p.WriteByte(s_number_two);

	p.WriteInt32(s_things_three);

	p.WriteByte(s_number_three);

	m_pNetwork->SendPacket(p.getBuffer());

}

void CPacketMgr::sendC2SSendSomeMessage(void)
{
	CCmdPacket p;
	p.BeginWrite();

	p.WriteByte(MSG_ROLE_SELECT_C2S::CMD_C2SHeart);

	m_pNetwork->SendPacket(p.getBuffer());

}

void CPacketMgr::sendC2SHonourUpdate(short id)
{
	CCmdPacket p;
	p.BeginWrite();

	p.WriteByte(MSG_MISC_C2S::CMD_changeHonour);

	p.WriteShort(id);

	m_pNetwork->SendPacket(p.getBuffer());

}

//小地图

void CPacketMgr::sendC2SMapActor( int mapID )
{

	//	std::string mapIDStr("mapActors:");
	//	char  mID[10];
	//	mapIDStr+=itoa(mapID,mID,10);
	//	MyGUI::UString mapIDStr("mapActors:%d",mapID);

	core::stringc mapIDStr("mapActors:%d",mapID);
	sendC2SCmd(mapIDStr.c_str());



}

void CPacketMgr::sendC2STeamMates( int mapID )
{
	core::stringc mapIDStr("reqTeamxy:%d",mapID);
	sendC2SCmd(mapIDStr.c_str());

}

//请求寻路数据
void CPacketMgr::sendC2SPathFinding( int mapDestID,int npcID,int DestX, int DestY )
{
	core::stringc searchMapStr("searchMap:%d:%d:%d:%d",mapDestID,npcID,DestX,DestY);
	sendC2SCmd(searchMapStr.c_str());

}
