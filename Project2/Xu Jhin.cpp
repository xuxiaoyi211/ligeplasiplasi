#include "PluginSDK.h"
#include <map>
//auto heal, auto trinket
//no q, W fix,auto w?,r no use,last hit q , q hit range + 300 then q target,flee,w gap close aa work?,heal problem

//3/23 lc, jg, heal, last hit q,r,harass w no target tick still use w

//3/24 heal issue
PluginSetup("Xu Jhin");

IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* JungleMenu;
IMenu* MiscMenu;
IMenu* FleeMenu;
IMenu* Drawings;
IMenu* ItemsMenu;
IMenu* PotionMenu;
IMenuOption* SemiR;
IMenuOption* UseIgnitecombo;
IMenuOption* ComboQ;
IMenuOption* AutoEGapcloser;
IMenuOption* AutoWGapcloser;
IMenuOption* ComboW;
IMenuOption* ComboWcc;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* RRange;
IMenuOption* HarassQ;
IMenuOption* HarassW;
IMenuOption* HarassWcc;
IMenuOption* HarassManaPercent;
IMenuOption* FarmQ;
IMenuOption* FarmW;
IMenuOption* FarmE;
IMenuOption* lasthitQ;
IMenuOption* FarmManaPercent;
IMenuOption* JungleQ;
IMenuOption* JungleW;
IMenuOption* JungleE;
IMenuOption* JungleManaPercent;
IMenuOption* UseIgnitekillsteal;
IMenuOption* KillstealQ;
IMenuOption* KillstealW;
IMenuOption* ImmobileW;
IMenuOption* ImmobileE;
IMenuOption* FleeKey;
IMenuOption* Blade_Cutlass;
IMenuOption* Ghost_Blade;
IMenuOption* AutoQSS;
IMenuOption* AutoHeal;
IMenuOption* MyHeal;
IMenuOption* MyHpPreBlade;
IMenuOption* EnemyHpPreBlade;
IMenuOption* usepotion;
IMenuOption* usepotionhpper;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawR;

IUnit* Player;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

ISpell* Ignite;
ISpell* Heal;

IInventoryItem* Tear;
IInventoryItem* Manamune;
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

//std::map<int, IMenuOption*> ChampionuseQ;
std::map<int, IMenuOption*> ChampionuseW;
std::map<int, IMenuOption*> ChampionuseW2;

static bool IsKeyDown(IMenuOption* menuOption)
{
	return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0;
}
bool semiR = false;

