<?php

class VideoCreation {

  function VideoCreation() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $this->executable = variable_get('islandora_ffm_path', '/usr/local/bin/ffmpeg');
  }

  function createFile($parameterArray = NULL, $dsid, $file, $file_ext) {
    global $base_path;
    $file_suffix = "_$dsid.$file_ext";
    $return_value = TRUE;
    $fullfile = $_SERVER['DOCUMENT_ROOT'] . $base_path . $file;
    $fullfile = str_replace('//', '/', $fullfile);
    $output = array();
    $executable = variable_get('islandora_ffm_path', '/usr/local/bin/ffmpeg');


    $command_string = "$this->executable -i $file -y -f mp4 -vcodec copy -acodec copy  ${file}.m4v";
    exec($command_string, $output, $returnValue);

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["PROXY_M4V"] = $file . '.m4v';
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  function get_thumbnail($parameterArray, $dsid, $file, $file_ext) {
    $defaults = array('ss' => '00:00:10', 's' => NULL);
    $params = array_merge($defaults, $parameterArray);
    $system = getenv('System');
    $file_suffix = '_' . $dsid . '.' . $file_ext;
    $returnValue = TRUE;
    $output = array();
    $size = '';
    $file = $_SERVER['DOCUMENT_ROOT'] . $base_path . $file;
    $file = str_replace('//', '/', $file);

    if ($params['s'] != NULL) {
      $size = ' -s ' . escapeshellarg($params['s']);
    }
    $command = "$this->executable -i " . escapeshellarg($file) . ' -r 1 -ss ' . escapeshellarg($params['ss']) . ' ' . $size . ' -t 1 ' . escapeshellarg($file . $file_suffix);
    exec("$this->executable -i " . escapeshellarg($file) . ' -r 1 -ss ' . escapeshellarg($params['ss']) . ' ' . $size . ' -t 1 ' . escapeshellarg($file . $file_suffix));

    if (!file_exists($file . $file_suffix)) {
      $stored_thumbnail = drupal_get_path('module', 'islandora_video_sp') . '/images/Crystal_Clear_app_camera.png';
      file_copy($stored_thumbnail, file_directory_path());
      $thumbnail = file_directory_path() . '/' . 'Crystal_Clear_app_camera.png';
      $_SESSION['fedora_ingest_files']["TN"] = $thumbnail;
      return TRUE;
    }

    $_SESSION['fedora_ingest_files']["$dsid"] = $file . $file_suffix;
    return TRUE;
  }

}

