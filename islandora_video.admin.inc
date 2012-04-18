<?php

/**
 * islandora video admin settings
 * @param array $form_state
 * @return array
 */
function islandora_video_admin_settings(&$form_state) {


  $ffm_path = isset($form_state['values']['islandora_ffm_path']) ? $form_state['values']['islandora_ffm_path'] : variable_get('islandora_ffm_path', '/usr/bin/ffmpeg');
  $ffm_avail = exec("ls $ffm_path");

  $confirmation_message = ($ffm_avail ? '<img src="' . url('misc/watchdog-ok.png') . '"/>'
          . t('FFMpeg found at !ffm_path', array('!ffm_path' => $ffm_path)) : '<img src="'
          . url('misc/watchdog-error.png') . '"/> '
          . t('Unable to locate FFMpeg at !ffm_path</p>', array('!ffm_path' => $ffm_path)));

  $ffm = isset($form_state['values']['islandora_video_do_ffm']) ? $form_state['values']['islandora_video_do_ffm'] : variable_get('islandora_video_do_ffm', TRUE);

  $form = array();
  $form['video_ahah_wrapper'] = array(
    '#prefix' => '<div id="ivideo-url">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['video_ahah_wrapper']['islandora_video_do_ffm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transform files locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => $ffm,
    '#ahah' => array(
      'path' => 'islandora/video/ffm',
      'wrapper' => 'ivideo-url',
      'effect' => 'fade',
      'event' => 'change'),
  );

  if ($ffm) {
    $form['video_ahah_wrapper']['islandora_ffm_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to ffm executable'),
      '#description' => t('Path to ffm program on your server'),
      '#default_value' => $ffm_path,
      '#ahah' => array(
        'path' => 'islandora/video/ffm',
        'wrapper' => 'ivideo-url',
        'effect' => 'fade',
        'event' => 'change'),
    );
    $form['video_ahah_wrapper']['infobox'] = array(
      '#type' => 'item',
      '#value' => $confirmation_message,
    );
  }

  //  check for exiftool


  $exif_path = isset($form_state['values']['islandora_exif_path']) ? $form_state['values']['islandora_exif_path'] : variable_get('islandora_exif_path', '/usr/bin/exiftool');
  $exif_avail = exec("ls $exif_path");

  $exif_confirmation_message = ($exif_avail ? '<img src="' . url('misc/watchdog-ok.png') . '"/>'
          . t('Exiftool found at !exif_path', array('!exif_path' => $exif_path)) : '<img src="'
          . url('misc/watchdog-error.png') . '"/> '
          . t('Unable to locate Exiftool at !exif_path</p>', array('!exif_path' => $exif_path)));

  $exif = isset($form_state['values']['islandora_video_do_exif']) ? $form_state['values']['islandora_video_do_exif'] : variable_get('islandora_video_do_exif', TRUE);

  $form['video_ahah_exif_wrapper'] = array(
    '#prefix' => '<div id="ivideo-exif-url">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['video_ahah_exif_wrapper']['islandora_video_do_exif'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extract Exif data locally ?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => $exif,
    '#ahah' => array(
      'path' => 'islandora/video/exif',
      'wrapper' => 'ivideo-exif-url',
      'effect' => 'fade',
      'event' => 'change'),
  );

  if ($exif) {
    $form['video_ahah_exif_wrapper']['islandora_exif_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to Exiftool executable'),
      '#description' => t('Path to exiftool program on your server'),
      '#default_value' => $exif_path,
      '#ahah' => array(
        'path' => 'islandora/video/exif',
        'wrapper' => 'ivideo-exif-url',
        'effect' => 'fade',
        'event' => 'change'),
    );
    $form['video_ahah_exif_wrapper']['infobox'] = array(
      '#type' => 'item',
      '#value' => $exif_confirmation_message,
    );
  }


  //check for ffmpeg2theora
  $theora_path = isset($form_state['values']['islandora_theora_path']) ? $form_state['values']['islandora_theora_path'] : variable_get('islandora_theora_path', '/usr/local/bin/ffmpeg2theora');
  $theora_avail = exec("ls $theora_path");

  $theora_confirmation_message = ($theora_avail ? '<img src="' . url('misc/watchdog-ok.png') . '"/>'
          . t('Theora2ffmepg found at !theora_path', array('!theora_path' => $theora_path)) : '<img src="'
          . url('misc/watchdog-error.png') . '"/> '
          . t('Unable to locate Theora2ffmpeg at !theora_path</p>', array('!theora_path' => $theora_path)));

  $theora = isset($form_state['values']['islandora_video_do_theora']) ? $form_state['values']['islandora_video_do_theora'] : variable_get('islandora_video_do_theora', TRUE);

  $form['video_ahah_theora_wrapper'] = array(
    '#prefix' => '<div id="ivideo-theora-url">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  $form['video_ahah_theora_wrapper']['islandora_video_do_theora'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create OGG locally?'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => $theora,
    '#ahah' => array(
      'path' => 'islandora/video/theora',
      'wrapper' => 'ivideo-theora-url',
      'effect' => 'fade',
      'event' => 'change'),
  );

  if ($theora) {
    $form['video_ahah_theora_wrapper']['islandora_theora_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to Theora executable'),
      '#description' => t('Path to theora program on your server'),
      '#default_value' => $theora_path,
      '#ahah' => array(
        'path' => 'islandora/video/theora',
        'wrapper' => 'ivideo-theora-url',
        'effect' => 'fade',
        'event' => 'change'),
    );
    $form['video_ahah_theora_wrapper']['infobox'] = array(
      '#type' => 'item',
      '#value' => $theora_confirmation_message,
    );
  }

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));
  $form['#submit'][] = 'video_settings_form_submit';
  $form['#theme'] = 'system_settings_form';
  return ($form);
}

