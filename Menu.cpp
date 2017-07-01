#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "Gui.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 715

MainWindow Menu::Window;

void ChangeName() {
	for (size_t i = 5; i < 5; i++);
	ConVar* name = Interfaces::CVar->FindVar("name");
	*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
	if (name)
	{
		name->SetValue("\r");
	}
}


void UnLoadCallbk()
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ChangeName, NULL, NULL, NULL);
	}
}

void SaveLegitCallbk()
{
	switch (Menu::Window.ColorsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "legit.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "ultralegit.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "closet.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "nonprime.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "rage.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "mmhvh.cfg");
		break;
	case 6:
		GUI.SaveWindowState(&Menu::Window, "nospreadhvh.cfg");
		break;
	case 7:
		GUI.SaveWindowState(&Menu::Window, "extra.cfg");
		break;
	}
}

void LoadLegitCallbk()
{
	switch (Menu::Window.ColorsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "legit.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "ultralegit.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "closet.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "nonprime.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "rage.cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "mmhvh.cfg");
		break;
	case 6:
		GUI.LoadWindowState(&Menu::Window, "nospreadhvh.cfg");
		break;
	case 7:
		GUI.LoadWindowState(&Menu::Window, "extra.cfg");
		break;
	}
}

void SaveRageCallbk()
{
	GUI.SaveWindowState(&Menu::Window, "rage.cfg");
}

void LoadRageCallbk()
{
	GUI.LoadWindowState(&Menu::Window, "rage.cfg");
}

void PanicCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void MainWindow::Setup()
{
	//SetPosition(50, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("NeonCheats - Version 4.5");

	RegisterTab(&LegitBotTab);
	RegisterTab(&RageBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&SkinchangerTab);
	RegisterTab(&ColorsTab);

	RECT Client = GetClientArea();
	Client.bottom -= 15;

	LegitBotTab.Setup();
	RageBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SkinchangerTab.Setup();
	ColorsTab.Setup();



#pragma region Bottom Buttons


	UnloadButton.SetText("Name Exploit");
	UnloadButton.SetCallback(UnLoadCallbk);
	UnloadButton.SetPosition(16, Client.bottom - 50);
	
	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveLegitCallbk);
	SaveButton.SetPosition(400, Client.bottom - 560);

//	ByPass.SetText("Sv_Cheats bypass - VAC!");
//	ByPass.SetCallback(Bypass);
//	ByPass.SetPosition(390, Client.bottom - 50);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadLegitCallbk);
	LoadButton.SetPosition(587, Client.bottom - 560);

	//SaveButton1.SetText("Save Rage");
	//SaveButton1.SetCallback(SaveRageCallbk);
	//SaveButton1.SetPosition(390, Client.bottom - 50);

	//LoadButton1.SetText("Load Rage");
	//LoadButton1.SetCallback(LoadRageCallbk);
	//LoadButton1.SetPosition(577, Client.bottom - 50);
	
	PanicButton.SetText("Panic");
	PanicButton.SetCallback(PanicCallbk);
	PanicButton.SetPosition(203, Client.bottom - 50);
	
//	FirstButton.SetText("Firstperson");
//	FirstButton.SetPosition(390, Client.bottom - 85);

//	ThirdButton.SetText("Thirdperson");
//	ThirdButton.SetPosition(577, Client.bottom - 85);
	
	
	//LegitBotTab.RegisterControl(&UnloadButton);
	//RageBotTab.RegisterControl(&UnloadButton);
	//VisualsTab.RegisterControl(&UnloadButton);
	MiscTab.RegisterControl(&UnloadButton);
//	MiscTab.RegisterControl(&ByPass);
	//SkinchangerTab.RegisterControl(&UnloadButton);
	//ColorsTab.RegisterControl(&UnloadButton);

	//LegitBotTab.RegisterControl(&SaveButton);
	//RageBotTab.RegisterControl(&SaveButton);
	//VisualsTab.RegisterControl(&SaveButton);
	//MiscTab.RegisterControl(&SaveButton);
	//SkinchangerTab.RegisterControl(&SaveButton);
	ColorsTab.RegisterControl(&SaveButton);
	ColorsTab.RegisterControl(&ConfigBox);

	//LegitBotTab.RegisterControl(&LoadButton);
	//RageBotTab.RegisterControl(&LoadButton);
	//VisualsTab.RegisterControl(&LoadButton);
	//MiscTab.RegisterControl(&LoadButton);
	//SkinchangerTab.RegisterControl(&LoadButton);
	ColorsTab.RegisterControl(&LoadButton);
	
	//LegitBotTab.RegisterControl(&SaveButton1);
	//RageBotTab.RegisterControl(&SaveButton1);
	//VisualsTab.RegisterControl(&SaveButton1);
	//MiscTab.RegisterControl(&SaveButton1);
	//SkinchangerTab.RegisterControl(&SaveButton1);
	//ColorsTab.RegisterControl(&SaveButton1);
	
	//LegitBotTab.RegisterControl(&LoadButton1);
	//RageBotTab.RegisterControl(&LoadButton1);
	//VisualsTab.RegisterControl(&LoadButton1);
	//MiscTab.RegisterControl(&LoadButton1);
	//SkinchangerTab.RegisterControl(&LoadButton1);
	//ColorsTab.RegisterControl(&LoadButton1);

	//LegitBotTab.RegisterControl(&PanicButton);
	//RageBotTab.RegisterControl(&PanicButton);
	////VisualsTab.RegisterControl(&PanicButton);
	MiscTab.RegisterControl(&PanicButton);
	//MiscTab.RegisterControl(&FirstButton);
	//MiscTab.RegisterControl(&ThirdButton);
	//SkinchangerTab.RegisterControl(&PanicButton);
	//ColorsTab.RegisterControl(&PanicButton);
	
	//RADAR
	Radar->SetPosition(Menu::Window.VisualsTab.OtherRadarXPosition.GetValue(), Menu::Window.VisualsTab.OtherRadarYPosition.GetValue());
	Radar->SetSize(200, 200);
