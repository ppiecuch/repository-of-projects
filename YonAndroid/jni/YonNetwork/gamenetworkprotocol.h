//	Network protocl header & struct define
#ifndef __GAME_NETWORK_PROTOCOL_H__
#define __GAME_NETWORK_PROTOCOL_H__

#include "NetDataType.h"

// 协议版本号定义，版本号用一个DWORD表示
// 主版本号使用HIWORD，次版本号使用LOWORD

#define	PROTOCOL_VERSION_MAJOR	1		// 一个MSG_PROTOCOL_TYPE,表示主版本号
#define	PROTOCOL_VERSION_MINOR	0		// 一个MSG_PROTOCOL_TYPE,表示次版本号


extern core::stringc getConnectIP();
extern int getConnectPort();

// #define LOGIN_SERVER_PORT		   30001




//
//	所有网络数据包编号分段
//
const MSG_PROTOCOL_TYPE MSG_BASE_ACCOUNT_S2C		= 0;
const MSG_PROTOCOL_TYPE MSG_BASE_ACCOUNT_C2S		= 0;
const MSG_PROTOCOL_TYPE MSG_BASE_ROLE_SELECT_S2C	= 0x10;
const MSG_PROTOCOL_TYPE MSG_BASE_ROLE_SELECT_C2S	= 0x10;
const MSG_PROTOCOL_TYPE MSG_BASE_MAP_S2C			= 0x20;
const MSG_PROTOCOL_TYPE MSG_BASE_MAP_C2S			= 0x20;
const MSG_PROTOCOL_TYPE MSG_BASE_BATTLE_S2C         = 0x30;
const MSG_PROTOCOL_TYPE MSG_BASE_BATTLE_C2S         = 0x30;
const MSG_PROTOCOL_TYPE MSG_BASE_ITEM_S2C			= 0x40;
const MSG_PROTOCOL_TYPE MSG_BASE_ITEM_C2S			= 0x40;

const MSG_PROTOCOL_TYPE MSG_BASE_MISC_S2C			= 0x50;
const MSG_PROTOCOL_TYPE MSG_BASE_MISC_C2S			= 0x50;






//	LSTC is short for LoginServer-To-Client
namespace MSG_ACCOUNT_S2C
{
	const MSG_PROTOCOL_TYPE	CMD_S2CLoginFail  			 = MSG_BASE_ACCOUNT_S2C + 0;
	const MSG_PROTOCOL_TYPE	CMD_S2CServerList  		     = MSG_BASE_ACCOUNT_S2C + 1;
	const MSG_PROTOCOL_TYPE	CMD_S2CLoginNetKey 		     = MSG_BASE_ACCOUNT_S2C + 2;
	const MSG_PROTOCOL_TYPE	CMD_S2CRegisterFail 		 = MSG_BASE_ACCOUNT_S2C + 5;
	const MSG_PROTOCOL_TYPE	CMD_S2CRegisterSucceed 		 = MSG_BASE_ACCOUNT_S2C + 4;
	const MSG_PROTOCOL_TYPE CMD_S2CModify_pswd           = MSG_BASE_ACCOUNT_S2C + 7;         //修改密码后的数据
	const MSG_PROTOCOL_TYPE CMD_S2CModify_ZipLoc         = MSG_BASE_ACCOUNT_S2C + 8;         //修改密保后的数据


};

namespace MSG_ACCOUNT_C2S
{
	const MSG_PROTOCOL_TYPE	CMD_C2SLoginReq			= MSG_BASE_ACCOUNT_C2S + 0;
	const MSG_PROTOCOL_TYPE	CMD_C2SRegisterReq			= MSG_BASE_ACCOUNT_C2S + 1;
};

namespace MSG_ROLE_SELECT_S2C
{
	const MSG_PROTOCOL_TYPE	CMD_S2CPlayerList			= MSG_BASE_ROLE_SELECT_S2C + 0;
	const MSG_PROTOCOL_TYPE	CMD_S2CGameLogin			= MSG_BASE_ROLE_SELECT_S2C + 1;
	const MSG_PROTOCOL_TYPE	CMD_S2CGameNetKey			= MSG_BASE_ROLE_SELECT_S2C + 2;
	const MSG_PROTOCOL_TYPE	CMD_S2CCreateFailed			= MSG_BASE_ROLE_SELECT_S2C + 3;
	const MSG_PROTOCOL_TYPE	CMD_S2CRoleParamUPdate		= MSG_BASE_ROLE_SELECT_S2C + 4;
	const MSG_PROTOCOL_TYPE	CMD_S2CRoleEffUPdate		= MSG_BASE_ROLE_SELECT_S2C + 5;
	const MSG_PROTOCOL_TYPE CMD_S2CNeedLead             = MSG_BASE_ROLE_SELECT_S2C + 6; 
	const MSG_PROTOCOL_TYPE CMD_S2CUpdateMp             = MSG_BASE_ROLE_SELECT_S2C + 7; 
	const MSG_PROTOCOL_TYPE CMD_S2CUpdateMpMax          = MSG_BASE_ROLE_SELECT_S2C + 8; 
};

