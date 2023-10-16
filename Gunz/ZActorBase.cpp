#include "stdafx.h"
#include "ZActorBase.h"

MImplementRTTI(ZActorBase, ZCharacterObject);

ZActorBase::ZActorBase( IGame* pGame )
: ZCharacterObject(pGame)
, m_pGame(pGame)
{

}

void ZActorBase::AllocObjectVMesh()
{
	m_pVMesh = new ZObjectVMesh;
}

ZOBJECTHITTEST ZActorBase::HitTest_Actor( bool bUseMeshPick, const rvector& origin, const rvector& to, float fTime, rvector *pOutPos )
{
	rvector footpos,actor_dir;

	if (!GetHistory(&footpos, &actor_dir, fTime)) {
		return ZOH_NONE;
	}

	if (bUseMeshPick)
	{
		rvector dir = to - origin;
		Normalize(dir);

		RPickInfo pickinfo;
		memset(&pickinfo,0,sizeof(RPickInfo));

		if (m_pVMesh->Pick(origin, dir, &pickinfo))
		{
			*pOutPos = pickinfo.vOut;
			if ((pickinfo.parts == eq_parts_head) || (pickinfo.parts == eq_parts_face)) {
				return ZOH_HEAD;
			}
			else {
				return ZOH_BODY;
			}
		}
	}
	else
	{
		rvector headpos = footpos;
		if (m_pVMesh)
		{
			headpos.z += m_Collision.GetHeight() - 5.0f;
		}

		rvector ap,cp;
		float fDist=GetDistanceBetweenLineSegment(origin,to,footpos,headpos,&ap,&cp);
		float fDistToThis=Magnitude(origin-cp);
		if(fDist < (m_Collision.GetRadius()-5.0f))
		{
			rvector dir = to - origin;
			Normalize(dir);

			rvector ap2cp=ap-cp;
			float fap2cpsq=D3DXVec3LengthSq(&ap2cp);
			float fdiff=sqrtf(m_Collision.GetRadius() * m_Collision.GetRadius() - fap2cpsq);

			if (pOutPos) {
				*pOutPos = ap - dir*fdiff;;
			}

			return ZOH_BODY;
		}
	}

	return ZOH_NONE;
}