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

SDL2pp::Font& TextureManager::get_font(const std::string& path, int size) {
    std::string key = path + ":" + std::to_string(size);

    auto it = font_cache.find(key);
    if (it != font_cache.end()) {
        return it->second;
    }

    auto result = font_cache.emplace(key, SDL2pp::Font(path, size));
    return result.first->second;
}

SDL2pp::Texture& TextureManager::get_text_texture(const std::string& text,
                                                  const std::string& font_path, int size,
                                                  const SDL2pp::Color& color) {

    std::string key = text + "|" + font_path + "|" + std::to_string(size) + "|" +
                      std::to_string(color.GetRed()) + "," + std::to_string(color.GetGreen()) +
                      "," + std::to_string(color.GetBlue());

    auto it = texture_cache.find(key);
    if (it != texture_cache.end()) {
        return it->second;
    }

    SDL2pp::Font& font = get_font(font_path, size);
    SDL2pp::Surface surface = font.RenderText_Solid(text, color);
    auto result = texture_cache.emplace(key, SDL2pp::Texture(renderer, surface));
    return result.first->second;
}
