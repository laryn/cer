<?php

/**
 * Implements hook_COMPONENT_features_export().
 */
function cer_features_export($data, &$export, $module) {
  $pipe = array();

  $export['dependencies']['cer'] = 'cer';

  foreach (array_map('cer_preset_load', $data) as $preset) {
    $export['features']['cer'][$preset->id] = $preset->id;
    
    foreach ($preset->left as $field) {
      $pipe = array_merge_recursive($pipe, $field->getExportPipe($export));
    }
    foreach ($preset->right as $field) {
      $pipe = array_merge_recursive($pipe, $field->getExportPipe($export));
    }
  }

  return $pipe;
}
