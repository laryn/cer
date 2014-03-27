<?php

/**
 * Implements hook_COMPONENT_features_export().
 */
function cer_features_export($data, &$export, $module) {
  $pipe = array();

  $export['dependencies'][] = 'cer';

  foreach (array_map('cer_preset_load', $data) as $preset) {
    $export['features']['cer'][$preset->id] = $preset->id;

    $pipe = array_merge_recursive($pipe, _cer_features_export_chain($preset->left()));
    $pipe = array_merge_recursive($pipe, _cer_features_export_chain($preset->right()));
  }

  return $pipe;
}

function _cer_features_export_chain(CerFieldChain $chain) {
  $pipe = array();
  
  foreach ($chain as $field) {
    $pipe['field_base'][] = $field->fieldInfo('field_name');
    $pipe['field_instance'][] = $field->instanceInfo('entity_type') . '-' . $field->instanceInfo('bundle') . '-' . $field->fieldInfo('field_name');
  }
  
  return $pipe;
}
