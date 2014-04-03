<?php

/**
 * @file
 *  Contains the CerPresetHandler object.
 */

/**
 * @class
 *  Contains the logic for performing CRUD operations on a single entity,
 *  using a single preset.
 */
class CerPresetHandler {

  /**
   * @var CerFieldChain
   */
  protected $left;
  
  /**
   * @var CerFieldChain
   */
  protected $right;
  
  /**
   * @var EntityDrupalWrapper
   */
  protected $entity;

  /**
   * @var array
   */
  protected $refIDs;

  public function __construct(CerPreset $preset, EntityDrupalWrapper $entity) {
    $this->left = $preset->left;
    $this->right = $preset->right;
    $this->entity = $entity;

    $this->refIDs = $this->left->getHandler( $entity )->getIDs();
  }

  public function insert() {
    foreach ($this->load( $this->refIDs ) as $ref) {
      $this->right->getHandler( $ref )->add( $this->entity->cer->owner );
    }
  }

  public function update() {
    $originalIDs = $this->left->getHandler( $this->entity->cer->original )->getIDs();

    $deleted = array_diff($originalIDs, $this->refIDs);
    if ($deleted) {
      foreach ($this->load( $deleted ) as $ref) {
        $this->right->getHandler( $ref )->delete( $this->entity->cer->owner );
      }
    }

    $added = array_diff($this->refIDs, $originalIDs);
    if ($added) {
      foreach ($this->load( $added ) as $ref) {
        $this->right->getHandler( $ref )->add( $this->entity->cer->owner );
      }
    }
  }

  public function bulk_update() {
    $thisID = $this->entity->getIdentifier();

    foreach ($this->load( $this->refIDs ) as $ref) {
      $handler = $this->right->getHandler( $ref );

      if (! in_array($thisID, $handler->getIDs())) {
        $handler->add( $this->entity );
      }
    }
  }

  public function delete() {
    foreach ($this->load( $this->refIDs ) as $ref) {
      $this->right->getHandler($ref)->delete( $this->entity->cer->owner );
    }
  }
  
  protected function load(array $IDs) {
    $refs = array();

    $entity_type = $this->left->end()->getTargetType();
    $entities = entity_load($entity_type, $IDs);
    foreach ($entities as $entity) {
      $refs[] = new EntityDrupalWrapper($entity_type, $entity);
    }
    
    return $refs;
  }

} 
