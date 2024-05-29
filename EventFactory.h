#pragma once

#include "GameEvent.h"
#include "Monster.h"
#include "Door.h"

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
        //case EventType::ITEM: {
        //    const ItemEventDescriptor& itemDesc = static_cast<const ItemEventDescriptor&>(descriptor);
        //    return new ItemEvent(itemDesc.itemID);
        //}
        //case EventType::TERRAIN: {
        //    const TerrainEventDescriptor& terrainDesc = static_cast<const TerrainEventDescriptor&>(descriptor);
        //    return new TerrainEvent(terrainDesc.terrainEffect);
        //}
        default:
            return nullptr;
        }
    }
};