#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include "nospread.h"


void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

void CRageBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	// Master switch
	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	// Anti-Aim
	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
		DoAntiAim(pCmd, bSendPacket);

	// Aimbot
	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	// No Spread
	if (Menu::Window.RageBotTab.AccuracySpread.GetState())
		DoNoSpread(pCmd);

	// Recoil
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	GameUtils::NormaliseViewAngle(pCmd->viewangles);
	// Aimstep
	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}
bool Ready2Silent(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float serverTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	if (pWeapon->GetNextPrimaryAttack() > serverTime)
		return false;

	return true;
}
bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

void CRageBot::NormalizeVector(Vector& vec)
{
	for (int i = 0; i < 3; ++i)
	{
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}

	vec[2] = 0.f;
}

void CRageBot::ClampAngles(Vector& vecAngles)
{
	if (vecAngles[0] > 89.f)
		vecAngles[0] = 89.f;
	if (vecAngles[0] < -89.f)
		vecAngles[0] = -89.f;
	if (vecAngles[1] > 180.f)
		vecAngles[1] = 180.f;
	if (vecAngles[1] < -180.f)
		vecAngles[1] = -180.f;

	vecAngles[2] = 0.f;
}

bool isAbleToShot(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	int iTickBase = pLocal->GetTickBase();
	float flNextPrimaryAttack = pWeapon->GetNextPrimaryAttack();
	float flCurTime = iTickBase * Interfaces::Globals->interval_per_tick;
	return(!(flNextPrimaryAttack > flCurTime));
}

