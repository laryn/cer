<?php

/**
 * Implements hook_cer_entity_insert().
 */
function cer_entity_settings_cer_entity_insert(array &$presets, EntityDrupalWrapper $entity) {
  _cer_entity_settings_process($presets, $entity);
}

/**
 * Implements hook_cer_entity_update().
 */
function cer_entity_settings_cer_entity_update(array &$presets, EntityDrupalWrapper $entity) {
  _cer_entity_settings_process($presets, $entity);
}

/**
 * Implements hook_cer_entity_bulk_update().
 */
function cer_entity_settings_cer_entity_bulk_update(array &$presets, EntityDrupalWrapper $entity) {
  $raw = $entity->value();
  
  $raw->cer = db_select('cer_entity_settings', 'ces')
    ->fields('ces', array(
      'pid',
    ))
    ->condition('entity_type', $entity->type())
    ->condition('entity_id', $entity->getIdentifier())
    ->condition('status', 1)
    ->execute()
    ->fetchCol();

  $entity->set($raw);

  _cer_entity_settings_process(array &$presets, $entity);
}

function _cer_entity_settings_process(array &$presets, EntityDrupalWrapper $entity) {
  $raw = $entity->value();

  if (isset($raw->cer)) {
    foreach (array_keys($presets) as $id) {
      if (! in_array($id, $raw->cer)) {
        unset($presets[$id]);
      }
    }
  }
}
