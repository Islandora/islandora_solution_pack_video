#!/bin/bash    
sudo apt-get -y install libvorbisenc2 libtheora0 libopencore-amrnb0 libopencore-amrwb0 libmp3lame0 libfaac0 libass4 ffmpeg2theora
wget http://jenkins.discoverygarden.ca:8080/ffmpeg_amd64.deb
wget http://jenkins.discoverygarden.ca:8080/x264_amd64.deb
sudo dpkg -i ffmpeg_amd64.deb
sudo dpkg -i x264_amd64.deb
sudo ln -s /usr/local/lib/libx264.so.133 /usr/lib/libx264.so.133
