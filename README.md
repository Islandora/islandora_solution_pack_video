# Video Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_video.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_video)

## Introduction

Adds all required Fedora objects to allow users to ingest and retrieve video files through the Islandora interface

**This module requires libfaac. Libfaac is not free for commercial use.**

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* FFmpeg - Compliation guides: [Ubuntu](http://ffmpeg.org/trac/ffmpeg/wiki/CentosCompilationGuide
), [CentOS](https://ffmpeg.org/trac/ffmpeg/wiki/UbuntuCompilationGuide).
* ffmpeg2theora (optional) - [Instructions](https://wiki.duraspace.org/display/ISLANDORA6122/Chapter+12+-+Installing+Solution+Pack+Dependencies)

FFmpeg version 1.1.1 has been tested. It can be downloaded [here](http://www.ffmpeg.org/releases/ffmpeg-1.1.1.tar.gz)

**NOTE**: remove the `--enable-x11grab` flag on the ffmpeg configure.


## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set the path for `ffmpeg`, select configuration options and viewer in Administration » Islandora » Video Solution Pack (admin/islandora/video).

![Configuration](http://i.imgur.com/OnR9lRl.png)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)


## Maintainers/Sponsors
Current maintainers:

* [Nick Ruest](https://github.com/ruebot)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

The Video Solution Pack includes an ingest/purge load test that is disabled and unusable by default. This particular solution pack was chosen for load testing due to the resource-heavy process of creating video derivatives and ingesting several potentially-massive files. To enable and use it:

1. Make sure that the Testing module is enabled and that all prerequisites for derivative creation are satisfied for the Video Solution Pack.
2. Edit the islandora_video.info file in this folder, removing the ; and whitespace before the line `files[] = tests/islandora_video_load_test.test`
3. Create a folder in the `tests/fixtures/` called `load`, and place as many videos in that folder as you would like to ingest. These files all must fit within the size specified by your php.ini file's maximum post size, and they must have a file extension supported by the Video Solution Pack, otherwise they will not be picked up by the load test.
4. Clear the Drupal cache.

The video load test should now be available to use by navigating to `http://path.to.your.site/admin/config/development/testing`, checking 'Video Load Test' in the Islandora Video section, and then clicking 'Run tests'. The test will compare the original file to the file ingested into Fedora to see if they match.

**NOTE**: THIS WILL NOT WORK ON OPERATING SYSTEMS THAT DO NOT SUPPORT THE PHP GLOB_BRACE FLAG (E.G. SOLARIS).

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