namespace MSG_ROLE_SELECT_C2S
{
	const MSG_PROTOCOL_TYPE	CMD_C2SPlayerListReq		= MSG_BASE_ROLE_SELECT_C2S + 0;
	const MSG_PROTOCOL_TYPE	CMD_C2SGameLoginReq			= MSG_BASE_ROLE_SELECT_C2S + 1;
	const MSG_PROTOCOL_TYPE	CMD_C2SCreatePlayerReq		= MSG_BASE_ROLE_SELECT_C2S + 2;
	const MSG_PROTOCOL_TYPE	CMD_C2SDelPlayerReq			= MSG_BASE_ROLE_SELECT_C2S + 3;
	const MSG_PROTOCOL_TYPE	CMD_C2SHeart 				= MSG_BASE_ROLE_SELECT_C2S + 4;

};

//场景地图协议 S->C 0x20
namespace MSG_MAP_S2C
{
	const MSG_PROTOCOL_TYPE	CMD_S2CSkipMap				= MSG_BASE_MAP_S2C + 0;
	const MSG_PROTOCOL_TYPE CMD_S2CLocation				= MSG_BASE_MAP_S2C + 1;
	const MSG_PROTOCOL_TYPE	CMD_S2CRegPlayer			= MSG_BASE_MAP_S2C + 2;
	const MSG_PROTOCOL_TYPE CMD_S2CRemoveActor			= MSG_BASE_MAP_S2C + 3;
	const MSG_PROTOCOL_TYPE CMD_S2CLocationList			= MSG_BASE_MAP_S2C + 5;
	const MSG_PROTOCOL_TYPE CMD_S2CLocationTarget		= MSG_BASE_MAP_S2C + 6;
	const MSG_PROTOCOL_TYPE CMD_S2CRegNPC				= MSG_BASE_MAP_S2C + 7;
	const MSG_PROTOCOL_TYPE CMD_S2CRegEnemy				= MSG_BASE_MAP_S2C + 8;
};

namespace MSG_MAP_C2S
{
	const MSG_PROTOCOL_TYPE CMD_C2SLocation             = MSG_BASE_MAP_C2S + 0;
	const MSG_PROTOCOL_TYPE CMD_C2SLocationList         = MSG_BASE_MAP_C2S + 2;
	const MSG_PROTOCOL_TYPE CMD_C2SLocationTarget       = MSG_BASE_MAP_C2S + 3;
};

namespace MSG_BATTLE_S2C
{
	const MSG_PROTOCOL_TYPE CMD_S2CAttack               = MSG_BASE_BATTLE_S2C + 0;
	const MSG_PROTOCOL_TYPE CMD_S2CSkills               = MSG_BASE_BATTLE_S2C + 1;
	const MSG_PROTOCOL_TYPE CMD_S2CAddBuff              = MSG_BASE_BATTLE_S2C + 2;
	const MSG_PROTOCOL_TYPE CMD_S2CPreAttack            = MSG_BASE_BATTLE_S2C + 3;
	const MSG_PROTOCOL_TYPE CMD_S2CDelBuff              = MSG_BASE_BATTLE_S2C + 4;
	const MSG_PROTOCOL_TYPE CMD_S2CShortcutList         = MSG_BASE_BATTLE_S2C + 5;
	const MSG_PROTOCOL_TYPE CMD_S2CBonus				= MSG_BASE_BATTLE_S2C + 6;
	const MSG_PROTOCOL_TYPE CMD_S2CDead					= MSG_BASE_BATTLE_S2C + 7;
	const MSG_PROTOCOL_TYPE CMD_S2CGameTitle			= MSG_BASE_BATTLE_S2C + 8;
	const MSG_PROTOCOL_TYPE CMD_S2CRegEffect			= MSG_BASE_BATTLE_S2C + 9;
	const MSG_PROTOCOL_TYPE CMD_S2CHangStart			= MSG_BASE_BATTLE_S2C + 0x0a;
	const MSG_PROTOCOL_TYPE CMD_S2CHangConfig			= MSG_BASE_BATTLE_S2C + 0x0b;
	const MSG_PROTOCOL_TYPE CMD_S2COtherSkill			= MSG_BASE_BATTLE_S2C + 0x0c;

}

