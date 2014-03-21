<?php

/**
 * @file
 * Contains callback functions for computed properties we expose to Entity API.
 */

/**
 * Gets the lineage of the entity as a string, in the format
 * entity_type:bundle:field::...
 */
function cer_get_entity_lineage($entity, array $options, $property, $entity_type, $info) {
  return implode( '::', _cer_get_entity_lineage($entity_type, $entity) );
}

/**
 * Gets the entity depth as an integer. Currently, this only applies to field
 * collections.
 */
function cer_get_entity_depth($entity, array $options, $property, $entity_type, $info) {
  if ($entity_type == 'field_collection_item') {
    return sizeof( _cer_get_entity_lineage($entity_type, $entity) );
  }
  else {
    return 0;
  }
}

/**
 * Gets the ultimate owner of the entity as an EntityDrupalWrapper. Currently,
 * this only applies to field collections.
 */
function cer_get_entity_owner($entity, array $options, $property, $entity_type, $info) {
  if ($entity_type == 'field_collection_item') {
    return cer_get_entity_owner($entity->hostEntity(), $options, $property, $entity->hostEntityType(), $info);
  }
  else {
    return new EntityDrupalWrapper($entity_type, $entity);
  }
}

/**
 * Recursively computes the lineage of an entity and returns it as an array of
 * strings in the format entity_type:bundle:field.
 */
function _cer_get_entity_lineage($entity_type, $entity, array $chain = array()) {
  list(, , $bundle) = entity_extract_IDs($entity_type, $entity);

  if (! $chain) {
    $chain[0] = "{$entity_type}:{$bundle}:%";
  }

  if ($entity_type == 'field_collection_item') {
    $host_type = $entity->hostEntityType();
    $host = $entity->hostEntity();
    list(, , $host_bundle) = entity_extract_IDs($host_type, $host);
    array_unshift($chain, "{$host_type}:{$host_bundle}:{$bundle}");

    return _cer_get_entity_lineage($host_type, $host);
  }
  else {
    return $chain;
  }
}
