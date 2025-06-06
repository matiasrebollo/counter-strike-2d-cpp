#include "texture_manager.h"

TextureManager::TextureManager(SDL2pp::Renderer& renderer): renderer(renderer) {}

SDL2pp::Texture& TextureManager::get_texture(const std::string& path) {
    auto it = texture_cache.find(path);
    if (it != texture_cache.end()) {
        return it->second;
    }

    SDL2pp::Surface surface(path);
    auto result = texture_cache.emplace(path, SDL2pp::Texture(renderer, surface));
    return result.first->second;
}
