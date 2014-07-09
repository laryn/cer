<?php

class CerPresetSelectionHandler implements EntityReference_SelectionHandler {

  private $entity;

  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new CerPresetSelectionHandler($entity_type, $entity);
  }

  public function __construct($entity_type, $entity) {
    if ($entity_type && $entity) {
      $this->entity = new EntityDrupalWrapper($entity_type, $entity);
    }
  }

  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {
    $options = array();

    if ($this->entity) {
      $lineage = $this->entity->cer->lineage->value();

      $baseQuery = new EntityFieldQuery();
      $baseQuery
        ->entityCondition('entity_type', 'cer')
        ->fieldCondition('cer_enabled', 'value', TRUE);

      $query = clone $baseQuery;
      $result = $query
        ->fieldCondition('cer_left', 'path', $lineage, 'STARTS_WITH')
        ->execute();

      if (isset($result['cer'])) {
        foreach (entity_load('cer', array_keys($result['cer'])) as $preset) {
          $variables = $preset->labelVariables();
          $options['cer'][$preset->pid] = $variables['@right'];
        }
      }

      $query = clone $baseQuery;
      $result = $query
        ->fieldCondition('cer_bidirectional', 'value', TRUE)
        ->fieldCondition('cer_right', 'path', $lineage, 'STARTS_WITH')
        ->execute();

      if (isset($result['cer'])) {
        foreach (entity_load('cer', array_keys($result['cer'])) as $preset) {
          $variables = $preset->labelVariables();
          $options['cer'][$preset->pid] = $variables['@left'];
        }
      }
    }
    
    return $options;
  }

  public function countReferencableEntities($match = NULL, $match_operator = 'CONTAINS') {
    return sizeof($this->getReferencableEntities());
  }

  public function validateReferencableEntities(array $IDs) {
    // Don't bother validating preset IDs.
    return $IDs;
  }

  public function validateAutocompleteInput($input, &$element, &$form_state, $form) {
    return NULL;
  }

  public function entityFieldQueryAlter(SelectQueryInterface $query) {
    // NOP
  }

  public function getLabel($entity) {
    return entity_label('cer', $entity);
  }

  public static function settingsForm($field, $instance) {
    return array();
  }

}
