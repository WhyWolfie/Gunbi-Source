#include "stdafx.h"

#ifndef _ZTAGSCANNER_H
#define _ZTAGSCANNER_H

enum ZTagNames
{
	ZTN_SHIELD = 0,
	ZTN_VANILLA,
	ZTN_DMG2,
	ZTN_DMG3,
	ZTN_NC,
	ZTN_NR,
	ZTN_NA,
	ZTN_NS,
	ZTN_NG,
	ZTN_NI,
	ZTN_G,
	ZTN_J,
	ZTN_S,
	ZTN_VAMP,
	ZTN_SNO,
	ZTN_SGO,
	ZTN_R,
	ZTN_IA,
	ZTN_NEX,
	ZTN_END = 32
};

class ZTagScanner 
{
protected:
	// Default Settings;
	float FRICTION			= 3000.0f;
	float GRAVITY			= 2500.0f;
	float TERMINAL_VELOCITY = -3000.0f;
	float SPEED_MULTIPLIER	= 1.0f;
	float ROCKET_SPEED = 2700.0f;
protected:
	float JUMP_FORCE = 900.0f;

	// Storage Settings;
	int nTags[32];
	int nNumber[32];
	string strOutput;
private:
	int SetTags(int A, int B, int C = 0)
	{
		   nTags[A] = B;
		   nNumber[A] = C;
	}
public:
	ZTagScanner() { };
	~ZTagScanner() { };


	bool Find(int A, int B)
	{
		if (nTags[A] == B)
			return true;
		else
			return false;
	}

	int Param(int A)
	{
		return nNumber[A];
	}

	string Output()
	{
		return strOutput;
	}

