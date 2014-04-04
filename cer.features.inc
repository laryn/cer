<?php

/**
 * Implements component hook_features_export().
 */
function cer_features_export($data, &$export, $module) {
  $pipe = array();

  $export['dependencies']['cer'] = 'cer';

  ctools_include('export');
  $presets = ctools_export_crud_load_multiple('cer', $data);

  foreach ($presets as $preset) {
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
