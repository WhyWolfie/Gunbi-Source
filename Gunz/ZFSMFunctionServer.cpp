#include "stdafx.h"
#include "ZFSMFunctionServer.h"

ZFSMFunctionServer::ZFSMFunctionServer()
: m_nDice(0)
{

};

bool ZFSMFunctionServer::ExecuteFunction(float fFrameDelta, int funcId, int nArg1, int nArg2, int nArg3, int nArg4 )
{
	switch (funcId)
	{
	case FSMFUNC_DONOTHING:
		break;
	case FSMFUNC_DICE:
		{
			Func_Dice(0, nArg1);
			break;
		}

	case FSMFUNC_FIND_TARGET:
		{
			Func_FindTarget();
			break;
		}

	case FSMFUNC_SET_TARGET_LAST_ATTACKER:
		{
			Func_SetTargetLastAttacker();
			break;
		}

	case FSMFUNC_ROTATE_TO_TARGET:
		{
			Func_RotateToTarget(fFrameDelta);
			break;
		}

	case FSMFUNC_FACE_TO_TARGET:
		{
			Func_FaceToTarget();
			break;
		}

	case FSMFUNC_REDUCE_GROGGY:
		{
			Func_ReduceGroggy(nArg1);
			break;
		}

	case FSMFUNC_BUILD_WAYPOINTS_TO_TARGET:
		{
			Func_BuildWaypointsToTarget();
			break;
		}

	case FSMFUNC_RUN_WAYPOINTS:
		{
			Func_RunWaypoints(fFrameDelta);
			break;
		}

	case FSMFUNC_DISAPPEAR:
		{
			Func_Disappear(fFrameDelta, nArg1, nArg2*0.01f);
			break;
		}

	case FSMFUNC_POST_DEAD:
		{
			Func_PostDead();
			break;
		}

	case FSMFUNC_ON_ENTER_BLASTED_SOAR:
		{
			Func_OnEnterBlastedSoar();
			break;
		}

	case FSMFUNC_ON_ENTER_BLASTED_THRUST:
		{
			Func_OnEnterBlastedThrust();
			break;
		}

	case FSMFUNC_ON_UPDATE_BLASTED_THRUST:
		{
			Func_OnUpdateBlastedThrust(fFrameDelta);
			break;
		}

	case FSMFUNC_EXIT_SUFFERING:
		{
			Func_ExitSuffering();
			break;
		}

	case FSMFUNC_ON_ENTER_DIE:
		{
			Func_OnEnterDie();
			break;
		}

	case FSMFUNC_FIND_TARGET_IN_HEIGHT:
		{
			Func_FindTargetInHeight(nArg1);
			break;
		}
	case FSMFUNC_RUN_ALONG_TARGET_ORBITAL:
		{
			Func_RunAlongTargetOrbital(fFrameDelta,nArg1);
			break;
		}
	case FSMFUNC_SPEED_ACCEL:
		{
			Func_SpeedAccel(nArg1);
			break;
		}
	case FSMFUNC_TURN_ORBITAL_DIRECTION:
		{
			Func_TurnOrbitalDir();
			break;
		}
	case FSMFUNC_CLEAR_WAYPOINTS:
		{
			Func_ClearWaypoints();
			break;
		}

	case FSMFUNC_FIND_TARGET_IN_DIST: 
		{
			Func_FindTargetInDist(nArg1);
			break;
		}

	case FSMFUNC_FACE_TO_LASTEST_ATTACKER:
		{
			Func_FaceToLastestAttacker();
			break;
		}
	
	case FSMFUNC_RUN_WAY_POINTS_ALONG_ROUTE:
		{
			Func_RunWaypointsAlongRoute();
			break;
		}
	}

	return true;
}

void ZFSMFunctionServer::Func_Dice(int nMin, int nMax)
{
	m_nDice = RandomNumber(nMin, nMax);
}