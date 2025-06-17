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
    map["background"] = static_cast<int>(game_map.background);

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

    std::vector<Vector2D<int>> ct_spawns;
    for (const auto& block: file["ct_spawns"]) {
        // cppcheck-suppress useStlAlgorithm
        ct_spawns.push_back(yaml_to_vector2d(block));
    }

    std::vector<Vector2D<int>> tt_spawns;
    for (const auto& block: file["tt_spawns"]) {
        // cppcheck-suppress useStlAlgorithm
        tt_spawns.push_back(yaml_to_vector2d(block));
    }

    std::vector<Vector2D<int>> sites;
    for (const auto& block: file["sites"]) {
        // cppcheck-suppress useStlAlgorithm
        sites.push_back(yaml_to_vector2d(block));
    }

    GameMap map{file["width"].as<int>(),
                file["height"].as<int>(),
                static_cast<Background>(file["background"].as<int>()),
                blocks,
                ct_spawns,
                tt_spawns,
                sites};
    return map;
}

ServerSettings YamlParser::load_server_settings(const std::string& path) {
    YAML::Node file = YAML::LoadFile(path);
    ServerSettings settings = {file["server_fps"].as<int>(),
                               file["rounds"].as<size_t>(),
                               file["buy_phase_duration"].as<int>(),
                               file["attack_phase_duration"].as<int>(),
                               file["waiting_players_phase_duration"].as<int>(),
                               file["between_rounds_phase_duration"].as<int>(),
                               file["player_speed"].as<int>(),
                               file["terrorists"].as<size_t>(),
                               file["counter_terrorists"].as<size_t>(),
                               file["player_initial_life"].as<int>(),
                               file["initial_money"].as<int>(),
                               file["clip_price"].as<int>(),
                               file["damage_variation_factor"].as<float>(),
                               load_guns_settings(file["guns_settings"]),
                               load_knife_settings(file["knife_settings"])};
    return settings;
}

std::unordered_map<GunType, GunSettings> YamlParser::load_guns_settings(const YAML::Node& node) {
    std::unordered_map<std::string, GunType> str_to_gun = {
            {"glock", GLOCK}, {"awp", AWP}, {"ak-47", AK47}, {"m3", M3}};

    std::unordered_map<GunType, GunSettings> guns;
    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
        std::string name = it->first.as<std::string>();
        guns[str_to_gun[name]] = load_gun_sett(it->second);
    }
    return guns;
}

GunSettings YamlParser::load_gun_sett(const YAML::Node& node) {
    GunSettings gun{node["initial_ammo"].as<int>(), node["precision"].as<float>(),
                    node["falloff"].as<int>(),      node["damage"].as<int>(),
                    node["rate_of_fire"].as<int>(), node["price"].as<int>(),
                    node["clip_size"].as<int>(),    node["kill_bonus"].as<int>()};
    return gun;
}

KnifeSettings YamlParser::load_knife_settings(const YAML::Node& node) {
    return {node["knife_distance"].as<int>(), node["knife_damage"].as<int>(),
            node["knife_ar"].as<int>(), node["knife_kill_bonus"].as<int>()};
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

YAML::Node YamlParser::vector2d_to_yaml(const Vector2D<int>& vector) {
    YAML::Node pos;
    pos["x"] = vector.x;
    pos["y"] = vector.y;
    return pos;
}

MapObject YamlParser::yaml_to_map_object(const YAML::Node& node) {
    std::vector<Vector2D<int>> positions;
    for (const auto& pos: node["positions"]) {
        // cppcheck-suppress useStlAlgorithm
        positions.push_back(yaml_to_vector2d(pos));
    }

    MapObject obj{positions, node["block_id"].as<int>(), node["collidable"].as<bool>()};
    return obj;
}

Vector2D<int> YamlParser::yaml_to_vector2d(const YAML::Node& node) {
    return Vector2D<int>{node["x"].as<int>(), node["y"].as<int>()};
}
