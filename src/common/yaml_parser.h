#ifndef YAML_PARSER_h
#define YAML_PARSER_h

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "game_map.h"
#include "map_object.h"
#include "settings.h"
#include "weapon_type.h"

class YamlParser {
public:
    YAML::Node game_map_to_Yaml(const GameMap& game_map);
    GameMap yaml_to_game_map(const std::string& path);
    ServerSettings load_server_settings(const std::string& path);
    ClientSettings load_client_settings(const std::string& path);

private:
    YAML::Node map_object_to_yaml(const MapObject& map_obj);
    YAML::Node vector2d_to_yaml(const Vector2D<int>& vector);
    YAML::Node guns_to_yaml(const std::map<GunType, std::vector<Vector2D<int>>>& guns);
    std::map<GunType, std::vector<Vector2D<int>>> yaml_to_guns(const YAML::Node& node);
    MapObject yaml_to_map_object(const YAML::Node& node);
    Vector2D<int> yaml_to_vector2d(const YAML::Node& node);
    std::unordered_map<GunType, GunSettings> load_guns_settings(const YAML::Node& node);
    GunSettings load_gun_sett(const YAML::Node& node);
    KnifeSettings load_knife_settings(const YAML::Node& node);
};

#endif