namespace MSG_BATTLE_C2S
{
	const MSG_PROTOCOL_TYPE CMD_C2SAttack               = MSG_BASE_BATTLE_C2S + 0;
	
	const MSG_PROTOCOL_TYPE CMD_C2SSkillUpLevel		     = MSG_BASE_BATTLE_C2S + 1;	
	const MSG_PROTOCOL_TYPE CMD_C2SSkillLoad			 = MSG_BASE_BATTLE_C2S + 2;	
	const MSG_PROTOCOL_TYPE CMD_C2SSkillSave			 = MSG_BASE_BATTLE_C2S + 3;	
	const MSG_PROTOCOL_TYPE CMD_C2SSkillReset			 = MSG_BASE_BATTLE_C2S + 4;	
	const MSG_PROTOCOL_TYPE CMD_C2SPreAttack             = MSG_BASE_BATTLE_C2S + 5;


}

namespace MSG_ITEM_S2C
{
	const MSG_PROTOCOL_TYPE CMD_S2CBagList               = MSG_BASE_ITEM_S2C + 0;
	const MSG_PROTOCOL_TYPE CMD_S2CUpdateBag             = MSG_BASE_ITEM_S2C + 1;
	const MSG_PROTOCOL_TYPE CMD_S2CDeleteBag             = MSG_BASE_ITEM_S2C + 2;
	const MSG_PROTOCOL_TYPE CMD_S2CItemInfo              = MSG_BASE_ITEM_S2C + 3;
	const MSG_PROTOCOL_TYPE CMD_S2CItemEquipResult       = MSG_BASE_ITEM_S2C + 4;
	const MSG_PROTOCOL_TYPE	CMD_S2CMoney 				 = MSG_BASE_ITEM_S2C + 5;
	const MSG_PROTOCOL_TYPE	CMD_S2CRmb  				 = MSG_BASE_ITEM_S2C + 6;
	const MSG_PROTOCOL_TYPE	CMD_S2CEquipInForgingList  	 = MSG_BASE_ITEM_S2C + 7;

	const MSG_PROTOCOL_TYPE	CMD_S2CEquipInForgingInfo  	 = MSG_BASE_ITEM_S2C + 8;

	const MSG_PROTOCOL_TYPE	CMD_S2CEquipHoldInfo  		 = MSG_BASE_ITEM_S2C + 0xa;
	const MSG_PROTOCOL_TYPE	CMD_S2CStoreList	 		 = MSG_BASE_ITEM_S2C + 0xb;
	const MSG_PROTOCOL_TYPE	CMD_S2CStoreMoney	 		 = MSG_BASE_ITEM_S2C + 0xc;
	const MSG_PROTOCOL_TYPE	CMD_S2COtherPlayerEquip      = MSG_BASE_ITEM_S2C + 0xd;
	const MSG_PROTOCOL_TYPE CMD_SCUpdateBind             = MSG_BASE_ITEM_S2C + 0x0e;
	const MSG_PROTOCOL_TYPE CMD_SCUpdateContainer        = MSG_BASE_ITEM_S2C + 0x0f;
}

namespace MSG_ITEM_C2S
{
	const MSG_PROTOCOL_TYPE CMD_C2SItemInfo               = MSG_BASE_ITEM_C2S + 0;
}

namespace MSG_MISC_S2C
{
	const MSG_PROTOCOL_TYPE CMD_S2CNotice				= MSG_BASE_MISC_S2C + 0;
	const MSG_PROTOCOL_TYPE CMD_S2Chonours 				= MSG_BASE_MISC_S2C + 1;
	const MSG_PROTOCOL_TYPE CMD_S2CUiSocial				= MSG_BASE_MISC_S2C + 2;
	// 组队邀请
	const MSG_PROTOCOL_TYPE CMD_S2CJoinTeam			= MSG_BASE_MISC_S2C + 4;

	//邮件相关
	const MSG_PROTOCOL_TYPE CMD_Mails				= MSG_BASE_MISC_S2C + 5;
	const MSG_PROTOCOL_TYPE CMD_Mailinfo			= MSG_BASE_MISC_S2C + 6;

		//聊天接收
	const MSG_PROTOCOL_TYPE CMD_UpChatMsg			= MSG_BASE_MISC_S2C + 10;

