#include "SaveData.h"
#include <iomanip>  // For std::hex and std::setw
#include <filesystem> // For save file path
#include "String.h"

GameState SaveData::GetGameState()
{
    GameState gameState{};
    gameState.playerData = Player::player->GetBattleData();
    gameState.playerCoord = Player::player->GetCoord();
    gameState.walkingSteps = Player::player->GetWalkingSteps();
    gameState.inventoryItems = Player::player->GetItems();
    gameState.visitedFloor = Player::player->GetVisitedFloor();
    gameState.curFloor = Player::player->GetCurFloor();

    gameState.mapTileIdx = MapStatic::mapTileIdx;
    gameState.eventParams = std::move(MapStatic::eventParams);

    gameState.totalTime = Timer::TotalTime();

    std::wstring timestamp = GetCurrentTimeString(); // Set the current time
    gameState.timestamp = timestamp;

    return gameState;
}

int SaveData::SaveGame(const std::string& filename)
{
    GameState state = GetGameState();

    std::string path = SavePath + filename;
    // Check if folder exists, if not create it
    if (!std::filesystem::exists(SavePath))
    {
        std::filesystem::create_directory(SavePath);
    }
    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        return -1;
    }


    // serialize total time
    WriteHex(ofs, state.totalTime);

    // serialize timestamp
    SerializeTimestamp(ofs, state.timestamp);

    // serialize base data types

    WriteHex(ofs, state.playerData.level);
    WriteHex(ofs, state.playerData.HP);
    WriteHex(ofs, state.playerData.MP);
    WriteHex(ofs, state.playerData.atk);
    WriteHex(ofs, state.playerData.def);
    WriteHex(ofs, state.playerData.mdf);
    WriteHex(ofs, state.playerData.exp);
    WriteHex(ofs, state.playerData.gold);

    WriteHex(ofs, state.walkingSteps);
    WriteHex(ofs, state.curFloor);

    // serialize Coord
    SerializeCoord(ofs, state.playerCoord);

    // serialize hash map
    // item
    SerializeInventoryItemMap(ofs, state.inventoryItems);

    // visited floor
    SerializeVisitedFloor(ofs, state.visitedFloor);

    // event params
    SerializeEventParamsMap(ofs, state.eventParams);

    // map tiles
    SerializeMapTileIdx(ofs, state.mapTileIdx);

    MapStatic::eventParams = std::move(state.eventParams);

    ofs.close();

    return 0;
}

int SaveData::LoadGame(const std::string& filename, GameState& gameState)
{
    std::ifstream ifs(SavePath + filename);
    if (!ifs.is_open()) 
    {
        return -1;
    }


    // deserialize total time
    ReadHex(ifs, gameState.totalTime);

    // deserialize timestamp
    DeserializeTimestamp(ifs, gameState.timestamp);

    // deserialize base data types
    ReadHex(ifs, gameState.playerData.level);
    ReadHex(ifs, gameState.playerData.HP);
    ReadHex(ifs, gameState.playerData.MP);
    ReadHex(ifs, gameState.playerData.atk);
    ReadHex(ifs, gameState.playerData.def);
    ReadHex(ifs, gameState.playerData.mdf);
    ReadHex(ifs, gameState.playerData.exp);
    ReadHex(ifs, gameState.playerData.gold);

    ReadHex(ifs, gameState.walkingSteps);
    ReadHex(ifs, gameState.curFloor);

    // deserialize Coord
    DeserializeCoord(ifs, gameState.playerCoord);

    // deserialize hash map
    // item
    DeserializeInventoryItemMap(ifs, gameState.inventoryItems);

    // visited floor
    DeserializeVisitedFloor(ifs, gameState.visitedFloor);

    // event params
    DeserializeEventParamsMap(ifs, gameState.eventParams);

    // map tiles
    DeserializeMapTileIdx(ifs, gameState.mapTileIdx);


    ifs.close();

    return 0;
}

bool SaveData::LoadPreviewData(UINT slotNum, float& totalTime, std::wstring& timestamp)
{
    char buffer[16]; // Enough to hold "SaveData" + 3 digits + ".dat" + null terminator
    std::snprintf(buffer, sizeof(buffer), "SaveData%03d.dat", slotNum);
    std::string filename(buffer);

    std::string filepath = SavePath + filename;

    // Check if file exists
    if (!std::filesystem::exists(filepath))
    {
        return false;
    }

    std::ifstream ifs(filepath);
    if (!ifs.is_open()) 
    {
        return false;
    }

    // Read file content
    ReadHex(ifs, totalTime);
    DeserializeTimestamp(ifs, timestamp);


    ifs.close();
    return true;
}

