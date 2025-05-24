#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <unordered_map>

#include "../common/skins.h"

#define PATH_SEAL_FORCE_SKIN "../assets/gfx/player/ct1.bmp"
#define PATH_GSG9_SKIN "../assets/gfx/player/ct2.bmp"
#define PATH_SAS_SKIN "../assets/gfx/player/ct3.bmp"
#define PATH_GIGN_SKIN "../assets/gfx/player/ct4.bmp"
#define PATH_PHEONIX_SKIN "../assets/gfx/player/t1.bmp"
#define PATH_L337_CREW_SKIN "../assets/gfx/player/t2.bmp"
#define PATH_ARTIC_SKIN "../assets/gfx/player/t3.bmp"
#define PATH_GUERRILLA_SKIN "../assets/gfx/player/t4.bmp"

class Client {
private:
    std::unordered_map<TerroristSkin, std::string> skins_tt;
    std::unordered_map<CounterTerroristSkin, std::string> skins_ct;

public:
    Client();

    void run(int argc, char* argv[]);
};

#endif  // CLIENT_H
