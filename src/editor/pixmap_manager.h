#ifndef PIXMAP_MANAGER_H
#define PIXMAP_MANAGER_H

#include <QPixmap>
#include <string>
#include <unordered_map>

#include "../common/block_texture_parser.h"
class PixmapManager {
private:
    std::unordered_map<std::string, QPixmap> pixmap_cache;
    std::unordered_map<int, QPixmap> block_pixmap_cache;
    BlockTextureParser texture_parser;

public:
    PixmapManager();

    QPixmap& get_block_pixmap(const int& block);
    QPixmap& get_pixmap(const std::string& path);
};

#endif