int TickDelay = 0;
bool Switch = false;

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;
	//IsLocked = false;

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	bool bReady2Silent = Ready2Silent(pLocal, pWeapon);
	if (pWeapon)
	{
		if (!bReady2Silent && bSendPacket == false)
			bSendPacket = true;
		if (!GameUtils::IsSniper(pWeapon)) {
			Switch = false;
			TickDelay = 0;
		}

		if (TickDelay)
			TickDelay--;

		if (Switch) {
			if (!TickDelay)
				Switch = false;

			if (!isAbleToShot)
				Switch = false;
		}

		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon) || !isAbleToShot(hackManager.pLocal(), pWeapon))
		{
			-IN_ATTACK;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		// Target selection type
		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		// Key
		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint = GetHitboxPosition(pTarget, HitBox);

		float Spread = pWeapon->GetAccuracyPenalty();
		//float HitChance = 75.f + (Menu::Window.RageBotTab.HitChanceAmount.GetValue() / 4);

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			if (Menu::Window.RageBotTab.AccuracyAutoCrouch.GetState() && !pLocal->IsDormant())
			{
				pCmd->buttons |= IN_DUCK;
			}
			if (Menu::Window.RageBotTab.AccuracyAutoScope.GetState() && !pLocal->IsScoped() && GameUtils::IsSniper(pWeapon))
			{
				pCmd->buttons |= IN_ATTACK2;

				if (!Switch) {
					if (isAbleToShot(pLocal, pWeapon)) {
						Switch = true;
						TickDelay = 16;
					}
				}
			}
			if (!Menu::Window.RageBotTab.AccuracySpreadLimit.GetState() || Spread <= (Menu::Window.RageBotTab.AccuracyMinimumSpread.GetValue() / 100.f))
				//if (!Menu::Window.RageBotTab.HitChance.GetState() || (1.0f - pWeapon->GetAccuracyPenalty()) * 100.f >= HitChance)
			{
				if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					IsLocked = true;
					if (Menu::Window.RageBotTab.AccuracyAutoScope.GetState() && GameUtils::IsSniper(pWeapon)) {
						if (TickDelay == 0) {

							pCmd->buttons |= IN_ATTACK;
						}
					}
					else {
						pCmd->buttons |= IN_ATTACK;
					}
					if (Menu::Window.RageBotTab.AimbotPSilent.GetState())
					{
						Vector point;
						IClientEntity* pLocal = hackManager.pLocal();
						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
						float qOldViewAnglesY = pCmd->viewangles.y;
						Vector vecOldMovement(pCmd->sidemove, pCmd->forwardmove, 0.f);
						Vector vOldView = pCmd->viewangles;

						float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

						bool bBulletTime = true;
						if (pWeapon->GetNextPrimaryAttack() > flServerTime)
							bBulletTime = false;

						// Do your stuff here
						static int iChokedPackets = 0;
						iChokedPackets++;

						if (iChokedPackets < 14)
						{
							if (IsAbleToShoot(pLocal))
								bSendPacket = false;
							else bSendPacket = true;
						}
						else
						{
							bSendPacket = true;
							iChokedPackets = 0;
						}

						if (bSendPacket)
						{
							pCmd->viewangles.x = 89;
							pCmd->viewangles.y -= 180;
							pCmd->sidemove = vecOldMovement.x;
							pCmd->forwardmove = vecOldMovement.y;
						}
						if (Menu::Window.MiscTab.OtherFakeLag.GetValue()) {
							int iChoke = 14;
							static int iFakeLag = 0;

							if (iFakeLag < iChoke)
								bSendPacket = false;

							if (iFakeLag > iChoke + 3)
								iFakeLag = 0;

							iFakeLag++;

						}
					}
				}
			}

		}

		// Auto Pistol
		static bool WasFiring = false;
		CSWeaponInfo WeaponInfo = pWeapon->GetCSWpnData(pWeapon, 456);
		if (!WeaponInfo.m_IsFullAuto && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
		{
			if (pCmd->buttons & IN_ATTACK)
			{
				if (WasFiring)
				{
					pCmd->buttons &= ~IN_ATTACK;
				}
			}

			WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
		}
		if (!bReady2Silent && !(pCmd->buttons & IN_ATTACK))
			bSendPacket = true;
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 360.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	// Target selection
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

class QAngleByValue;
class QAngle
{
public:
	float x, y, z;

	QAngle(void);
	QAngle(float X, float Y, float Z);

	operator QAngleByValue &() { return *((QAngleByValue *)(this)); }
	operator const QAngleByValue &() const { return *((const QAngleByValue *)(this)); }

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	bool IsValid() const;
	void Invalidate();

	bool IsZero()
	{
		CHECK_VALID(*this);
		if (this->x == 0.f && this->y == 0.f && this->z == 0.f)
			return true;

		return false;
	}

	float operator[](int i) const;
	float& operator[](int i);

	float* Base();
	float const* Base() const;

	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	QAngle& operator+=(const QAngle &v);
	QAngle& operator-=(const QAngle &v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	float   Length() const;
	float   LengthSqr() const;

	QAngle& operator=(const QAngle& src);

	QAngle  operator-(void) const;

	QAngle  operator+(const QAngle& v) const;
	QAngle  operator-(const QAngle& v) const;
	QAngle  operator*(float fl) const;
	QAngle  operator/(float fl) const;

	QAngle Clamp();
	QAngle Mod(float N);

	inline QAngle QAngle::Normalize()
	{
		QAngle vector;
		float length = this->Length();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else
			vector.x = vector.y = 0.0f; vector.z = 1.0f;

		return vector;
	}

	QAngle Normalized()
	{
		if (this->x != this->x)
			this->x = 0;
		if (this->y != this->y)
			this->y = 0;
		if (this->z != this->z)
			this->z = 0;

		if (this->x > 89.f)
			this->x = 89.f;
		if (this->x < -89.f)
			this->x = -89.f;

		while (this->y > 180)
			this->y -= 360;
		while (this->y <= -180)
			this->y += 360;

		if (this->y > 180.f)
			this->y = 180.f;
		if (this->y < -180.f)
			this->y = -180.f;

		this->z = 0;

		return *this;
	}
};

int CRageBot::GetTargetHealth()
{
	// Target selection
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	std::vector<int> HitBoxesToScan;
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();

	// Get the hitboxes to scan
#pragma region GetHitboxesToScan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	if (HitScanMode == 0)
	{
		// No Hitscan, just a single hitbox
		switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
		{
		case 0:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			break;
		case 1:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			break;
		case 2:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			break;
		case 3:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			break;
		}
	}
	else
	{
		switch (HitScanMode)
		{
		case 1:
			// head/body
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			break;
		case 2:
			// basic +(arms, thighs)
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		case 3:
			// heaps ++(just all the random shit)
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		}
	}
#pragma endregion Get the list of shit to scan

	// check hits
	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);
			if (CanHit(Point, &Damage))
			{
				c = Color(0, 255, 0, 255);
				if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue() / 01)
				{
					return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}

	return -1;
}

void CRageBot::DoNoSpread(CUserCmd *pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (GameUtils::IsBallisticWeapon(pWeapon))
	{
		NoSpread.RollSpread(pWeapon, pCmd->random_seed, pCmd, pCmd->viewangles);
	}
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	// Ghetto rcs shit, implement properly later
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd)
{
	bool ReturnValue = false;
	// Get the full angles
	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	/*
	WTF IS THIS STUPID CHECK? OMG

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
	return ReturnValue;
	}
	*/

	IsLocked = true;
	//-----------------------------------------------

	// Aim Step Calcs
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState() || Menu::Window.RageBotTab.AimbotPSilent.GetState())
	{
		pCmd->viewangles = angles;
	}

	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState() && !Menu::Window.RageBotTab.AimbotPSilent.GetState())
		Interfaces::Engine->SetViewAngles(angles);

	return ReturnValue;
	LastAngle = pCmd->viewangles;
	Interfaces::Engine->GetViewAngles(angles);
}

