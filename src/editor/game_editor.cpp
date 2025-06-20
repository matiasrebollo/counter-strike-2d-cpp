#include "game_editor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/yaml_parser.h"
#include "./ui_game_editor.h"

#define MAP_PATH "../maps"


Game_editor::Game_editor(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Game_editor),
        texture_parser(),
        pixmap_manager(),
        logic_map(),
        mode(std::make_unique<BlocksSetter>()),
        first_left_click_done(false),
        first_right_click_done(false),
        has_entry_create(false) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    this->setupToolbar();
    this->setupBlockList();
    this->setupBackgroundList();
    this->setupGunBar();
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setBlock(const int& row, const int& column, const bool& to_delete) {
    this->logic_map.setBlock(row, column, to_delete);
    this->renderBlockInfo(row, column);
}

void Game_editor::setCtSpawn(const int& row, const int& column, const bool& to_delete) {
    this->logic_map.setCtSpawn(row, column, to_delete);
    this->renderBlockInfo(row, column);
}

void Game_editor::setTTSpawn(const int& row, const int& column, const bool& to_delete) {
    this->logic_map.setTTSpawn(row, column, to_delete);
    this->renderBlockInfo(row, column);
}

void Game_editor::setBombSite(const int& row, const int& column, const bool& to_delete) {
    this->logic_map.setBombSite(row, column, to_delete);
    this->renderBlockInfo(row, column);
}

void Game_editor::setGun(const int& row, const int& column, const bool& to_delete) {
    this->logic_map.setGun(row, column, to_delete);
    this->renderBlockInfo(row, column);
}

void Game_editor::setupEditorUi() { this->setupGridMap(); }

void Game_editor::setupBlockList() {
    QLayout* oldLayout = ui->scrollAreaWidgetContents->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    auto* gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);

    ui->scrollAreaWidgetContents->setLayout(gridLayout);

    const int columns = 3;
    int count_blocks = 0;

    for (const auto& block: texture_parser.get_blocks_keys()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(50, 50);

        BlockTextureInfo texture = texture_parser.get_texture_info(block);
        QPixmap& tile = pixmap_manager.get_block_pixmap(block);
        label->setPixmap(tile.scaled(50, 50));
        label->setCursor(Qt::CrossCursor);

        int row = count_blocks / columns;
        int col = count_blocks % columns;
        gridLayout->addWidget(label, row, col);

        if (texture.collidable) {
            markAsCollidable(label);
        }

        connect(label, &ClickableLabel::left_clicked, [this, block]() {
            mode = std::make_unique<BlocksSetter>();
            logic_map.selectBlock(block);
            first_left_click_done = false;
        });

        count_blocks++;
    }
}

void Game_editor::setupToolbar() {
    ClickableLabel* labelTT = new ClickableLabel();
    ui->GameAreas->addStretch();
    labelTT->setFixedSize(60, 80);
    labelTT->setText("TT");
    labelTT->setCursor(Qt::CrossCursor);
    labelTT->setStyleSheet("color:white;"
                           "border-width: 1px;"
                           "border-style: solid;"
                           "border-color: white;");
    connect(labelTT, &ClickableLabel::left_clicked, [this]() {
        first_left_click_done = false;
        mode = std::make_unique<TTSpawnsSetter>();
    });

    ClickableLabel* labelCT = new ClickableLabel();
    labelCT->setFixedSize(60, 80);
    labelCT->setText("CT");
    labelCT->setCursor(Qt::CrossCursor);
    labelCT->setStyleSheet("color:white;"
                           "border-width: 1px;"
                           "border-style: solid;"
                           "border-color: white;");
    connect(labelCT, &ClickableLabel::left_clicked, [this]() {
        first_left_click_done = false;
        mode = std::make_unique<CTSpawnsSetter>();
    });

    ClickableLabel* labelBombSites = new ClickableLabel();
    labelBombSites->setFixedSize(60, 80);
    labelBombSites->setText("SITES");
    labelBombSites->setCursor(Qt::CrossCursor);
    labelBombSites->setStyleSheet("color:white;"
                                  "border-width: 1px;"
                                  "border-style: solid;"
                                  "border-color: white;");
    connect(labelBombSites, &ClickableLabel::left_clicked, [this]() {
        first_left_click_done = false;
        mode = std::make_unique<BombSiteSetter>();
    });
    ui->GameAreas->addWidget(labelTT, 0, Qt::AlignHCenter);
    ui->GameAreas->addWidget(labelCT, 0, Qt::AlignHCenter);
    ui->GameAreas->addWidget(labelBombSites, 0, Qt::AlignHCenter);
    ui->GameAreas->addStretch();
}

