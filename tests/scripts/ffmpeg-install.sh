#!/bin/bash

apt-get -y update
apt-get -y install libjpeg-dev libpng12-dev libtiff4-dev curl apache2 wget imagemagick libimage-exiftool-perl unzip lame autoconf build-essential checkinstall git libass-dev libfaac-dev libgpac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev librtmp-dev libtheora-dev libtool libvorbis-dev pkg-config texi2html zlib1g-dev ffmpeg2theora poppler-utils

mkdir ~/ffmpeg-source
cd ~/ffmpeg-source

# Install yasm
curl "https://copy.com/wIWKi9wcX1Je" > yasm.deb
dpkg -i yasm.deb

# Install x264
curl "https://copy.com/G0FFBm80SRK0" > x264.deb
dpkg -i x264.deb

# Install fdk-aac
curl "https://copy.com/ZuVUUydN1WiD" > fdk-aac.deb
dpkg -i fdk-aac.deb

# Install libvpx
curl "https://copy.com/TdXe3Ca3ht6K" > libvpx.deb
dpkg -i libvpx.deb

# Install opus
curl "https://copy.com/LrNpf7J7IgJB" > opus.deb
dpkg -i opus.deb

# Install ffmpeg
curl "https://copy.com/hYDjFUAYbDHC" > ffmpeg.deb
dpkg -i ffmpeg.deb
