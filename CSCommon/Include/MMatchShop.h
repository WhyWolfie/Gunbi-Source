#ifndef _MMATCHSHOP_H
#define _MMATCHSHOP_H

#include "winsock2.h"
#include "MXml.h"
#include "MUID.h"
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#include "MMatchItem.h"
#include "MQuestItem.h"
#include "MMatchDBGambleItem.h"

/// ���� ������ ���
/// - ���� ���������� MMatchItem.h��, �Ⱓ������ ���⿡ �ִ� ������ �Ǿ������.
/// - �ű���� �ʹ� ������ ������ ���⿡ �Ⱓ������ �ø��� ������ ��ħ.
/// - ���Ŀ� �������������� MMatchItem.h�� �ִ� BountyPrice, BountyValue�� ����� �̻�����ּ���. - bird
/// - ���� ������ ��������� �۾��� �ּ�ȭ �ϱ� ���ؼ� ZItem�� �״�� �����ϵ��� ��. - by SungE 2007-06-28
struct ShopItemNode
{
	unsigned int	nItemID;			/// ������ ID
	int				nItemCount;
	bool			bIsRentItem;		/// ��� ����. �Ⱓ ���������� ����(�Ϲ� �����۸� ����)
	int				nRentPeriodHour;	/// ��� ����. �Ⱓ(1 = 1�ð�)
	

	ShopItemNode() : nItemID(0), nItemCount(0), bIsRentItem(false), nRentPeriodHour(0) {}
};

enum MSHOPMODE 
{
	SHOP_NORMAL = 0,
	SHOP_CASH,
};


class MMatchShop1
{
private:
protected:
	vector<ShopItemNode*>					m_ItemNodeVector;
	map<unsigned int, ShopItemNode*>		m_ItemNodeMap;

	void ParseSellItem(MXmlElement& element);
	bool ReadXml(const char* szFileName);

public:
	MMatchShop1();
	virtual ~MMatchShop1();
	bool Create(const char* szDescFileName);
	void Destroy();

	void Clear();
	int GetCount() { return static_cast< int >( m_ItemNodeVector.size() ); }
	bool IsSellItem(const unsigned long int nItemID);		// �ش� �������� �Ȱ� �ִ��� ����
	ShopItemNode* GetSellItemByIndex(int nListIndex);			// ������ �Ȱ� �ִ� nListIndex��° ������ �� ��ȯ
	ShopItemNode* GetSellItemByItemID(int nItemID);				// ������ �Ȱ� �ִ� nItemID ������ �� ��ȯ
	
	static MMatchShop1* GetInstance();
};

////////////////////////////////////////
class MMatchShop2
{
private:
protected:
	vector<ShopItemNode*>					m_ItemNodeVector;
	map<unsigned int, ShopItemNode*>		m_ItemNodeMap;

	void ParseSellItem(MXmlElement& element);
	bool ReadXml(const char* szFileName);

public:
	MMatchShop2();
	virtual ~MMatchShop2();
	bool Create(const char* szDescFileName);
	void Destroy();

	void Clear();
	int GetCount() { return static_cast< int >(m_ItemNodeVector.size()); }
	bool IsSellItem(const unsigned long int nItemID);		// �ش� �������� �Ȱ� �ִ��� ����
	ShopItemNode* GetSellItemByIndex(int nListIndex);			// ������ �Ȱ� �ִ� nListIndex��° ������ �� ��ȯ
	ShopItemNode* GetSellItemByItemID(int nItemID);				// ������ �Ȱ� �ִ� nItemID ������ �� ��ȯ

	static MMatchShop2* GetInstance();
};

////////////////////////////////////////
class MMatchShop3
{
private:
protected:
	vector<ShopItemNode*>					m_ItemNodeVector;
	map<unsigned int, ShopItemNode*>		m_ItemNodeMap;

	void ParseSellItem(MXmlElement& element);
	bool ReadXml(const char* szFileName);

public:
	MMatchShop3();
	virtual ~MMatchShop3();
	bool Create(const char* szDescFileName);
	void Destroy();

	void Clear();
	int GetCount() { return static_cast< int >(m_ItemNodeVector.size()); }
	bool IsSellItem(const unsigned long int nItemID);		// �ش� �������� �Ȱ� �ִ��� ����
	ShopItemNode* GetSellItemByIndex(int nListIndex);			// ������ �Ȱ� �ִ� nListIndex��° ������ �� ��ȯ
	ShopItemNode* GetSellItemByItemID(int nItemID);				// ������ �Ȱ� �ִ� nItemID ������ �� ��ȯ

	static MMatchShop3* GetInstance();
};

////////////////////////////////////////
class MMatchShop4
{
private:
protected:
	vector<ShopItemNode*>					m_ItemNodeVector;
	map<unsigned int, ShopItemNode*>		m_ItemNodeMap;

	void ParseSellItem(MXmlElement& element);
	bool ReadXml(const char* szFileName);

public:
	MMatchShop4();
	virtual ~MMatchShop4();
	bool Create(const char* szDescFileName);
	void Destroy();

	void Clear();
	int GetCount() { return static_cast< int >(m_ItemNodeVector.size()); }
	bool IsSellItem(const unsigned long int nItemID);		// �ش� �������� �Ȱ� �ִ��� ����
	ShopItemNode* GetSellItemByIndex(int nListIndex);			// ������ �Ȱ� �ִ� nListIndex��° ������ �� ��ȯ
	ShopItemNode* GetSellItemByItemID(int nItemID);				// ������ �Ȱ� �ִ� nItemID ������ �� ��ȯ

	static MMatchShop4* GetInstance();
};

inline MMatchShop1* MGetMatchShop1() { return MMatchShop1::GetInstance(); }
inline MMatchShop2* MGetMatchShop2() { return MMatchShop2::GetInstance(); }
inline MMatchShop3* MGetMatchShop3() { return MMatchShop3::GetInstance(); }
inline MMatchShop4* MGetMatchShop4() { return MMatchShop4::GetInstance(); }

#define MTOK_SELL					"SELL"
#define MTOK_SELL_ITEMID			"itemid"
#define MTOK_SELL_ITEM_COUNT		"item_Count"		
#define MTOK_SELL_RENT_PERIOD_HOUR	"rent_period"




#endif