# image-gen
image-gen generates random images.

## Dependencies
1. Ubuntu 20.04+
2. ImageMagick 6, Magick++, graphicsmagick-libmagick-dev-compat libgraphicsmagick++1-dev libmagick++-6-headers
3. g++ 9.4.0+
4. make

## Usage
* in order to build, you will need ImageMagick, Magick++, graphicsmagick-libmagick-dev-compat, libgraphicsmagick++1-dev and libmagick++-6-headers
run `sudo apt install `imagemagick libmagick++-dev graphicsmagick-libmagick-dev-compat libgraphicsmagick++1-dev libmagick++-6-headers`
1. Simple Usage:
    1. clone repository, run `make`
    2. `./image-gen`
2. Create a .deb and install (currently only for amd64 architecture)
    1. clone repository
    2. run `make deb`
    3. run `sudo apt install ./path_to_deb.deb`
    4. `image-gen`

## Flags
image-gen takes some command line flags:

    --help  print usage info
    --width set output canvas width (default == 1920)
    --height        set output canvas height (default == 1080)
    --resolution    set output image resolution (default == 72x72)
    --num_shapes    set number of shapes in output image (default == 1)
    --out   set output file name (default == test.png)

## Settings
default settings can be modified using the settings file at /etc/image-gen/settings