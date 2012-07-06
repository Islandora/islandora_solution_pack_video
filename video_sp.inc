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
    $clean_name = preg_replace('/^form-.*?_/', '', $old_filename);
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
      $mp4file = $clean_name . '_.mp4';
      $command_string = "$this->executable -i $file -f mp4 -vcodec libx264 -vpre normal -acodec libfaac -ab 128k -ac 2 -async 1 $mp4file";
      exec($command_string, $output, $returnValue);

      if ($returnValue == '0') {
        $_SESSION['fedora_ingest_files']["MP4"] = $mp4file;
        drupal_set_message('MP4 created successfully');
        $success = TRUE;
      }

      //makes mkv file, adds file in ingest list
      if (variable_get('video_make_archive', TRUE)) {
        $mkvfile = $clean_name . '_.mkv';
        $command_string = "$this->executable -i $file -f mp4 $mkvfile";
        exec($command_string, $output, $returnValue);

        if ($returnValue == '0') {
          $_SESSION['fedora_ingest_files']["MKV"] = $mkvfile;
          drupal_set_message('MKV created successfully');
          $success = TRUE;
        }
      }
    }

    if (variable_get('islandora_video_do_theora', TRUE)) {
      //makes ogg file, adds file in ingest list
      $theora = variable_get('islandora_theora_path', '/usr/local/bin/ffmpeg2theora');
      $oggfile = $clean_name . '_.ogg';
      $command_string = "$theora $file -o $oggfile";
      exec($command_string, $output, $returnValue);

      if ($returnValue == '0') {
        $_SESSION['fedora_ingest_files']["OGG"] = $oggfile;
        drupal_set_message('OGG created successfully');
        $success = TRUE;
      }
    }
    // restores original filename, allowing succesful comletion of content model rules
    if ($changed) {
      rename($file, $original);
    }
    return $success;
  }

  function get_thumbnail($parameterArray, $dsid, $file, $file_ext) {


    $stored_thumbnail = drupal_get_path('module', 'islandora_video_sp') . '/images/Crystal_Clear_app_camera.png';
    $copied = file_copy($stored_thumbnail, file_directory_path());
    $thumbnail = file_directory_path() . '/' . 'Crystal_Clear_app_camera.png';

    $_SESSION['fedora_ingest_files']["TN"] = $thumbnail;
    return TRUE;
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

    $old_filename = pathinfo($file, PATHINFO_FILENAME);
    $new_file_name = preg_replace("/[^a-z\.\d]/i", "", $old_filename);
    $clean_name = preg_replace('/\/form-.*?_/', '/', $file);
    $clean_name .= "_EXIF.xml";
    drupal_set_message("clean name is $clean_name");
    $returnValue = TRUE;
    $output = array();
    exec('exiftool -X ' . escapeshellarg($file) . '', $output);
    file_put_contents($clean_name, implode("\n", $output));
    $_SESSION['fedora_ingest_files']["$dsid"] = $clean_name;
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