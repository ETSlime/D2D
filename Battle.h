#pragma once

#include "Util.h"
#include "Player.h"
#include "Database.h"

class Battle
{
public:
	static int CalculateDamage(MonsterData monsterData)
	{
        PlayerData playerData = Player::player->GetBattleData();

        if (playerData.atk <= monsterData.defense)
            return -1;

        int monsterDamagePerHit = monsterData.attack - playerData.def;
        if (monsterDamagePerHit < 0)
        {
            monsterDamagePerHit = 0;
        }

        int hits = monsterData.HP / max(1, (playerData.atk - monsterData.defense));
        int totalDamage = hits * monsterDamagePerHit;

        return totalDamage;
	}
};