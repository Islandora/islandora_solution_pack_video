<?php

class VideoCreation {

  function VideoCreation() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $this->executable = variable_get('islandora_ffm_path', '/usr/local/bin/ffmpeg');
  }

  function createFile($parameterArray = NULL, $dsid, $file, $file_ext) {
    global $base_path;
    if (!variable_get('islandora_video_do_ffm', TRUE)) {
      return;
    }
    $path_info = pathinfo($file);
    $newfile = str_replace('.' . $path_info['extension'], '.m4v', $file);
    $file_suffix = "_$dsid.$file_ext";
    $return_value = TRUE;
    $fullfile = $_SERVER['DOCUMENT_ROOT'] . $base_path . $file;
    $fullfile = str_replace('//', '/', $fullfile);
    $output = array();
    $executable = variable_get('islandora_ffm_path', '/usr/local/bin/ffmpeg');


    $command_string = "$this->executable -i $file -y -f mp4 -vcodec copy -acodec copy  $newfile";
    exec($command_string, $output, $returnValue);

    if ($returnValue == '0') {
      $_SESSION['fedora_ingest_files']["PROXY_M4V"] = $newfile;
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  function get_thumbnail($parameterArray, $dsid, $file, $file_ext) {
    if (!variable_get('islandora_video_do_ffm', TRUE)) {
      return;
    }
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

class VideoExiftool {

  private $pid = NULL;
  private $item = NULL;

  /**
   * Constructor
   * @param type $pid
   */
  function __construct($pid = null) {
    //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $this->pid = $pid;
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if ($pid) {
      $this->item = new Fedora_Item($this->pid);
    }
  }

  /**
   * extract metadata ??
   * @param type $parameterArray
   * @param type $dsid
   * @param type $file
   * @param type $file_ext
   * @return type
   */
  function extractMetadata($parameterArray, $dsid, $file, $file_ext) {
    $path_info = pathinfo($file);
    $newfile = str_replace('.' . $path_info['extension'], '_EXIF.xml', $file);
    $returnValue = TRUE;
    $output = array();
    exec('exiftool -X ' . escapeshellarg($file) . '', $output);
    file_put_contents($newfile, implode("\n", $output));
    $_SESSION['fedora_ingest_files']["$dsid"] = $newfile;
    return TRUE;
  }

  /**
   * display metadata ???
   * @return type
   */
  function displayMetadata() {
    $output = '';
    $exif = $this->item->get_datastream_dissemination('EXIF');
    if (trim($exif) != '') {
      $exifDom = DOMDocument::loadXML($this->item->get_datastream_dissemination('EXIF'));
      if ($exifDom != NULL) {
        $description = $exifDom->getElementsByTagNameNS('http://www.w3.org/1999/02/22-rdf-syntax-ns#', 'Description');
        if ($description->length > 0) {
          $description = $description->item(0);
          $output .= '<div class="fedora_technical_metadata"><ul>';
          for ($i = 0; $i < $description->childNodes->length; $i++) {
            $name = $description->childNodes->item($i)->nodeName;
            $value = $description->childNodes->item($i)->nodeValue;
            if ($name != '#text' && !preg_match('/^System\:.*$/', $name) && trim($value) != '') {
              list($type, $name) = preg_split('/\:/', $name);
              $name = trim(preg_replace('/(?<!^)([A-Z][a-z]|(?<=[a-z])[A-Z])/', " $1", $name));
              $output .= '<li><b>' . $name . '</b>:  ' . $value . ' </li>';
            }
          }
          $output .= '</ul></div>';

          $fieldset = array(
            '#title' => t("!text", array('!text' => 'Technical Metadata')),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#value' => $output
          );
          $output = theme('fieldset', $fieldset);
        }
      }
    }
    return $output;
  }

}