/**
 * update ffm div
 */
function update_ffm_div() {

  $form = video_callback_prep();
  $changed_elements = $form['video_ahah_wrapper'];
  unset($changed_elements['#prefix'], $changed_elements['#suffix']);
  $output = theme('status_messages') . drupal_render($changed_elements);
  drupal_json(array(
    'status' => TRUE,
    'data' => $output,
  ));
}

/**
 * update exif div
 */
function update_exif_div() {

  $form = video_callback_prep();
  $changed_elements = $form['video_ahah_exif_wrapper'];
  unset($changed_elements['#prefix'], $changed_elements['#suffix']);
  $output = theme('status_messages') . drupal_render($changed_elements);
  drupal_json(array(
    'status' => TRUE,
    'data' => $output,
  ));
}

/**
 * update exif div
 */
function update_theora_div() {

  $form = video_callback_prep();
  $changed_elements = $form['video_ahah_theora_wrapper'];
  unset($changed_elements['#prefix'], $changed_elements['#suffix']);
  $output = theme('status_messages') . drupal_render($changed_elements);
  drupal_json(array(
    'status' => TRUE,
    'data' => $output,
  ));
}


/**
 * book callback prep
 * @return type 
 */
function video_callback_prep() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'rebuild' => TRUE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  // Enable the submit/validate handlers to determine whether AHAH-submittted.
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  return $form;
}

/**
 * book settings form submit ??
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function video_settings_form_submit($form, &$form_state) {

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($form_state['ahah_submission'] == TRUE) {
    $form_state['rebuild'] = TRUE;
    return;
  }
  if ($form_state['clicked_button']['#id'] != 'edit-submit' && $op != t('Reset to defaults')) {
    $form_state['rebuild'] = TRUE;
    return;
  }


  // Exclude unnecessary elements.
  unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);

  foreach ($form_state['values'] as $key => $value) {
    if ($op == t('Reset to defaults')) {
      variable_del($key);
    }
    else {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      variable_set($key, $value);
    }
  }
  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    drupal_set_message(t('The Islandora Video configuration options have been saved.'));
  }

  cache_clear_all();
  drupal_rebuild_theme_registry();
}

/**
 * video admin refresh
 * @param array $form
 * @param array $form_state 
 */
function video_admin_refresh($form, &$form_state) {
  $values = $form_state['values'];
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['rebuild'] = TRUE;
}

