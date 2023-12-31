#ifndef _ZCHARACTERITEM_H
#define _ZCHARACTERITEM_H

#include "MMatchItem.h"
#include "ZItem.h"
#include "ZFile.h"
#include <list>
#include <algorithm>
using namespace std;


/// 캐릭터가 장비하고 있는 아이템들
class ZCharacterItem
{
private:
protected:
	ZItem					m_Items[MMCIP_END];
	int					m_OldItems[MMCIP_END];
	MMatchCharItemParts		m_nSelectedWeapon;		// 선택한 Item에 대한 m_Items에 해당하는 인덱스
	bool Confirm(MMatchCharItemParts parts, MMatchItemDesc* pDesc);
	bool IsWeaponItem(MMatchCharItemParts parts);
public:
	ZCharacterItem();
	virtual ~ZCharacterItem();
	void SelectWeapon(MMatchCharItemParts parts);
	bool EquipItem(MMatchCharItemParts parts, int nItemDescID, int nItemCount = 1);

	bool Reload();

	ZItem* GetItem(MMatchCharItemParts parts)
	{
		if ((parts < MMCIP_HEAD) || (parts >= MMCIP_END))
		{
			_ASSERT(0);
			return NULL;
		}
		return &m_Items[(int)parts]; 
	}

	int GetOldItem(MMatchCharItemParts parts)
	{
		if ((parts < MMCIP_HEAD) || (parts >= MMCIP_END))
		{
			_ASSERT(0);
			return NULL;
		}
		return m_OldItems[(int) parts];
	}
	void SetOldItem(MMatchCharItemParts parts, int itemId)
	{
		if ((parts < MMCIP_HEAD) || (parts >= MMCIP_END))
		{
			return;
		}
		else
		{
			m_OldItems[(int) parts] = itemId;
		}
	}
	ZItem* GetSelectedWeapon(); 
	MMatchCharItemParts GetSelectedWeaponParts() { return (MMatchCharItemParts)m_nSelectedWeapon; }

	MMatchCharItemParts GetSelectedWeaponType() {
		return m_nSelectedWeapon;
	}

	bool Save(ZFile *file);
	bool Load(ZFile *file, int nReplayVersion);

	void ShiftFugitiveValues();
	
	/*
	void SetWarppingItemDesc(DWORD tick)
	{ 
		for(int i = 0; i < (int)MMCIP_END; ++i)
		{
			MMatchItemDesc* pDesc = ((MMatchItem)m_Items[i]).GetDesc();
			if(pDesc)
				pDesc->m_pMItemName->SetWarpingAdd(tick);
		}
	}
	*/
};

#endif