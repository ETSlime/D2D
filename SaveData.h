#pragma once

#include <fstream>
#include <codecvt>

#include "Player.h"
#include "Util.h"
#include "MapStatic.h"


struct GameState 
{
    // player status
    PlayerData playerData;
    Coord playerCoord;
    UINT walkingSteps;
    UINT curFloor;
    float totalTime;
    std::map<ItemID, UINT> inventoryItems;

    // events
    std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>> eventParams;

    // maps
    std::unordered_map<int, std::unordered_map<Coord, UINT>> mapTileIdx;

    // time
    std::wstring timestamp;

    GameState() 
    { 
        curFloor = 0;
        walkingSteps = 0;
        totalTime = 0;
        playerData.atk = 0;
        playerData.def = 0;
        playerData.exp = 0;
        playerData.gold = 0;
        playerData.HP = 0;
        playerData.level = 0;
        playerData.mdf = 0;
        playerData.MP = 0;
        timestamp = L""; 
    };
    
};

enum class EventTypeIdentifier 
{
    MonsterParams,
    ItemParams,
    DoorParams,
    StairParams,
    NPCParams
};

class SaveData
{
public:

    static int SaveGame(const std::string& filename);
    static int LoadGame(const std::string& filename, GameState& gameState);
    static bool LoadPreviewData(UINT slotNum, float& totalTime, std::wstring& timestamp);

private:

    static GameState GetGameState();

    // Function to get current system time as a formatted string
    static std::wstring GetCurrentTimeString();

    // Function to write data as hex
    template <typename T>
    static void WriteHex(std::ofstream& ofs, const T& value);

    // Function to read data from hex
    template <typename T>
    static void ReadHex(std::ifstream& ifs, T& value);

    // serialize and deserialize timestamp
    static void SerializeTimestamp(std::ofstream& ofs, const std::wstring& timestamp);
    static void DeserializeTimestamp(std::ifstream& ifs, std::wstring& timestamp);

    // serialize and deserialize inventory item map
    static void SerializeInventoryItemMap(std::ofstream& ofs, const std::map<ItemID, UINT>& inventoryItems);
    static void DeserializeInventoryItemMap(std::ifstream& ifs, std::map<ItemID, UINT>& inventoryItems);

    // serialize and deserialize EventType, including derived types
    static void SerializeEventType(std::ofstream& ofs, const EventType& type);
    static void DeserializeEventType(std::ifstream& ifs, EventType& type);

    // serialize and deserialize Coord
    static void SerializeCoord(std::ofstream& ofs, const Coord& coord);
    static void DeserializeCoord(std::ifstream& ifs, Coord& coord);

    // serialize and deserialize EventParams
    static void SerializeEventParams(std::ofstream& ofs, const EventParams& params);
    static std::unique_ptr<EventParams> DeserializeEventParams(std::ifstream& ifs);

    // serialize and deserialize EventParams unordered_map
    static void SerializeEventParamsMap(std::ofstream& ofs, const std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>>& map);
    static void DeserializeEventParamsMap(std::ifstream& ifs, std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>>& map);

    // serialize and deserialize tile maps
    static void SerializeMapTileIdx(std::ofstream& ofs, const std::unordered_map<int, std::unordered_map<Coord, UINT>>& mapTileIdx);
    static void DeserializeMapTileIdx(std::ifstream& ifs, std::unordered_map<int, std::unordered_map<Coord, UINT>>& mapTileIdx);

    // Function to convert wide string to string
    static std::string WideStringToString(const std::wstring& wstr);
    // Function to convert string to wide string
    static std::wstring StringToWideString(const std::string& str);

};