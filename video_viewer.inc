<?php

// $Id$
/*
 * Created on 18 March 2011 by Alexander O'Neill and Donald Moses
 */
class IslandoraVideoPlayer {

  private $pid = NULL;

  function __construct($pid) {
    $this->pid = $pid;
  }

  function showM4V() {

    global $base_url;

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $model = new fedora_item($this->pid);
    $ds_list = $model->get_datastreams_list_as_array();

    if (isset($ds_list['MP4'])) { // only display if M4V datastream exists
      $swfURL = $base_url . '/fedora/repository/' . $this->pid . '/MP4/' . $ds_list['MP4']['label'];


      $swfImageURL = $base_url . '/fedora/repository/' . $this->pid . '/TN';

      //PROXY_MP3 is the datastream id
      $path = drupal_get_path('module', 'Fedora_Repository');
      $fullPath = base_path() . $path;
      $content = "";

      //swfobject.js is being loaded from swftools module but we are adding the path to jwplayer.js as it is needed as well
      // if swftools are not enabled uncomment this to here drupal_add_js("sites/all/libraries/swfobject/swfobject.js");
      $pathTojs = "sites/all/libraries/mediaplayer4/jwplayer.js";
      drupal_add_js("$pathTojs");

      //swfobject.js is being loaded from swftools module but we are adding the path to jwplayer.js as it is needed as well
      // if swftools are not enabled uncomment this to here drupal_add_js("sites/all/libraries/swfobject/swfobject.js");
      drupal_add_js("sites/all/libraries/swfobject/swfobject.js");

      $content .= '<video id="flashContent"><a href="http://www.macromedia.com/go/getflashplayer">Get the Flash Player</a> to see this player.</video>';
      $content .= '<div id="dl_link"><a href="' . $swfURL . '">Download MP4</a></div>';


      // player bits

      $js_string = <<<ENDJS

var swfVersionStr = "10.0.0";
var flashvars = { file:escape("$swfURL"),image:escape("$swfImageURL"),autostart:'false' };
var params = { allowfullscreen:'false', allowscriptaccess:'false' };
var attributes = { id:'VideoViewer', name:'VideoViewer' };
swfobject.embedSWF('$base_url/sites/all/libraries/mediaplayer4/player.swf','flashContent','400','200','9.0.115','false',flashvars, params, attributes);
swfobject.createCSS("#flashContent", "display:block;text-align:left;");
ENDJS;
      drupal_add_js($js_string, 'inline', 'header');

      $collection_fieldset = array(
        '#title' => t('Video'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#value' => $content);
      $content = theme('fieldset', $collection_fieldset);
    }
    else {// PROXY_M4V does not exist... likely that video has not been processed yet.
      $content = "<div>The video you uploaded is currently being processed. Please check back shortly.</div>";

      $collection_fieldset = array(
        '#title' => t('Video'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#value' => $content);
      $content = theme('fieldset', $collection_fieldset);
    }

    $tabset['pages_tab'] = array(
      '#type' => 'tabpage',
      '#title' => $model->objectProfile->objLabel,
      '#content' => $content,
    );
    return $tabset;
  }

}

