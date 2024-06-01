#pragma once

#include "GameEvent.h"
#include "Monster.h"
#include "Door.h"
#include "Item.h"
#include "Stair.h"

class EventFactory {
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
        //case EventType::TERRAIN: 
        //{
        //    const TerrainEventDescriptor& terrainDesc = static_cast<const TerrainEventDescriptor&>(descriptor);
        //    return std::make_shared<Stair>(terrainDesc.coord, terrainDesc.stairType, terrainDesc.eventName);
        //}
        default:
            return nullptr;
        }
    }
};