void Game_editor::setupGunBar() {
    std::vector<GunType> guns_vec = {GLOCK, AWP, AK47, M3};
    for (const auto& gun: guns_vec) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 80);
        QPixmap& gun_image = pixmap_manager.get_gun_pixmap(gun);
        label->setPixmap(gun_image.scaled(50, 50));
        label->setCursor(Qt::CrossCursor);
        connect(label, &ClickableLabel::left_clicked, [this, gun]() {
            first_left_click_done = false;
            this->logic_map.selectGun(gun);
            mode = std::make_unique<GunsSetter>();
        });
        ui->gunsArea->addWidget(label, 0, Qt::AlignHCenter);
    }
}

void Game_editor::setupBackgroundList() {
    for (const auto& background: texture_parser.get_backgrounds()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 60);
        std::string background_path = texture_parser.get_background_path(background);
        QPixmap& background_image = pixmap_manager.get_pixmap(background_path);
        label->setPixmap(background_image.scaled(50, 50));
        ui->backgrounds_list->addWidget(label);
        ui->backgrounds_list->addWidget(label, 0, Qt::AlignHCenter);
        label->setCursor(Qt::CrossCursor);
        connect(label, &ClickableLabel::left_clicked, [this, background, background_path]() {
            this->onBackgroundLabelClicked(background, background_path);
        });
    }
}

void Game_editor::setupGridMap() {
    this->clearGridMap();
    int width = logic_map.getWidth();
    int height = logic_map.getHeight();
    ui->scrollAreaGridMap->setMinimumSize(50 * width, 50 * height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            addGridMapCell(i, j);
        }
    }
    ui->scrollArea_2->setWidget(ui->scrollAreaGridMap);
    ui->scrollArea_2->setWidgetResizable(true);
    ui->scrollAreaGridMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->scrollAreaGridMap->setMinimumSize(50 * width, 50 * height);
    ui->editMapPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->centralwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Game_editor::addGridMapCell(const int& i, const int& j) {
    ClickableLabel* cell = new ClickableLabel();
    cell->setFixedSize(50, 50);
    ui->grid_map->addWidget(cell, i, j);
    this->renderBlockInfo(i, j);

    connect(cell, &ClickableLabel::left_clicked, this, [this, cell, i, j]() {
        first_left_click = {j, i};
        first_left_click_done = true;
    });

    connect(cell, &ClickableLabel::right_clicked, this, [this, cell, i, j]() {
        first_right_click = {j, i};
        first_right_click_done = true;
    });

    connect(cell, &ClickableLabel::dropped, this, [this, i, j]() {
        if (first_left_click_done) {
            second_left_click = {j, i};
            mode->handle(first_left_click, second_left_click, *this, false);
            first_left_click_done = false;
        } else if (first_right_click_done) {
            second_right_click = {j, i};
            mode->handle(first_right_click, second_right_click, *this, true);
            first_right_click_done = false;
        }
    });

    connect(cell, &ClickableLabel::double_click_left, this, [this, cell, i, j]() {
        first_left_click = {j, i};
        second_left_click = {j, i};
        mode->handle(first_left_click, second_left_click, *this, false);
        first_left_click_done = false;
    });

    connect(cell, &ClickableLabel::double_click_right, this, [this, cell, i, j]() {
        first_right_click = {j, i};
        second_right_click = {j, i};
        mode->handle(first_right_click, second_right_click, *this, true);
        first_right_click_done = false;
    });
}


void Game_editor::clearGridMap() {
    QLayoutItem* item;
    while ((item = ui->grid_map->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            ui->grid_map->removeWidget(widget);
            widget->deleteLater();
        }
        delete item;
    }
}

void Game_editor::loadMapFromFile(const std::string& map_name) {
    YamlParser parser;
    GameMap map = parser.yaml_to_game_map(std::string(MAP_PATH) + "/" + map_name + ".yaml");

    logic_map.loadMap(map);

    std::string background_path = texture_parser.get_background_path(logic_map.getBackground());
    this->onBackgroundLabelClicked(logic_map.getBackground(), background_path);
}


void Game_editor::onBackgroundLabelClicked(const Background& background,
                                           const std::string& background_path) {
    QString qss = QString("#scrollAreaGridMap {"
                          "border-image: url(%1) 0 0 0 0 stretch stretch;"
                          "}")
                          .arg(QString::fromStdString(background_path));
    logic_map.selectBackground(background);
    ui->scrollAreaGridMap->setStyleSheet(qss);
    first_left_click_done = false;
}

