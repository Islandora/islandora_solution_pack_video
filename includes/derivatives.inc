<?php
/**
 * @file
 *   This file holds supporting code for Islandora video.
 */

/**
 * This function will create the derivatives for an Islandora video object.
 *
 * @param object $fedora_object
 *   The Tuque object to make derivatives for.
 */
function islandora_video_create_all_derivatives($fedora_object) {
  if (!isset($fedora_object["OBJ"])) {
    drupal_set_message("Could not create derivatives for %s. No file was uploaded.", array("%s" => $fedora_object->id), "error");
    return FALSE;
  }
  // Get original file or quit.
  if (variable_get('islandora_video_make_mp4_locally', TRUE) ||
      variable_get('islandora_video_make_fits_locally', TRUE) ||
      variable_get('islandora_video_make_ogg_locally', TRUE) ||
      variable_get('islandora_video_make_archive', TRUE) ||
      variable_get('islandora_video_make_thumbnail_locally', TRUE)) {
    // @TODO: this file name code could be library I stole it from audio pack.
    $mime_detect = new MimeDetect();
    $ext = $mime_detect->getExtension($fedora_object['OBJ']->mimeType);
    $archival_file = str_replace(":", "-", $fedora_object->id) . ".OBJ.{$ext}";
    $archival_file = file_create_filename($archival_file, 'temporary://');
    $fedora_object['OBJ']->getContent($archival_file);
  }
  else {
    return;
  }

  // Make and ingest MP4.
  if (variable_get('islandora_video_make_mp4_locally', TRUE)) {
    $mp4_file = islandora_video_create_mp4($archival_file);
    if ($mp4_file) {
      islandora_video_add_datastream($fedora_object, 'MP4', $mp4_file, 'Display MP4.');
      file_unmanaged_delete($mp4_file);
    }
  }

  // Make and ingest OGG.
  if (variable_get('islandora_video_make_ogg_locally', TRUE)) {
    $ogg_file = islandora_video_create_ogg($archival_file);
    if ($ogg_file) {
      islandora_video_add_datastream($fedora_object, 'OGG', $ogg_file);
      file_unmanaged_delete($ogg_file);
    }
  }

  // Make and ingest MKV.
  if (variable_get('islandora_video_make_mkv_locally', TRUE)) {
    $mkv_file = islandora_video_create_mkv($archival_file);
    if ($mkv_file) {
      islandora_video_add_datastream($fedora_object, 'MKV', $mkv_file);
      file_unmanaged_delete($mkv_file);
    }
  }


  if (variable_get('islandora_video_make_thumbnail_locally', TRUE)) {
    $tn_file = islandora_video_create_thumbnail($archival_file);

    if ($tn_file) {
      islandora_video_add_datastream($fedora_object, 'TN', $tn_file);
      file_unmanaged_delete($tn_file);
      drupal_set_message(t('TN created successfully with ffmpeg.'));
    }
    // Resort to adding the generic TN because Ffmpeg failed.
    else {
      $stored_thumbnail = drupal_get_path('module', 'islandora_video') . '/images/crystal_clear_app_camera.png';
      islandora_video_add_datastream($fedora_object, 'TN', $stored_thumbnail);
      drupal_set_message(t('Default TN added.'));
    }
  }

  file_unmanaged_delete($archival_file);
}

/**
 * This function will create an MP4 file for upload.
 *
 * @param string $archival_file
 *   The file to create the mp4 from.
 *
 * @return mixed
 *   $out_file the location of the derivative
 *   FALSE on failure.
 */
function islandora_video_create_mp4($archival_file) {
  $archival_file = drupal_realpath($archival_file);
  $out_file = $archival_file . '.mp4';

  $ffmpeg_executable = variable_get('islandora_video_ffmpeg_path', 'ffmpeg');
  $command = "$ffmpeg_executable -i $archival_file -f mp4 -vcodec libx264 -preset medium -acodec libfaac -ab 128k -ac 2 -async 1 -movflags faststart $out_file";
  $return_value = FALSE;
  exec($command, $output, $return_value);

  if ($return_value == '0') {
    drupal_set_message('MP4 created successfully.');
    return $out_file;
  }
  else {
    drupal_set_message('MP4 creation failed.');
  }
  return FALSE;
}

