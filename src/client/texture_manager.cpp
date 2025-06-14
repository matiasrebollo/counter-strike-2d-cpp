#include "texture_manager.h"

#include <limits>
#include <utility>
#include <vector>

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
    SDL2pp::Surface surface = font.RenderText_Blended(text, color);
    auto result = texture_cache.emplace(key, SDL2pp::Texture(renderer, surface));
    return result.first->second;
}


SDL2pp::Texture& TextureManager::get_fov_texture(int angle_deg, float opacity, int diagonal) {
    if (angle_deg == 180) {
        angle_deg = 179;
    }

    const std::string key = "fov_" + std::to_string(angle_deg) + "_" +
                            std::to_string((int)(opacity * 255)) + "_" + std::to_string(diagonal);

    auto it = texture_cache.find(key);
    if (it != texture_cache.end()) {
        return it->second;
    }

    SDL2pp::Texture texture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, diagonal,
                            diagonal);
    texture.SetBlendMode(SDL_BLENDMODE_BLEND);

    renderer.SetTarget(texture);
    if (angle_deg < 180) {
        renderer.SetDrawColor(0, 0, 0, static_cast<Uint8>(opacity * 255));
    } else {
        renderer.SetDrawColor(255, 255, 255, 0);
    }
    renderer.Clear();

    SDL2pp::Point center(diagonal / 2, diagonal / 2);

    float half_angle = angle_deg / 2.0f;
    float angle1_rad = -half_angle * M_PI / 180.0f;
    float angle2_rad = half_angle * M_PI / 180.0f;

    float cone_length = static_cast<float>(std::numeric_limits<int>::max());
    SDL2pp::Point point1(center.x + static_cast<int>(cone_length * std::cos(angle1_rad)),
                         center.y + static_cast<int>(cone_length * std::sin(angle1_rad)));

    SDL2pp::Point point2(center.x + static_cast<int>(cone_length * std::cos(angle2_rad)),
                         center.y + static_cast<int>(cone_length * std::sin(angle2_rad)));

    std::array<SDL_Vertex, 3> vertices_cono;

    SDL2pp::Color white(255, 255, 255, 0);
    SDL2pp::Color black(0, 0, 0, static_cast<Uint8>(opacity * 255));

    SDL2pp::Point points[3] = {center, point1, point2};
    for (int i = 0; i < 3; ++i) {
        vertices_cono[i].position.x = static_cast<float>(points[i].x);
        vertices_cono[i].position.y = static_cast<float>(points[i].y);
        vertices_cono[i].color = angle_deg < 180 ? white : black;
        vertices_cono[i].tex_coord = {0.0f, 0.0f};
    }

    SDL_RenderGeometry(renderer.Get(), nullptr, vertices_cono.data(), 3, nullptr, 0);

    int radio = 50;
    int segments = 32;

    SDL_Vertex center_vertex;
    center_vertex.position.x = static_cast<float>(center.x);
    center_vertex.position.y = static_cast<float>(center.y);
    center_vertex.color = white;
    center_vertex.tex_coord = {0.0f, 0.0f};

    for (int i = 0; i < segments; ++i) {
        float angulo1 = 2.0f * M_PI * i / segments;
        float angulo2 = 2.0f * M_PI * (i + 1) / segments;

        SDL_Vertex v1, v2;

        v1.position.x = center.x + std::cos(angulo1) * radio;
        v1.position.y = center.y + std::sin(angulo1) * radio;
        v1.color = white;
        v1.tex_coord = {0.0f, 0.0f};

        v2.position.x = center.x + std::cos(angulo2) * radio;
        v2.position.y = center.y + std::sin(angulo2) * radio;
        v2.color = white;
        v2.tex_coord = {0.0f, 0.0f};

        SDL_Vertex triangle[3] = {center_vertex, v1, v2};
        SDL_RenderGeometry(renderer.Get(), nullptr, triangle, 3, nullptr, 0);
    }

    renderer.SetTarget();  // Volvemos al render target original
    renderer.SetDrawColor(0, 0, 0, 255);

    texture_cache.emplace(key, std::move(texture));
    return texture_cache.at(key);
}


SDL2pp::Chunk& TextureManager::get_sound(const std::string& path) {
    auto it = sound_cache.find(path);
    if (it != sound_cache.end()) {
        return it->second;
    }

    auto result = sound_cache.emplace(path, SDL2pp::Chunk(path));
    return result.first->second;
}