std::wstring SaveData::GetCurrentTimeString()
{
    std::time_t now = std::time(nullptr);
    std::tm now_tm;
    localtime_s(&now_tm, &now);
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M");
    return String::ToWstring(oss.str());
}

void SaveData::SerializeTimestamp(std::ofstream& ofs, const std::wstring& timestamp)
{
    size_t timestampSize = timestamp.size();
    WriteHex(ofs, timestampSize);
    for (char c : timestamp)
    {
        WriteHex(ofs, c);
    }
}

void SaveData::DeserializeTimestamp(std::ifstream& ifs, std::wstring& ts)
{
    size_t timestampSize;
    ReadHex(ifs, timestampSize);
    std::string timestamp(timestampSize, '\0');
    for (size_t i = 0; i < timestampSize; ++i)
    {
        char c;
        ReadHex(ifs, c);
        timestamp[i] = c;
    }
    ts = String::ToWstring(timestamp);
}

void SaveData::SerializeInventoryItemMap(std::ofstream& ofs, const std::map<ItemID, UINT>& inventoryItems)
{
    size_t  inventorySize = inventoryItems.size();

    WriteHex(ofs, inventorySize);
    for (const auto& item : inventoryItems) 
    {
        UINT itemID = static_cast<UINT>(item.first);
        WriteHex(ofs, itemID);
        WriteHex(ofs, item.second);
    }
}

void SaveData::DeserializeInventoryItemMap(std::ifstream& ifs, std::map<ItemID, UINT>& inventoryItems)
{
    size_t inventorySize;
    ReadHex(ifs, inventorySize);

    for (size_t i = 0; i < inventorySize; ++i) 
    {
        ItemID itemID;
        UINT itemValue;
        ReadHex(ifs, itemID);
        ReadHex(ifs, itemValue);
        inventoryItems[itemID] = itemValue;
    }
}

void SaveData::SerializeVisitedFloor(std::ofstream& ofs, const std::set<int>& visitedFloor)
{
    size_t size = visitedFloor.size();
    WriteHex(ofs, size);  // Write the size of the set
    for (const int& floor : visitedFloor) 
    {
        WriteHex(ofs, floor);  // Write each element
    }
}
void SaveData::DeserializeVisitedFloor(std::ifstream& ifs, std::set<int>& visitedFloor)
{
    size_t size;
    ReadHex(ifs, size);  // Read the size of the set
    visitedFloor.clear();
    for (size_t i = 0; i < size; ++i) 
    {
        int floor;
        ReadHex(ifs, floor);  // Read each element
        visitedFloor.insert(floor);
    }
}

void SaveData::SerializeEventType(std::ofstream& ofs, const EventType& type)
{
    int typeInt = static_cast<int>(type);
    WriteHex(ofs, typeInt);
}

void SaveData::DeserializeEventType(std::ifstream& ifs, EventType& type)
{
    int typeInt;
    ReadHex(ifs, typeInt);
    type = static_cast<EventType>(typeInt);
}

void SaveData::SerializeCoord(std::ofstream& ofs, const Coord& coord)
{
    WriteHex(ofs, coord.x);
    WriteHex(ofs, coord.y);
}

void SaveData::DeserializeCoord(std::ifstream& ifs, Coord& coord)
{
    ReadHex(ifs, coord.x);
    ReadHex(ifs, coord.y);
}

