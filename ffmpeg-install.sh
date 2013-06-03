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
     rm -rf ~/ffmpeg-source
     a2enmod rewrite
     a2enmod proxy
     a2enmod proxy_http
     pecl install uploadprogress
     sed -i '949iextension=uploadprogress.so' /etc/php5/apache2/php.ini
     sed -i "s|memory_limit = 128M|memory_limit = 512M|g" /etc/php5/apache2/php.ini
     sed -i "s|post_max_size = 8M|post_max_size = 2048M|g" /etc/php5/apache2/php.ini
     sed -i "s|upload_max_filesize = 2M|upload_max_filesize = 2048M|g" /etc/php5/apache2/php.ini
     echo -e '<VirtualHost *:80>\n        ServerAdmin webmaster@localhost\n\n        DocumentRoot /var/www/drupal7\n        <Directory />\n                Options FollowSymLinks\n                AllowOverride None\n        </Directory>\n        <Directory /var/www/drupal7/>\n                Options Indexes FollowSymLinks MultiViews\n                AllowOverride all\n                Order allow,deny\n                allow from all\n        </Directory>\n\n        ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/\n        <Directory "/usr/lib/cgi-bin">\n                AllowOverride None\n                Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch\n                Order allow,deny\n                Allow from all\n        </Directory>\n\n        ErrorLog ${APACHE_LOG_DIR}/error.log\n\n        # Possible values include: debug, info, notice, warn, error, crit,\n        # alert, emerg.\n        LogLevel warn\n\n        CustomLog ${APACHE_LOG_DIR}/access.log combined\n\n    Alias /doc/ "/usr/share/doc/"\n    <Directory "/usr/share/doc/">\n        Options Indexes MultiViews FollowSymLinks\n        AllowOverride None\n        Order deny,allow\n        Deny from all\n        Allow from 127.0.0.0/255.0.0.0 ::1/128\n    </Directory>\n\n        ProxyPass /adore-djatoka http://localhost:8080/adore-djatoka\n        ProxyPassReverse /adore-djatoka http://localhost:8080/adore-djatoka\n\n</VirtualHost>' > /etc/apache2/sites-available/default
   ;;
