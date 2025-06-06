#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    std::unordered_map<std::string, SDL2pp::Texture> texture_cache;
    std::unordered_map<std::string, SDL2pp::Font> font_cache;

    SDL2pp::Font& get_font(const std::string& path, int size);

public:
    explicit TextureManager(SDL2pp::Renderer& renderer);

    SDL2pp::Texture& get_texture(const std::string& path);
    SDL2pp::Texture& get_text_texture(const std::string& text, const std::string& font_path,
                                      int size, const SDL2pp::Color& color);
};
