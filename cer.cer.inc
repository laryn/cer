<?php

/**
 * @file
 * Contains CER hook implementations.
 */

/**
 * Implements hook_cer_preset_create().
 */
function cer_cer_preset_create(CerPreset $preset) {
  // Prevents a race condition when deleting Commerce products which are referred to
  // by Product Reference fields managed by CER.
  if ($preset->right()->end() instanceof CerCommerceProductReferenceField) {
    drupal_set_message(t('In order to prevent a race condition, bidirectionality has been disabled for this preset.'), 'warning');
    $preset->bidirectional = FALSE;
  }
}

/**
 * Implements hook_cer_fields().
 */
function cer_cer_fields() {
  $fields = array();

  if (module_exists('taxonomy')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('taxonomy_term_reference', 'CerTaxonomyTermReferenceField'));
  }
  if (module_exists('entityreference')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('entityreference', 'CerEntityReferenceField'));
  }
  if (module_exists('node_reference')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('node_reference', 'CerNodeReferenceField'));
  }
  if (module_exists('user_reference')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('user_reference', 'CerUserReferenceField'));
  }
  if (module_exists('commerce_product_reference')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('commerce_product_reference', 'CerCommerceProductReferenceField'));
  }
  if (module_exists('file')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('file', 'CerFileField'));
  }
  if (module_exists('image')) {
    $fields = array_merge($fields, _cer_collect_fields_of_type('image', 'CerFileField'));
  }

  return $fields;
}

function _cer_collect_fields_of_type($field_type, $class) {
  $fields = array();

  $result = db_query("SELECT CONCAT_WS(':', fci.entity_type, fci.bundle, fci.field_name) FROM {field_config_instance} fci INNER JOIN {field_config} fc ON fc.id = fci.field_id WHERE fc.type = :type", array(':type' => $field_type))->fetchCol();
  foreach ($result as $identifier) {
    $fields[$identifier]['class'] = $class;
  }

  return $fields;
}
