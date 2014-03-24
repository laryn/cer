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
   * @var CerFieldChainHandler
   */
  protected $references;

  public function __construct(CerPreset $preset, EntityDrupalWrapper $entity) {
    $this->left = $preset->left();
    $this->right = $preset->right();
    $this->entity = $entity;

    $this->references = $this->left->getHandler( $this->entity );
  }

  public function insert(array $IDs = NULL) {
    $refs = (isset($IDs) ? $this->load($IDs) : $this->references);

    foreach ($refs as $ref) {
      $this->right->getHandler( $ref )->add( $this->entity->cer->owner );
    }
  }

  public function update() {
    $original = $this->left->getHandler( $this->entity->cer->original );
    
    $deleted = array_diff($original->getIDs(), $this->references->getIDs());
    if ($deleted) {
      foreach ($this->load($deleted) as $ref) {
        $this->right->getHandler( $ref )->delete( $this->entity->cer->owner );
      }
    }

    $added = array_diff($this->references->getIDs(), $original->getIDs());
    $this->insert($added ? $added : NULL);
  }

  public function delete() {
    foreach ($this->references as $ref) {
      // Without this sanity check, bulk delete is likely to fail with
      // an EntityMalformedException.
      if ($ref) {
        $this->right->getHandler($ref)->delete( $this->entity->cer->owner );
      }
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
