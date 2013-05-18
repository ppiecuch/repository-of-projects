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

	//��¼
	void sendC2SLoginPacket(const char* account, const char* password, const char* lianMeng, const char* ua);
	//ע��
	void sendC2SRegisterPacket(const char* account, const char* password,int type);
	//�����ɫ�б�
	void sendC2SPlayerListReqPacket(char* serverKey);

	//ѡ���ɫ������Ϸ
	void sendC2SEnterGamePacket(char* charKey);

	//������ɫ
	void sendC2SCreateRolePacket(const char* name, int jobId, int sex,bool isVisitor);

	//ɾ����ɫ
	void sendC2SDeleteRolePacket(char* charKey);

	//����������
	void sendC2SUpSkillLevel(int skillID);

	//ʹ�ü���
	void sendC2SAttack(int uuid, int skillID);

	//ʹ�ü���
	void sendC2SPreAttack(int uuid, int skillID);

	//����ͬ��
	void sendC2SLocation(int posX, int posY);

	//����ı�(ҡ��)
	void sendC2SLocationList(core::array<core::vector2df>& pos);

	//����ı�(Ѱ·)
	void sendC2SMoveTo(core::array<core::vector2df>& pos);

	//npc�Ի�
	void sendC2STalk2NPC(int uuid, int contextType, int taskId);

	//�ɼ�
	void sendC2SCollect(int type, int uuid);

	//��ͼ�������
	void sendC2SMapLoadComplete();

	//����������
	void sendC2SSkillSave(int which);

	//����������
	void sendC2SSkillLoad(int which);

	//����������
	void sendC2SSkillReset();

	//�ı�ɱ¾/����/��ƽģʽ
	void sendC2SChangeAttackType(int attackType);

	//
	// cmd����Э��
	//
	// strCmd - ������utf-8����
	void sendC2SCmd(const char* strCmd);
	void sendC2SCmd(std::string& strCmd);
	// strCmd - UString����
	void sendC2SCmd(const MyGUI::UString& strCmd);

	void sendC2SItemInfo(int uuid, int itemServerId);

	//�ʼ�����
	void sendEmailInfoC2S(const MyGUI::UString &s_name,unsigned int s_uuid,const MyGUI::UString &s_title,const MyGUI::UString &s_info,int s_money,
		unsigned int s_things_one,unsigned char s_number_one,	
		unsigned int s_things_two,unsigned char s_number_two,
		unsigned int s_things_three,unsigned char s_number_three);
	//����
	void sendC2SSendChatMsg(const MyGUI::UString& srolename, int chattype, const MyGUI::UString& chatontent);

	void sendC2SSendSomeMessage(void);

	void sendC2SHonourUpdate(short id);

	//�����ͼ�����б�
	void sendC2SMapActor(int mapID);

	//�����ͼ�����б�
	void sendC2STeamMates(int mapID);

	//����Ѱ·����
	void sendC2SPathFinding(int mapDestID,int npcID,int DestX, int DestY);

	bool IsSenderThreadExit();
};

#define GetPacketMgr()		CPacketMgr::GetInstance()

#endif