namespace AntiAims
{
	// Pitches
	void StaticPitch(CUserCmd *pCmd)
	{
		// Up
		pCmd->viewangles.x = 89.f;
	}

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up) pCmd->viewangles.x = -89.f;
		else pCmd->viewangles.x = 89.f;
		up = !up;
	}
	void Emotion2(CUserCmd *pCmd, bool& bSendPacket) {
		pCmd->viewangles.x = -89.f;
		if (bSendPacket)
			pCmd->viewangles.x = 89.f;
	}
	void clickbait(CUserCmd *pCmd)
	{
		{
			int random = rand() % 100;
			int random2 = rand() % 1000;

			static bool dir;
			static float current_x = pCmd->viewangles.x;

			if (random == 1) dir = !dir;

			if (dir)
				current_x += 700;
			else
				current_x -= 34;

			pCmd->viewangles.x = current_x;

			if (random == random2)
				pCmd->viewangles.x += random;

		}

	}

	// Yaws
	void FastSpint(CUserCmd *pCmd)
	{
		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 100;
		else
			current_y -= 100;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;
	}

	void SlowSpin(CUserCmd *pCmd)
	{
		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 10;
		else
			current_y -= 10;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}

	void BackJitter(CUserCmd *pCmd)
	{
		int random = 179 - rand() % 100;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		pCmd->viewangles.y = current_y;

	}
	void MoveFix(CUserCmd *cmd, Vector &realvec)
	{
		Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		VectorAngles(vMove, vMove2);

		flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
		cmd->forwardmove = cos(flYaw) * flSpeed;
		cmd->sidemove = sin(flYaw) * flSpeed;

		if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
			cmd->forwardmove = -cmd->forwardmove;
	}

	void Flip(CUserCmd *pCmd)
	{
		static bool back = false;
		back = !back;
		if (back)
			pCmd->viewangles.y -= rand() % 100;
		else
			pCmd->viewangles.y += rand() % 100;

	}

	void Backwards(CUserCmd *pCmd)
	{
		static bool back = false;
		back = !back;
		if (back)
			pCmd->viewangles.y -= 180.0f;
		else
			pCmd->viewangles.y += 180.0f;
	}
}
Vector localPlyViewPos;
void aimAA(CUserCmd *pCmd) {

	float len, closest = 8192;
	Vector pos;
	for (int i = 1; i <= Interfaces::Globals->maxClients; i++) {
		IClientEntity *ent = Interfaces::EntList->GetClientEntity(i);


		pos = ent->GetHeadPos();
		if (len > closest)
			continue;

		closest = len;
	}

	pCmd->viewangles.y = pos.y;
}

