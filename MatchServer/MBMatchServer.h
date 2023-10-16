#ifndef MBMATCHSERVER_H
#define MBMATCHSERVER_H

#include "MMatchServer.h"
#include "MBMatchServerConfigReloader.h"
#include "MBMatchSecurity.h"
#include "MBMatchMonitor.h"
#include "MBMatchKillTracker.h"
#include "MBMatchTest.h"

#include "MFeatureDefine.h"
#include "MMatchShop.h"

class COutputView;
class CCommandLogView;
class MAsyncJob;

class MBMatchServer : public MMatchServer
{
private :
	friend class MBMatchTest;

private :
	MBMatchServerConfigReloader m_ConfigReloader;

public:
	COutputView*		m_pView;
	CCommandLogView*	m_pCmdLogView;

	MUID m_uidKeeper;

	MBMatchSecurity		m_Security;
	MBMatchKillTracker	m_KillTracker;
	MBMatchTest			m_Tester;


protected:
	/// Create()ȣ��ÿ� �Ҹ��� �Լ�
	virtual bool OnCreate(void);
	/// Destroy()ȣ��ÿ� �Ҹ��� �Լ�
	virtual void OnDestroy(void);
	/// Ŀ�ǵ带 ó���ϱ� ����
	virtual void OnPrepareCommand(MCommand* pCommand);
	/// ����� Ŀ�ǵ� ó��
	virtual bool OnCommand(MCommand* pCommand);

	virtual void OnNetClear(const MUID& CommUID);

public:
	MBMatchServer(COutputView* pView=NULL);
	~MBMatchServer();
	virtual void Shutdown();
	virtual void Log(unsigned int nLogLevel, const char* szLog);
	void OnViewServerStatus();

private :
	// ������ ���۰� �Բ� ��ϵǴ� ������.
	bool InitSubTaskSchedule();
	bool AddClanServerAnnounceSchedule();
	bool AddClanServerSwitchDownSchedule();
	bool AddClanServerSwitchUpSchedule();

	// ������ ó�� �Լ�.
	// ��������.
	void OnScheduleAnnounce( const char* pszAnnounce );
	// Ŭ������ 
	void OnScheduleClanServerSwitchDown();
	void OnScheduleClanServerSwitchUp();

	const MUID GetKeeperUID() const { return m_uidKeeper; }
	void SetKeeperUID( const MUID& uidKeeper ) { m_uidKeeper = uidKeeper; }

	// MD5 Hash Value File Load
	bool LoadFileMD5HashValue();

	void WriteServerInfoLog();

protected :
	// Keeper����.
	bool IsKeeper( const MUID& uidKeeper );

	void OnResponseServerStatus( const MUID& uidSender );
	void OnRequestServerHearbeat( const MUID& uidSender );
	void OnResponseServerHeartbeat( const MUID& uidSender );
	void OnRequestConnectMatchServer( const MUID& uidSender );
	void OnResponseConnectMatchServer( const MUID& uidSender );
	void OnRequestKeeperAnnounce( const MUID& uidSender, const char* pszAnnounce );
	void OnRequestStopServerWithAnnounce( const MUID& uidSender );
	void OnResponseStopServerWithAnnounce( const MUID& uidSender );
	void OnRequestSchedule( const MUID& uidSender, 
		const int nType, 
		const int nYear, 
		const int nMonth, 
		const int nDay, 
		const int nHour, 
		const int nMin,
		const int nCount,
		const int nCommand,
		const char* pszAnnounce );
	void OnResponseSchedule( const MUID& uidSender, 
		const int nType, 
		const int nYear, 
		const int nMonth, 
		const int nDay, 
		const int nHour, 
		const int nMin,
		const int nCount,
		const int nCommand,
		const char* pszAnnounce );
	void OnRequestKeeperStopServerSchedule( const MUID& uidSender, const char* pszAnnounce );
	void OnResponseKeeperStopServerSchedule( const MUID& uidSender, const char* pszAnnounce );
	void OnRequestDisconnectServerFromKeeper( const MUID& uidSender );
	void OnRequestReloadServerConfig( const MUID& uidSender, const string& strFileList );
	void OnResponseReloadServerConfig( const MUID& uidSender, const string& strFileList );

	// command
	void OnRequestAccountCharList(const MUID& uidPlayer, unsigned char* pbyGuidAckMsg);
	void OnGameKill(const MUID& uidAttacker, const MUID& uidVictim);
	void ProcessOnGameKill(MMatchStage* pStage, MMatchObject* pAttacker, MMatchObject* pVictim);
	void OnStageJoin(const MUID& uidPlayer, const MUID& uidStage);
	void OnRequestGamble(const MUID& uidPlayer, const MUID& uidItem);
	MCommandParameterBlob* MakeShopGambleItemListBlob() const;
	MCommandParameterBlob* MakeShopItemListBlob( const int nFirstItemIndex, const int nItemCount, const MUID& uidPlayer ) const;
	void OnRequestBuyItem(const MUID& uidPlayer, const unsigned long int nItemID, int nItemCount);
	bool ResponseBuyItem( MMatchObject* pObj
		, const unsigned long int nItemID
		, const int nItemCount
		, const bool bIsGambleItem
		, const DWORD dwPrice
		, const WORD dwRentHourPeriod
		, const bool bIsSpendableItem
		#ifdef _EXTRASHOPS
		, MSHOPMODE nShopMode
		#endif
		);
	const bool CheckUserCanBuyItem( MMatchObject* pObj, const int nItemID, const int nItemCnt, const DWORD dwPrice 
		#ifdef _EXTRASHOPS
		, MSHOPMODE nShopMode
		#endif
		);
	bool CheckUserCanDistributeRewardItem( MMatchObject* pObj);
	void OnRequestCharacterItemList(const MUID& uidPlayer);
	void OnRequestCharacterItemListForce(const MUID& uidPlayer);
	bool ResponseCharacterItemList(const MUID& uidPlayer);
	void OnRequestSellItem(const MUID& uidPlayer, const MUID& uidItem, int nItemCount);
	
