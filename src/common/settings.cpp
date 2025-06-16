#include "settings.h"

#include "yaml_parser.h"

Settings::Settings(): serverSettings(YamlParser().load_server_settings(PATH_SETTINGS)) {}

Settings& Settings::getInstance() {
    static Settings instance;
    return instance;
}

int Settings::get_fps_server() { return serverSettings.fps; }
