<?php

class VideoCreation {

  function VideoCreation() {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $this->executable = variable_get('islandora_ffm_path', '/usr/local/bin/ffmpeg');
  }

  function createFile($parameterArray = NULL, $dsid, $file, $file_ext) {
    global $base_path;

    //removes spaces from filename

    $old_filename = pathinfo($file, PATHINFO_FILENAME);
    $new_file_name = preg_replace("/[^a-z\.\d]/i", "", $old_filename);
    if ($new_file_name != $old_filename) {
      $original = $file;
      $new_fullname = str_replace($old_filename, $new_file_name, $file);
      $success = rename("$file", "$new_fullname");
      $file = $new_fullname;
      $changed = TRUE;
    }
    $success = FALSE;

    if (variable_get('islandora_video_do_ffm', TRUE)) {


      $path_info = pathinfo($file);

      //makes mp4 file, adds file in ingest list
      $mp4file = str_replace('.' . $path_info['extension'], '_.mp4', $file);
      $command_string = "$this->executable -i $file -f mp4 -vcodec libx264 -vpre normal -acodec libfaac -ab 128k -ac 2 -async 1 $mp4file";
      exec($command_string, $output, $returnValue);

      if ($returnValue == '0') {
        $_SESSION['fedora_ingest_files']["MP4"] = $mp4file;
        $success = TRUE;
      }

      //makes mkv file, adds file in ingest list
      $mkvfile = str_replace('.' . $path_info['extension'], '_.mkv', $file);
      $command_string = "$this->executable -i $file -f mp4 $mkvfile";
      exec($command_string, $output, $returnValue);

      if ($returnValue == '0') {
        $_SESSION['fedora_ingest_files']["MKV"] = $mkvfile;
        $success = TRUE;
      }
    }

    if (variable_get('islandora_video_do_theora', TRUE)) {
      //makes ogg file, adds file in ingest list
      $theora = variable_get('islandora_theora_path', '/usr/local/bin/ffmpeg2theora');
      $oggfile = str_replace('.' . $path_info['extension'], '_.ogg', $file);
      $command_string = "$theora $file -o $oggfile";
      exec($command_string, $output, $returnValue);

      if ($returnValue == '0') {
        $_SESSION['fedora_ingest_files']["OGG"] = $oggfile;
        $success = TRUE;
      }
    }
    // restores original filename, allowing succesful comletion of conetnt model rules
    if ($changed) {
      rename($file, $original);
    }
    return $success;
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
  }

}

class VideoExiftool {

  private $pid = NULL;
  private $item = NULL;

  /**
   * Constructor
   * @param string $pid
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
   * @param array $parameterArray
   * @param string $dsid
   * @param string $file
   * @param string $file_ext
   * @return boolean
   */
  function extractMetadata($parameterArray, $dsid, $file, $file_ext) {
    if (!variable_get('islandora_video_do_exif', TRUE)) {
      return;
    }
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
   * display metadata 
   * @return html
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