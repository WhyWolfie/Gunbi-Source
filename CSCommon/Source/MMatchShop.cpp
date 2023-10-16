#include "stdafx.h"
#include "MMatchShop.h"
#include "MMatchConfig.h"
#include "MMatchGambleMachine.h"

MMatchShop1::MMatchShop1()
{

}
MMatchShop1::~MMatchShop1()
{

}
bool MMatchShop1::Create(const char* szDescFileName)
{
	return ReadXml(szDescFileName);

	return true;
}

void MMatchShop1::Destroy()
{
	Clear();
}

bool MMatchShop1::ReadXml(const char* szFileName)
{
	MXmlDocument	xmlDocument;

	xmlDocument.Create();

	if (!xmlDocument.LoadFromFile(szFileName))
	{
		xmlDocument.Destroy();
		return false;
	}

	MXmlElement rootElement, chrElement, attrElement;
	char szTagName[256];

	rootElement = xmlDocument.GetDocumentElement();

	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++)
	{
		chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!strcmp(szTagName, MTOK_SELL))
		{
			ParseSellItem(chrElement);
		}
	}

	xmlDocument.Destroy();

	return true;
}

void MMatchShop1::ParseSellItem(MXmlElement& element)
{
	ShopItemNode *pNewItemNode = new ShopItemNode;

	int nDescID = 0;
	int	nItemCount = 1;
	if( nItemCount == 0 )	{ pNewItemNode->nItemCount = 1; } 
	else					{ pNewItemNode->nItemCount = nItemCount; }	

	element.GetAttribute(&nDescID, MTOK_SELL_ITEMID);
	element.GetAttribute(&pNewItemNode->nRentPeriodHour, MTOK_SELL_RENT_PERIOD_HOUR, 0);

	pNewItemNode->nItemID = nDescID;
	pNewItemNode->bIsRentItem = (pNewItemNode->nRentPeriodHour > 0);	
	

	MMatchItemDesc* pItemDesc = MGetMatchItemDescMgr()->GetItemDesc(nDescID);
	if (pItemDesc != NULL) 
	{
		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
	else
	{
		MQuestItemDesc* pQuestItemDesc = GetQuestItemDescMgr().FindQItemDesc( nDescID );
		if( 0 == pQuestItemDesc )
		{
			delete pNewItemNode;
			return;
		}

		m_ItemNodeVector.push_back( pNewItemNode );
		m_ItemNodeMap.insert( map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode) );
	}
}

void MMatchShop1::Clear()
{
	int nVectorSize = (int)m_ItemNodeVector.size();
	for (int i = 0; i < nVectorSize; i++)
	{
		ShopItemNode* pNode = m_ItemNodeVector[i];
		delete pNode;
	}

	m_ItemNodeVector.clear();
	m_ItemNodeMap.clear();
}

MMatchShop1* MMatchShop1::GetInstance()
{
	static MMatchShop1 g_stMatchShop;
	return &g_stMatchShop;
}

bool MMatchShop1::IsSellItem(const unsigned long int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor != m_ItemNodeMap.end()) {
		return true;
	}

	return false;
}

ShopItemNode* MMatchShop1::GetSellItemByIndex(int nListIndex)
{
	if ((nListIndex < 0) || (nListIndex >= GetCount())) return NULL;

	return m_ItemNodeVector[nListIndex];
}

ShopItemNode* MMatchShop1::GetSellItemByItemID(int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor == m_ItemNodeMap.end()) {
		return NULL;
	}

	return (ShopItemNode*)(itor->second);
}





MMatchShop2::MMatchShop2()
{

}
MMatchShop2::~MMatchShop2()
{

}
bool MMatchShop2::Create(const char* szDescFileName)
{
	return ReadXml(szDescFileName);

	return true;
}

void MMatchShop2::Destroy()
{
	Clear();
}

bool MMatchShop2::ReadXml(const char* szFileName)
{
	MXmlDocument	xmlDocument;

	xmlDocument.Create();

	if (!xmlDocument.LoadFromFile(szFileName))
	{
		xmlDocument.Destroy();
		return false;
	}

	MXmlElement rootElement, chrElement, attrElement;
	char szTagName[256];

	rootElement = xmlDocument.GetDocumentElement();

	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++)
	{
		chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!strcmp(szTagName, MTOK_SELL))
		{
			ParseSellItem(chrElement);
		}
	}

	xmlDocument.Destroy();

	return true;
}

