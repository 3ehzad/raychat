<?php

/**
 * @file
 * Administrative page callbacks for the raychat module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 *
 * @param array $form_state
 *   structure associative drupal form array
 * @return array
 */
function raychat_admin_settings_form() {
  $form['raychat'] = array(
    '#type' => 'vertical_tabs',
  );

  // General Settings
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
    '#group' => 'raychat',
  );
  $form['account']['raychat_account'] = array(
    '#type' => 'textfield',
    '#title' => t('raychat token id'),
    '#default_value' => variable_get('raychat_account', ''),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => TRUE,
    '#description' => '<p>' . t('The raychat token id is unique to the websites domain and can be found in the script given to you by the raychat channel settings.') . '</p>' .
                      '<p>' . t('Go to !url, login, click the settings tab and look at the code you are asked to paste into your site.', array('!url' => l('app.raychat.io', 'http://app.raychat.io/', array('attibutes' => array('target' => '_blank'))))) . '</p>',
  );
  // END General Settings

  // Role specific visibility configurations.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific script settings'),
    '#collapsible' => TRUE,
    '#group' => 'raychat',
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['raychat_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Remove script for specific roles'),
    '#default_value' => variable_get('raychat_roles', array()),
    '#options' => $role_options,
    '#description' => t('Remove script only for the selected role(s). If none of the roles are selected, all roles will have the script. Otherwise, any roles selected here will NOT have the script.'),
  );
  // END Role specific visibility configurations.

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific script settings'),
    '#collapsible' => TRUE,
    '#group' => 'raychat',
  );

  $access = user_access('use PHP for raychat visibility');
  $visibility = variable_get('raychat_visibility', 0);
  $pages = variable_get('raychat_pages', '');

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php tags. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['raychat_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add script to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['raychat_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
    );
  }
  // END Page specific visibility configurations.

  return system_settings_form($form);
}

/**
 * Implementation of hook_admin_settings_form_validate().
 *
 * @param array $form
 *   structured associative drupal form array.
 * @param array $form_state
 */
function raychat_admin_settings_form_validate($form, &$form_state) {
  if (empty($form_state['values']['raychat_account'])) {
    form_set_error('raychat_account', t('A valid raychat token id is needed.'));
  }
}