void  Menu()
{
	MainMenu = GPluginSDK->AddMenu("Xu Jhin");

	ComboMenu = MainMenu->AddMenu("Combo Settings");
	UseIgnitecombo = ComboMenu->CheckBox("Use Ignite", true);
	ComboQ = ComboMenu->CheckBox("Use Q", true);
	ComboW = ComboMenu->CheckBox("Use W", true);
	ComboWcc = ComboMenu->CheckBox("Only Use W On Spotted Target", true);
	for (auto Enemys : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Use W On - " + std::string(Enemys->ChampionName());
		ChampionuseW[Enemys->GetNetworkId()] = ComboMenu->CheckBox(szMenuName.c_str(), false);
	}
	ComboE = ComboMenu->CheckBox("Use E", true);
	ComboR = ComboMenu->AddKey("R Fire Key", 84);
	RRange = ComboMenu->AddInteger("R Search Range", 0, 3500, 3500);

	HarassMenu = MainMenu->AddMenu("Harass Setting");
	HarassQ = HarassMenu->CheckBox("Use Q", true);
	HarassW = HarassMenu->CheckBox("Use W", true);
	for (auto Enemys : GEntityList->GetAllHeros(false, true))
	{
		std::string szMenuName = "Use W on - " + std::string(Enemys->ChampionName());
		ChampionuseW2[Enemys->GetNetworkId()] = HarassMenu->CheckBox(szMenuName.c_str(), false);
	}
	HarassWcc = HarassMenu->CheckBox("Only Use W On Spotted Target", true);
	HarassManaPercent = HarassMenu->AddInteger("Mana Percent for harass", 10, 100, 70);
	Drawings = MainMenu->AddMenu("Drawings");

	FarmMenu = MainMenu->AddMenu("LaneClear Setting");
	FarmQ = FarmMenu->CheckBox("Use Q Farm", true);
	FarmW = FarmMenu->CheckBox("Use W Farm", false);
	FarmE = FarmMenu->CheckBox("Use E Farm", true);
	lasthitQ = FarmMenu->CheckBox("Try Use Q Lasthit", true);
	FarmManaPercent = FarmMenu->AddInteger("Mana Percent for Farm", 10, 100, 70);

	JungleMenu = MainMenu->AddMenu("Jungle Setting");
	JungleQ = JungleMenu->CheckBox("Use Q Jungle", true);
	JungleW = JungleMenu->CheckBox("Use W Jungle", false);
	JungleE = JungleMenu->CheckBox("Use E Jungle", true);
	JungleManaPercent = JungleMenu->AddInteger("Mana Percent for Farm", 10, 100, 70);

	MiscMenu = MainMenu->AddMenu("Misc Setting");
	//UseIgnitekillsteal = MiscMenu->CheckBox("Use Ignite to killsteal", false);
	KillstealQ = MiscMenu->CheckBox("Use Q to killsteal", true);
	KillstealW = MiscMenu->CheckBox("Use W to killsteal", true);
	ImmobileW = MiscMenu->CheckBox("Use W in Immobile", true);
	ImmobileE = MiscMenu->CheckBox("Use E in Immobile", true);
	AutoEGapcloser = MiscMenu->CheckBox("Use E to Gapclose", true);
	AutoWGapcloser = MiscMenu->CheckBox("Use W to Gapclose", true);
	AutoQSS = MiscMenu->CheckBox("Auto QSS", true);
	AutoHeal = MiscMenu->CheckBox("Auto Heal", true);
	MyHeal = MiscMenu->AddInteger("Use Heal When My Hp <", 10, 100, 35);
	//AutoHealT = MiscMenu->CheckBox("Auto Heal Teammate", true);

	FleeMenu = MainMenu->AddMenu("Flee");
	FleeKey = FleeMenu->AddKey("Flee Key", 74);

	ItemsMenu = MainMenu->AddMenu("Items Setting");
	Blade_Cutlass = ItemsMenu->CheckBox("Blade-Cutlass", true);
	Ghost_Blade = ItemsMenu->CheckBox("Blade-Cutlass", true);
	MyHpPreBlade = ItemsMenu->AddInteger("Use Blade-Cutlass if my HP <", 10, 100, 35);
	EnemyHpPreBlade = ItemsMenu->AddInteger("Use Blade-Cutlass if Enemy HP <", 10, 100, 35);

	PotionMenu = MainMenu->AddMenu("Potion Setting");
	usepotion = PotionMenu->CheckBox("Use potions", true);
	usepotionhpper = PotionMenu->AddInteger("Use potions if HP <", 10, 100, 35);

	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);
	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", false);
	DrawE = Drawings->CheckBox("Draw E", false);
	DrawR = Drawings->CheckBox("Draw R", true);
}
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, true, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall| kCollidesWithHeroes));
	E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithNothing));
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));

	Q->SetOverrideRange(600);
	W->SetOverrideRange(2500);
	E->SetOverrideRange(750);
	R->SetOverrideRange(3500);

	W->SetOverrideDelay(0.75);
	E->SetOverrideDelay(0.25);
	R->SetOverrideDelay(0.24);

	W->SetOverrideRadius(40);
	E->SetOverrideRadius(120);
	R->SetOverrideRadius(80);

	Q->SetOverrideSpeed(2000);
	W->SetOverrideSpeed(10000);
	E->SetOverrideSpeed(1600);
	R->SetOverrideSpeed(5000);

	auto slot1 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot1);
	auto slot2 = GPluginSDK->GetEntityList()->Player()->GetSpellName(kSummonerSlot2);

	if (strcmp(slot1, "SummonerDot") == 0)
	{
		Ignite = GPluginSDK->CreateSpell(kSummonerSlot1, 600);
	}
	if (strcmp(slot2, "SummonerDot") == 0)
	{
		Ignite = GPluginSDK->CreateSpell(kSummonerSlot2, 600);
	}
	else Ignite == nullptr;

	if (strcmp(slot1, "SummonerDot") == 0)
	{
		Heal = GPluginSDK->CreateSpell(kSummonerSlot1, 850);
	}
	if (strcmp(slot2, "SummonerDot") == 0)
	{
		Heal = GPluginSDK->CreateSpell(kSummonerSlot2, 850);
	}
	else Heal == nullptr;

	Tear = GPluginSDK->CreateItemForId(3070, 0);
	Manamune = GPluginSDK->CreateItemForId(3004, 0);
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
}