void MMatchShop2::ParseSellItem(MXmlElement& element)
{
	ShopItemNode *pNewItemNode = new ShopItemNode;

	int nDescID = 0;
	int	nItemCount = 1;
	if (nItemCount == 0)	{ pNewItemNode->nItemCount = 1; }
	else					{ pNewItemNode->nItemCount = nItemCount; }

	element.GetAttribute(&nDescID, MTOK_SELL_ITEMID);
	element.GetAttribute(&pNewItemNode->nRentPeriodHour, MTOK_SELL_RENT_PERIOD_HOUR, 0);

	pNewItemNode->nItemID = nDescID;
	pNewItemNode->bIsRentItem = (pNewItemNode->nRentPeriodHour > 0);


	MMatchItemDesc* pItemDesc = MGetMatchItemDescMgr()->GetItemDesc(nDescID);
	if (pItemDesc != NULL)
	{
		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
	else
	{
		MQuestItemDesc* pQuestItemDesc = GetQuestItemDescMgr().FindQItemDesc(nDescID);
		if (0 == pQuestItemDesc)
		{
			delete pNewItemNode;
			return;
		}

		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
}

void MMatchShop2::Clear()
{
	int nVectorSize = (int) m_ItemNodeVector.size();
	for (int i = 0; i < nVectorSize; i++)
	{
		ShopItemNode* pNode = m_ItemNodeVector[i];
		delete pNode;
	}

	m_ItemNodeVector.clear();
	m_ItemNodeMap.clear();
}

MMatchShop2* MMatchShop2::GetInstance()
{
	static MMatchShop2 g_stMatchShop;
	return &g_stMatchShop;
}

bool MMatchShop2::IsSellItem(const unsigned long int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor != m_ItemNodeMap.end()) {
		return true;
	}

	return false;
}

ShopItemNode* MMatchShop2::GetSellItemByIndex(int nListIndex)
{
	if ((nListIndex < 0) || (nListIndex >= GetCount())) return NULL;

	return m_ItemNodeVector[nListIndex];
}

ShopItemNode* MMatchShop2::GetSellItemByItemID(int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor == m_ItemNodeMap.end()) {
		return NULL;
	}

	return (ShopItemNode*) (itor->second);
}





MMatchShop3::MMatchShop3()
{

}
MMatchShop3::~MMatchShop3()
{

}
bool MMatchShop3::Create(const char* szDescFileName)
{
	return ReadXml(szDescFileName);

	return true;
}

void MMatchShop3::Destroy()
{
	Clear();
}

bool MMatchShop3::ReadXml(const char* szFileName)
{
	MXmlDocument	xmlDocument;

	xmlDocument.Create();

	if (!xmlDocument.LoadFromFile(szFileName))
	{
		xmlDocument.Destroy();
		return false;
	}

	MXmlElement rootElement, chrElement, attrElement;
	char szTagName[256];

	rootElement = xmlDocument.GetDocumentElement();

	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++)
	{
		chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!strcmp(szTagName, MTOK_SELL))
		{
			ParseSellItem(chrElement);
		}
	}

	xmlDocument.Destroy();

	return true;
}

void MMatchShop3::ParseSellItem(MXmlElement& element)
{
	ShopItemNode *pNewItemNode = new ShopItemNode;

	int nDescID = 0;
	int	nItemCount = 1;
	if (nItemCount == 0)	{ pNewItemNode->nItemCount = 1; }
	else					{ pNewItemNode->nItemCount = nItemCount; }

	element.GetAttribute(&nDescID, MTOK_SELL_ITEMID);
	element.GetAttribute(&pNewItemNode->nRentPeriodHour, MTOK_SELL_RENT_PERIOD_HOUR, 0);

	pNewItemNode->nItemID = nDescID;
	pNewItemNode->bIsRentItem = (pNewItemNode->nRentPeriodHour > 0);


	MMatchItemDesc* pItemDesc = MGetMatchItemDescMgr()->GetItemDesc(nDescID);
	if (pItemDesc != NULL)
	{
		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
	else
	{
		MQuestItemDesc* pQuestItemDesc = GetQuestItemDescMgr().FindQItemDesc(nDescID);
		if (0 == pQuestItemDesc)
		{
			delete pNewItemNode;
			return;
		}

		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
}

void MMatchShop3::Clear()
{
	int nVectorSize = (int) m_ItemNodeVector.size();
	for (int i = 0; i < nVectorSize; i++)
	{
		ShopItemNode* pNode = m_ItemNodeVector[i];
		delete pNode;
	}

	m_ItemNodeVector.clear();
	m_ItemNodeMap.clear();
}

MMatchShop3* MMatchShop3::GetInstance()
{
	static MMatchShop3 g_stMatchShop;
	return &g_stMatchShop;
}

bool MMatchShop3::IsSellItem(const unsigned long int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor != m_ItemNodeMap.end()) {
		return true;
	}

	return false;
}

ShopItemNode* MMatchShop3::GetSellItemByIndex(int nListIndex)
{
	if ((nListIndex < 0) || (nListIndex >= GetCount())) return NULL;

	return m_ItemNodeVector[nListIndex];
}

ShopItemNode* MMatchShop3::GetSellItemByItemID(int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor == m_ItemNodeMap.end()) {
		return NULL;
	}

	return (ShopItemNode*) (itor->second);
}





