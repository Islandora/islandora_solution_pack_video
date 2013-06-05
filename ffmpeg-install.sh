     apt-get -y install oracle-java6-installer libjpeg-dev libpng12-dev libtiff4-dev php5 php5-cli php5-curl php5-dev php5-gd php5-ldap php5-mysql php5-xsl \
     php-soap php-xml-htmlsax3 php-xml-parser php-xml-rpc php-xml-rpc2 php-xml-rss php-xml-serializer php5-imagick php5-mcrypt php-xml* mysql-server vim curl apache2 \
     rsync wget imagemagick ant libimage-exiftool-perl unzip lame autoconf build-essential checkinstall git libass-dev libfaac-dev libgpac-dev libmp3lame-dev \
     libopencore-amrnb-dev libopencore-amrwb-dev librtmp-dev libtheora-dev libtool libvorbis-dev pkg-config texi2html zlib1g-dev ffmpeg2theora poppler-utils

     ## Install ffmpeg from source
     mkdir ~/ffmpeg-source
     cd ~/ffmpeg-source
     wget http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz
     tar xzvf yasm-1.2.0.tar.gz && rm -rf yasm-1.2.0.tar.gz
     cd yasm-1.2.0
     ./configure
     make
     checkinstall --pkgname=yasm --pkgversion="1.2.0" --backup=no --deldoc=yes --fstrans=no --default
     cd ~/ffmpeg-source
     git clone --depth 1 git://git.videolan.org/x264.git
     cd x264
     ./configure --enable-static
     make
     checkinstall --pkgname=x264 --pkgversion="3:$(./version.sh | awk -F'[" ]' '/POINT/{print $4"+git"$5}')" --backup=no --deldoc=yes  --fstrans=no --default
     cd ~/ffmpeg-source
     git clone --depth 1 git://github.com/mstorsjo/fdk-aac.git
     cd fdk-aac
     autoreconf -fiv
     ./configure --disable-shared
     make
     checkinstall --pkgname=fdk-aac --pkgversion="$(date +%Y%m%d%H%M)-git" --backup=no --deldoc=yes --fstrans=no --default
     cd ~/ffmpeg-source
     git clone --depth 1 http://git.chromium.org/webm/libvpx.git
     cd libvpx
     ./configure --disable-examples --disable-unit-tests
     make
     checkinstall --pkgname=libvpx --pkgversion="1:$(date +%Y%m%d%H%M)-git" --backup=no --deldoc=yes --fstrans=no --default
     cd ~/ffmpeg-source
     git clone --depth 1 git://git.xiph.org/opus.git
     cd opus
     ./autogen.sh
     ./configure --disable-shared
     make
     checkinstall --pkgname=libopus --pkgversion="$(date +%Y%m%d%H%M)-git" --backup=no --deldoc=yes --fstrans=no --default
     cd ~/ffmpeg-source
     wget http://www.ffmpeg.org/releases/ffmpeg-1.1.1.tar.gz
     tar xf ffmpeg-1.1.1.tar.gz && rm -rf ffmpeg-1.1.1.tar.gz
     cd ffmpeg-1.1.1
     ./configure --enable-gpl --enable-libass --enable-libfaac --enable-libfdk-aac --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-librtmp \
       --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-nonfree --enable-version3 --enable-libopus
     make
     checkinstall --pkgname=ffmpeg --pkgversion="7:$(date +%Y%m%d%H%M)-git" --backup=no --deldoc=yes --fstrans=no --default
     hash -r
     ## Cleanup ffmpeg mess
     cd ~
