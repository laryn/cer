<?php

/**
 * Implements hook_views_default_views().
 */
function cer_views_default_views() {
  $view = new view();
  $view->name = 'cer_presets';
  $view->description = '';
  $view->tag = 'CER';
  $view->base_table = 'cer_preset';
  $view->human_name = 'CER Presets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Corresponding Entity Reference';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer cer settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<p>No presets have been created.</p>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Bulk operations: CER Preset */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'cer_preset';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Delete selected presets',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::cer_disable_preset' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Disable selected presets',
    ),
    'rules_component::cer_enable_preset' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Enable selected presets',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::cer_invert_preset' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::cer_make_preset_bidirectional' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Make selected presets bi-directional',
    ),
    'rules_component::cer_make_preset_one_directional' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Make selected presets one-directional',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 0,
        'display_values' => array(),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'rules_component::cer_replace_preset_with_inverse' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Invert selected presets',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: CER Preset: Enabled */
  $handler->display->display_options['fields']['cer_enabled']['id'] = 'cer_enabled';
  $handler->display->display_options['fields']['cer_enabled']['table'] = 'field_data_cer_enabled';
  $handler->display->display_options['fields']['cer_enabled']['field'] = 'cer_enabled';
  $handler->display->display_options['fields']['cer_enabled']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cer_enabled']['alter']['text'] = '&#10003;';
  $handler->display->display_options['fields']['cer_enabled']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['cer_enabled']['empty_zero'] = TRUE;
  /* Field: CER Preset: Left Field */
  $handler->display->display_options['fields']['cer_left']['id'] = 'cer_left';
  $handler->display->display_options['fields']['cer_left']['table'] = 'field_data_cer_left';
  $handler->display->display_options['fields']['cer_left']['field'] = 'cer_left';
  $handler->display->display_options['fields']['cer_left']['click_sort_column'] = 'fiid';
  $handler->display->display_options['fields']['cer_left']['type'] = 'field_reference_label';
  $handler->display->display_options['fields']['cer_left']['group_column'] = '';
  /* Field: CER Preset: Right Field */
  $handler->display->display_options['fields']['cer_right']['id'] = 'cer_right';
  $handler->display->display_options['fields']['cer_right']['table'] = 'field_data_cer_right';
  $handler->display->display_options['fields']['cer_right']['field'] = 'cer_right';
  $handler->display->display_options['fields']['cer_right']['click_sort_column'] = 'fiid';
  $handler->display->display_options['fields']['cer_right']['type'] = 'field_reference_label';
  $handler->display->display_options['fields']['cer_right']['group_column'] = '';
  /* Field: CER Preset: Bidirectional */
  $handler->display->display_options['fields']['cer_bidirectional']['id'] = 'cer_bidirectional';
  $handler->display->display_options['fields']['cer_bidirectional']['table'] = 'field_data_cer_bidirectional';
  $handler->display->display_options['fields']['cer_bidirectional']['field'] = 'cer_bidirectional';
  $handler->display->display_options['fields']['cer_bidirectional']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['cer_bidirectional']['alter']['text'] = '&#10003;';
  $handler->display->display_options['fields']['cer_bidirectional']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['cer_bidirectional']['empty_zero'] = TRUE;
  /* Sort criterion: CER Preset: Weight (cer_weight) */
  $handler->display->display_options['sorts']['cer_weight_value']['id'] = 'cer_weight_value';
  $handler->display->display_options['sorts']['cer_weight_value']['table'] = 'field_data_cer_weight';
  $handler->display->display_options['sorts']['cer_weight_value']['field'] = 'cer_weight_value';
  $handler->display->display_options['sorts']['cer_weight_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['cer_weight_value']['expose']['label'] = 'Weight';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/content/cer';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'CER';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $export[$view->name] = $view;

  return $export;
}
