/*
 * Aspire Hearthstone
 * Copyright (C) 2008 AspireDev <http://www.aspiredev.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define AB_BUFF_RESPAWN_TIME 90000

enum ABBuffs
{
	AB_BUFF_STABLES						= 0,
	AB_BUFF_BLACKSMITH					= 1,
	AB_BUFF_FARM						= 2,
	AB_BUFF_LUMBERMILL					= 3,
	AB_BUFF_MINE						= 4,
	AB_NUM_BUFFS						= 5,
};

enum ABControlPoints
{
	AB_CONTROL_POINT_STABLE				= 0,
	AB_CONTROL_POINT_FARM				= 1,
	AB_CONTROL_POINT_BLACKSMITH			= 2,
	AB_CONTROL_POINT_MINE				= 3,
	AB_CONTROL_POINT_LUMBERMILL			= 4,
	AB_NUM_CONTROL_POINTS				= 5,
};

enum ABSpawnTypes
{
	AB_SPAWN_TYPE_NEUTRAL				= 0,
	AB_SPAWN_TYPE_ALLIANCE_ASSAULT		= 1,
	AB_SPAWN_TYPE_HORDE_ASSAULT			= 2,
	AB_SPAWN_TYPE_ALLIANCE_CONTROLLED	= 3,
	AB_SPAWN_TYPE_HORDE_CONTROLLED		= 4,
	AB_NUM_SPAWN_TYPES					= 5,
};

class ArathiBasin : public CBattleground
{
public:
	shared_ptr<GameObject> m_buffs[AB_NUM_BUFFS];
	shared_ptr<GameObject> m_controlPoints[AB_NUM_CONTROL_POINTS];
	shared_ptr<GameObject> m_controlPointAuras[AB_NUM_CONTROL_POINTS];
	bool m_nearingVictory[2];

protected:
	list<shared_ptr<GameObject>> m_gates;
	
	uint32 m_resources[2];
	uint32 m_capturedBases[2];
	uint32 m_lastHonorGainResources[2];
	int32 m_basesLastOwnedBy[AB_NUM_CONTROL_POINTS];
	int32 m_basesOwnedBy[AB_NUM_CONTROL_POINTS];
	int32 m_basesAssaultedBy[AB_NUM_CONTROL_POINTS];
	bool m_flagIsVirgin[AB_NUM_CONTROL_POINTS];
	CreaturePointer m_spiritGuides[AB_NUM_CONTROL_POINTS];
	uint32 m_lgroup;

public:
	ArathiBasin(shared_ptr<MapMgr> mgr, uint32 id, uint32 lgroup, uint32 t);
	~ArathiBasin();
	virtual void Init();

	void HookOnPlayerDeath(PlayerPointer plr);
	void HookFlagDrop(PlayerPointer plr, shared_ptr<GameObject> obj);
	void HookFlagStand(PlayerPointer plr, shared_ptr<GameObject> obj);
	void HookOnMount(PlayerPointer plr);
	void HookOnAreaTrigger(PlayerPointer plr, uint32 id);
	bool HookHandleRepop(PlayerPointer plr);
	void OnAddPlayer(PlayerPointer plr);
	void OnRemovePlayer(PlayerPointer plr);
	void OnCreate();
	void HookOnPlayerKill(PlayerPointer plr, UnitPointer pVictim);
	void HookOnHK(PlayerPointer plr);
	void HookOnShadowSight();
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	void DropFlag(PlayerPointer plr);

	static shared_ptr<CBattleground> Create(shared_ptr<MapMgr> m, uint32 i, uint32 l, uint32 t) { return shared_ptr<ArathiBasin>(new ArathiBasin(m, i, l, t)); }

	const char * GetName() { return "Arathi Basin"; }
	void OnStart();

	void EventUpdateResources(uint32 Team);
	bool HookSlowLockOpen(shared_ptr<GameObject> pGo, PlayerPointer pPlayer, shared_ptr<Spell>pSpell);

	/* AB Game Mechanics */
	void SpawnControlPoint(uint32 Id, uint32 Type);
	void CaptureControlPoint(uint32 Id, uint32 Team);
	void AssaultControlPoint(PlayerPointer pPlayer, uint32 Id);

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(shared_ptr<Player>plr, shared_ptr<Corpse>pCorpse);

	void SetIsWeekend(bool isweekend);
};