static bool InFountain(IUnit *unit)
{
	//TODO: Implement
	return unit->HasBuff("kappachino");
}

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

float GetDistance(IUnit* Player, IUnit* target)
{
	return (Player->GetPosition() - target->GetPosition()).Length2D();
}

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

/*void CastQ(IUnit* target)
{
	AdvPredictionOutput prediction_output;
	Q->RunPrediction(target + 300, true, kCollidesWithYasuoWall, &prediction_output);
	if (prediction_output.HitChance >= kHitChanceHigh)
	{
		Q->CastOnTarget(target, kHitChanceCollision);
	}
}*/
void Combo()
{
	if (Ignite != nullptr)
	{
		auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
		if (UseIgnitecombo->Enabled() && Ignite->IsReady())
		{
			if (Enemy != nullptr && Enemy->IsValidTarget(Player, 570))
			{
				if (Enemy->HealthPercent() <= 30)
				{
					Ignite->CastOnUnit(Enemy);
				}
			}
		}
	}

	if (ComboQ->Enabled())
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
			if (Player->IsValidTarget(target, Q->Range()))
			{
				Q->CastOnUnit(target);
				GGame->PrintChat("cast q");
			}
		}
	}
	if (ComboW->Enabled())
	{
		if (W->IsReady())
		{
			for (auto Enemys : GEntityList->GetAllHeros(false, true))
			{
				auto target = Enemys;
				if (Player->IsValidTarget(target, W->Range()))
				{
					AdvPredictionOutput prediction_output;
					W->RunPrediction(target, true, kCollidesWithYasuoWall | kCollidesWithHeroes, &prediction_output);
					if (ChampionuseW[Enemys->GetNetworkId()]->Enabled() && ComboWcc->Enabled() && !target->IsInvulnerable() && prediction_output.HitChance >= kHitChanceHigh)
					{
						if (target->HasBuff("jhinespotteddebuff"))
							W->CastOnTarget(target, kHitChanceHigh);
						GGame->PrintChat("w buff");
					}
					if (ChampionuseW[Enemys->GetNetworkId()]->Enabled() && !ComboWcc->Enabled() && !target->IsInvulnerable() && prediction_output.HitChance >= kHitChanceHigh)
					{
						if (target->HasBuff("jhinespotteddebuff"))
							W->CastOnTarget(target, kHitChanceHigh);
						GGame->PrintChat("choose target, no buff w");
					}
				}
			}
		}
	}

	/*if (ComboW->Enabled())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
		AdvPredictionOutput prediction_output;
		W->RunPrediction(target, true, kCollidesWithYasuoWall | kCollidesWithHeroes, &prediction_output);
		if (!target->IsInvulnerable() && prediction_output.HitChance >= kHitChanceHigh)
		{
			W->CastOnTarget(target, kHitChanceHigh);
		}
	}*/

	if (ComboE->Enabled() && E->IsReady())
	{
		if (Player->GetMana() > Q->ManaCost() + W->ManaCost())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
			if (Player->IsValidTarget(target, E->Range()) && target != nullptr)
			{
				AdvPredictionOutput prediction_output;
				E->RunPrediction(target, true, kCollidesWithNothing, &prediction_output);
				if (!target->IsInvulnerable() && prediction_output.HitChance >= kHitChanceHigh)
				{
					E->CastOnTarget(target);
					GGame->PrintChat("cast e");
				}
			}
		}
	}

	if (ComboR->Enabled() && R->IsReady())
	{
		for (auto Enemy : GEntityList->GetAllHeros(false, true))
		{
			if (Enemy != nullptr && !Enemy->IsDead())
			{
				auto dmg = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotR);
				if (Player->IsValidTarget(Enemy, R->Range()) && !Enemy->IsInvulnerable()
					&& !Player->IsValidTarget(Enemy, Q->Range()))
				{
					if (Enemy->GetHealth() <= dmg && R->IsReady())
					{
						R->CastOnTarget(Enemy, kHitChanceHigh);
						GGame->PrintChat("cast r");
					}
				}
			}
		}
	}
}
void lasthitq()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear || GOrbwalking->GetOrbwalkingMode() == kModeLastHit)
	{
		for (auto minions : GEntityList->GetAllMinions(false, true, false))
		{
			if (minions != nullptr && Player->IsValidTarget(minions, Q->Range()))
			{
				auto dmg = GDamage->GetSpellDamage(Player, minions, kSlotQ);
				if (lasthitQ->Enabled() && Q->IsReady() && minions->GetHealth() <= dmg)
				{
					Q->CastOnUnit(minions);
					GGame->PrintChat("last hit q");
				}
			}
		}
	}
}