//	Radar->SetTitle("    Radar");

	// Save and Load

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("a");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("RageBot");
	AimbotGroup.SetSize(376, 380); // 280
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 360.f);
	AimbotFov.SetValue(0.f);
	AimbotGroup.PlaceLabledControl("Field of View", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AimbotPSilent.SetFileId("aim_psilent");
	AimbotGroup.PlaceLabledControl("Perfect Silent", this, &AimbotPSilent);

	AimbotAutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aim Step", this, &AimbotAimStep);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);
	
	AntiSpawnProtection.SetFileId("aim_antispawnprotection");
	AimbotGroup.PlaceLabledControl("Anti Spawn Protection", this, &AntiSpawnProtection);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AimbotGroup.PlaceLabledControl("No Recoil", this, &AccuracyRecoil);

	AccuracyAutoCrouch.SetFileId("acc_crouch");
	AimbotGroup.PlaceLabledControl("Auto Crouch", this, &AccuracyAutoCrouch);

	AccuracyAutoScope.SetFileId("acc_scope");
	AimbotGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	//AccuracySpreadLimit.SetFileId("acc_spreadlimon");
	//AimbotGroup.PlaceLabledControl("Hitchance", this, &AccuracySpreadLimit);

	//AccuracyMinimumSpread.SetFileId("acc_spreadlim");
	//AccuracyMinimumSpread.SetBoundaries(0.f, 5.0f);
	//AccuracyMinimumSpread.SetValue(1.5f);
	//AccuracyGroup.PlaceLabledControl("Hitchance Amount", this, &AccuracyMinimumSpread);


#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(408, 270);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(360, 200);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Closest To Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Lowest Health");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off"); //0
	TargetHitscan.AddItem("Low"); // 1
	TargetHitscan.AddItem("Medium"); // 2
	TargetHitscan.AddItem("High"); // 3
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	AccuracyAutoWall.SetFileId("acc_awall");
	TargetGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(0.f, 100.f);
	AccuracyMinimumDamage.SetValue(0.f);
	TargetGroup.PlaceLabledControl("Min Damage", this, &AccuracyMinimumDamage);

#pragma endregion Targetting controls 


#pragma region AntiAim
	AntiAimGroup.SetPosition(408, 48);
	AntiAimGroup.SetText("HvH");
	AntiAimGroup.SetSize(360, 220); //360
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("None");
	AntiAimPitch.AddItem("Down");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Angel Down");
	AntiAimPitch.AddItem("Angel UP");
	AntiAimPitch.AddItem("Fake Angel");
	//AntiAimPitch.AddItem("Reflex");

	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	//AntiAimYaw.SetFileId("aa_y");
	//AntiAimYaw.AddItem("None");
	//AntiAimYaw.AddItem("Spin Fast");
	//AntiAimYaw.AddItem("Spin Slow");
	//AntiAimYaw.AddItem("Backwards");
	//AntiAimYaw.AddItem("Jitter");
	//AntiAimYaw.AddItem("Flip");

	AntiAimYaw.AddItem("None");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("Fake Backward");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("Jitter Synced");
	AntiAimYaw.AddItem("Sideways");
	AntiAimYaw.AddItem("Fake Sideways");
	AntiAimYaw.AddItem("Fake Forward");
	AntiAimYaw.AddItem("Slow Spinbot");
	AntiAimYaw.AddItem("Fast Spinbot");
	AntiAimYaw.AddItem("Angel Backward");
	AntiAimYaw.AddItem("Angel Inverse");
	AntiAimYaw.AddItem("Angel Spin");
	AntiAimYaw.AddItem("Angel Fake Spin");
	AntiAimYaw.AddItem("Fake Angel");
	

	AntiAimGroup.PlaceLabledControl("Yaw", this, &AntiAimYaw);

	EdgeAntiAim.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Edge", this, &EdgeAntiAim);

	AtTargets.SetFileId("aa_attargets");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AtTargets);

	AccuracyAngleFix.SetFileId("acc_aaa");
	AntiAimGroup.PlaceLabledControl("Resolver", this, &AccuracyAngleFix);

	AccuracySpread.SetFileId("acc_nospread");
	AntiAimGroup.PlaceLabledControl("No Spread HvH", this, &AccuracySpread);

#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("b");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 210);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key Bind", this, &AimbotKeyBind);

	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl("Smoke Check", this, &AimbotSmokeCheck);
