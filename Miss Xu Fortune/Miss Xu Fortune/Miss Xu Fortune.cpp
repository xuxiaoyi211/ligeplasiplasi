#include "PluginSDK.h"
#include <map>
#include <string>

//Q2,semiR,R cant stop, and casue error

PluginSetup("Miss Xu Fortune");

IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* JungleMenu;
IMenu* MiscMenu;
IMenu* Drawings;
IMenu* ItemsMenu;
IMenu* PotionMenu;
IMenu* TrinketMenu;

IMenuOption* ComboQ;
IMenuOption* ComboQ2;
IMenuOption* ComboW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* SafeR;
IMenuOption* Blade_Cutlass;
IMenuOption* Ghost_Blade;

IMenuOption* RRange;

IMenuOption* HarassQ;
IMenuOption* HarassQ2;
IMenuOption* HarassW;
IMenuOption* HarassE;
IMenuOption* HarassManaPercent;

IMenuOption* FarmQ;
IMenuOption* FarmW;
IMenuOption* FarmE;
IMenuOption* FarmManaPercent;
IMenuOption* lasthitQ;

IMenuOption* KillstealQ;
IMenuOption* KillstealE;
IMenuOption* KillstealR;
IMenuOption* AutoR;
IMenuOption* AutoRNumber;
IMenuOption* AutoQSS;
IMenuOption* AutoHeal;
IMenuOption* MyHeal;
IMenuOption* MyHpPreBlade;
IMenuOption* EnemyHpPreBlade;

IMenuOption* usepotion;
IMenuOption* usepotionhpper;

IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawQ2;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawR;
IMenuOption* UseTrinket;

//Items
IInventoryItem* Ghostblade;
IInventoryItem* blade;
IInventoryItem* Cutlass;
IInventoryItem* HealthPot;
IInventoryItem* CorruptPot;
IInventoryItem* Biscuit;
IInventoryItem* RefillPot;
IInventoryItem* hunter;
IInventoryItem* Mersci;
IInventoryItem* QuickSiv;
IInventoryItem* Trinket;
IInventoryItem* Yellow;

IUnit* Player;
IUnit* Target;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

ISpell* Heal;

std::map<int, IMenuOption*> ChampionuseTrinket;

static bool IsKeyDown(IMenuOption* menuOption)
{
	return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0;
}