/*bool FleeJ()
{
	if (!IsKeyDown(FleeKey))
	{
		return false;
	}
	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
}*/

PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
{

		
}
void Harass()
{
	if (Player->ManaPercent() < HarassManaPercent->GetInteger())
		return;
	if (HarassQ->Enabled())
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
			if (Player->IsValidTarget(target, Q->Range()))
				Q->CastOnTarget(target, kHitChanceHigh);
		}
	}
	if (HarassW->Enabled())
	{
		if (W->IsReady())
		{
			for (auto Enemys : GEntityList->GetAllHeros(false, true))
			{
				auto target = Enemys;
				if (Player->IsValidTarget(target, W->Range()))
				{
					if (ChampionuseW2[Enemys->GetNetworkId()]->Enabled() && HarassWcc->Enabled() && !target->IsInvulnerable())
					{
						if (target->HasBuff("jhinespotteddebuff"))
						W->CastOnTarget(target, kHitChanceHigh);
						GGame->PrintChat("harass w buff");
					}
					if (ChampionuseW2[Enemys->GetNetworkId()]->Enabled() && !HarassWcc->Enabled() && !target->IsInvulnerable())
					{
						W->CastOnTarget(target, kHitChanceHigh);
						GGame->PrintChat("harass w");
					}
				}
			}
		}
	}
}

void laneclear()
{
	int MinionAround = 0;
	for (auto minions : GEntityList->GetAllMinions(false, true, false))
	{
		if (minions->IsValidTarget(Player, Q->Range()))
			MinionAround++;
		if (Player->ManaPercent() < FarmManaPercent->GetInteger())
			return;
		if (minions != nullptr && FarmQ->Enabled() && Q->IsReady())
		{
			if (MinionAround > 3)
				Q->CastOnUnit(minions);
			GGame->PrintChat("lc q 3");
		}
	}
	int MinionDie = 0;
	for (auto minions : GEntityList->GetAllMinions(false, true, false))
	{
		if (minions != nullptr && Player->IsValidTarget(minions, Q->Range()))
		{
			auto dmg = GDamage->GetSpellDamage(Player, minions, kSlotQ);
			auto dmg1 = GDamage->GetAutoAttackDamage(Player, minions, true);
			if (minions->GetHealth() <= dmg || minions->GetHealth() <= dmg1 || minions->GetHealth() <= dmg1 + dmg)
				MinionDie++;

			if (Player->ManaPercent() < FarmManaPercent->GetInteger())
				return;
			if (Q->IsReady() && lasthitQ->Enabled())
			{
				if (MinionDie > 1)
					Q->CastOnUnit(minions) || Q->LastHitMinion();
				GGame->PrintChat("lc q | ls q");
			}

			if (FarmW->Enabled() && W->IsReady())
			{
				if (Player->IsValidTarget(minions, W->Range()));
				W->CastOnTarget(minions) || W->LastHitMinion();
				GGame->PrintChat("lc  w");
			}
			if (FarmE->Enabled() && E->IsReady())
			{
				if (Player->IsValidTarget(minions, E->Range()));
				E->CastOnUnit(minions) || E->LastHitMinion();
				GGame->PrintChat("lc  e");
			}
		}
	}
}

