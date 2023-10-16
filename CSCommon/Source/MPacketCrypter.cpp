#include "stdafx.h"
#include "MPacketCrypter.h"
#include "MPacket.h"

int MPacketCrypter::m_nSHL = (MCOMMAND_VERSION % 6) + 1;
unsigned char MPacketCrypter::m_ShlMask = 0;


////////////////////////////////////////////////////////////////////////
bool MPacketCrypter::InitKey(MPacketCrypterKey* pKey)
{
	memcpy(&m_Key, pKey, sizeof(MPacketCrypterKey));
	return true;
}

bool MPacketCrypter::Encrypt(const char* pSource, int nSrcLen, char* pTarget, int nTarLen)
{
	MPacketCrypter::Encrypt(pSource, nSrcLen, pTarget, nTarLen, &m_Key);
	return true;
}

bool MPacketCrypter::Decrypt(const char* pSource, int nSrcLen, char* pTarget, int nTarLen)
{
	MPacketCrypter::Decrypt(pSource, nSrcLen, pTarget, nTarLen, &m_Key);
	return true;
}

bool MPacketCrypter::Encrypt(char* pSource, int nSrcLen)
{
	MPacketCrypter::Encrypt(pSource, nSrcLen, &m_Key);
	return true;
}

bool MPacketCrypter::Decrypt(char* pSource, int nSrcLen)
{
	MPacketCrypter::Decrypt(pSource, nSrcLen, &m_Key);
	return true;
}

bool MPacketCrypter::Encrypt(const char* pSource, int nSrcLen, char* pTarget, int nTarLen, MPacketCrypterKey* pKey)
{
	int nKeyIndex = 0;
	for (int i = 0; i < nSrcLen; i++)
	{
		*pTarget = _Enc(*pSource, pKey->szKey[nKeyIndex]);

		nKeyIndex++;
		if (nKeyIndex >= PACKET_CRYPTER_KEY_LEN) nKeyIndex = 0;
		pTarget++;
		pSource++;
	}

	return true;
}

bool MPacketCrypter::Decrypt(const char* pSource, int nSrcLen, char* pTarget, int nTarLen, MPacketCrypterKey* pKey)
{
	int nKeyIndex = 0;
	for (int i = 0; i < nSrcLen; i++)
	{
		*pTarget = _Dec(*pSource, pKey->szKey[nKeyIndex]);

		nKeyIndex++;
		if (nKeyIndex >= PACKET_CRYPTER_KEY_LEN) nKeyIndex = 0;
		pTarget++;
		pSource++;
	}

	return true;
}

#define KEY1 78	//1-255
#define KEY2 5	//1-7

char MPacketCrypter::_Enc(char s, char key)
{
	BYTE b = (s - KEY1) % 256;
	b ^= key;
	_asm {
		rol b, KEY2
	}
	return b;
}

char MPacketCrypter::_Dec(char s, char key)
{
	BYTE b = s;
	_asm {
		ror b, KEY2
	}
	b ^= key;
	b = (b + KEY1) % 256;
	return b;
}


bool MPacketCrypter::Encrypt(char* pSource, int nSrcLen, MPacketCrypterKey* pKey)
{
	int nKeyIndex = 0;
	for (int i = 0; i < nSrcLen; i++)
	{
		*pSource = _Enc(*pSource, pKey->szKey[nKeyIndex]);

		nKeyIndex++;
		if (nKeyIndex >= PACKET_CRYPTER_KEY_LEN) nKeyIndex = 0;
		pSource++;
	}
	return true;
}

bool MPacketCrypter::Decrypt(char* pSource, int nSrcLen, MPacketCrypterKey* pKey)
{
	int nKeyIndex = 0;
	for (int i = 0; i < nSrcLen; i++)
	{
		*pSource = _Dec(*pSource, pKey->szKey[nKeyIndex]);

		nKeyIndex++;
		if (nKeyIndex >= PACKET_CRYPTER_KEY_LEN) nKeyIndex = 0;
		pSource++;
	}

	return true;
}

MPacketCrypter::MPacketCrypter()
{
	InitConst();
	memset(&m_Key, 0, sizeof(MPacketCrypterKey));
}

void MPacketCrypter::InitConst()
{
	m_nSHL = (MCOMMAND_VERSION % 6) + 1;

	m_ShlMask=0;
	for (int i = 0; i < m_nSHL; i++)
	{
		m_ShlMask += (1 << i);
	}
}