void SaveData::SerializeEventParams(std::ofstream& ofs, const EventParams& params)
{
    EventTypeIdentifier typeIdentifier;

    if (dynamic_cast<const MonsterParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::MonsterParams;
    }
    else if (dynamic_cast<const ItemParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::ItemParams;
    }
    else if (dynamic_cast<const DoorParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::DoorParams;
    }
    else if (dynamic_cast<const StairParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::StairParams;
    }
    else if (dynamic_cast<const NPCParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::NPCParams;
    }
    else if (dynamic_cast<const ArrowParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::ArrowParams;
    }
    else if (dynamic_cast<const TerrainParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::TerrainParams;
    }
    else if (dynamic_cast<const GeneralEventParams*>(&params)) {
        typeIdentifier = EventTypeIdentifier::GeneralEventParams;
    }
    else {
        throw std::runtime_error("Unknown EventParams type");
    }

    WriteHex(ofs, typeIdentifier);

    int typeInt = static_cast<int>(params.type);
    WriteHex(ofs, typeInt);
    WriteHex(ofs, params.coord.x);
    WriteHex(ofs, params.coord.y);

    switch (typeIdentifier) {
    case EventTypeIdentifier::MonsterParams: {
        const MonsterParams& mp = static_cast<const MonsterParams&>(params);
        WriteHex(ofs, mp.monsterID);
        break;
    }
    case EventTypeIdentifier::ItemParams: {
        const ItemParams& ip = static_cast<const ItemParams&>(params);
        WriteHex(ofs, ip.itemID);
        break;
    }
    case EventTypeIdentifier::DoorParams: {
        const DoorParams& dp = static_cast<const DoorParams&>(params);
        WriteHex(ofs, dp.doorType);
        break;
    }
    case EventTypeIdentifier::StairParams: {
        const StairParams& sp = static_cast<const StairParams&>(params);
        WriteHex(ofs, sp.stairType);
        WriteHex(ofs, sp.newPlayerCoord.x);
        WriteHex(ofs, sp.newPlayerCoord.y);
        break;
    }
    case EventTypeIdentifier::NPCParams: {
        const NPCParams& np = static_cast<const NPCParams&>(params);
        WriteHex(ofs, np.NPCID);
        WriteHex(ofs, np.dialogueID);
        break;
    }
    case EventTypeIdentifier::ArrowParams: {
        const ArrowParams& ap = static_cast<const ArrowParams&>(params);
        WriteHex(ofs, ap.arrowDir);
        break;
    }
    case EventTypeIdentifier::TerrainParams: {
        const TerrainParams& tp = static_cast<const TerrainParams&>(params);
        WriteHex(ofs, tp.terrainType);
        break;
    }
    case EventTypeIdentifier::GeneralEventParams: {
        const GeneralEventParams& gp = static_cast<const GeneralEventParams&>(params);
        WriteHex(ofs, gp.triggerID);
        WriteHex(ofs, gp.triggerOnce);
        WriteHex(ofs, gp.colliderType);
        break;
    }
    }
}

std::unique_ptr<EventParams> SaveData::DeserializeEventParams(std::ifstream& ifs)
{
    EventTypeIdentifier typeIdentifier;
    ReadHex(ifs, typeIdentifier);

    int typeInt;
    ReadHex(ifs, typeInt);
    EventType type = static_cast<EventType>(typeInt);

    Coord coord;
    ReadHex(ifs, coord.x);
    ReadHex(ifs, coord.y);

    switch (typeIdentifier) {
    case EventTypeIdentifier::MonsterParams: {
        UINT monsterID;
        ReadHex(ifs, monsterID);
        return std::make_unique<MonsterParams>(type, coord, monsterID);
    }
    case EventTypeIdentifier::ItemParams: {
        ItemID itemID;
        ReadHex(ifs, itemID);
        return std::make_unique<ItemParams>(type, coord, itemID);
    }
    case EventTypeIdentifier::DoorParams: {
        DoorType doorType;
        ReadHex(ifs, doorType);
        return std::make_unique<DoorParams>(type, coord, doorType);
    }
    case EventTypeIdentifier::StairParams: {
        StairType stairType;
        ReadHex(ifs, stairType);
        Coord newPlayerCoord;
        ReadHex(ifs, newPlayerCoord.x);
        ReadHex(ifs, newPlayerCoord.y);
        return std::make_unique<StairParams>(type, coord, stairType, newPlayerCoord);
    }
    case EventTypeIdentifier::NPCParams: {
        UINT NPCID;
        UINT dialogueID;
        ReadHex(ifs, NPCID);
        ReadHex(ifs, dialogueID);
        return std::make_unique<NPCParams>(type, coord, NPCID, dialogueID);
    }
    case EventTypeIdentifier::ArrowParams: {
        ArrowDirection arrowDir;
        ReadHex(ifs, arrowDir);
        return std::make_unique<ArrowParams>(type, coord, arrowDir);
        break;
    }
    case EventTypeIdentifier::TerrainParams: {
        TerrainType terrainType;
        ReadHex(ifs, terrainType);
        return std::make_unique<TerrainParams>(type, coord, terrainType);
        break;
    }
    case EventTypeIdentifier::GeneralEventParams: {
        UINT triggerID;
        bool triggerOnce;
        ColliderType colliderType;
        ReadHex(ifs, triggerID);
        ReadHex(ifs, triggerOnce);
        ReadHex(ifs, colliderType);
        return std::make_unique<GeneralEventParams>(type, coord, triggerID, triggerOnce, colliderType);
        break;
    }
    default:
        throw std::runtime_error("Unknown EventParams type");
    }
}