void jungleclear()
{
	if (JungleQ->Enabled() && Q->IsReady())
	{
		for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
		{
			if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, Q->Range()))
			{
				if (Player->ManaPercent() < JungleManaPercent->GetInteger())
					return;
				Q->CastOnUnit(jMinion);
				GGame->PrintChat("jg q");
			}
		}
	}
	if (JungleW->Enabled() && W->IsReady())
	{
		for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
		{
			if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, W->Range()))
			{
				if (Player->ManaPercent() < JungleManaPercent->GetInteger())
					return;
				W->CastOnTarget(jMinion);
				GGame->PrintChat("jg  w");
			}
		}
	}
	if (JungleE->Enabled() && E->IsReady())
	{
		for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
		{
			if (jMinion != nullptr && !jMinion->IsDead() && Player->IsValidTarget(jMinion, E->Range()))
			{
				if (Player->ManaPercent() < JungleManaPercent->GetInteger())
					return;
				E->CastOnTarget(jMinion);
				GGame->PrintChat("jg  e");
			}
		}
	}
}

void AutoImmobile()
{
	if (GGame->IsChatOpen()) return;
	if (W->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
		if (target != nullptr && Player->IsValidTarget(target, W->Range()) && !target->IsInvulnerable())
		{
			if (ImmobileW->Enabled() && target->HasBuff("jhinespotteddebuff") || target->HasBuffOfType(BUFF_Snare)|| target->HasBuffOfType(BUFF_Taunt)
				|| target->HasBuffOfType(BUFF_Suppression)|| target->HasBuffOfType(BUFF_Stun)|| target->HasBuffOfType(BUFF_Knockup)
				|| target->HasBuffOfType(BUFF_Suppression)|| target->HasBuffOfType(BUFF_Charm))
			{
				if (GPrediction->GetCollisionFlagsForPoint(target->GetPosition()) == 0)
				{
					W->CastOnTarget(target, kHitChanceImmobile);
					GGame->PrintChat("imbobile w");
				}
			}
		}
	}
	if (E->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
		if (target != nullptr && Player->IsValidTarget(target, E->Range()) && !target->IsInvulnerable())
		{
			if (ImmobileE->Enabled())
			{
				{
					E->CastOnTarget(target, kHitChanceImmobile);
					GGame->PrintChat("imbobile e");
				}
			}
		}
	}
}
void killsteal()
{
	if (GGame->IsChatOpen()) return;
	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		if (Enemy != nullptr && !Enemy->IsDead())
		{
			if (KillstealQ->Enabled())
			{
				auto dmg = GDamage->GetSpellDamage(GEntityList->Player(), Enemy, kSlotQ);
				if (Player->IsValidTarget(Enemy, Q->Range()) && !Enemy->IsInvulnerable())
				{
					if (Enemy->GetHealth() <= dmg && Q->IsReady())
					{
						Q->CastOnTarget(Enemy);
					}
				}
			}
			if (KillstealW->Enabled())
			{
				auto dmg = GDamage->GetSpellDamage(Player, Enemy, kSlotW);
				if (Player->IsValidTarget(Enemy, W->Range()) && !Enemy->IsInvulnerable())
				{
					if (Enemy->GetHealth() <= dmg && W->IsReady())
					{
						W->CastOnTarget(Enemy, kHitChanceHigh);
					}
				}
			}
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

void AHeal()
{
	if (AutoHeal->Enabled() && !Player->IsRecalling() && EnemiesInRange(Player, 550) && Player->HealthPercent() < MyHeal->GetInteger())
	{
		Heal->CastOnPlayer();
	}
}

/*void Flee()
{
	if (!FleeJ(Flee))
		return;
	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
	auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	if (Ghost_Blade->Enabled() && Player->IsValidTarget(target, 550) && Ghostblade->IsOwned() && Ghostblade->IsReady)
	{
		Ghostblade->CastOnPlayer();
	}
}*/

PLUGIN_EVENT(void) OnRender()
{
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

		if (E->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

		if (R->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), RRange->GetInteger()); }

	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

		if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), RRange->GetInteger()); }
	}
}

