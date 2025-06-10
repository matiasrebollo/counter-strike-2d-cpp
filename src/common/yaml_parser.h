#ifndef YAML_PARSER_h
#define YAML_PARSER_h

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "game_map.h"
#include "map_object.h"

class YamlParser {
public:
    YAML::Node game_map_to_Yaml(const GameMap& game_map);
    GameMap yaml_to_game_map(const std::string& path);

private:
    YAML::Node map_object_to_yaml(const MapObject& map_obj);
    YAML::Node vector2d_to_yaml(const Vector2D<int>& vector);
    MapObject yaml_to_map_object(const YAML::Node& node);
    Vector2D<int> yaml_to_vector2d(const YAML::Node& node);
};

#endif
