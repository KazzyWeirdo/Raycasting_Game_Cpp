#pragma once
#include <string>
#include <vector>
#include "Map.hpp"

class MapManager {
public:
    static void ensureAssetsDirectory();

    static bool saveMap(const std::string& filename, LevelData& data);

    static LevelData loadMap(const std::string& filename);

    static std::vector<std::string> listAvailableMaps();
};