#!/bin/bash

BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

chmod +x "$0"

echo -e "${BLUE}Bienvenido al instalador del CS2D Remake para Fedora!${NC}"
echo -e "${BLUE}Descargando lo necesario para jugar ...${NC}"

# Actualización de paquetes
sudo dnf update -y

# Herramientas básicas
sudo dnf install -y @development-tools cmake pkgconf-pkg-config make

# SDL y sonido
sudo dnf install -y \
  SDL2-devel \
  SDL2_image-devel \
  SDL2_ttf-devel \
  SDL2_mixer-devel \
  alsa-lib-devel \
  fluidsynth \
  fluidsynth-devel \
  libxmp-devel \
  wavpack-devel

# Compiladores C++
sudo dnf install -y gcc-c++ clang

# Dependencias comunes C++
sudo dnf install -y \
  boost-devel \
  libcurl-devel \
  openssl-devel

# GTest y GMock
sudo dnf install -y \
  gtest-devel \
  gmock-devel

# YAML
sudo dnf install -y yaml-cpp-devel

# Qt5
sudo dnf install -y \
  qt5-qtbase-devel \
  qt5-qtdeclarative-devel \
  qt5-qtmultimedia-devel \
  qt5-qttools-devel \
  qt5-qtsvg-devel \
  qt5-qtbase-gui

# Audio codecs y formatos
sudo dnf install -y \
  pulseaudio-libs-devel \
  opusfile-devel \
  libmodplug-devel \
  libsndfile-devel \
  mpg123-devel \
  lame-devel \
  libogg-devel \
  libvorbis-devel \
  opus-devel

# Sistema y red
sudo dnf install -y \
  dbus-devel \
  glib2-devel

# Tipografía y texto
sudo dnf install -y \
  freetype-devel \
  harfbuzz-devel \
  graphite2-devel

# Compresión
sudo dnf install -y \
  bzip2-devel \
  zstd-devel \
  xz-devel \
  lz4-devel

# OpenGL y X11
sudo dnf install -y \
  libX11-devel \
  libXext-devel \
  mesa-libGL-devel \
  mesa-libGLU-devel

echo -e "${GREEN}Descargas completadas :)${NC}"

echo -e "${BLUE}Instalando juego ...${NC}"

echo -e "${RED}Verificando dependencias faltantes...${NC}"
ldd build/taller_client | grep "not found"
ldd build/taller_server | grep "not found"
ldd build/taller_editor | grep "not found"

make clean
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
sudo cp assets/cs_regular.ttf /usr/share/fonts/
sudo fc-cache -f -v

# Copiando archivo de configuración
sudo mkdir -p /etc/cs2d-remake
sudo cp -r settings.yaml /etc/cs2d-remake/

# Copiando carpeta maps
sudo mkdir -p /etc/cs2d-remake/maps
sudo cp -r maps/* /etc/cs2d-remake/maps

echo -e "${GREEN}Juego instalado! Esperemos que lo disfrutes :)${NC}"