MMatchShop4::MMatchShop4()
{

}
MMatchShop4::~MMatchShop4()
{

}
bool MMatchShop4::Create(const char* szDescFileName)
{
	return ReadXml(szDescFileName);

	return true;
}

void MMatchShop4::Destroy()
{
	Clear();
}

bool MMatchShop4::ReadXml(const char* szFileName)
{
	MXmlDocument	xmlDocument;

	xmlDocument.Create();

	if (!xmlDocument.LoadFromFile(szFileName))
	{
		xmlDocument.Destroy();
		return false;
	}

	MXmlElement rootElement, chrElement, attrElement;
	char szTagName[256];

	rootElement = xmlDocument.GetDocumentElement();

	int iCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < iCount; i++)
	{
		chrElement = rootElement.GetChildNode(i);
		chrElement.GetTagName(szTagName);
		if (szTagName[0] == '#') continue;

		if (!strcmp(szTagName, MTOK_SELL))
		{
			ParseSellItem(chrElement);
		}
	}

	xmlDocument.Destroy();

	return true;
}

void MMatchShop4::ParseSellItem(MXmlElement& element)
{
	ShopItemNode *pNewItemNode = new ShopItemNode;

	int nDescID = 0;
	int	nItemCount = 1;
	if (nItemCount == 0)	{ pNewItemNode->nItemCount = 1; }
	else					{ pNewItemNode->nItemCount = nItemCount; }

	element.GetAttribute(&nDescID, MTOK_SELL_ITEMID);
	element.GetAttribute(&pNewItemNode->nRentPeriodHour, MTOK_SELL_RENT_PERIOD_HOUR, 0);

	pNewItemNode->nItemID = nDescID;
	pNewItemNode->bIsRentItem = (pNewItemNode->nRentPeriodHour > 0);


	MMatchItemDesc* pItemDesc = MGetMatchItemDescMgr()->GetItemDesc(nDescID);
	if (pItemDesc != NULL)
	{
		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
	else
	{
		MQuestItemDesc* pQuestItemDesc = GetQuestItemDescMgr().FindQItemDesc(nDescID);
		if (0 == pQuestItemDesc)
		{
			delete pNewItemNode;
			return;
		}

		m_ItemNodeVector.push_back(pNewItemNode);
		m_ItemNodeMap.insert(map<unsigned int, ShopItemNode*>::value_type(pNewItemNode->nItemID, pNewItemNode));
	}
}

void MMatchShop4::Clear()
{
	int nVectorSize = (int) m_ItemNodeVector.size();
	for (int i = 0; i < nVectorSize; i++)
	{
		ShopItemNode* pNode = m_ItemNodeVector[i];
		delete pNode;
	}

	m_ItemNodeVector.clear();
	m_ItemNodeMap.clear();
}

MMatchShop4* MMatchShop4::GetInstance()
{
	static MMatchShop4 g_stMatchShop;
	return &g_stMatchShop;
}

bool MMatchShop4::IsSellItem(const unsigned long int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor != m_ItemNodeMap.end()) {
		return true;
	}

	return false;
}

ShopItemNode* MMatchShop4::GetSellItemByIndex(int nListIndex)
{
	if ((nListIndex < 0) || (nListIndex >= GetCount())) return NULL;

	return m_ItemNodeVector[nListIndex];
}

ShopItemNode* MMatchShop4::GetSellItemByItemID(int nItemID)
{
	map<unsigned int, ShopItemNode*>::iterator itor = m_ItemNodeMap.find(nItemID);
	if (itor == m_ItemNodeMap.end()) {
		return NULL;
	}

	return (ShopItemNode*) (itor->second);
}




