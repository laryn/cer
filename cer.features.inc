<?php

/**
 * Implements hook_COMPONENT_features_export_options().
 */
function cer_features_export_options() {
  $options = array();
  
  foreach (cer_preset_load() as $preset) {
    $options[$preset->id] = sprintf('%s <> %s', html_entity_decode($preset->left()->getLabel()), html_entity_decode($preset->right()->getLabel()));
  }

  return $options;
}

/**
 * Implements hook_COMPONENT_features_export().
 */
function cer_features_export(array $data, array &$export, $module) {
  $pipe = array();

  // Obviously, CER itself is a dependency for exporting CER presets :)
  $export['dependencies']['cer'] = 'cer';

  foreach ($data as $id) {
    $preset = cer_preset_load($id);
    _cer_features_export_chain($preset->left(), $pipe);
    _cer_features_export_chain($preset->right(), $pipe);

    $export['features']['cer'][] = $id;
  }

  return $pipe;
}

function _cer_features_export_chain(CerFieldChain $chain, array &$pipe) {
  foreach ($chain as $field) {
    $key = $field->instanceInfo('entity_type') . '-' . $field->instanceInfo('bundle') . '-' . $field->fieldInfo('field_name');
    $pipe['field_instance'][$key] = $key;
  }
}

/**
 * Implements hook_COMPONENT_features_export_render().
 */
function cer_features_export_render($module, array $data, array $export = NULL) {
  $code = array();
  $code[] = "  \$presets = array();\n\n";

  foreach ($data as $id) {
    $preset = cer_preset_load($id);

    $code[] = "  // {$preset}\n";
    $code[] = "  \$preset = new " . get_class($preset) . "();\n\n";
    $code = array_merge($code, _cer_features_export_render_chain($preset->left()));
    $code[] = "  \$preset->left(\$chain);\n\n";
    $code = array_merge($code, _cer_features_export_render_chain($preset->right()));
    $code[] = "  \$preset->right(\$chain);\n\n";
    $code[] = "  \$presets[] = \$preset;\n";
  }

  $code[] = "\n";
  $code[] = "  return \$presets;";

  return array('cer_default_presets' => implode('', $code));
}

function _cer_features_export_render_chain(CerFieldChain $chain) {
  $code = array();
  $code[] = "  \$chain = new " . get_class($chain) . "();\n";

  $fields = array();
  foreach ($chain as $field) {
    $fields[] = sprintf("  \$chain->addField(new %s('%s', '%s', '%s'));\n", get_class($field), $field->instanceInfo('entity_type'), $field->instanceInfo('bundle'), $field->fieldInfo('field_name'));
  }

  return array_merge($code, array_reverse($fields));
}