void Game_editor::renderBlockInfo(const int& row, const int& column) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, column);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            CellInfo cell_info = logic_map.getCellInfo(row, column);

            this->renderBlock(cell, cell_info.block);
            if (cell_info.tt_spawn) {
                markAsTTSpawn(cell);
            }
            if (cell_info.ct_spawn) {
                markAsCtSpawn(cell);
            }
            if (cell_info.bomb_site) {
                markAsBombSite(cell);
            }
            if (cell_info.gun != NONE) {
                markWithGun(cell, cell_info.gun);
            }
        }
    }
}

void Game_editor::renderBlock(ClickableLabel* cell, const int& block) {
    QPixmap& tile = pixmap_manager.get_block_pixmap(block);
    cell->setPixmap(tile.scaled(50, 50));
}

void Game_editor::markAsCollidable(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/collidable.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = result.width() - scaledOverlay.width();
    int y = 0;
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::markAsTTSpawn(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/terrorist_logo.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = result.width() - scaledOverlay.width();
    int y = result.height() - scaledOverlay.height();
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::markAsCtSpawn(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/counter_terrorist_logo.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = 0;
    int y = result.height() - scaledOverlay.height();
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::markAsBombSite(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/weapons/bomb.bmp");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = 0;
    int y = 0;
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::markWithGun(ClickableLabel* label, const GunType& gun) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_gun_pixmap(gun);
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = result.width() - scaledOverlay.width();
    int y = 0;
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::on_save_button_clicked() {
    try {
        GameMap map = this->logic_map.createMap();
        YamlParser parser;
        YAML::Node yaml = parser.game_map_to_Yaml(map);

        QString fileName = QFileDialog::getSaveFileName(
                this, "Guardar Mapa", "", "Archivos YAML (*.yaml);;Todos los archivos (*)");

        if (fileName.isEmpty()) {
            return;
        }

        if (!fileName.endsWith(".yaml", Qt::CaseInsensitive)) {
            fileName += ".yaml";
        }

        std::ofstream fout(fileName.toStdString());
        fout << yaml;
        close();
    } catch (const std::runtime_error&) {
        return;
    }
}

void Game_editor::on_go_to_create_button_clicked() {
    if (!this->has_entry_create) {
        this->logic_map.clearMap();

        std::string background_path =
                texture_parser.get_background_path(this->logic_map.getBackground());
        this->onBackgroundLabelClicked(this->logic_map.getBackground(), background_path);
        this->setupEditorUi();
        this->has_entry_create = true;
    }
    ui->stack->setCurrentIndex(1);
}

void Game_editor::on_load_map_clicked() {
    ui->stack->setCurrentIndex(2);

    ui->maps_list->clear();

    for (const auto& entry: std::filesystem::directory_iterator(MAP_PATH)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename().string();
            this->format_string(name);
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
            item->setTextAlignment(Qt::AlignCenter);
            ui->maps_list->addItem(item);
        }
    }
    if (ui->maps_list->count() == 0) {
        ui->message->setText("Tenes que crear algún mapa para modificarlo");
    }
}

void Game_editor::on_back_button_clicked() { ui->stack->setCurrentIndex(0); }

void Game_editor::on_back_button_2_clicked() { ui->stack->setCurrentIndex(0); }

void Game_editor::on_load_map_button_clicked() {
    if (not(ui->maps_list->currentIndex().isValid())) {
        QMessageBox::information(this, TITLE_MSG_EDIT, MSG_MAP_NOT_SELECTED);
        return;
    }

    std::string map_name = ui->maps_list->currentItem()->text().toStdString();
    this->loadMapFromFile(map_name);
    this->setupEditorUi();
    this->has_entry_create = false;
    // Not very sure if this will work always, it should load the blocks at least
    ui->stack->setCurrentIndex(1);
}

void Game_editor::on_add_columns_button_clicked() {
    int collumns_actual = this->logic_map.getWidth();
    this->logic_map.addColumns();
    int rows = this->logic_map.getHeight();
    int collumns_new = this->logic_map.getWidth();

    ui->scrollAreaGridMap->setMinimumSize(50 * collumns_new, 50 * rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = collumns_actual; j < collumns_new; ++j) {
            addGridMapCell(i, j);
        }
    }
}

void Game_editor::on_add_rows_button_clicked() {
    int rows_actual = this->logic_map.getHeight();
    this->logic_map.addRows();
    int rows_new = this->logic_map.getHeight();
    int collumns = this->logic_map.getWidth();

    ui->scrollAreaGridMap->setMinimumSize(50 * collumns, 50 * rows_new);

    for (int i = rows_actual; i < rows_new; ++i) {
        for (int j = 0; j < collumns; ++j) {
            addGridMapCell(i, j);
        }
    }
}

void Game_editor::format_string(std::string& s) { s.erase(s.length() - 5); }
