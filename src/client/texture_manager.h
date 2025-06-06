#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

class TextureManager {
private:
    SDL2pp::Renderer& renderer;
    std::unordered_map<std::string, SDL2pp::Texture> texture_cache;

public:
    explicit TextureManager(SDL2pp::Renderer& renderer);

    SDL2pp::Texture& get_texture(const std::string& path);
};
