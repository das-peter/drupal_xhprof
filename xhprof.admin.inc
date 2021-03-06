<?php

/**
 * @file
 * Admin page callbacks for the XHProf module.
 */

/**
 * Administrative settings form for XHProf module.
 */
function xhprof_admin_settings() {
  if (xhprof_extension_check()) {
    $description = t('Profile requests with the xhprof PHP extension.');
  }
  else {
    $description = '<span class="warning">' . t('You must enable the <a href="!url">xhprof PHP extension</a> to use this feature.', array(
      '!url' => url('https://www.drupal.org/node/946182'),
    )) . '</span>';
  }

  $form['xhprof_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable profiling of page views and <a href="!drush">drush</a> requests.', array('!drush' => url('http://drush.ws'))),
    '#default_value' => variable_get('xhprof_enabled', FALSE),
    '#description' => $description,
    '#disabled' => !xhprof_extension_check(),
  );
  $form['settings'] = array(
    '#title' => t('Profiling settings'),
    '#type' => 'fieldset',
    '#states' => array(
      'invisible' => array(
        'input[name="xhprof_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['settings']['xhprof_use_cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cookie to toggle xhprof'),
    '#default_value' => variable_get('xhprof_use_cookie', FALSE),
    '#description' => t('If enabled profiling is just enabled if the cookie <em>XHProf_Profile</em> is present. There are handy browser addons for <a href="https://chrome.google.com/webstore/detail/xhprof-helper/adnlhmmjijeflmbmlpmhilkicpnodphi/related">Chrome</a> and <a href="https://addons.mozilla.org/nl/firefox/addon/xhprof-helper-for-firefox">Firefox</a>'),
  );
  $form['settings']['xhprof_flags_cpu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include CPU time in profiling output.'),
    '#description' => t('Tracking CPU time adds some overhead. You may consider disabling this when debugging an issue that is not I/O related.'),
    '#default_value' => variable_get('xhprof_flags_cpu', TRUE),
  );
  $form['settings']['xhprof_flags_memory'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include memory usage in profiling output.'),
    '#description' => t('Tracking memory adds some overhead, but not as much as CPU.'),
    '#default_value' => variable_get('xhprof_flags_memory', TRUE),
  );
  $form['settings']['xhprof_disable_admin_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable profiling of admin pages'),
    '#default_value' => variable_get('xhprof_disable_admin_paths', TRUE),
  );

  if (module_exists('admin_menu')) {
    $form['settings']['xhprof_disable_admin_menu_paths'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable profiling of Admin-Menu paths'),
      '#description' => t('The Admin-Menu module is installed, check this setting to avoid profiling the admin-menu XHR requests.'),
      '#default_value' => variable_get('xhprof_disable_admin_menu_paths', TRUE),
    );
  }

  $form['settings']['xhprof_interval'] = array(
    '#type' => 'textfield',
    '#title' => 'Profiling interval',
    '#default_value' => variable_get('xhprof_interval', ''),
    '#description' => t('The approximate number of requests between XHProf samples. Leave empty to profile all requests'),
  );

  $form['settings']['xhprof_ignore_functions'] = array(
    '#type' => 'textfield',
    '#title' => 'Ignored functions',
    '#default_value' => variable_get('xhprof_ignore_functions', ''),
    '#description' => t('Comma separated list of ignored functions.'),
  );

  $options = drupal_map_assoc(xhprof_get_classes());
  $form['settings']['xhprof_default_class'] = array(
    '#type' => 'radios',
    '#title' => t('XHProf storage'),
    '#default_value' => variable_get('xhprof_default_class', 'XHProfRunsFile'),
    '#options' => $options,
    '#description' => t('Choose an XHProf runs class.'),
  );

  return system_settings_form($form);
}

function xhprof_admin_settings_validate($form, &$form_state) {
  // TODO: Simplify this.
  if (isset($form_state['values']['xhprof_interval']) && $form_state['values']['xhprof_interval'] != '' && (!is_numeric($form_state['values']['xhprof_interval']) || $form_state['values']['xhprof_interval'] <= 0 || $form_state['values']['xhprof_interval'] > mt_getrandmax())) {
    form_set_error('xhprof_interval', 'The profiling interval must be set to a positive integer.');
  }
}
