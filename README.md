# Entrega obligatoria 10.06

## Grupo 5

Integrantes: 

- Fernandez, Facundo 
- Molina, Taiel
- Rebollo, Matías
- Rocha Diaz, Tomás

Corrector: Capón, Mateo.

[Google Docs con features done / feature to do)](https://docs.google.com/document/d/1pZF0eVW9CNA9GqI3HR6EKuBi1kEObvm5I7G2kT7rWyc/edit?hl=es&tab=t.0)

### Cosas a instalar

Para que ande lo que está en el cmake, instalar todo esto:

Instalar herramientas esenciales:

```
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config
```

Instalar bibliotecas necesarias para SDL2 y ALSA:

```
sudo apt-get install libsdl2-dev libasound2-dev
```

Instalar compiladores (si no los tienes):

```
sudo apt-get install gcc g++ clang
```

Instalar dependencias comunes para C++:

```
sudo apt-get install libboost-all-dev libcurl4-openssl-dev libssl-dev 
```

Para gtest:

```
sudo apt install libgtest-dev  libgmock-dev
```

Para SDL2:

```
sudo apt install libopusfile-dev libxmp-dev libfluidsynth-dev libwavpack-dev \
                 libmpg123-dev libvorbis-dev libogg-dev libmodplug-dev
```

Para qt:

```
sudo apt update
sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```

Para el yaml:

```
sudo apt update
sudo apt install libyaml-cpp-dev
```

Para la font del cs (lobby), parado sobre la carpeta principal:

```
mkdir ~/.fonts
cp ./assets/cs_regular.ttf ~/.fonts
fc-cache -f -v
```

Nota: Taiel utilizó Fedora 40 y sacando las diffs de algunas libraries para instalar las equivalentes en su distro, no tuvo que instalar más que esto tampoco.