void  Menu()
{
	//Main
	MainMenu = GPluginSDK->AddMenu("Miss Xu Fortune");

	//Combo
	ComboMenu = MainMenu->AddMenu("Combo Settings");
	ComboQ = ComboMenu->CheckBox("Use Q", true);
	ComboQ2 = ComboMenu->CheckBox("Try Bounce Q To Enemy", true);
	ComboW = ComboMenu->CheckBox("Use W", true);
	ComboE = ComboMenu->CheckBox("Use E", true);
	ComboR = ComboMenu->CheckBox("Use R", true);
	SafeR = ComboMenu->AddInteger("Safe Range To R In Combo", 0, 1000, 600);

	//Harass
	HarassMenu = MainMenu->AddMenu("Harass Setting");
	HarassQ = HarassMenu->CheckBox("Use Q", true);
	HarassQ2 = HarassMenu->CheckBox("Bounce Q To Enemy", true);
	HarassW = HarassMenu->CheckBox("Use W", true);
	HarassE = HarassMenu->CheckBox("Use E", true);
	HarassManaPercent = HarassMenu->AddInteger("Mana Percent for harass", 10, 100, 45);

	//Farm
	FarmMenu = MainMenu->AddMenu("Farm Setting");
	FarmQ = FarmMenu->CheckBox("Use Q Farm", true);
	FarmW = FarmMenu->CheckBox("Use W Farm", false);
	FarmE = FarmMenu->CheckBox("Use E Farm", false);
	FarmManaPercent = FarmMenu->AddInteger("Mana Percent for Farm", 10, 100, 45);
	lasthitQ = FarmMenu->CheckBox("Q Lasthit", true);

	//Misc
	MiscMenu = MainMenu->AddMenu("Misc Setting");
	KillstealQ = MiscMenu->CheckBox("Use Q to killsteal", true);
	KillstealE = MiscMenu->CheckBox("Use E to killsteal", true);
	KillstealR = MiscMenu->CheckBox("Use R to killsteal", true);
	AutoRNumber = MiscMenu->AddInteger("Auto R X Enemies", 0, 5, 3);
	AutoQSS = MiscMenu->CheckBox("Auto QSS", true);
	AutoHeal = MiscMenu->CheckBox("Auto Heal", true);
	MyHeal = MiscMenu->AddInteger("Use Heal When My Hp <", 10, 100, 35);
	AutoR = MiscMenu->AddKey("Semi R", 84);


	//Items
	ItemsMenu = MainMenu->AddMenu("Items Setting");
	Blade_Cutlass = ItemsMenu->CheckBox("Blade-Cutlass", true);
	Ghost_Blade = ItemsMenu->CheckBox("Ghost-Blade", true);
	MyHpPreBlade = ItemsMenu->AddInteger("Use Blade-Cutlass if my HP <", 10, 100, 35);
	EnemyHpPreBlade = ItemsMenu->AddInteger("Use Blade-Cutlass if Enemy HP <", 10, 100, 35);

	//Potions
	PotionMenu = MainMenu->AddMenu("Potion Setting");
	usepotion = PotionMenu->CheckBox("Use Potions", true);
	usepotionhpper = PotionMenu->AddInteger("Use Potions if HP <", 10, 100, 75);

	//Drawings
	Drawings = MainMenu->AddMenu("Drawings");
	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);
	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawQ2 = Drawings->CheckBox("Draw Q2", true);
	DrawE = Drawings->CheckBox("Draw E", false);
	DrawR = Drawings->CheckBox("Draw R", true);

	//Trinkets
	TrinketMenu = MainMenu->AddMenu("Auto Trinket");
	UseTrinket = TrinketMenu->CheckBox("Enable Combo Trinket", true);
	for (auto Enemys : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Use Trinket on - " + std::string(Enemys->ChampionName());
		ChampionuseTrinket[Enemys->GetNetworkId()] = TrinketMenu->CheckBox(szMenuName.c_str(), false);
	}
}

//Declear Spells && Items
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, true, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
	W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
	E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithNothing));
	R = GPluginSDK->CreateSpell2(kSlotR, kConeCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));

	Q->SetSkillshot(0.25f, 25.f, 1000.f, 550.f);
	W->SetSkillshot(0.25f, 0.f, 1000.f, 0.f);
	E->SetSkillshot(0.35f, 200.f, 3200.f, 1000.f);
	R->SetSkillshot(0.25f, 0.f, 1000.f, 1400.f);

	//SS
	auto slot1 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot1);
	auto slot2 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot2);

	if (strcmp(slot1, "SummonerHeal") == 0) { Heal = GPluginSDK->CreateSpell(kSummonerSlot1, 850); }
	if (strcmp(slot2, "SummonerHeal") == 0) { Heal = GPluginSDK->CreateSpell(kSummonerSlot2, 850); }

	//Items
	Ghostblade = GPluginSDK->CreateItemForId(3142, 0);
	blade = GPluginSDK->CreateItemForId(3153, 550);
	Cutlass = GPluginSDK->CreateItemForId(3144, 550);
	Mersci = GPluginSDK->CreateItemForId(3140, 550);
	QuickSiv = GPluginSDK->CreateItemForId(3137, 0);
	HealthPot = GPluginSDK->CreateItemForId(2003, 0);
	CorruptPot = GPluginSDK->CreateItemForId(2033, 0);
	RefillPot = GPluginSDK->CreateItemForId(2031, 0);
	Biscuit = GPluginSDK->CreateItemForId(2010, 0);
	hunter = GPluginSDK->CreateItemForId(2032, 0);
	Trinket = GPluginSDK->CreateItemForId(3363, 4000);
	Yellow = GPluginSDK->CreateItemForId(3340, 600);
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

