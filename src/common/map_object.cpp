#include "map_object.h"

#include <map>
#include <string>

static std::map<MapObjectType, std::string> objToStr = {
        {AZTEC_STONE_WALL_1, "AZTEC_STONE_WALL_1"}, {AZTEC_STONE_WALL_2, "AZTEC_STONE_WALL_2"},
        {AZTEC_STONE_WALL_3, "AZTEC_STONE_WALL_3"}, {AZTEC_STONE_WALL_4, "AZTEC_STONE_WALL_4"},
        {AZTEC_STONE_WALL_5, "AZTEC_STONE_WALL_5"}, {AZTEC_STONE_WALL_6, "AZTEC_STONE_WALL_6"},
        {AZTEC_STONE_WALL_7, "AZTEC_STONE_WALL_7"}, {AZTEC_STONE_WALL_8, "AZTEC_STONE_WALL_8"}};

static std::map<std::string, MapObjectType> strToObj = {
        {"AZTEC_STONE_WALL_1", AZTEC_STONE_WALL_1}, {"AZTEC_STONE_WALL_2", AZTEC_STONE_WALL_2},
        {"AZTEC_STONE_WALL_3", AZTEC_STONE_WALL_3}, {"AZTEC_STONE_WALL_4", AZTEC_STONE_WALL_4},
        {"AZTEC_STONE_WALL_5", AZTEC_STONE_WALL_5}, {"AZTEC_STONE_WALL_6", AZTEC_STONE_WALL_6},
        {"AZTEC_STONE_WALL_7", AZTEC_STONE_WALL_7}, {"AZTEC_STONE_WALL_8", AZTEC_STONE_WALL_8}};

std::string map_obj_to_str(MapObjectType obj) { return objToStr[obj]; }
MapObjectType str_to_map_obj(const std::string& str) { return strToObj[str]; }