//bool EdgeAntiAim(IClientEntity *localPlayer, CUserCmd *cmd, float flWall, float flCornor)
//{
//	static CTraceFilter filter;
//
//	bool ret = false;
//
//	Vector localPosition = localPlayer->GetEyePosition();
//
//	for (int y = 0; y < 360; y++)
//	{
//		Vector tmp(10.0f, cmd->viewangles.y, 0.0f);
//		tmp.y += y;
//		Normalize(tmp, tmp);
//
//		Vector forward;
//		AngleVectors(tmp, &forward);
//
//		float length = ((16.0f + 3.0f) + ((16.0f + 3.0f) * sin(10.0f * PI / 180))) + 7.0f;
//		forward *= length;
//
//		Ray_t ray;
//		trace_t traceData;
//
//		ray.Init(localPosition, (localPosition + forward));
//		Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &traceData);
//
//		if (traceData.fraction != 1.0f)
//		{
//			Vector angles;
//			Vector Negate = traceData.plane.normal;
//
//			Negate *= -1;
//			VectorAngles(Negate, angles);
//
//			tmp.y = angles.y;
//			Normalize(tmp, tmp);
//			trace_t leftTrace, rightTrace;
//
//			Vector left, right;
//			AngleVectors(tmp + Vector(0.0f, 30.0f, 0.0f), &left);
//			AngleVectors(tmp - Vector(0.0f, 30.0f, 0.0f), &right);
//
//			left *= (length + (length * sin(30.0f * PI / 180)));
//			right *= (length + (length * sin(30.0f * PI / 180)));
//
//			ray.Init(localPosition, (localPosition + left));
//			Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &leftTrace);
//
//			ray.Init(localPosition, (localPosition + right));
//			Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &rightTrace);
//
//			if ((leftTrace.fraction == 1.0f)
//				&& (rightTrace.fraction != 1.0f))
//			{
//				tmp.y -= flCornor;
//				// LEFT
//			}
//			else if ((leftTrace.fraction != 1.0f)
//				&& (rightTrace.fraction == 1.0f))
//			{
//				tmp.y += flCornor;
//				// RIGHT
//			}
//
//			cmd->viewangles.y = tmp.y;
//			//cmd->viewangles.y -= flWall;
//			ret = true;
//		}
//	}
//
//	return ret;
//}