//Excute Combo Logic
void Combo()
{
	/*if (ComboQ->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto Enemy = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy2 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);
		for (auto minion : GEntityList->GetAllMinions(false, true, true))
			if (minion != nullptr && !minion->IsDead())
		{
			if (Q->IsReady() && ComboQ2->Enabled() && minion->IsValidTarget(Player, Q->Range()))
			{
				for (auto Enemy : GEntityList->GetAllHeros(false, true))
				{
					//Sometimes it cast Q to random minion
					if ((GEntityList->Player()->GetPosition() - Enemy->GetPosition()).Length2D() <= Q->Range() && (Enemy->GetPosition() - Enemy2->GetPosition()).Length2D() <= 250 && minion->IsValidTarget(GEntityList->Player(), Q->Range()))
					{
						Q->CastOnTarget(minion, 5);
					}
				}
			}
			else
			{
				if (Q->IsReady() && Enemy->IsValidTarget(Player, Q->Range()))
				{
					Q->CastOnTarget(Enemy);
				}
			}
		}
	}*/

	if (ComboQ->Enabled() && Q->IsReady() && ComboQ2->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);
		for (auto Enemy : GEntityList->GetAllHeros(false, true));
		for (auto minion : GEntityList->GetAllMinions(false, true, true));
		{
			if (minion != nullptr && Enemy != nullptr)
			{
				if ((GEntityList->Player()->GetPosition() - minion->GetPosition()).Length2D() <= Q->Range() && 250 >= (minion->GetPosition() - Enemy->GetPosition()).Length2D() <= (GEntityList->Player()->GetPosition() - Enemy->GetPosition()).Length2D() && Enemy->IsValidTarget(GEntityList->Player(), Q->Range()) && GPluginSDK->GetDamage()->GetSpellDamage(GEntityList->Player(), Enemy, kSlotQ) > minion->GetHealth())
				{
					Q->LastHitMinion();
				}
			}
		}
	}
	/*if (ComboQ->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy1 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy2 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);

		if (minion != nullptr && !minion->IsDead())
		{
			if (Q->IsReady() && ComboQ2->Enabled() && minion->IsValidTarget(Player, Q->Range()))
			{
				for (auto Enemy1 : GEntityList->GetAllHeros(false, true))
					for (auto Enemy2 : GEntityList->GetAllHeros(false, true))
					{
						if ((GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() <= Q->Range() && (Enemy1->GetPosition() - Enemy2->GetPosition()).Length2D() <= 250 > (GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() && Enemy1->IsValidTarget(GEntityList->Player(), Q->Range()))
						{
							Q->CastOnTarget(minion, 5);
						}
					}
			}
			else
			{
				if (Q->IsReady() && Enemy1->IsValidTarget(Player, Q->Range()))
				{
					Q->CastOnTarget(Enemy1);
				}
			}
		}
	}*/

	if (ComboW->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, 575);
		if (target != nullptr && !target->IsDead() && W->IsReady() && target->IsValidTarget(Player, 575))
		{
			W->CastOnPlayer();
		}
	}

	if (ComboE->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, E->Range());
		if (target != nullptr && !target->IsDead() && E->IsReady() && target->IsValidTarget(Player, E->Range()))
		{
			E->CastOnTarget(target);
		}
	}

	if (ComboR->Enabled())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());

		if (target != nullptr && !target->IsValidTarget(Player, R->Range()) && Player->HasBuff("MissFortuneBulletSound"))
		{
			GGame->IssueOrder(Player, kMoveTo, GGame->CursorPosition());
		}
		else
		{
			if (!target->IsDead() && R->IsReady() && target->IsValidTarget(Player, R->Range()) && !target->IsValidTarget(Player, SafeR->GetInteger()))
			{
				R->CastOnTarget(target);
			}
		}
	}
}

