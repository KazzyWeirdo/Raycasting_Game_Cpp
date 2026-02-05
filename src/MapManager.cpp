#include "../include/MapManager.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void MapManager::ensureAssetsDirectory() {
    if(!fs::exists("assets")) {
        fs::create_directory("assets");
    }
}

bool MapManager::saveMap(const std::string& filename, LevelData& data) {
    ensureAssetsDirectory();
    std::ofstream file("assets/" + filename + ".map");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return false;
    }

    file << data.width << " " << data.height << "\n";

    file << (int)data.skyR << " " << (int)data.skyG << " " << (int)data.skyB << "\n";
    file << (int)data.floorR << " " << (int)data.floorG << " " << (int)data.floorB << "\n";
    file << (int)data.wallR << " " << (int)data.wallG << " " << (int)data.wallB << "\n";
    file << data.fogIntensity << "\n";

    for (int tile : data.tiles) {
        file << tile << " ";
    }

    return true;
}

LevelData MapManager::loadMap(const std::string& filename) {
    LevelData data;
    std::ifstream file("assets/" + filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return data;
    }

    file >> data.width >> data.height;

    int skyR, skyG, skyB;
    file >> skyR >> skyG >> skyB;
    data.skyR = skyR; data.skyG = skyG; data.skyB = skyB;

    int floorR, floorG, floorB;
    file >> floorR >> floorG >> floorB;
    data.floorR = floorR; data.floorG = floorG; data.floorB = floorB;

    int wallR, wallG, wallB;
    file >> wallR >> wallG >> wallB;
    data.wallR = wallR; data.wallG = wallG; data.wallB = wallB;

    float intensity;
    file >> intensity;
    data.fogIntensity = intensity;

    data.tiles.resize(data.width * data.height);
    for (int& tile : data.tiles) {
        file >> tile;
    }
    return data;
}

std::vector<std::string> MapManager::listAvailableMaps() {
    std::vector<std::string> mapFiles;
    ensureAssetsDirectory();

    for (const auto& entry : fs::directory_iterator("assets")) {
        if (entry.path().extension() == ".map") {
            mapFiles.push_back(entry.path().filename().string());
        }
    }
    return mapFiles;
}