void VectorAngles3D(const Vector&vecForward, Vector&vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void AtTarget(IClientEntity *Target, CUserCmd *pCmd) {
	if (!Target)
		return;

	if ((Target->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) || Target->IsDormant() || !Target->IsAlive() || Target->GetHealth() <= 0)
		return;

	Vector TargetPosition = Target->GetEyePosition();
	CalcAngle(hackManager.pLocal()->GetEyePosition(), TargetPosition, pCmd->viewangles);
}

//CTraceFilterNoPlayer tfNoPlayers;
//
//void ClampAngles(Vector& vecAngles)
//{
//	if (vecAngles[0] > 89.f)
//		vecAngles[0] = 89.f;
//	if (vecAngles[0] < -89.f)
//		vecAngles[0] = -89.f;
//	if (vecAngles[1] > 180.f)
//		vecAngles[1] = 180.f;
//	if (vecAngles[1] < -180.f)
//		vecAngles[1] = -180.f;
//
//	vecAngles[2] = 0.f;
//}
//
//void vectorAnglesVec(Vector forward, Vector &angles)
//{
//	if (forward[1] == 0.0f && forward[0] == 0.0f)
//	{
//		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
//		angles[1] = 0.0f;
//	}
//	else
//	{
//		float len2d = sseSqrt((forward[0] * forward[0]) + (forward[1] * forward[1]));
//
//		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
//		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
//
//		if (angles[0] < 0.0f) angles[0] += 360.0f;
//		if (angles[1] < 0.0f) angles[1] += 360.0f;
//	}
//
//	angles[2] = 0.0f;
//}
//void EdgeAntiAim(IClientEntity *pLocal, float &fYawOut) {
//	Vector vEyePos = pLocal->GetEyePosition();
//
//	ITraceFilter* tracefilter = (ITraceFilter*)&tfNoPlayers;
//
//	for (int y = 0; y < 360; y++)
//	{
//		Vector qTmp(10.0f, fYawOut, 0.0f);
//		qTmp.y += y;
//
//		if (qTmp.y > 180.0)
//			qTmp.y -= 360.0;
//		else if (qTmp.y < -180.0)
//			qTmp.y += 360.0;
//
//		ClampAngles(qTmp);
//
//		Vector vForward;
//
//		vectorAnglesVec(qTmp, vForward);
//
//		float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
//		vForward *= fLength;
//
//		trace_t trData;
//
//		Vector vTraceEnd = vEyePos + vForward;
//
//		Ray_t ray;
//
//		ray.Init(vEyePos, vTraceEnd);
//		Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trData);
//
//
//		if (trData.fraction != 1.0f)
//		{
//			Vector angles;
//
//			Vector vNegative = Vector(trData.plane.normal.x * -1.0f, trData.plane.normal.y * -1.0f, trData.plane.normal.z * -1.0f);
//
//			vectorAnglesVec(vNegative, angles);
//
//			NormalizeVector(angles);
//
//			qTmp.y = angles.y;
//
//			ClampAngles(qTmp);
//
//			trace_t trLeft, trRight;
//
//			Vector vLeft, vRight;
//			vectorAnglesVec(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
//			vectorAnglesVec(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);
//
//			vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
//			vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
//
//			vTraceEnd = vEyePos + vLeft;
//
//			ray.Init(vEyePos, vTraceEnd);
//			Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trLeft);
//
//			vTraceEnd = vEyePos + vRight;
//
//			ray.Init(vEyePos, vTraceEnd);
//			Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trRight);
//
//			if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
//				qTmp.y -= 90.f;
//			else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
//				qTmp.y += 90.f;
//
//			if (qTmp.y > 180.0)
//				qTmp.y -= 360.0;
//			else if (qTmp.y < -180.0)
//				qTmp.y += 360.0;
//
//			fYawOut = qTmp.y;
//
//			if (fYawOut > 180.0)
//				fYawOut -= 360.0;
//			else if (fYawOut < -180.0)
//				fYawOut += 360.0;
//
//			//if (!bHeadOnWall)
//			//	bHeadOnWall = true;
//
//			return;
//		}
//	}
//
//	fYawOut -= 180.0f;
//
//	if (fYawOut > 180.0)
//		fYawOut -= 360.0;
//	else if (fYawOut < -180.0)
//		fYawOut += 360.0;
//
//	//if (bHeadOnWall)
//	//	bHeadOnWall = false;
//}
//

void NormalizeVector(Vector& vec) {
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}


void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}
void AngleVectors2(const Vector& qAngles, Vector& vecForward)
{
	float sp, sy, cp, cy;
	SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
	SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

	vecForward[0] = cp*cy;
	vecForward[1] = cp*sy;
	vecForward[2] = -sp;
}

bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CUserCmd* cmd, float flWall, float flCornor)
{
	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocalBaseEntity;

	auto bRetVal = false;
	auto vecCurPos = pLocalBaseEntity->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		AngleVectors2(vecDummy, vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			VectorAngles2(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			AngleVectors2(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

			Vector vecRight;
			AngleVectors2(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right			

			cmd->viewangles.y = vecDummy.y;
			cmd->viewangles.y -= flWall;
			cmd->viewangles.x = 89.0f;
			bRetVal = true;
		}
	}
	return bRetVal;
}

// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool& bSendPacket)
{
	// If the aimbot is doing something don't do anything
	if (IsAimStepping || pCmd->buttons & IN_USE)
		return;

	// Weapon shit
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon) {
		CSWeaponInfo pWeaponInfo = pWeapon->GetCSWpnData(pWeapon, 456);
		// Knives or grenades
		if (!GameUtils::IsBallisticWeapon(pWeapon))
			return;
	}

	if (Menu::Window.RageBotTab.AtTargets.GetState()) {
		IClientEntity *Target = Interfaces::EntList->GetClientEntity(this->TargetID);
		AtTarget(Target, pCmd);
	}

	if (Menu::Window.RageBotTab.EdgeAntiAim.GetState()) {
		auto bEdge = EdgeAntiAim(hackManager.pLocal(), pCmd, 360.f, 89.f);
		if (bEdge)
			return;
	}

	// Anti-Aim Pitch
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex())
	{
	case 1:
		// up
		AntiAims::StaticPitch(pCmd);
		break;
	case 2:
		// up
		AntiAims::JitterPitch(pCmd);
		break;
	case 3:
		// Jitter
		pCmd->viewangles.x = 89.000000;
		break;
	case 4:
		//reflex
		pCmd->viewangles.x = 36000088.000000;
		break;
	case 5:
		pCmd->viewangles.x = -35999912.000000;
	break;
	case 6 :
		pCmd->viewangles.x = -89;
		break;
	}

	//Anti-Aim Yaw
	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
	{
	case 0:
		// No Yaw AA
		break;
		/*case 1:
		// Fast Spin
		AntiAims::FastSpint(pCmd);
		break;
		case 2:
		// Slow Spin
		AntiAims::SlowSpin(pCmd);
		break;
		case 3:
		// Inverse
		pCmd->viewangles.y -= 180;
		break;
		case 4:
		// Jitter
		AntiAims::BackJitter(pCmd);
		break;
		case 5:
		// Flip
		AntiAims::Flip(pCmd);
		break;

		case 6:
		// BackWards
		AntiAims::Backwards(pCmd);
		break; */

	case 1: {
		Vector OriginalY = pCmd->viewangles;
		pCmd->viewangles.y = OriginalY.y + 180.0;
		break;
	}

	case 2: {
		if (!(pCmd->command_number % 2)) {
			Vector OriginalY2 = pCmd->viewangles;
			pCmd->viewangles.y = OriginalY2.y + 180.0;
		}
		break;
	}

	case 3: {
		int v36 = pCmd->command_number % 3;
		if (v36 == 1) {
			pCmd->viewangles.y = pCmd->viewangles.y - 160.0;
		}
		else if (v36 == 2) {
			pCmd->viewangles.y = pCmd->viewangles.y - 200.0;
		}
		break;
	}

	case 4: {
		if (pCmd->command_number % 3)
			pCmd->viewangles.y = pCmd->viewangles.y + 200.0;
		break;
	}

	case 5: {
		Vector v37 = pCmd->viewangles;
		pCmd->viewangles.y = v37.y + 90.0;
		break;
	}

	case 6: {
		Vector v372 = pCmd->viewangles;
		if (pCmd->command_number % 2) {
			pCmd->viewangles.y = v372.y + 90.0;
		}
		else {
			pCmd->viewangles.y = v372.y - 90.0;
		}
		break;
	}

	case 7: {
		if (!(pCmd->command_number % 2)) {
			pCmd->viewangles.y = pCmd->viewangles.y + 180.0;
		}
		break;
	}

	case 8: {
		float CalculatedCurTime = (Interfaces::Globals->curtime * 1000.0);
		pCmd->viewangles.y = CalculatedCurTime;
		break;
	}

	case 9: {
		float CalculatedCurTime = (Interfaces::Globals->curtime * 5000.0);
		pCmd->viewangles.y = CalculatedCurTime;
		break;
	}

	case 10: {
		Vector OriginalYX = pCmd->viewangles;
		pCmd->viewangles.y = OriginalYX.y + 36000180.0;
		break;
	}

	case 11: {
		pCmd->viewangles.y = 36000180.000000;
		break;
	}

	case 12: {
		float v42 = (Interfaces::Globals->curtime * 5000.0);
		pCmd->viewangles.y = v42 + 36000000.0;
		break;
	}

	case 13: {
		if (pCmd->command_number % 2) {
			pCmd->viewangles.y = pCmd->viewangles.y + 36000180.0;
		}
		float CalculatedCurTime_2 = (Interfaces::Globals->curtime * 5000.0);
		pCmd->viewangles.y = CalculatedCurTime_2 + 36000000.0;
		break;
	}

	case 14: {
		pCmd->viewangles.y = 0;
		break;
	}
	}
}