void Harass()
{
	if (Player->ManaPercent() < HarassManaPercent->GetInteger())
		return;
	if (HarassQ->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy1 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy2 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);


		if (Q->IsReady() && HarassQ2->Enabled() && minion->IsValidTarget(Player, Q->Range()))
		{
			for (auto Enemy1 : GEntityList->GetAllHeros(false, true))
				for (auto Enemy2 : GEntityList->GetAllHeros(false, true))
				{
					if ((GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() <= Q->Range() && (Enemy1->GetPosition() - Enemy2->GetPosition()).Length2D() <= 250 > (GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() && Enemy1->IsValidTarget(GEntityList->Player(), Q->Range()))
					{
						Q->CastOnTarget(minion, 5);
					}
				}
		}
		else
		{
			if (Q->IsReady() && Enemy1->IsValidTarget(Player, Q->Range()))
			{
				Q->CastOnTarget(Enemy1);
			}
		}
	}


	if (HarassW->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, 575);
		if (target != nullptr && !target->IsDead() && W->IsReady() && target->IsValidTarget(Player, 575))
		{
			W->CastOnPlayer();
		}
	}


	if (HarassE->Enabled() && !Player->HasBuff("MissFortuneBulletSound"))
	{
		auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, E->Range());
		if (target != nullptr && !target->IsDead() && E->IsReady() && target->IsValidTarget(Player, E->Range()))
		{
			E->CastOnTarget(target);
		}
	}

}

void Farm()
{
	int MinionAround = 0;
	for (auto minions : GEntityList->GetAllMinions(false, true, true))
	{
		if (minions->IsValidTarget(Player, Q->Range()))
			MinionAround++;
		if (Player->ManaPercent() < FarmManaPercent->GetInteger())
			return;
		if (minions != nullptr && FarmQ->Enabled() && Q->IsReady())
		{
			if (MinionAround > 2)
				Q->CastOnUnit(minions);
		}
		if (minions != nullptr && FarmW->Enabled() && W->IsReady())
		{
			if (MinionAround > 3)
				W->CastOnPlayer();
		}
		if (minions != nullptr && FarmE->Enabled() && E->IsReady())
		{
			if (MinionAround > 3)
				E->CastOnUnit(minions);
		}
	}

	/*if (HarassQ->Enabled())
	{
		auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, Q->Range());

		if (target != nullptr && !target->IsDead() && target->IsHero())
		{
			if (Q->IsReady() && HarassQ2->Enabled() && target->IsValidTarget(Player, Q->Range()))
			{
				MinQ2();
			}
			else
			{
				if (Q->IsReady() && target->IsValidTarget(Player, Q->Range()))
				{
					Q->CastOnTarget(target);
				}
			}
		}
	}*/
}

void QLast()
{
		for (auto minions : GEntityList->GetAllMinions(false, true, false))
		{
			if (minions != nullptr && Player->IsValidTarget(minions, Q->Range()))
			{
				auto dmg = GDamage->GetSpellDamage(Player, minions, kSlotQ);
				if (lasthitQ->Enabled() && Q->IsReady() && minions->GetHealth() <= dmg)
				{
					Q->CastOnUnit(minions);
				}
			}
		}
}

void KS()
{
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		auto QDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotQ);
		auto EDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotE);
		auto RDamage = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotR);
		if (Enemy != nullptr && !Enemy->IsDead())
		{
			if (KillstealE->Enabled() && E->IsReady() && Enemy->IsValidTarget(GEntityList->Player(), E->Range()) && EDamage >= Enemy->GetHealth())
			{
				E->CastOnTarget(Enemy);
			}
		}

		if (Enemy != nullptr && !Enemy->IsDead())
		{
			if (KillstealQ->Enabled() && Q->IsReady() && Enemy->IsValidTarget(GEntityList->Player(), Q->Range()) && QDamage >= Enemy->GetHealth())
			{
				Q->CastOnTarget(Enemy);
			}
		}

		if (Enemy != nullptr && !Enemy->IsDead())
		{
			if (KillstealR->Enabled() && R->IsReady() && Enemy->IsValidTarget(GEntityList->Player(), R->Range()) && RDamage >= Enemy->GetHealth())
			{
				R->CastOnTarget(Enemy);
			}
		}
	}
}

