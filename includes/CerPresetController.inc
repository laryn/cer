<?php

/**
 * The controller class for CerPreset entities.
 */
class CerPresetController extends EntityAPIControllerExportable {

  /**
   * Overridden.
   */
  public function export($entity, $prefix = '') {
    $variables = get_object_vars($entity);
    unset($variables['pid'], $variables['wrapper'], $variables['status'], $variables['module'], $variables['label_variables'], $variables['uid']);

    return entity_var_json_export($variables, $prefix);
  }

  /**
   * Overridden.
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);

    foreach ($queried_entities as $preset) {
      // Attach variables used to build the human-readable preset label.
      // @see CerPreset::label() and CerPreset::defaultLabel()
      $fields = field_attach_view('cer', $preset, 'default');

      $preset->label_variables = array(
        '@left' =>
          render($fields['cer_left'][0]),
        '@right' =>
          render($fields['cer_right'][0]),
        '!operator' =>
          $preset->wrapper->cer_bidirectional->value() ? '<>' : '>',
      );
    }
  }

}