#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(272, 48);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 210);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl("On Key", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("Key Bind", this, &TriggerKeyBind);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl("Recoil", this, &TriggerRecoil);

	//TriggerHitChance.SetFileId("trig_hitchance");
	//TriggerGroup.PlaceLabledControl("Hitchance", this, &TriggerHitChance);

	//TriggerHitChanceAmmount.SetFileId("trig_hitchanceamount");
	//TriggerHitChanceAmmount.SetBoundaries(1, 100);
	//TriggerHitChanceAmmount.SetValue(50);
	//TriggerGroup.PlaceLabledControl("Hitchance Amount", this, &TriggerHitChanceAmmount);

	TriggerDelay.SetFileId("trig_delay");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	TriggerGroup.PlaceLabledControl("Delay", this, &TriggerDelay);

	//TriggerBurst.SetFileId("trig_burst");
	//TriggerBurst.SetBoundaries(0, 5);
	//TriggerBurst.SetValue(1);
	//TriggerGroup.PlaceLabledControl("Burst", this, &TriggerBurst);

	//TriggerBreak.SetFileId("trig_break");
	TriggerBreak.SetBoundaries(0, 100);
	TriggerBreak.SetValue(1);
    //TriggerGroup.PlaceLabledControl("Break", this, &TriggerBreak);

#pragma endregion Triggerbot stuff