void SemiR()
{
	if (IsKeyDown(AutoR) && GEntityList->Player()->GetSpellState(kSlotR) == Ready)
	{
		if (!Player->HasBuff("MissFortuneBulletSound"))
		{
			GGame->IssueOrder(Player, kMoveTo, GGame->CursorPosition());
		}
		
		auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
		{
			auto target = Enemy;
			if (target != nullptr && !target->IsInvulnerable())
			{
				R->CastOnTarget(target, 5);
			}
		}
	}
}

void AutoQss()
{
	auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	if (AutoQSS->Enabled() && Player->IsValidTarget(Enemy, 650))
	{
		if (Player->HasBuffOfType(BUFF_Snare) || Player->HasBuffOfType(BUFF_Taunt)
			|| Player->HasBuffOfType(BUFF_Suppression) || Player->HasBuffOfType(BUFF_Stun) || Player->HasBuffOfType(BUFF_Knockup)
			|| Player->HasBuffOfType(BUFF_Suppression) || Player->HasBuffOfType(BUFF_Charm) || Player->HasBuffOfType(BUFF_Polymorph)
			|| Player->HasBuffOfType(BUFF_Blind) || Player->HasBuffOfType(BUFF_Fear))
		{
			if (Mersci->IsOwned() && Mersci->IsReady())
				Mersci->CastOnPlayer();
			if (QuickSiv->IsOwned() && QuickSiv->IsReady())
				QuickSiv->CastOnPlayer();
		}
	}
}

void UseItems()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
			if (Blade_Cutlass->Enabled() && Player->IsValidTarget(enemy, 650))
			{
				if (Player->HealthPercent() < MyHpPreBlade->GetInteger() || enemy->HealthPercent() < EnemyHpPreBlade->GetInteger())
				{
					if (blade->IsOwned() && blade->IsReady())
						blade->CastOnTarget(enemy);
					if (Cutlass->IsOwned() && Cutlass->IsReady())
						Cutlass->CastOnTarget(enemy);
				}
			}

		for (auto enemy : GEntityList->GetAllHeros(false, true))
			if (Ghost_Blade->Enabled() && Player->IsValidTarget(enemy, 550))
			{
				if (Ghostblade->IsOwned() && Ghostblade->IsReady())
				{
					Ghostblade->CastOnPlayer();
				}
			}
	}
}


void AHeal()
{
	auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	if (AutoHeal->Enabled() && Player->IsValidTarget(Enemy, 650) && Player->HealthPercent() < MyHeal->GetInteger())
	{
		if (Heal->IsReady())
		{
			Heal->CastOnPlayer();
		}
	}
}

//RyTakAIO
void Q2()
{
	{
		auto minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy1 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
		auto Enemy2 = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);
		for (auto Enemy1 : GEntityList->GetAllHeros(false, true))
			for (auto Enemy2 : GEntityList->GetAllHeros(false, true))
			{
				if ((GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() <= Q->Range() && (Enemy1->GetPosition() - Enemy2->GetPosition()).Length2D() <= 250 > (GEntityList->Player()->GetPosition() - Enemy1->GetPosition()).Length2D() && Enemy1->IsValidTarget(GEntityList->Player(), Q->Range()))
				{
					Q->CastOnTarget(minion, 5);
				}
			}
	}
}
//RyTakAIO
void MinQ2()
{
	auto minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
	auto Enemy = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
		for (auto minion : GEntityList->GetAllMinions(false, true, true))
		{
			if ((GEntityList->Player()->GetPosition() - minion->GetPosition()).Length2D() <= Q->Range() && (minion->GetPosition() - Enemy->GetPosition()).Length2D() <= 250 > (GEntityList->Player()->GetPosition() - Enemy->GetPosition()).Length2D() && Enemy->IsValidTarget(GEntityList->Player(), Q->Range()))
			{
				Q->CastOnTarget(minion, 5);
			}
		}
}

void Usepotion()
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

