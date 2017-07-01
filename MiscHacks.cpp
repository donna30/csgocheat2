#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
}


void CMiscHacks::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	static ConVar* nameConvar = Interfaces::CVar->FindVar("name");
	static int Times = 250;

	if (Menu::Window.MiscTab.OtherAimware.GetState()) {
		std::string text = "";
		static bool useSpace = false;
		text = useSpace ? "NeonCheats " : "NeonCheats";
		NET_SetConVar noob("name", text.c_str());
		Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(noob);
		useSpace = !useSpace;
	}
	if (Menu::Window.MiscTab.OtherInterwebz.GetState()) {
		std::string text = "";
		static bool useSpace = false;
		text = useSpace ? "NeonCheats.uk.to " : "NeonCheats.uk.to";
		NET_SetConVar noob("name", text.c_str());
		Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(noob);
		useSpace = !useSpace;
	}
	if (Menu::Window.MiscTab.Othernamesteal.GetState()) {
		std::string text = "";
		static bool useSpace = false;
		text = useSpace ? "NeonCheats.uk.to" : "NeonCheats.uk.to";
		NET_SetConVar noob("name", text.c_str());
		Interfaces::Engine->GetNetChannelInfo()->SendNetMsg(noob);
		useSpace = !useSpace;
	}
	// Any Move Stuff


	// Bhop
	if (Menu::Window.MiscTab.OtherBunnyhop.GetState())
		AutoJump(pCmd);

	if (Menu::Window.MiscTab.OtherTeleportHack.GetState())
		TeleportHack(pCmd);

	// Strafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	if (Menu::Window.MiscTab.OtherAutoStrafe.GetState())
		AutoStrafe(pCmd);


	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Regular
		ChatSpamRegular();
		break;
	case 2:
		// Report Spam
		ChatSpamReports();
		break;
	case 3:
		// Report Spam
		ChatSpam1();
		break;
	case 4:
		// Report Spam
		ChatSpam2();
		break;
	case 5:
		// Report Spam
		ChatSpam3();
		break;
	}

	/*switch (Menu::Window.MiscTab.Stealer.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Regular
		ChatSpamRegular();
		break;
	case 2:
		// Report Spam
		ChatSpamReports();
		break;
	case 3:
		// Report Spam
		ChatSpam1();
		break;
	case 4:
		// Report Spam
		ChatSpam2();
		break;
	case 5:
		// Report Spam
		ChatSpam3();
		break;
	}
	*/
}


void CMiscHacks::TeleportHack(CUserCmd *pCmd)
{
	if (GetAsyncKeyState(VK_F2))   //Klawisz do teleportowania
	{
		*(DWORD*)(&pCmd->viewangles.z) = 0x7F7FFFFF;
	}
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 45)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::AutoStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	static bool bDirection = true;

	bool bKeysPressed = true;

	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44))
		bKeysPressed = false;
	if (pCmd->buttons & IN_ATTACK)
		bKeysPressed = false;

	float flYawBhop = 0.f;
	if (pLocal->GetVelocity().Length() > 45.f)
	{
		float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

		z = x / y;
		z = fabsf(z);

		a = x * z;

		flYawBhop = a;
	}

	if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{

		if (bDirection)
		{
			AutoStrafeView -= flYawBhop;
			GameUtils::NormaliseViewAngle(AutoStrafeView);
			pCmd->sidemove = -450;
			bDirection = false;
		}
		else
		{
			AutoStrafeView += flYawBhop;
			GameUtils::NormaliseViewAngle(AutoStrafeView);
			pCmd->sidemove = 430;
			bDirection = true;
		}

		if (pCmd->mousedx < 0)
		{
			pCmd->forwardmove = 22;
			pCmd->sidemove = -450;
		}

		if (pCmd->mousedx > 0)
		{
			pCmd->forwardmove = +22;
			pCmd->sidemove = 450;
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}
void CMiscHacks::ChatSpam3()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.3)
		return;

	// Loop through all active entitys
	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() != entity->GetTeamNum())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	int randomIndex = rand() % Names.size();
	char buffer[128];
	static unsigned long int meme = 3091961887844204720;
	sprintf_s(buffer, "%s is gey", Names[randomIndex].c_str(), meme);
	meme += 1;
	SayInChat(buffer);
	start_t = clock();
}

#include <random>
__forceinline int RandomInt(int Min, int Max) {
	std::random_device RandomDevice;
	std::mt19937 MT(RandomDevice());
	std::uniform_int_distribution<int>Compartment(Min, Max);
	return Compartment(MT);
}

std::vector<std::string> chatspamss =
{ "We are unityhacks. We are legion. We do not forgive. We do not forget. Expect us!", " www.unityhacks.com - #1 for a reason", "Get rekt - by unityhacks.com", "www.unityhacks.com - best featuers without lack of security", "www.unityhacks.com","www.unityhacks.com - we provide you with skill", "www.unityhacks.com - your premium hacks provider"};

void CMiscHacks::ChatSpamReports()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspamss[rand() % chatspamss.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}

std::vector<std::string> chatspam =
{ "AIMWARE.NET | Premium Rage/Legit CSGO Cheat" };

void CMiscHacks::ChatSpamRegular()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspam[rand() % chatspam.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}

}

std::vector<std::string> chatspams =
{ "INTERWEBZ.CC OWNS ME AND ALL" };

void CMiscHacks::ChatSpam1()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspams[rand() % chatspams.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}
std::vector<std::string> chatspamsss =
{ "NeonCheats - $30 Lifetime", "NeonCheats - I'm for the memes", "NeonCheats - Destroying Kids", "NeonCheats - Who needs skill?", "NeonCheats - If you had fun, you won.","NeonCheats - Forged by gods", "NeonCheats - Premium framework", "NeonCheats - b1g cheat", "NeonCheats - Send nudes for a discount" };

void CMiscHacks::ChatSpam2()
{
	static DWORD lastspammed = 0;
	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspamsss[rand() % chatspamsss.size()];
		Interfaces::Engine->ClientCmd_Unrestricted(p.c_str());
	}
}