#pragma region TriggerbotFilter
	TriggerFilterGroup.SetPosition(528, 48);
	TriggerFilterGroup.SetText("Triggerbot Hitboxes");
	TriggerFilterGroup.SetSize(240, 210);
	RegisterControl(&TriggerFilterGroup);

	TriggerHead.SetFileId("trig_head");
	TriggerFilterGroup.PlaceLabledControl("Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	TriggerFilterGroup.PlaceLabledControl("Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	TriggerFilterGroup.PlaceLabledControl("Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	TriggerFilterGroup.PlaceLabledControl("Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	TriggerFilterGroup.PlaceLabledControl("Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	TriggerFilterGroup.PlaceLabledControl("Friendly Fire", this, &TriggerTeammates);
#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 270);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(240, 175);
	RegisterControl(&WeaponMainGroup);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 100.f);
	WeaponMainSpeed.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("Max Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(3.5f);
	WeaponMainGroup.PlaceLabledControl("Field of View", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
	WeaponMainRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("Recoil", this, &WeaponMainRecoil);

	WeaponMainAimtime.SetFileId("main_aimtime");
	WeaponMainAimtime.SetBoundaries(0, 3);
	WeaponMainAimtime.SetValue(0);
	WeaponMainGroup.PlaceLabledControl("Aim Time", this, &WeaponMainAimtime);

	WeaoponMainStartAimtime.SetFileId("main_aimstart");
	WeaoponMainStartAimtime.SetBoundaries(0, 5);
	WeaoponMainStartAimtime.SetValue(0);
	WeaponMainGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponMainStartAimtime);
#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(272, 270);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 175);
	RegisterControl(&WeaponPistGroup);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multihitbox");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(3.f);
	WeaponPistGroup.PlaceLabledControl("Field of View", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(1.f);
	WeaponPistGroup.PlaceLabledControl("Recoil", this, &WeaponPistRecoil);

	WeaponPistAimtime.SetFileId("pist_aimtime");
	WeaponPistAimtime.SetBoundaries(0, 3);
	WeaponPistAimtime.SetValue(0);
	WeaponPistGroup.PlaceLabledControl("Aim Time", this, &WeaponPistAimtime);

	WeaoponPistStartAimtime.SetFileId("pist_aimstart");
	WeaoponPistStartAimtime.SetBoundaries(0, 5);
	WeaoponPistStartAimtime.SetValue(0);
	WeaponPistGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponPistStartAimtime);
#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(528, 270);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 175);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multihitbox");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(1.5f);
	WeaponSnipGroup.PlaceLabledControl("Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(2.f);
	WeaponSnipGroup.PlaceLabledControl("Field of View", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(1.f);
	WeaponSnipGroup.PlaceLabledControl("Recoil", this, &WeaponSnipRecoil);

	WeaponSnipAimtime.SetFileId("snip_aimtime");
	WeaponSnipAimtime.SetBoundaries(0, 3);
	WeaponSnipAimtime.SetValue(0);
	WeaponSnipGroup.PlaceLabledControl("Aim Time", this, &WeaponSnipAimtime);

	WeaoponSnipStartAimtime.SetFileId("pist_aimstart");
	WeaoponSnipStartAimtime.SetBoundaries(0, 5);
	WeaoponSnipStartAimtime.SetValue(0);
	WeaponSnipGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponSnipStartAimtime);
#pragma endregion

#pragma region MPs
	WeaponMpGroup.SetPosition(16, 460);
	WeaponMpGroup.SetText("MPs");
	WeaponMpGroup.SetSize(240, 176);
	RegisterControl(&WeaponMpGroup);

	WeaponMpHitbox.SetFileId("mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Multihitbox");
	WeaponMpGroup.PlaceLabledControl("Hitbox", this, &WeaponMpHitbox);

	WeaponMpSpeed.SetFileId("mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(1.0f);
	WeaponMpGroup.PlaceLabledControl("Max Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(4.f);
	WeaponMpGroup.PlaceLabledControl("Field of View", this, &WeaponMpFoV);

	WeaponMpRecoil.SetFileId("mps_recoil");
	WeaponMpRecoil.SetBoundaries(0.f, 2.f);
	WeaponMpRecoil.SetValue(1.f);
	WeaponMpGroup.PlaceLabledControl("Recoil", this, &WeaponMpRecoil);

	WeaponMpAimtime.SetFileId("mps_aimtime");
	WeaponMpAimtime.SetBoundaries(0, 3);
	WeaponMpAimtime.SetValue(0);
	WeaponMpGroup.PlaceLabledControl("Aim Time", this, &WeaponMpAimtime);

	WeaoponMpStartAimtime.SetFileId("mps_aimstart");
	WeaoponMpStartAimtime.SetBoundaries(0, 5);
	WeaoponMpStartAimtime.SetValue(0);
	WeaponMpGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponMpStartAimtime);
#pragma endregion

#pragma region Shotguns
	WeaponShotgunGroup.SetPosition(272, 460);
	WeaponShotgunGroup.SetText("Shotguns");
	WeaponShotgunGroup.SetSize(240, 176);
	RegisterControl(&WeaponShotgunGroup);

	WeaponShotgunHitbox.SetFileId("shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Multihitbox");
	WeaponShotgunGroup.PlaceLabledControl("Hitbox", this, &WeaponShotgunHitbox);

	WeaponShotgunSpeed.SetFileId("shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(1.0f);
	WeaponShotgunGroup.PlaceLabledControl("Max Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(3.f);
	WeaponShotgunGroup.PlaceLabledControl("Field of View", this, &WeaponShotgunFoV);

	WeaponShotgunRecoil.SetFileId("shotgun_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.f, 2.f);
	WeaponShotgunRecoil.SetValue(1.f);
	WeaponShotgunGroup.PlaceLabledControl("Recoil", this, &WeaponShotgunRecoil);

	WeaponShotgunAimtime.SetFileId("shotgun_aimtime");
	WeaponShotgunAimtime.SetBoundaries(0, 3);
	WeaponShotgunAimtime.SetValue(0);
	WeaponShotgunGroup.PlaceLabledControl("Aim Time", this, &WeaponShotgunAimtime);

	WeaoponShotgunStartAimtime.SetFileId("shotgun_aimstart");
	WeaoponShotgunStartAimtime.SetBoundaries(0, 5);
	WeaoponShotgunStartAimtime.SetValue(0);
	WeaponShotgunGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponShotgunStartAimtime);
#pragma endregion

#pragma region Machineguns
	WeaponMGGroup.SetPosition(528, 460);
	WeaponMGGroup.SetText("Machineguns");
	WeaponMGGroup.SetSize(240, 176);
	RegisterControl(&WeaponMGGroup);

	WeaponMGHitbox.SetFileId("mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Multihitbox");
	WeaponMGGroup.PlaceLabledControl("Hitbox", this, &WeaponMGHitbox);

	WeaponMGSpeed.SetFileId("mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(1.0f);
	WeaponMGGroup.PlaceLabledControl("Max Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(4.f);
	WeaponMGGroup.PlaceLabledControl("Field of View", this, &WeaponMGFoV);

	WeaponMGRecoil.SetFileId("mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.f, 2.f);
	WeaponMGRecoil.SetValue(1.f);
	WeaponMGGroup.PlaceLabledControl("Recoil", this, &WeaponMGRecoil);

	WeaponMGAimtime.SetFileId("mg_aimtime");
	WeaponMGAimtime.SetBoundaries(0, 3);
	WeaponMGAimtime.SetValue(0);
	WeaponMGGroup.PlaceLabledControl("Aim Time", this, &WeaponMGAimtime);

	WeaoponMGStartAimtime.SetFileId("mg_aimstart");
	WeaoponMGStartAimtime.SetBoundaries(0, 5);
	WeaoponMGStartAimtime.SetValue(0);
	WeaponMGGroup.PlaceLabledControl("Start Aim Time", this, &WeaoponMGStartAimtime);
#pragma endregion
}
// Hier bin ich
void CVisualTab::Setup()
{
	SetTitle("c");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(16, 48);
	OptionsGroup.SetSize(193, 600);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("Off");
	OptionsBox.AddItem("Edge");
	OptionsBox.AddItem("Full");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsName.AddItem("Off");
	OptionsName.AddItem("Normal");
	OptionsName.AddItem("Rainbow");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("Off");
	OptionsHealth.AddItem("Bottom");
	OptionsHealth.AddItem("Side");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsWeapon.AddItem("Off");
	OptionsWeapon.AddItem("Text");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl("Chams - Broken", this, &OptionsChams);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsSkeleton.AddItem("Off");
	OptionsSkeleton.AddItem("");
	OptionsSkeleton.AddItem("Rainbow");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsAimSpot.SetFileId("opt_aimspot");
	OptionsGroup.PlaceLabledControl("Aim Spot", this, &OptionsAimSpot);

	OptionsDilights.SetFileId("opt_dilights");
	OptionsGroup.PlaceLabledControl("Dilights", this, &OptionsDilights);

	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Rank Reveal", this, &OptionsCompRank);

	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceLabledControl("Glow", this, &OptionsGlow);

	GlowZ.SetFileId("GlowZ");
	GlowZ.SetBoundaries(0.f, 255.f);
	GlowZ.SetValue(235.f);
	OptionsGroup.PlaceLabledControl("Glow Opacity", this, &GlowZ);


#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(225, 48);
	FiltersGroup.SetSize(193, 600);
	RegisterControl(&FiltersGroup);

	FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceLabledControl("All", this, &FiltersAll);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersChickens.SetFileId("ftr_chickens");
	FiltersGroup.PlaceLabledControl("Chickens", this, &FiltersChickens);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);
#pragma endregion Setting up the Filters controls


#pragma region VisualMiscs
	VisualMiscGroup.SetText("Other");
	VisualMiscGroup.SetPosition(434, 48);
	VisualMiscGroup.SetSize(334, 600);
	RegisterControl(&VisualMiscGroup);

	OtherSpectators.SetFileId("otr_speclist");
	VisualMiscGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherCrosshairType.SetFileId("otr_crosstype");
	OtherCrosshairType.AddItem("Off");
	OtherCrosshairType.AddItem("Static");
	OtherCrosshairType.AddItem("Dynamic");
	VisualMiscGroup.PlaceLabledControl("Crosshair Type", this, &OtherCrosshairType);

	OtherCrosshair.SetFileId("otr_cross");
	OtherCrosshair.AddItem("Cross");
	OtherCrosshair.AddItem("Big Cross");
	OtherCrosshair.AddItem("Circle");
	OtherCrosshair.AddItem("Quadrat");
	OtherCrosshair.AddItem("Little Swastika");
	OtherCrosshair.AddItem("Big Swastika");
	VisualMiscGroup.PlaceLabledControl("Crosshair Style", this, &OtherCrosshair);


	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow");
	VisualMiscGroup.PlaceLabledControl("Hands", this, &OtherNoHands);


	OtherRadar.SetFileId("otr_radar");
	VisualMiscGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	OtherRadarEnemyOnly.SetFileId("otr_radar_enemy_only");
	VisualMiscGroup.PlaceLabledControl("Radar Enemy Only", this, &OtherRadarEnemyOnly);

	OtherRadarVisibleOnly.SetFileId("otr_radar_visible_only");
	VisualMiscGroup.PlaceLabledControl("Radar Visible Only", this, &OtherRadarVisibleOnly);

	OtherRadarXPosition.SetFileId("otr_radar_position_x");
	OtherRadarYPosition.SetFileId("otr_radar_position_y");

	OtherRadarScale.SetFileId("otr_radar_scale");
	OtherRadarScale.SetBoundaries(0, 10000);
	OtherRadarScale.SetValue(10000);
    VisualMiscGroup.PlaceLabledControl("Radar Range", this, &OtherRadarScale);

#pragma endregion Setting up the VisualMiscs controls
}

void CMiscTab::Setup()
{
	SetTitle("f");

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(16, 48);
	OtherGroup.SetSize(376, 500);
	RegisterControl(&OtherGroup);

	OtherBunnyhop.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl("Bunnyhop", this, &OtherBunnyhop);

	OtherAutoStrafe.SetFileId("otr_autostrafe");
	OtherGroup.PlaceLabledControl("Auto Strafe", this, &OtherAutoStrafe);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti-Untrusted", this, &OtherSafeMode);

	OtherChatSpam.SetFileId("otr_spam");
	OtherChatSpam.AddItem("Off");
	OtherChatSpam.AddItem("AIMWARE.NET");
	OtherChatSpam.AddItem("UNITYHACKS.COM");
	OtherChatSpam.AddItem("INTERWEBZ.CC");
	OtherChatSpam.AddItem("NeonCheats");
	//OtherChatSpam.AddItem("GEY SPAM");
	OtherGroup.PlaceLabledControl("Chat Spam", this, &OtherChatSpam);

	ClanTag.SetFileId("clantag - Broken");
	ClanTag.AddItem("None");
	ClanTag.AddItem("NeonCheats");
	ClanTag.AddItem("NeonCheats Animated");
	ClanTag.AddItem("Valve");
	OtherGroup.PlaceLabledControl("Clan Tag", this, &ClanTag);

	Othernamesteal.SetFileId("otr_namesteal");
	OtherGroup.PlaceLabledControl("Name Steal", this, &Othernamesteal);

#pragma endregion Setting up the Other controls

#pragma region Fakelag
	FakeLagGroup.SetText("Name Changers - Broken");
	FakeLagGroup.SetPosition(408, 48);
	FakeLagGroup.SetSize(300, 110);
	RegisterControl(&FakeLagGroup);

	OtherAimware.SetFileId("otr_aimware");
	FakeLagGroup.PlaceLabledControl("NeonCheats", this, &OtherAimware);

	OtherInterwebz.SetFileId("otr_interwebz");
	FakeLagGroup.PlaceLabledControl("NeonCheats.uk.to", this, &OtherInterwebz);

	
#pragma endregion Fakelagend

	
#pragma region ClandTags
	ClandTags.SetText("ClanTag Changer - Broken");
	ClandTags.SetPosition(408, 175);
	ClandTags.SetSize(300, 300);
	RegisterControl(&ClandTags);


	ClanTag1.SetFileId("clantag_key1");
	ClandTags.PlaceLabledControl("NeonCheats $30 Lifetime", this, &ClanTag1);

	ClanTag2.SetFileId("clantag_key2");
	ClandTags.PlaceLabledControl("NeonCheats.uk.to", this, &ClanTag2);

	ClanTag3.SetFileId("clantag_key3");
	ClandTags.PlaceLabledControl("NeonCheats", this, &ClanTag3);

//	ClanTag4.SetFileId("clantag_key4");
//	ClandTags.PlaceLabledControl("izakMods TAG", this, &ClanTag4);
//
//	ClanTag5.SetFileId("clantag_key5");
//	ClandTags.PlaceLabledControl("Valve Tag", this, &ClanTag5);
//
//	ClanTag7.SetFileId("clantag_key7");
//	ClandTags.PlaceLabledControl("m0neywaster Tag", this, &ClanTag7);
//
//	ClanTag6.SetFileId("clantag_key6");
//	ClandTags.PlaceLabledControl("GITARA SIEMA Tag", this, &ClanTag6);
//
//	ClanTag8.SetFileId("clantag_key8");
//	ClandTags.PlaceLabledControl("ZE4CHEATS Tag", this, &ClanTag8);
//
//	ClanTag9.SetFileId("clantag_key9");
//	ClandTags.PlaceLabledControl("Trump Supporter Tag", this, &ClanTag9);
	

#pragma end region

    OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);


	OtherEventSpam.SetFileId("otr_eventspam");
	OtherGroup.PlaceLabledControl("Event Spam", this, &OtherEventSpam);


//	OtherTeleportHack.SetFileId("otr_teleporthack");
//	OtherGroup.PlaceLabledControl("Teleport (F2)", this, &OtherTeleportHack);

//	OtherThirdperson.SetFileId("otr_forcethirdperson");
//	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	OtherFakeLag.SetFileId("otr_fakelag");
	OtherGroup.PlaceLabledControl("FakeLag", this, &OtherFakeLag);
	OtherFakeLag.SetBoundaries(0.0f, 20.f);
	OtherFakeLag.SetValue(0.f);

	OtherCircle.SetFileId("otr_circle");
	OtherGroup.PlaceLabledControl("Circle Key", this, &OtherCircle);

	CircleAmount.SetFileId("otr_circleamount");
	OtherGroup.PlaceLabledControl("Circle Amount", this, &CircleAmount);
	CircleAmount.SetBoundaries(0.0f, 10.f);
	CircleAmount.SetValue(0.f);

}

void CColorsTab::Setup()
{
	SetTitle("d");
#pragma region Visual Colors
	ColorsGroup.SetPosition(16, 48);
	ColorsGroup.SetText("Colors");
	ColorsGroup.SetSize(360, 500);
	RegisterControl(&ColorsGroup);

	CTColorVisR.SetFileId("ct_color_vis_r");
	CTColorVisR.SetBoundaries(0.f, 255.f);
	CTColorVisR.SetValue(120.f);
	ColorsGroup.PlaceLabledControl("CT ESP - Visible R", this, &CTColorVisR);

	CTColorVisG.SetFileId("ct_color_vis_g");
	CTColorVisG.SetBoundaries(0.f, 255.f);
	CTColorVisG.SetValue(210.f);
	ColorsGroup.PlaceLabledControl("CT ESP - Visible G", this, &CTColorVisG);

	CTColorVisB.SetFileId("ct_color_vis_b");
	CTColorVisB.SetBoundaries(0.f, 255.f);
	CTColorVisB.SetValue(26.f);
	ColorsGroup.PlaceLabledControl("CT ESP - Visible B", this, &CTColorVisB);

	CTColorNoVisR.SetFileId("ct_color_no_vis_r");
	CTColorNoVisR.SetBoundaries(0.f, 255.f);
	CTColorNoVisR.SetValue(15.f);
	ColorsGroup.PlaceLabledControl("CT ESP - NonVisible R", this, &CTColorNoVisR);

	CTColorNoVisG.SetFileId("ct_color_no_vis_g");
	CTColorNoVisG.SetBoundaries(0.f, 255.f);
	CTColorNoVisG.SetValue(110.f);
	ColorsGroup.PlaceLabledControl("CT ESP - NonVisible G", this, &CTColorNoVisG);

	CTColorNoVisB.SetFileId("ct_color_no_vis_b");
	CTColorNoVisB.SetBoundaries(0.f, 255.f);
	CTColorNoVisB.SetValue(220.f);
	ColorsGroup.PlaceLabledControl("CT ESP - NonVisible B", this, &CTColorNoVisB);

	TColorVisR.SetFileId("t_color_vis_r");
	TColorVisR.SetBoundaries(0.f, 255.f);
	TColorVisR.SetValue(235.f);
	ColorsGroup.PlaceLabledControl("T ESP - Visible R", this, &TColorVisR);

	TColorVisG.SetFileId("t_color_vis_g");
	TColorVisG.SetBoundaries(0.f, 255.f);
	TColorVisG.SetValue(200.f);
	ColorsGroup.PlaceLabledControl("T ESP - Visible G", this, &TColorVisG);

	TColorVisB.SetFileId("t_color_vis_b");
	TColorVisB.SetBoundaries(0.f, 255.f);
	TColorVisB.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("T ESP - Visible B", this, &TColorVisB);


	TColorNoVisR.SetFileId("t_color_no_vis_r");
	TColorNoVisR.SetBoundaries(0.f, 255.f);
	TColorNoVisR.SetValue(235.f);
	ColorsGroup.PlaceLabledControl("T ESP - NonVisible R", this, &TColorNoVisR);

	TColorNoVisG.SetFileId("t_color_no_vis_g");
	TColorNoVisG.SetBoundaries(0.f, 255.f);
	TColorNoVisG.SetValue(50.f);
	ColorsGroup.PlaceLabledControl("T ESP - NonVisible G", this, &TColorNoVisG);

	TColorNoVisB.SetFileId("t_color_no_vis_b");
	TColorNoVisB.SetBoundaries(0.f, 255.f);
	TColorNoVisB.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("T ESP - NonVisible B", this, &TColorNoVisB);

	GlowR.SetFileId("GlowR");
	GlowR.SetBoundaries(0.f, 255.f);
	GlowR.SetValue(255.f);
	ColorsGroup.PlaceLabledControl("Glow (R)", this, &GlowR);

	GlowG.SetFileId("GlowG");
	GlowG.SetBoundaries(0.f, 255.f);
	GlowG.SetValue(255.f);
	ColorsGroup.PlaceLabledControl("Glow (G)", this, &GlowG);

	GlowB.SetFileId("GlowB");
	GlowB.SetBoundaries(0.f, 255.f);
	GlowB.SetValue(255.f);
	ColorsGroup.PlaceLabledControl("Glow (B)", this, &GlowB);

	ConfigGroup.SetPosition(408, 48);
	ConfigGroup.SetText("Config");
	ConfigGroup.SetSize(360, 500);
	RegisterControl(&ConfigGroup);

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Legit");
	ConfigBox.AddItem("Ultra-Legit");
	ConfigBox.AddItem("Closet");
	ConfigBox.AddItem("Non-Prime");
	ConfigBox.AddItem("Rage");
	ConfigBox.AddItem("MM-HvH");
	ConfigBox.AddItem("NoSpread-HvH");
	ConfigBox.AddItem("Extra");
	ConfigGroup.PlaceLabledControl("Selected Config :", this, &ConfigBox);



#pragma endregion Visual Colors
}

void CSkinchangerTab::Setup()
{
	SetTitle("e");

	SkinActive.SetPosition(16, 16);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(66, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 490);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma region Knife
	KnifeGroup.SetPosition(16, 48);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(376, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408, 48);
	MachinegunsGroup.SetText("Machineguns");
	MachinegunsGroup.SetSize(360, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("VariCamo");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("CaliCamo");
	NEGEVSkin.AddItem("Terrain");
	NEGEVSkin.AddItem("Army Sheen");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("Contrast Spray");
	M249Skin.AddItem("Blizzard Marbleized");
	M249Skin.AddItem("Jungle DDPAT");
	M249Skin.AddItem("Gator Mesh");
	M249Skin.AddItem("Magma");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16, 135);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(376, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408, 135);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(360, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Metallic DDPAT");
	MAG7Skin.AddItem("Silver");
	MAG7Skin.AddItem("Storm");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	MAG7Skin.AddItem("Sand Dune");
	MAG7Skin.AddItem("Irradiated Alert");
	MAG7Skin.AddItem("Memento");
	MAG7Skin.AddItem("Hazard");
	MAG7Skin.AddItem("Heaven Guard");
	MAG7Skin.AddItem("Firestarter");
	MAG7Skin.AddItem("Seabird");
	MAG7Skin.AddItem("Cobalt Core");
	MAG7Skin.AddItem("Praetorian");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Blaze Orange");
	XM1014Skin.AddItem("VariCamo Blue");
	XM1014Skin.AddItem("Bone Mask");
	XM1014Skin.AddItem("Blue Steel");
	XM1014Skin.AddItem("Blue Spruce");
	XM1014Skin.AddItem("Grassland");
	XM1014Skin.AddItem("Urban Perforated");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("Fallout Warning");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("CaliCamo");
	XM1014Skin.AddItem("Pit Viper");
	XM1014Skin.AddItem("Tranquility");
	XM1014Skin.AddItem("Red Python");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Red Leather");
	XM1014Skin.AddItem("Bone Machine");
	XM1014Skin.AddItem("Quicksilver");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Black Tie");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("First Class");
	SAWEDOFFSkin.AddItem("Forest DDPAT");
	SAWEDOFFSkin.AddItem("Contrast Spray");
	SAWEDOFFSkin.AddItem("Snake Camo");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("Copper");
	SAWEDOFFSkin.AddItem("Origami");
	SAWEDOFFSkin.AddItem("Sage Spray");
	SAWEDOFFSkin.AddItem("VariCamo");
	SAWEDOFFSkin.AddItem("Irradiated Alert");
	SAWEDOFFSkin.AddItem("Mosaico");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Amber Fade");
	SAWEDOFFSkin.AddItem("Full Stop");
	SAWEDOFFSkin.AddItem("Highwayman");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Rust Coat");
	SAWEDOFFSkin.AddItem("Bamboo Shadow");
	SAWEDOFFSkin.AddItem("Bamboo Forest");
	SAWEDOFFSkin.AddItem("Yorick");
	SAWEDOFFSkin.AddItem("Fubar");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16, 270);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(376, 195);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG552", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16, 475);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(376, 165);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Tornado");
	MAC10Skin.AddItem("Candy Apple");
	MAC10Skin.AddItem("Silver");
	MAC10Skin.AddItem("Forest DDPAT");
	MAC10Skin.AddItem("Urban DDPAT");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Leather");
	P90Skin.AddItem("Virus");
	P90Skin.AddItem("Contrast Spray");
	P90Skin.AddItem("Storm");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Glacier Mesh");
	P90Skin.AddItem("Sand Spray");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Ash Wood");
	P90Skin.AddItem("Fallout Warning");
	P90Skin.AddItem("Scorched");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Teardown");
	P90Skin.AddItem("Blind Spot");
	P90Skin.AddItem("Trigon");
	P90Skin.AddItem("Desert Warfare");
	P90Skin.AddItem("Module");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Elite Build");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Shallow Grave");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Forest DDPAT");
	UMP45Skin.AddItem("Gunsmoke");
	UMP45Skin.AddItem("Urban DDPAT");
	UMP45Skin.AddItem("Grand Prix");
	UMP45Skin.AddItem("Carbon Fiber");
	UMP45Skin.AddItem("Caramel");
	UMP45Skin.AddItem("Fallout Warning");
	UMP45Skin.AddItem("Scorched");
	UMP45Skin.AddItem("Bone Pile");
	UMP45Skin.AddItem("Delusion");
	UMP45Skin.AddItem("Corporal");
	UMP45Skin.AddItem("Indigo");
	UMP45Skin.AddItem("Labyrinth");
	UMP45Skin.AddItem("Minotaur's Labyrinth");
	UMP45Skin.AddItem("Riot");
	UMP45Skin.AddItem("Primal Saber");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Modern Hunter");
	BIZONSkin.AddItem("Forest Leaves");
	BIZONSkin.AddItem("Bone Mask");
	BIZONSkin.AddItem("Carbon Fiber");
	BIZONSkin.AddItem("Sand Dashed");
	BIZONSkin.AddItem("Urban Dashed");
	BIZONSkin.AddItem("Brass");
	BIZONSkin.AddItem("VariCamo");
	BIZONSkin.AddItem("Irradiated Alert");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Water Sigil");
	BIZONSkin.AddItem("Night Ops");
	BIZONSkin.AddItem("Cobalt Halftone");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Chemical Green");
	BIZONSkin.AddItem("Bamboo Print");
	BIZONSkin.AddItem("Bamboo Forest");
	BIZONSkin.AddItem("Fuel Rod");
	BIZONSkin.AddItem("Photic Zone");
	BIZONSkin.AddItem("Judgement of Anubis");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Forest DDPAT");
	MP7Skin.AddItem("Anodized Navy");
	MP7Skin.AddItem("Skulls");
	MP7Skin.AddItem("Gunsmoke");
	MP7Skin.AddItem("Contrast Spray");
	MP7Skin.AddItem("Bone Mask");
	MP7Skin.AddItem("Ossified");
	MP7Skin.AddItem("Orange Peel");
	MP7Skin.AddItem("VariCamo");
	MP7Skin.AddItem("Army Recon");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Ocean Foam");
	MP7Skin.AddItem("Full Stop");
	MP7Skin.AddItem("Urban Hazard");
	MP7Skin.AddItem("Olive Plaid");
	MP7Skin.AddItem("Armor Core");
	MP7Skin.AddItem("Asterion");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Special Delivery");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Bone Mask");
	MP9Skin.AddItem("Hot Rod");
	MP9Skin.AddItem("Storm");
	MP9Skin.AddItem("Bulldozer");
	MP9Skin.AddItem("Hypnotic");
	MP9Skin.AddItem("Sand Dashed");
	MP9Skin.AddItem("Orange Peel");
	MP9Skin.AddItem("Dry Season");
	MP9Skin.AddItem("Dark Age");
	MP9Skin.AddItem("Rose Iron");
	MP9Skin.AddItem("Green Plaid");
	MP9Skin.AddItem("Setting Sun");
	MP9Skin.AddItem("Dart");
	MP9Skin.AddItem("Deadly Poison");
	MP9Skin.AddItem("Pandora's Box");
	MP9Skin.AddItem("Bioleak");
	MP9Skin.AddItem("Airlock");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408, 270);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(360, 215);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Ossified");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Red Quartz");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion

#pragma region Skinsettings
	SkinsettingsGroup.SetPosition(408, 500);
//	SkinsettingsGroup.SetText("Settings");
	SkinsettingsGroup.SetSize(360, 140);
	RegisterControl(&SkinsettingsGroup);

//	StatTrakEnable.SetFileId("skin_stattrack");
//	SkinsettingsGroup.PlaceLabledControl("Stat Track", this, &StatTrakEnable);

//	StatTrackAmount.SetFileId("skin_stamount");
//	SkinsettingsGroup.PlaceLabledControl("Value", this, &StatTrackAmount);

	KnifeName.SetFileId("knife_name");
	SkinsettingsGroup.PlaceLabledControl("Knife Name", this, &KnifeName);

	SkinName.SetFileId("skin_name");
	SkinsettingsGroup.PlaceLabledControl("Skin Name", this, &SkinName);

#pragma endregion


#pragma endregion other random options
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	GUI.Update();
	GUI.Draw();


}