	bool IsValidSellItem(const MUID& uidPlayer, const MUID& uidItem, int nItemCount = 1);
	bool ResponseSellItem(const MUID& uidPlayer, const MUID& uidItem);
	bool ResponseSellSpendableItem(const MUID& uidPlayer, const MUID& uidItem, int nItemCount);

	bool ResponseSellGItem( const MUID& uidPlayer, const MUID& uidItem, int nItemCount);


	void OnRequestBringAccountItem(const MUID& uidPlayer, unsigned int nAIID, unsigned int nItemID, unsigned int nItemCnt);	
	void OnRequestBringBackAccountItem(const MUID& uidPlayer, const MUID& uidItem, unsigned int nItemCount);

	const bool SendGambleItemList( const MMatchObject* pObj );
	void OnRequestEquipItem(const MUID& uidPlayer, const MUID& uidItem, const long int nEquipmentSlot);
	void ResponseEquipItem(const MUID& uidPlayer, const MUID& uidItem, const MMatchCharItemParts parts);
	void OnRequestTakeoffItem(const MUID& uidPlayer, const unsigned long int nEquipmentSlot);
	void ResponseTakeoffItem(const MUID& uidPlayer, const MMatchCharItemParts parts);

	int ValidateStageJoin(const MUID& uidPlayer, const MUID& uidStage);

	// filter
	void OnLocalUpdateUseCountryFilter();
	void OnLocalGetDBIPtoCountry();
	void OnLocalGetDBBlockCountryCode();
	void OnLocalGetDBCustomIP();
	void OnLocalUpdateIPtoCountry();
	void OnLocalUpdateBlockCountryCode();
	void OnLocalUpdateCustomIP();
	void OnLocalUpdateAcceptInvaildIP();

	// async
	void OnProcessAsyncJob( MAsyncJob* pJob );
	void OnAsyncGetAccountCharInfo(MAsyncJob* pJobResult);
	void OnAsyncGetCharInfo(MAsyncJob* pJobResult);
	void OnAsyncGetCpuUsage(MAsyncJob* pJobResult);	
	void OnAsyncBuyBountyItem( MAsyncJob* pJobResult );
	
	void OnAsyncSelling_Item_ToBounty( MAsyncJob* pJobResult );
	void OnAsyncSelling_SpendableItem_ToBounty( MAsyncJob* pJobResult );

	void OnAsyncGambleItem( MAsyncJob* pJobResult );
	void OnAsyncGambleQuestItem( MAsyncJob* pJobResult );		
	void OnUpdateEquipItem( MAsyncJob* pJobResult );
	void OnTakeoffItem( MAsyncJob* pJobResult );

	void OnAsyncBringAccountItem(MAsyncJob* pJobResult);
	void OnAsyncBringAccountItemStackable(MAsyncJob* pJobResult);

	void OnBringBackAccountItem( MAsyncJob* pJobResult );
	void OnBringBackAccountItemStackable( MAsyncJob* pJobResult );

	/// ����� ����
	virtual void OnRun(void);
	virtual void SafePushMonitorUDP( const DWORD dwIP, const WORD wPort, const char* pData, const DWORD dwDataSize );
	virtual bool SendMonitorUDP(const DWORD dwIP, const USHORT nPort, const string& strMonitorCommand);
public:
	// HShield
	virtual ULONG	HShield_MakeGuidReqMsg(unsigned char *pbyGuidReqMsg, unsigned char *pbyGuidReqInfo);
	virtual ULONG	HShield_AnalyzeGuidAckMsg(unsigned char *pbyGuidAckMsg, unsigned char *pbyGuidReqInfo, unsigned long **ppCrcInfo);
	virtual ULONG   HShield_MakeReqMsg(unsigned long *pCrcInfo, unsigned char *pbyReqMsg, unsigned char *pbyReqInfo, unsigned long ulOption);
	virtual ULONG   HShield_AnalyzeAckMsg(unsigned long *pCrcInfo, unsigned char *pbyAckMsg, unsigned char *pbyReqInfo);

	char	m_strFileCrcDataPath[MAX_PATH];

	void OnRequestShopItemList(const MUID& uidPlayer, const int nFirstItemIndex, const int nItemCount);
	void ResponseShopItemList(const MUID& uidPlayer, const int nFirstItemIndex, const int nItemCount);

private :
	bool InitSecrity();

	// ------------------------- security helper ----------------------------------

private :
	MBMatchKillTracker& GetKillTracker() { return m_KillTracker; }

private :
	void ReleaseSecurity();
};


#endif