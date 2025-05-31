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
    return map;
}

GameMap YamlParser::yaml_to_game_map(const std::string& path) {
    YAML::Node file = YAML::LoadFile(path);

    GameMap map{file["width"].as<int>(), file["height"].as<int>(), {}};
    for (const auto& block: file["blocks"]) {
        map.map_objects.push_back(yaml_to_map_object(block));
    }

    return map;
}

YAML::Node YamlParser::map_object_to_yaml(const MapObject& map_obj) {
    YAML::Node obj;
    obj["name"] = map_obj_to_str(map_obj.type);

    YAML::Node positions(YAML::NodeType::Sequence);
    // NOLINT
    for (const auto& pos: map_obj.positions) {
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
    MapObject obj{
            {}, str_to_map_obj(node["name"].as<std::string>()), node["collidable"].as<bool>()};
    for (const auto& pos: node["positions"]) {
        obj.positions.push_back(yaml_to_vector2d(pos));
    }
    return obj;
}

Vector2D YamlParser::yaml_to_vector2d(const YAML::Node& node) {
    return Vector2D{node["x"].as<int>(), node["y"].as<int>()};
}