/*void SkinChanger()
{
if (myHero->GetSkinId() != ChangeSkin->GetInteger())
{
myHero->SetSkinId(ChangeSkin->GetInteger());
}
}*/

PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& args)
{
	if (args.Sender != nullptr && args.Sender != Player && Player->IsValidTarget(args.Sender, W->Range())
		&& W->IsReady() && args.Sender->IsEnemy(Player) && AutoWGapcloser->Enabled()
		&& (Player->GetPosition() - args.EndPosition).Length() < 250)
	{
		if (!Player->HasBuff("JhinRShot") && args.Sender->HasBuff("jhinespotteddebuff"))
		{
			W->CastOnPosition(args.EndPosition);
			GGame->PrintChat("Gap w");
		}
		if (!Player->HasBuff("JhinRShot") && !args.Sender->HasBuff("jhinespotteddebuff") && GOrbwalking->CanAttack())
		{
			GGame->IssueOrder(Player, kAttackUnit, args.Sender);
			GGame->PrintChat("Gap aa");
			W->CastOnPosition(args.EndPosition);
			GGame->PrintChat("Gap aaw");
		}

	}

	if (args.Sender != nullptr && args.Sender != Player && Player->IsValidTarget(args.Sender, E->Range())
		&& E->IsReady() && args.Sender->IsEnemy(Player) && AutoEGapcloser->Enabled()
		&& (Player->GetPosition() - args.EndPosition).Length() < 250)
	{
		E->CastOnPosition(args.EndPosition);
		GGame->PrintChat("Gap E");
	}

	auto Enemy = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
	if (Player->IsValidTarget(Enemy, E->Range()) && Enemy->IsChasing(Player) && Enemy->IsDashing() && Enemy != nullptr)
	{
		E->CastOnTarget(Enemy);
		GGame->PrintChat("Gap dashing E");
	}
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLastHit)
	{
		lasthitq();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		Harass();
		lasthitq();

	}
	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		laneclear();
		jungleclear();
	}
	AutoImmobile();
	killsteal();
	UseItems();
	Usepotion();
	AutoQss();
	AHeal();
	if (IsKeyDown(ComboR) && ComboR->Enabled() && GEntityList->Player()->GetSpellState(kSlotR) == Ready)
	{
		IUnit *rTarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, RRange->GetInteger());
		if (rTarget != nullptr && rTarget && !Player->IsValidTarget(rTarget, Q->Range()))
		{
			R->CastOnTarget(rTarget, 5);
		}
	}

/*	if (FleeJ())
	{
		return;
	}*/
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);
	Menu();
	LoadSpells();
	Player = GEntityList->Player();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
	if (strcmp(GEntityList->Player()->ChampionName(), "Jhin") == 0)
	{
		GGame->PrintChat("<b><font color = \"#f8a101\">Xiao Jhin</font><font color=\"#7FFF00\"> - Loaded</font></b>");
	}
	else
	{
		GGame->PrintChat("This plugin is for Jhin");
	}
}


PLUGIN_API void OnUnload()
{
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
}