#pragma once

#include "GameEvent.h"
#include "Monster.h"
#include "Door.h"
#include "Item.h"
#include "Stair.h"
#include "GameNPC.h"
#include "Arrow.h"
#include "GeneralEvent.h"
#include "Terrain.h"

class EventFactory 
{
public:
    static std::shared_ptr<GameEvent> CreateGameEvent(const EventDescriptor& descriptor) 
    {
        switch (descriptor.eventType) 
        {
        case EventType::MONSTER: 
        {
            const BattleEventDescriptor& battleDesc = static_cast<const BattleEventDescriptor&>(descriptor);
            return std::make_shared<Monster>(battleDesc.coord, battleDesc.monsterID, battleDesc.eventName);
        }
        case EventType::DOOR:
        {
            const DoorEventDescriptor& doorDesc = static_cast<const DoorEventDescriptor&>(descriptor);
            return std::make_shared<Door>(doorDesc.coord, doorDesc.doorType, doorDesc.eventName);
        }
        case EventType::ITEM:
        {
            const ItemEventDescriptor& itemDesc = static_cast<const ItemEventDescriptor&>(descriptor);
            return std::make_shared<Item>(itemDesc.coord, itemDesc.itemID, itemDesc.eventName);
        }
        case EventType::STAIR: 
        {
            const StairEventDescriptor& stairDesc = static_cast<const StairEventDescriptor&>(descriptor);
            return std::make_shared<Stair>(stairDesc.coord, stairDesc.stairType, stairDesc.newPlayerCoord, stairDesc.eventName);
        }
        case EventType::NPC:
        {
            const NPCEventDescriptor& NPCDesc = static_cast<const NPCEventDescriptor&>(descriptor);
            return std::make_shared<GameNPC>(NPCDesc.coord, NPCDesc.NPCID, NPCDesc.dialogueID, NPCDesc.eventName);
        }
        case EventType::ARROW:
        {
            const ArrowEventDescriptor& ArrowDesc = static_cast<const ArrowEventDescriptor&>(descriptor);
            return std::make_shared<Arrow>(ArrowDesc.coord, ArrowDesc.arrowDir, ArrowDesc.eventName);
        }
        case EventType::DEFAULT:
        {
            const GeneralEventDescriptor& generalEventDesc = static_cast<const GeneralEventDescriptor&>(descriptor);
            return std::make_shared<GeneralEvent>(generalEventDesc.coord, generalEventDesc.triggerID, 
                generalEventDesc.colliderType, generalEventDesc.triggerOnce, generalEventDesc.eventName);
        }
        case EventType::TERRAIN: 
        {
            const TerrainEventDescriptor& terrainDesc = static_cast<const TerrainEventDescriptor&>(descriptor);
            return std::make_shared<Terrain>(terrainDesc.coord, terrainDesc.terrainType, terrainDesc.eventName);
        }
        default:
            return nullptr;
        }
    }
};