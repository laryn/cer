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

  protected $left;
  
  protected $right;
  
  protected $entity;

  protected $references;

  public function __construct(CerPreset $preset, EntityDrupalWrapper $entity) {
    $this->left = $preset->left();
    $this->right = $preset->right();
    $this->entity = $entity;

    $this->references = $this->left->getHandler( $this->entity );
  }

  public function insert() {
    foreach ($this->references as $ref) {
      $this->right->getHandler( $ref )->addIfNotContains( $this->entity->cer->owner );
    }
  }

  public function update() {
    $original_left = $this->left->getHandler( $this->entity->cer->original );

    foreach ($original_left as $ref) {
      if (! $this->references->contains($ref)) {
        $this->right->getHandler($ref)->delete( $this->entity->cer->owner );
      }
    }

    $this->insert();
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

} 