void trinket()
{
	if (UseTrinket->Enabled() && (Trinket->IsOwned() && Trinket->IsReady() || Yellow->IsOwned() && Yellow->IsReady()))
	{
		auto Enemys = GEntityList->GetAllHeros(false, true);

		for (auto target : Enemys)
		{
			auto lastPos = target->GetPosition();
			if (!target->IsVisible())
			{
				if (ChampionuseTrinket[target->GetNetworkId()]->Enabled() && (GEntityList->Player()->GetPosition() - lastPos).Length2D() <= RRange->GetInteger())
				{
					Trinket->CastOnPosition(lastPos) || Yellow->CastOnPosition(lastPos);
				}
			}
		}
	}
}

PLUGIN_EVENT(void) OnRender()
{
	auto target = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, Q->Range());

	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
		
		if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }


		if (target != nullptr && Q->IsReady() && DrawQ2->Enabled() && target->IsValidTarget(Player, Q->Range()))
		{
			auto Enemy = GTargetSelector->FindTarget(ClosestToCursorPriority, PhysicalDamage, Q->Range());
			for (auto minion : GEntityList->GetAllMinions(false, true, true))
			{
				//minion = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range());
				Enemy = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, Q->Range() + 250);
				{
					{
						if ((GEntityList->Player()->GetPosition() - minion->GetPosition()).Length2D() <= Q->Range() && 250 >= (minion->GetPosition() - Enemy->GetPosition()).Length2D() <= (GEntityList->Player()->GetPosition() - Enemy->GetPosition()).Length2D())
						{
							//AMBER <3
							Vec3 From = GEntityList->Player()->GetPosition(); // My Position
							Vec3 To = minion->GetPosition(); // Target Position
							Vec3 Vector = To - (From - To).VectorNormalize() * 500; // Extend the Vector to 250range
																					// Now we'll make the two point for the triange
							Vec3 A = Vector + (Vector - To).VectorNormalize().Perpendicular() * (-100);
							Vec3 B = Vector + (Vector - To).VectorNormalize().Perpendicular() * (100);
							Vec2 C1; Vec2 A1; Vec2 B1; GGame->Projection(To, &C1); GGame->Projection(A, &A1); GGame->Projection(B, &B1);
							// Now we have 3 points, just draw 3 lines
							GRender->DrawLine(C1, A1, Vec4(255, 255, 255, 255));
							GRender->DrawLine(C1, B1, Vec4(255, 255, 255, 255));
							GRender->DrawLine(A1, B1, Vec4(255, 255, 255, 255));
						}
					}
				}
			}
		}
	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

		if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

		if (target != nullptr && DrawQ2->Enabled() && target->IsValidTarget(Player, Q->Range()))
		{
			Vec3 From = GEntityList->Player()->GetPosition(); // My Position
			Vec3 To = target->GetPosition(); // Target Position
			Vec3 Vector = To - (From - To).VectorNormalize() * 250; // Extend the Vector to 250range
																	// Now we'll make the two point for the triange
			Vec3 A = Vector + (Vector - To).VectorNormalize().Perpendicular() * (-100);
			Vec3 B = Vector + (Vector - To).VectorNormalize().Perpendicular() * (100);
			Vec2 C1; Vec2 A1; Vec2 B1; GGame->Projection(To, &C1); GGame->Projection(A, &A1); GGame->Projection(B, &B1);
			// Now we have 3 points, just draw 3 lines
			GRender->DrawLine(C1, A1, Vec4(255, 255, 255, 255));
			GRender->DrawLine(C1, B1, Vec4(255, 255, 255, 255));
			GRender->DrawLine(A1, B1, Vec4(255, 255, 255, 255));
		}
	}
}


PLUGIN_EVENT(void) OnGameUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
		//trinket();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLastHit)
	{
		QLast();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		Harass();
		QLast();

	}
	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		Farm();
	}
	KS();
	/*Usepotion();
	UseItems();
	AutoQss();*/
	SemiR();
	AHeal();

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
	LoadSpells();
	Player = GEntityList->Player();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);



}


PLUGIN_API void OnUnload()
{
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GGame->PrintChat("Good Bye <3.Plugin Now Unloaded");
}