	bool Create(const char* StageName)
	{
#ifndef _ROOMTAGS
		return false;
#endif
		char RoomName[STAGENAME_LENGTH];
		memcpy(RoomName, StageName, STAGENAME_LENGTH);

		for (int i = 0; i < STAGENAME_LENGTH; i++)
		{
			RoomName[i] = tolower(RoomName[i]);
		}

		for (int i = 0; i < 32; i++)
		{
			nTags[i] = 0;
			nNumber[i] = 0;
		}
	
		strOutput = "";

		int nRoomMod = 0;
		int nNumber = 0;

		char szPreOutput[512] = "";

#define ADD_ROOMMOD_STR(name)    \
		{                                \
			strcat(szPreOutput, name);        \
			strcat(szPreOutput, " - ");    \
			nRoomMod++;                    \
		}

#define FINALIZE_ROOMMOD_STR()                    \
		{                                                \
			int nStrLen = (int) strlen(szPreOutput);        \
			static const int nRemoveLen = (int) strlen(" - ");    \
			szPreOutput[nStrLen - nRemoveLen] = '\0';        \
		}

#ifdef _ROOMTAGS 
#ifdef _ROOMTAG_SHIELD 
		if (strstr(RoomName, "[shield]"))
		{
			this->SetTags(ZTN_SHIELD, TRUE);
			ADD_ROOMMOD_STR("Shield mode");
		}
		else
		{
			this->SetTags(ZTN_SHIELD, FALSE);
		}
#endif
#ifdef _ROOMTAG_VANILLA 
		if (strstr(RoomName, "[v]"))
		{
			this->SetTags(ZTN_VANILLA, TRUE);
			ADD_ROOMMOD_STR("Vanilla mode");
		}
		else
		{
			this->SetTags(ZTN_VANILLA, FALSE);
		}
#endif
#ifdef _ROOMTAG_DMG 
		if (strstr(RoomName, "[dmg2]"))
		{
			this->SetTags(ZTN_DMG2, TRUE);
			ADD_ROOMMOD_STR("Multiple damage");
		}
		else
		{
			this->SetTags(ZTN_DMG2, FALSE);
		}

		if (strstr(RoomName, "[dmg3]"))
		{
			this->SetTags(ZTN_DMG3, TRUE);
			ADD_ROOMMOD_STR("Triple damage");
		}
		else
		{
			this->SetTags(ZTN_DMG2, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOCLOTHES 
		if (strstr(RoomName, "[nc]"))
		{
			this->SetTags(ZTN_NC, TRUE);
			ADD_ROOMMOD_STR("No clothese");
		}
		else
		{
			this->SetTags(ZTN_NC, FALSE);
		}
#endif
#ifdef _ROOMTAG_NORINGS 
		if (strstr(RoomName, "[nr]"))
		{
			this->SetTags(ZTN_NR, TRUE);
			ADD_ROOMMOD_STR("No rings");
		}
		else
		{
			this->SetTags(ZTN_NR, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOAVATAR 
		if (strstr(RoomName, "[na]"))
		{
			this->SetTags(ZTN_NA, TRUE);
			ADD_ROOMMOD_STR("No avatar");
		}
		else
		{
			this->SetTags(ZTN_NA, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOMELEE 
		if (strstr(RoomName, "[ns]"))
		{
			this->SetTags(ZTN_NS, TRUE);
			ADD_ROOMMOD_STR("No melee");
		}
		else
		{
			this->SetTags(ZTN_NS, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOGUNS 
		if (strstr(RoomName, "[ng]"))
		{
			this->SetTags(ZTN_NG, TRUE);
			ADD_ROOMMOD_STR("No ranged");
		}
		else
		{
			this->SetTags(ZTN_NG, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOGUNS 
		if (strstr(RoomName, "[ni]"))
		{
			this->SetTags(ZTN_NI, TRUE);
			ADD_ROOMMOD_STR("No items");
		}
		else
		{
			this->SetTags(ZTN_NI, FALSE);
		}
#endif
#ifdef _ROOMTAG_GRAVITY
		if (strstr(RoomName, "[g"))
		{
			if (sscanf(strstr(RoomName, "[g"), "[g%i]", &nNumber) == 1)
			{
				if (nNumber > 0 && nNumber < 10)
				{
					this->SetTags(ZTN_G, TRUE, nNumber);
					char output[255];
					sprintf(output, "Gravity x%i", nNumber);
					ADD_ROOMMOD_STR(output);
				}
				else
				{
					this->SetTags(ZTN_G, FALSE);
				}
			}
			else
			{
				this->SetTags(ZTN_G, FALSE);
			}
		}
		else
		{
			this->SetTags(ZTN_G, FALSE);
		}
#endif
#ifdef _ROOMTAG_JUMP 
		if (strstr(RoomName, "[j"))
		{
			if (sscanf(strstr(RoomName, "[j"), "[j%i]", &nNumber) == 1)
			{
				if (nNumber > 0 && nNumber < 10)
				{
					this->SetTags(ZTN_J, TRUE, nNumber);
					char output[255];
					sprintf(output, "Jump hight x%i", nNumber);
					ADD_ROOMMOD_STR(output);
				}
				else
				{
					this->SetTags(ZTN_J, FALSE);
				}
			}
			else
			{
				this->SetTags(ZTN_J, FALSE);
			}
		}
		else
		{
			this->SetTags(ZTN_J, FALSE);
		}
#endif
#ifdef _ROOMTAG_SPEED 
		if (strstr(RoomName, "[s"))
		{
			if (sscanf(strstr(RoomName, "[s"), "[s%i]", &nNumber) == 1)
			{
				if (nNumber > 0 && nNumber < 10)
				{
					this->SetTags(ZTN_S, TRUE, nNumber);
					char output[255];
					sprintf(output, "Speed x%i", nNumber);
					ADD_ROOMMOD_STR(output);
				}
				else
				{
					this->SetTags(ZTN_S, FALSE);
				}
			}
			else
			{
				this->SetTags(ZTN_S, FALSE);
			}
		}
		else
		{
			this->SetTags(ZTN_S, FALSE);
		}
#endif
#ifdef _ROOMTAG_VAMP 
		if (strstr(RoomName, "[vamp]"))
		{
			this->SetTags(ZTN_VAMP, TRUE);
			ADD_ROOMMOD_STR("Vampire mode");
		}
		else
		{
			this->SetTags(ZTN_VAMP, FALSE);
		}
#endif
#ifdef _ROOMTAG_ONLYSNIPER 
		if (strstr(RoomName, "[sno]"))
		{
			this->SetTags(ZTN_SNO, TRUE);
			ADD_ROOMMOD_STR("Sniper only");
		}
		else
		{
			this->SetTags(ZTN_SNO, FALSE);
		}
#endif
#ifdef _ROOMTAG_ONLYSHOTGUN 
		if (strstr(RoomName, "[sgo]"))
		{
			this->SetTags(ZTN_SGO, TRUE);
			ADD_ROOMMOD_STR("Shotgun only");
		}
		else
		{
			this->SetTags(ZTN_SGO, FALSE);
		}
#endif
#ifdef _ROOMTAG_RELOAD 
		if (strstr(RoomName, "[r]"))
		{
			this->SetTags(ZTN_R, TRUE);
			ADD_ROOMMOD_STR("Auto reload");
		}
		else
		{
			this->SetTags(ZTN_R, FALSE);
		}
#endif
#ifdef _ROOMTAG_INFINITEAMMO 
		if (strstr(RoomName, "[ia]"))
		{
			this->SetTags(ZTN_IA, TRUE);
			ADD_ROOMMOD_STR("Infinite ammo");
		}
		else
		{
			this->SetTags(ZTN_IA, FALSE);
		}
#endif
#ifdef _ROOMTAG_NOEXPLOSION
		if (strstr(RoomName, "[nex]"))
		{
			this->SetTags(ZTN_NEX, TRUE);
			ADD_ROOMMOD_STR("No explosions");
		}
		else
		{
			this->SetTags(ZTN_NEX, FALSE);
		}
#endif
#endif
		if (nRoomMod != 0)
		{
			FINALIZE_ROOMMOD_STR();
			char szOutput[1024];
			sprintf(szOutput, "[Room Modifiers (%d)] %s", nRoomMod, szPreOutput);
			strOutput = szOutput;
		}
		return true;
	}
};

#endif

//float mod = 10.0f;
//GRAVITY = 2500.0f * 0.1f * mod;
//SPEED_MULTIPLIER = 1.0f * 0.1f * mod;
//FRICTION = 3000.0f * 0.1f * mod;
//JUMP_FORCE = 900.0f * 0.1f * mod;
//ROCKET_SPEED = 2700.0f * 0.1f * mod;
//TERMINAL_VELOCITY = -3000.0f * 0.1f * mod;