BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_video.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_video)

CI Server:
http://jenkins.discoverygarden.ca

CONTENTS OF THIS FILE
---------------------

 * summary
 * requirements
 * installation
 * configuration
 * licensing

SUMMARY
-------

Adds all required Fedora objects to allow users to ingest and retrieve video
files through the Islandora interface

REQUIREMENTS
------------

ffmpeg2theora see
https://wiki.duraspace.org/display/ISLANDORA6122/Chapter+12+-+Installing+Solution+Pack+Dependencies
for instalation help.
A specific ffmpeg version has been tested:
http://www.ffmpeg.org/releases/ffmpeg-1.1.1.tar.gz
These instructions should be applicable except for using this tar file instead of downloading HEAD of dev.
One will also need to remove the --enable-x11grab flag on the ffmpeg configure.
http://ffmpeg.org/trac/ffmpeg/wiki/CentosCompilationGuide
https://ffmpeg.org/trac/ffmpeg/wiki/UbuntuCompilationGuide

INSTALLATION
------------

For technical metadata extraction enable the Islandora FITS module.

CONFIGURATION
-------------

For compatability with the D6 version of the video solution pack one may wish to
configure the FITS DSID to FITS in the Islandora FITS module's admin.

The Video Solution Pack includes an ingest/purge load test that is disabled and
unusable by default. This particular solution pack was chosen for load testing
due to the resource-heavy process of creating video derivatives and ingesting
several potentially-massive files. To enable and use it:
 1) Make sure that the Testing module is enabled and that all prerequisites
    for derivative creation are satisfied for the Video Solution Pack.
 2) Edit the islandora_video.info file in this folder, removing the ; and
    whitespace before the line 'files[] = tests/islandora_video_load_test.test'
 3) Create a folder in the tests/fixtures/ called 'load', and place as many
    videos in that folder as you would like to ingest. These files all must fit
    within the size specified by your php.ini file's maximum post size, and
    they must have a file extension supported by the Video Solution Pack,
    otherwise they will not be picked up by the load test.
 4) Clear the Drupal cache.
The video load test should now be available to use by navigating to
http://path.to.your.site/admin/config/development/testing, checking 'Video Load
Test' in the Islandora Video section, and then clicking 'Run tests'. The test
will compare the original file to the file ingested into Fedora to see if they
match.
NOTE: THIS WILL NOT WORK ON OPERATING SYSTEMS THAT DO NOT SUPPORT THE PHP
GLOB_BRACE FLAG (E.G. SOLARIS).

LICENSING
---------

This module requires libfaac.  libfaac is not free for comercial use.
