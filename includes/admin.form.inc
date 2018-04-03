<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_video_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form = array();
  // Get viewer table.
  $viewer_table = islandora_viewers_form('islandora_video_viewers', 'video/mp4');
  $form += $viewer_table;

  // Viewer stuff.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_video_admin_submit'),
  );

  // Playback options.
  $form['playback'] = array(
    '#type' => 'fieldset',
    '#title' => t('Playback'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $description = t("When Off (default), only the MP4 datastream will be played in a viewer (if a viewer is configured above). With this box checked, the uploaded file (OBJ datastream) will be played if:");
  $description .= '<ul><li>' . t('the MP4 datastream is not present') . '</li>';
  $description .= '<li>' . t('the OBJ datastream has mimetype video/mp4') . '</li>';
  $description .= '<li>' . t('the OBJ datastream is smaller than the size configured below:') . '</li></ul>';
  $form['playback']['islandora_video_play_obj'] = array(
    '#type' => 'checkbox',
    '#title' => t('Play OBJ datastream in viewer'),
    '#description' => $description,
    '#default_value' => variable_get('islandora_video_play_obj', TRUE),
  );
  $form['playback']['islandora_video_max_obj_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum file size for playing OBJ (in MB)'),
    '#description' => t("Set a maximum size (in megabytes) for sending the OBJ datastream to the player."),
    '#default_value' => variable_get('islandora_video_max_obj_size', '500'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_video_play_obj"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Derivative Options.
  $form['derivatives'] = array(
    '#type' => 'fieldset',
    '#title' => t('Derivatives'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // FFMPEG configuration.
  $form['derivatives']['islandora_video_ffmpeg_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to FFmpeg'),
    '#default_value' => variable_get('islandora_video_ffmpeg_path', 'ffmpeg'),
    '#description' => t('Path to the FFmpeg binary. For example: <kbd>/usr/local/bin/ffmpeg</kbd>  . Used if creating TN, MP4, or MKV derivatives.'),
    '#required' => TRUE,
  );
  // TN derivatives.
  $form['derivatives']['islandora_video_make_thumbnail_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create TN datastream locally'),
    '#description' => t('If On (default), the Drupal server will use FFmpeg to create a thumbnail screenshot from the video, or fall back to a default thumbnail. If Off, no thumbnail will be created.'),
    '#default_value' => variable_get('islandora_video_make_thumbnail_locally', TRUE),
  );
  // MP4 derivatives.
  $form['derivatives']['islandora_video_make_mp4_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create MP4 datastream locally'),
    '#description' => t('If On (default), the Drupal server will use FFmpeg to create a web-friendly mp4 version of the uploaded file. Disable if another server creates derivatives, or if you intend to play the uploaded files directly (see Playback options above). This feature requires a valid MP4 audio codec.'),
    '#default_value' => variable_get('islandora_video_make_mp4_locally', TRUE),
  );
  $form['derivatives']['islandora_video_mp4_audio_codec'] = array(
    '#type' => 'textfield',
    '#title' => t('MP4 audio codec'),
    '#description' => t("Defaults to libfaac, a non-free encoder. FFmpeg must have been compiled from source with that encoder enabled. See !FFMPEG for more info.",
      array(
        '!FFMPEG' => l(t("FFmpeg's AAC encoding guide"), 'https://trac.ffmpeg.org/wiki/Encode/AAC'),
      )),
    '#default_value' => variable_get('islandora_video_mp4_audio_codec', 'libfaac'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_video_make_mp4_locally"]' => array('checked' => TRUE),
      ),
    ),
  );
  // MKV derivatives.
  $form['derivatives']['islandora_video_make_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create MKV datastream locally'),
    '#description' => t('If On (default), the Drupal server will use FFmpeg to create a Matroska container derivative. This may be useful for archival purposes, but is not used by Islandora.'),
    '#default_value' => variable_get('islandora_video_make_archive', TRUE),
  );
  // OGG/Theora derivatives.
  $form['derivatives']['islandora_video_make_ogg_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create OGG datastream locally'),
    '#description' => t("If On (default is Off), the Drupal server will use ffmpeg2theora to create an Ogg Theora container derivative. This is a legacy option only, as this datastream is no longer used by Islandora."),
    '#default_value' => variable_get('islandora_video_make_ogg_locally', FALSE),
  );
  $form['derivatives']['islandora_video_ffmpeg2theora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to ffmpeg2theora executable'),
    '#description' => t('Path to Theora conversion program on your server'),
    '#default_value' => variable_get('islandora_video_ffmpeg2theora_path', 'ffmpeg2theora'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_video_make_ogg_locally"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Submit function for the system_settings_form to reset config variables.
 */
function islandora_video_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_video_viewers');
      variable_del('islandora_video_ffmpeg_path');
      variable_del('islandora_video_make_archive');
      variable_del('islandora_video_make_mp4_locally');
      variable_del('islandora_video_make_ogg_locally');
      variable_del('islandora_video_ffmpeg2theora_path');
      variable_del('islandora_video_mp4_audio_codec');
      variable_del('islandora_video_play_obj');
      variable_del('islandora_video_max_obj_size');
      break;
  }
}

/**
 * Form validation ensuring the selected ffmpeg options are available.
 */
function islandora_video_admin_validate($form, &$form_state) {
  // Don't validate if resetting.
  $op = $form_state['clicked_button']['#id'];
  if ($op == 'edit-reset') {
    return;
  }
  // Ensure the mp4 audio codec is present if MP4 derivative is enabled.
  if ($form_state['values']['islandora_video_make_mp4_locally'] == TRUE) {
    // If no value is given, set the form to use and validate the default.
    $raw_value = $form_state['values']['islandora_video_mp4_audio_codec'];
    $codec = strtolower(trim($raw_value));
    if (!$codec) {
      $codec = 'libfaac';
    }
    $form_state['values']['islandora_video_mp4_audio_codec'] = $codec;
    if (preg_match('/[^0-9a-zA-Z_-]/', $codec) === 1) {
      form_set_error('islandora_video_mp4_audio_codec', "The value entered for MP4 audio codec contains forbidden characters.");
      return;
    }

    // Test that FFMPEG path is valid.
    $ffmpeg = ($form_state['values']['islandora_video_ffmpeg_path'] !== variable_get('islandora_video_ffmpeg_path', 'ffmpeg') ? $form_state['values']['islandora_video_ffmpeg_path'] : variable_get('islandora_video_ffmpeg_path', 'ffmpeg'));
    if (preg_match('/[^0-9a-zA-Z\\/\\\\_-]/', $ffmpeg) === 1) {
      form_set_error('islandora_video_ffmpeg_path', "The value entered for FFmpeg path contains forbidden characters.");
      return;
    }
    $command = $ffmpeg . ' -version';
    exec($command, $output, $ret);
    if ($ret !== 0) {
      form_set_error('islandora_video_ffmpeg_path', "FFmpeg was not found. A valid FFmpeg path is required if MP4 derivative generation is enabled.");
    }
    else {
      // Test that the specified codec is present.
      unset($output);
      $command = $ffmpeg . ' -encoders 2>/dev/null | grep "^ ...... ' . $codec . ' "';
      exec($command, $output, $ret);
      if (!$output) {
        form_set_error('islandora_video_mp4_audio_codec', 'The selected MP4 codec was not found in ffmpeg. Try using aac or enable the desired codec.');
      }
    }
  }
}
