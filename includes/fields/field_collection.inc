<?php

/**
 * @file
 * Contains the CerFieldCollectionField object.
 */

class CerFieldCollectionField extends CerField {

  public function getTargetType() {
    return 'field_collection_item';
  }

  public function createInnerEntity(EntityDrupalWrapper $owner) {
    $collection = new EntityDrupalWrapper('field_collection_item', entity_create('field_collection_item', array('field_name' => $this->field)));
    $collection->host_entity->set( $owner );
    $collection->save(TRUE);

    $this->getHandler($owner)->add($collection);

    return $collection;
  }

}
