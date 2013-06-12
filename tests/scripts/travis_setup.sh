#!/bin/bash    
sudo apt-get -y install libvorbisenc2 libtheora0 libopencore-amrnb0 libopencore-amrwb0 libmp3lame0 libfaac0 libass4
wget http://jenkins.discoverygarden.ca:8080/ffmpeg_travisci.deb
wget http://jenkins.discoverygarden.ca:8080/x264_travisci.deb
sudo dpkg -i ffmpeg_travisci.deb
sudo dpkg -i x264_travisci.deb
sudo ln -s /usr/local/lib/libx264.so.133 /usr/lib/libx264.so.133
