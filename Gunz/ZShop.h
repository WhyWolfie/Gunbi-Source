#ifndef _ZSHOP_H
#define _ZSHOP_H

#include "ZPrerequisites.h"
#include "MMatchShop.h"
#include <vector>
using namespace std;

class ZShop
{
private:
protected:
	int m_nPage;			// ������ ����Ʈ ������
	bool m_bCreated;
	vector<MTD_ShopItemInfo*>		m_vShopItem;
	vector<MTD_GambleItemNode*>		m_vGItemList;

	#ifdef _EXTRASHOPS
	MSHOPMODE m_nShopMode;
	#endif

public:
	int m_ListFilter;

public:
	ZShop();
	virtual ~ZShop();
	bool Create();
	void Destroy();
	void ClearShop();
	void ClearGamble();
	void Serialize();

	bool CheckTypeWithListFilter(int type, bool bEnchantItem);

	#ifdef _EXTRASHOPS
	MSHOPMODE GetShopMode() { return m_nShopMode; }
	void SetShopMode(MSHOPMODE nShopMode) { m_nShopMode = nShopMode; }
	#endif

	int GetItemCount() { return (int)m_vShopItem.size(); }
	void SetItemsAll(const vector< MTD_ShopItemInfo*>& vShopItemList);
	void SetItemsGamble( const vector<MTD_GambleItemNode*>& vGItemList);
	int GetPage() { return m_nPage; }
	unsigned long int GetItemID(int nIndex);
	static ZShop* GetInstance();
};

inline ZShop* ZGetShop() { return ZShop::GetInstance(); }

#endif