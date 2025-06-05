#include "yaml_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "game_map.h"
#include "map_object.h"

YAML::Node YamlParser::game_map_to_Yaml(const GameMap& game_map) {
    YAML::Node map;
    map["width"] = game_map.width;
    map["height"] = game_map.height;

    YAML::Node blocks(YAML::NodeType::Sequence);
    for (const auto& obj: game_map.map_objects) {
        if (obj.type != NONE_BLOCK) {
            blocks.push_back(map_object_to_yaml(obj));
        }
    }
    map["blocks"] = blocks;

    YAML::Node ct_spawns(YAML::NodeType::Sequence);
    for (const auto& pos: game_map.ct_spawns) {
        // cppcheck-suppress useStlAlgorithm
        ct_spawns.push_back(vector2d_to_yaml(pos));
    }
    map["ct_spawns"] = ct_spawns;

    YAML::Node tt_spawns(YAML::NodeType::Sequence);
    for (const auto& pos: game_map.tt_spawns) {
        // cppcheck-suppress useStlAlgorithm
        tt_spawns.push_back(vector2d_to_yaml(pos));
    }
    map["tt_spawns"] = tt_spawns;

    YAML::Node sites(YAML::NodeType::Sequence);
    for (const auto& pos: game_map.sites) {
        // cppcheck-suppress useStlAlgorithm
        sites.push_back(vector2d_to_yaml(pos));
    }
    map["sites"] = sites;

    return map;
}

GameMap YamlParser::yaml_to_game_map(const std::string& path) {
    YAML::Node file = YAML::LoadFile(path);

    std::vector<MapObject> blocks;
    for (const auto& block: file["blocks"]) {
        // cppcheck-suppress useStlAlgorithm
        blocks.push_back(yaml_to_map_object(block));
    }

    std::vector<Vector2D> ct_spawns;
    for (const auto& block: file["ct_spawns"]) {
        // cppcheck-suppress useStlAlgorithm
        ct_spawns.push_back(yaml_to_vector2d(block));
    }

    std::vector<Vector2D> tt_spawns;
    for (const auto& block: file["tt_spawns"]) {
        // cppcheck-suppress useStlAlgorithm
        tt_spawns.push_back(yaml_to_vector2d(block));
    }

    std::vector<Vector2D> sites;
    for (const auto& block: file["sites"]) {
        // cppcheck-suppress useStlAlgorithm
        sites.push_back(yaml_to_vector2d(block));
    }

    GameMap map{
            file["width"].as<int>(), file["height"].as<int>(), blocks, ct_spawns, tt_spawns, sites};
    return map;
}

YAML::Node YamlParser::map_object_to_yaml(const MapObject& map_obj) {
    YAML::Node obj;
    obj["block_id"] = int(map_obj.type);

    YAML::Node positions(YAML::NodeType::Sequence);
    for (const auto& pos: map_obj.positions) {
        // cppcheck-suppress useStlAlgorithm
        positions.push_back(vector2d_to_yaml(pos));
    }
    obj["positions"] = positions;

    obj["collidable"] = map_obj.collidable;
    return obj;
}

YAML::Node YamlParser::vector2d_to_yaml(const Vector2D& vector) {
    YAML::Node pos;
    pos["x"] = vector.x;
    pos["y"] = vector.y;
    return pos;
}

MapObject YamlParser::yaml_to_map_object(const YAML::Node& node) {
    std::vector<Vector2D> positions;
    for (const auto& pos: node["positions"]) {
        // cppcheck-suppress useStlAlgorithm
        positions.push_back(yaml_to_vector2d(pos));
    }

    MapObject obj{positions, node["block_id"].as<int>(), node["collidable"].as<bool>()};
    return obj;
}

Vector2D YamlParser::yaml_to_vector2d(const YAML::Node& node) {
    return Vector2D{node["x"].as<int>(), node["y"].as<int>()};
}
