#!/bin/bash

BLUE='\033[0;34m'
GREEN='\033[0;32m'
NC='\033[0m'

chmod +x "$0"

echo -e "${BLUE}Bienvenido al instalador del CS2D Remake!${NC}"
echo -e "${BLUE}Descargando lo necesario para jugar ...${NC}"

# Actualización de paquetes
sudo apt-get update

# Herramientas básicas
sudo apt-get install -y build-essential cmake pkg-config
sudo apt-get install -y make

# SDL y sonido
sudo apt-get install -y \
  libsdl2-dev \
  libsdl2-image-dev \
  libsdl2-ttf-dev \
  libsdl2-mixer-dev \
  libasound2-dev

# Compiladores C++
sudo apt-get install -y gcc g++ clang

# Dependencias comunes C++
sudo apt-get install -y \
  libboost-all-dev \
  libcurl4-openssl-dev \
  libssl-dev

# GTest
sudo apt-get install -y \
  libgtest-dev \
  libgmock-dev

# YAML
sudo apt-get install -y libyaml-cpp-dev

# Qt5
sudo apt-get install -y \
  qtbase5-dev \
  qt5-qmake \
  qtdeclarative5-dev \
  libqt5multimedia5-dev \
  libqt5network5-dev

# Audio codecs y formatos
sudo apt-get install -y \
  libpulse-dev \
  libsndfile1-dev \
  libmpg123-dev \
  libmp3lame-dev \
  libogg-dev \
  libvorbis-dev \
  libopus-dev

# Sistema y red
sudo apt-get install -y \
  libdbus-1-dev \
  libglib2.0-dev

# Tipografía y texto
sudo apt-get install -y \
  libfreetype6-dev \
  libharfbuzz-dev \
  libgraphite2-dev

# Compresión
sudo apt-get install -y \
  libbz2-dev \
  libzstd-dev \
  liblzma-dev \
  liblz4-dev

# OpenGL y X11
sudo apt-get install -y \
  libx11-dev \
  libxext-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev

echo -e "${GREEN}Descargas completadas :)${NC}"

echo -e "${BLUE}Instalando juego ...${NC}"

make run-tests

# Copio binarios

# Copiando server
sudo cp build/taller_server /usr/bin/cs2d-remake-server
sudo chmod +x /usr/bin/cs2d-remake-server

# Copiando cliente
sudo cp build/taller_client /usr/bin/cs2d-remake-client
sudo chmod +x /usr/bin/cs2d-remake-client

# Copiando editor
sudo cp build/taller_editor /usr/bin/cs2d-remake-editor
sudo chmod +x /usr/bin/cs2d-remake-editor

# Copio configuraciones

# Copiando assets
sudo mkdir -p /var/cs2d-remake
sudo cp -r assets/* /var/cs2d-remake/

# Instalo fuente cs2
sudo cp assets/cs_regular.ttf /usr/local/share/fonts/
sudo fc-cache -f -v

# Copiando archivo de configuración
sudo mkdir -p /etc/cs2d-remake
sudo cp -r settings.yaml /etc/cs2d-remake/

echo -e "${GREEN}Juego instalado! Esperemos que lo disfrutes :)${NC}"