void SaveData::SerializeEventParamsMap(std::ofstream& ofs, const std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>>& map)
{   
    size_t outerSize = map.size();
    WriteHex(ofs, outerSize);

    for (const auto& outerPair : map) 
    {
        WriteHex(ofs, outerPair.first);
        size_t innerSize = outerPair.second.size();
        WriteHex(ofs, innerSize);

        for (const auto& innerPair : outerPair.second) 
        {
            std::string key = WideStringToString(innerPair.first);
            size_t keySize = key.size();
            WriteHex(ofs, keySize);
            for (char c : key) 
            {
                WriteHex(ofs, c);
            }
            SerializeEventParams(ofs, *innerPair.second);
        }
    }
}

void SaveData::DeserializeEventParamsMap(std::ifstream& ifs, std::unordered_map<int, std::unordered_map<std::wstring, std::unique_ptr<EventParams>>>& map)
{
    size_t outerSize;
    ReadHex(ifs, outerSize);

    for (size_t i = 0; i < outerSize; ++i) {
        int outerKey;
        ReadHex(ifs, outerKey);

        size_t innerSize;
        ReadHex(ifs, innerSize);

        std::unordered_map<std::wstring, std::unique_ptr<EventParams>> innerMap;
        for (size_t j = 0; j < innerSize; ++j) {
            size_t keySize;
            ReadHex(ifs, keySize);

            std::string key(keySize, '\0');
            for (size_t k = 0; k < keySize; ++k) {
                char c;
                ReadHex(ifs, c);
                key[k] = c;
            }
            std::wstring wKey = StringToWideString(key);

            auto eventParams = DeserializeEventParams(ifs);
            innerMap[wKey] = std::move(eventParams);
        }

        map[outerKey] = std::move(innerMap);
    }
}

void SaveData::SerializeMapTileIdx(std::ofstream& ofs, const std::unordered_map<int, std::unordered_map<Coord, UINT>>& mapTileIdx)
{
    size_t outerSize = mapTileIdx.size();
    WriteHex(ofs, outerSize);

    for (const auto& outerPair : mapTileIdx) 
    {
        WriteHex(ofs, outerPair.first);
        size_t innerSize = outerPair.second.size();
        WriteHex(ofs, innerSize);

        for (const auto& innerPair : outerPair.second) 
        {
            SerializeCoord(ofs, innerPair.first);
            WriteHex(ofs, innerPair.second);
        }
    }
}

void SaveData::DeserializeMapTileIdx(std::ifstream& ifs, std::unordered_map<int, std::unordered_map<Coord, UINT>>& mapTileIdx)
{
    size_t outerSize;
    ReadHex(ifs, outerSize);

    for (size_t i = 0; i < outerSize; ++i) 
    {
        int outerKey;
        ReadHex(ifs, outerKey);

        size_t innerSize;
        ReadHex(ifs, innerSize);

        std::unordered_map<Coord, UINT> innerMap;
        for (size_t j = 0; j < innerSize; ++j) 
        {
            Coord coord;
            DeserializeCoord(ifs, coord);

            UINT value;
            ReadHex(ifs, value);
            innerMap[coord] = value;
        }

        mapTileIdx[outerKey] = std::move(innerMap);
    }
}

std::string SaveData::WideStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

std::wstring SaveData::StringToWideString(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

template <typename T>
void SaveData::WriteHex(std::ofstream& ofs, const T& value)
{
    const unsigned char* byteData = reinterpret_cast<const unsigned char*>(&value);
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        ofs << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byteData[i]);
    }
}

template <typename T>
void SaveData::ReadHex(std::ifstream& ifs, T& value)
{
    size_t dataSize = sizeof(T) * 2; // each byte corresponding to two hex char
    std::string hexStr(dataSize, '\0');
    ifs.read(&hexStr[0], dataSize);

    unsigned char* byteData = reinterpret_cast<unsigned char*>(&value);
    for (size_t i = 0; i < sizeof(T); ++i) 
    {
        std::string byteStr = hexStr.substr(i * 2, 2);
        byteData[i] = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
    }
}