/**
 * This function will create an OGG file for upload.
 *
 * @param string $archival_file
 *   The file to create the ogg from.
 *
 * @return mixed
 *   $out_file the location of the derivative
 *   FALSE on failure.
 */
function islandora_video_create_ogg($archival_file) {
  $archival_file = drupal_realpath($archival_file);
  $out_file = $archival_file . '.ogg';

  $theora_executable = variable_get('islandora_video_ffmpeg2theora_path', 'ffmpeg2theora');
  $command = "$theora_executable $archival_file -o $out_file";
  $return_value = FALSE;
  exec($command, $output, $return_value);

  if ($return_value == '0') {
    drupal_set_message('OGG created successfully.');
    return $out_file;
  }
  else {
    drupal_set_message('OGG creation failed.');
  }
  return FALSE;
}

/**
 * This function will create an MKV file for upload.
 *
 * @param string $archival_file
 *   The file to create the mkv from.
 *
 * @return mixed
 *   $out_file the location of the derivative
 *   FALSE on failure.
 */
function islandora_video_create_mkv($archival_file) {
  $archival_file = drupal_realpath($archival_file);
  $out_file = $archival_file . '.mkv';

  $ffmpeg_executable = variable_get('islandora_video_ffmpeg_path', 'ffmpeg');
  $command = "$ffmpeg_executable -i $archival_file -f mp4 $out_file";
  $return_value = FALSE;
  exec($command, $output, $return_value);

  if ($return_value == '0') {
    drupal_set_message('MKV created successfully.');
    return $out_file;
  }
  else {
    drupal_set_message('MKV creation failed.');
  }
  return FALSE;
}


function islandora_video_create_thumbnail($archival_file) {
  $archival_file = drupal_realpath($archival_file);
  $out_file = $archival_file . '-TN.jpg';

  $ffmpeg_executable = variable_get('islandora_video_ffmpeg_path', 'ffmpeg');

  $ret_value = FALSE;
  $vid_length = "$ffmpeg_executable -i $archival_file 2>&1";
  exec($vid_length, $time_output, $ret_value);

  $dur_match = FALSE;
  $duration = '';
  foreach ($time_output as $key => $value) {
    preg_match('/Duration: (.*), start/', $value, $time_match);
    if (count($time_match)) {
      $dur_match = TRUE;
      $duration = $time_match[1];
      break;
    }
  }

  if ($dur_match) {
    // Snip off the ms because we don't care about them.
    $time_val = preg_replace('/\.(.*)/', '', $duration);
    $time_array = explode(':', $time_val);
    $output_time = floor((($time_array[0] * 360) + ($time_array[1] * 60) + $time_array[2]) / 2);

    $tn_creation = "$ffmpeg_executable -itsoffset -4 -ss $output_time -i $archival_file -vcodec mjpeg -vframes 1 -an -f rawvideo $out_file";

    $return_value = FALSE;
    exec($tn_creation, $output, $return_value);

    if ($return_value == '0') {
      return $out_file;
    }
    else {
      return FALSE;
    }
  }
  return FALSE;
}

/**
 * Adds the file as a datastream to the given object using the
 * given datastream id to identify it.
 *
 * @param FedoraObject $fedora_object
 *   The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 */
function islandora_video_add_datastream($fedora_object, $datastream_id, $file_uri, $label = NULL) {
  try {
    $mime_detector = new MimeDetect();
    $ds = $fedora_object->constructDatastream($datastream_id, "M");
    $ds->label = $label ? $label : $datastream_id;
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri));
    $fedora_object->ingestDatastream($ds);
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
  }
}