	// 队伍新增队员列表
	const MSG_PROTOCOL_TYPE CMD_S2CTeamMember			= MSG_BASE_MISC_S2C + 7;
	// 删除队员
	const MSG_PROTOCOL_TYPE CMD_S2CTeamDel				= MSG_BASE_MISC_S2C + 8;
	// 转移队长
	const MSG_PROTOCOL_TYPE CMD_S2CTeamChangeCap		= MSG_BASE_MISC_S2C + 9;
	// NPC 对话内容
	const MSG_PROTOCOL_TYPE CMD_S2CNPCMsg				= MSG_BASE_MISC_S2C + 0x0b;
	// 周围队伍
	const MSG_PROTOCOL_TYPE CMD_S2CTeamAround			= MSG_BASE_MISC_S2C + 0x0c;
	//	正在进行的任务
	const MSG_PROTOCOL_TYPE CMD_S2CTaskInProgressList	= MSG_BASE_MISC_S2C + 0x0d;
	//	任务详情
	const MSG_PROTOCOL_TYPE CMD_S2CTaskDetail			= MSG_BASE_MISC_S2C + 0x0e;
	//	可接任务列表
	const MSG_PROTOCOL_TYPE CMD_S2CAcceptableTaskList	= MSG_BASE_MISC_S2C + 0x15;
	//	所有活动列表
	const MSG_PROTOCOL_TYPE CMD_S2CTaskActivityList		= MSG_BASE_MISC_S2C	+ 0x16;
	//	npc接任务
	const MSG_PROTOCOL_TYPE CMD_S2CTaskNPCDialog		= MSG_BASE_MISC_S2C	+ 0x17;
	//工会列表
	const MSG_PROTOCOL_TYPE CMD_S2CTongs				= MSG_BASE_MISC_S2C + 0x13;
	//其他工会信息
	const MSG_PROTOCOL_TYPE CMD_S2COtherTongInfo 		= MSG_BASE_MISC_S2C + 0x14;
	//工会成员列表
	const MSG_PROTOCOL_TYPE CMD_S2CTongMember 			= MSG_BASE_MISC_S2C + 0x10;
	//工会申请列表
	const MSG_PROTOCOL_TYPE CMD_S2CapplyTongMem  		= MSG_BASE_MISC_S2C + 0x11;
	//json数据下发
	const MSG_PROTOCOL_TYPE CMD_S2CJson  				= MSG_BASE_MISC_S2C + 0x18;
	//界面提示信息下发
	const MSG_PROTOCOL_TYPE CMD_S2CMsgLogo 				= MSG_BASE_MISC_S2C + 0x19;
	// 服务端打开ui
	const MSG_PROTOCOL_TYPE CMD_S2CApplyOpenUI			= MSG_BASE_MISC_S2C + 0x1a;
	//打开商店UI界面初始化信息
	const MSG_PROTOCOL_TYPE CMD_S2CShopTab 				= MSG_BASE_MISC_S2C + 0x1b;
	//打开商店列表信息
	const MSG_PROTOCOL_TYPE CMD_S2Cshop 				= MSG_BASE_MISC_S2C + 0x1c;
	
	const MSG_PROTOCOL_TYPE CMD_S2CLifeTab				= MSG_BASE_MISC_S2C + 0x1d;

	const MSG_PROTOCOL_TYPE CMD_S2CRebornXY				= MSG_BASE_MISC_S2C + 0x1f;
	
	const MSG_PROTOCOL_TYPE CMD_S2CMILEPOST				= MSG_BASE_MISC_S2C + 0x1e;

	const MSG_PROTOCOL_TYPE CMD_S2CMainTask 			= MSG_BASE_MISC_S2C + 0x20;

	const MSG_PROTOCOL_TYPE CMD_S2CUpdateMainTask 		= MSG_BASE_MISC_S2C + 0x21;

	const MSG_PROTOCOL_TYPE CMD_S2CIsOtherOnLine 		= MSG_BASE_MISC_S2C + 0x22;

	const MSG_PROTOCOL_TYPE CMD_SCBountyHunters 		= MSG_BASE_MISC_S2C + 0x23;

}

namespace MSG_MISC_C2S
{
	const MSG_PROTOCOL_TYPE CMD_C2SCmd				= MSG_BASE_MISC_C2S + 0;
	const MSG_PROTOCOL_TYPE CMD_changeHonour 		= MSG_BASE_MISC_C2S + 2;
	const MSG_PROTOCOL_TYPE CMD_WriteMail			= MSG_BASE_MISC_C2S + 3;
//聊天发送
	const MSG_PROTOCOL_TYPE CMD_SendChatMsg			= MSG_BASE_MISC_C2S + 4;
	const MSG_PROTOCOL_TYPE CMD_C2STalk2NPC			= MSG_BASE_MISC_C2S + 5;
}

namespace MSG_MINIMAP_S2C
{
	const  MSG_PROTOCOL_TYPE  MSG_BASE_MAP_ACTORS_S2C          =0x29;
	const  MSG_PROTOCOL_TYPE  MSG_BASE_TEAM_MATES_S2C          =0x2A;
	const  MSG_PROTOCOL_TYPE  MSG_BASE_PATH_FINDING_S2C        =0x2B;



}




#endif
