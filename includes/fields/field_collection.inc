<?php

/**
 * @file
 * Contains the CER plugin for Field Collection fields.
 */

class CerFieldCollectionField extends CerField {

  public function getTargetType() {
    return 'field_collection_item';
  }

  /**
   * This method is unique to this plugin because field collections need special
   * handling. This creates a new field collection that is owned by $owner, in case
   * we need to create a reference on a field collection field which hasn't got
   * any items yet.
   */
  public function createInnerEntity(EntityDrupalWrapper $owner) {
    // Create an empty field collection item.
    $collection = new EntityDrupalWrapper('field_collection_item', entity_create('field_collection_item', array('field_name' => $this->field)));
    $collection->host_entity->set( $owner );
    $collection->save(TRUE);

    // 'Reference' the newly created field collection item.
    $this->getHandler($owner)->add($collection);

    return $collection;
  }

}
