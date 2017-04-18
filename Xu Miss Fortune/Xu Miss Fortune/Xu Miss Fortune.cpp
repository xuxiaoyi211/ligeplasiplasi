#include "PluginSDK.h"
#include <map>
#include <string>


PluginSetup("Xu Miss Fortune");

IMenu* MainMenu;
IMenu* PotionMenu;

IMenuOption* usepotion;
IMenuOption* usepotionhpper;

IInventoryItem* HealthPot;
IInventoryItem* CorruptPot;
IInventoryItem* Biscuit;
IInventoryItem* RefillPot;
IInventoryItem* hunter;

IUnit* Player;
IUnit* Target;

void  Menu()
{
	//Main
	MainMenu = GPluginSDK->AddMenu("Miss Xu Fortune");


	//Potions
	PotionMenu = MainMenu->AddMenu("Potion Setting");
	usepotion = PotionMenu->CheckBox("Use Potions", true);
	usepotionhpper = PotionMenu->AddInteger("Use Potions if HP <", 10, 100, 75);
}

//Fountain Buff
static bool InFountain(IUnit *unit)
{
	return unit->HasBuff("kappachino");
}

//Count Enemys In Range
int EnemiesInRange(IUnit* Source, float range)
{
	auto Targets = GEntityList->GetAllHeros(false, true);
	auto enemiesInRange = 0;

	for (auto target : Targets)
	{
		if (target != nullptr)
		{
			auto flDistance = (target->GetPosition() - Source->GetPosition()).Length();
			if (flDistance < range)
			{
				enemiesInRange++;
			}
		}
	}
	return enemiesInRange;
}


//Calculate Distence Between Player and Target
float GetDistance(IUnit* Player, IUnit* target)
{
	return (Player->GetPosition() - target->GetPosition()).Length2D();
}

//Count Not ally heros in range
int CountEnemiesInRange(float range)
{
	int enemies = 0;
	for (auto enemy : GEntityList->GetAllHeros(false, true))
	{
		if (enemy != nullptr && GetDistance(GEntityList->Player(), enemy) <= range)
		{
			enemies++;
		}
	}
	return enemies;
}

void Usepotion2()
{
	if (usepotion->Enabled() && !Player->IsRecalling() && !Player->IsDead())
	{
		if (CountEnemiesInRange(2000) > 0)
		{
			bool usepotions = Player->GetHealth() < Player->GetMaxHealth()* usepotionhpper->GetInteger() / 100;
			if (usepotions)
			{
				if (Player->GetBuffDataByName("ItemDarkCrystalFlask") || Player->GetBuffDataByName("ItemMiniRegenPotion")
					|| Player->GetBuffDataByName("ItemCrystalFlask") || Player->GetBuffDataByName("RegenerationPotion") || Player->HasBuff("ItemCrystalFlaskJungle"))
					return;

				if (Biscuit->IsOwned() && !InFountain(Player) && Biscuit->IsReady())
				{
					Biscuit->CastOnPlayer();
				}
				else if (HealthPot->IsOwned() && !InFountain(Player) && HealthPot->IsReady())
				{
					HealthPot->CastOnPlayer();
				}
				else if (CorruptPot->IsOwned() && CorruptPot->IsReady())
				{
					CorruptPot->CastOnPlayer();
				}
				else if (RefillPot->IsOwned() && RefillPot->IsReady())
				{
					RefillPot->CastOnPlayer();
				}
				else if (hunter->IsOwned() && hunter->IsReady())
				{
					hunter->CastOnPlayer();
				}
			}
		}
	}
}



PLUGIN_EVENT(void) OnGameUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLastHit)
	{
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{

	}
	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
	}
	Usepotion2();

}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	std::string playerHero = GEntityList->Player()->ChampionName();

	if (playerHero == "MissFortune")
	{
		GGame->PrintChat("<b><font color = \"#F535AA\">Miss Xu Fortune</font><font color=\"#4EE2EC\"> - Loaded</font></b>");
	}

	Menu();
	Player = GEntityList->Player();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);



}


PLUGIN_API void OnUnload()
{
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GGame->PrintChat("Good Bye <3.Plugin Now Unloaded");
}