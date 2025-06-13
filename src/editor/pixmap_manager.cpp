#include "pixmap_manager.h"

PixmapManager::PixmapManager(): texture_parser() {
    QPixmap base(50, 50);
    base.fill(QColor(255, 255, 255, 80));
    this->block_pixmap_cache[NONE_BLOCK] = base;
}

QPixmap& PixmapManager::get_block_pixmap(const int& block) {
    auto it = block_pixmap_cache.find(block);
    if (it != block_pixmap_cache.end()) {
        return it->second;
    }
    BlockTextureInfo texture = texture_parser.get_texture_info(block);
    QPixmap tileset(QString::fromStdString(texture.tileset_path));
    auto result = block_pixmap_cache.insert(
            {block, tileset.copy(texture.x, texture.y, texture.width, texture.height)});
    return result.first->second;
}

QPixmap& PixmapManager::get_pixmap(const std::string& path) {
    auto it = pixmap_cache.find(path);
    if (it != pixmap_cache.end()) {
        return it->second;
    }
    QPixmap tileset(QString::fromStdString(path));
    pixmap_cache[path] = tileset;
    return pixmap_cache[path];
}
