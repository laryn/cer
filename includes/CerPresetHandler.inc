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

  protected $preset;
  
  protected $entity;

  protected $references;

  public function __construct(CerPreset $preset, $entity) {
    $this->preset = $preset;
    $this->entity = $entity;

    $this->references = $preset->left()->getHandler($entity);
  }

  public function insert() {
    foreach ($this->references as $ref) {
      $this->preset->right()->getHandler($ref)->addIfNotContains($this->entity);
    }
  }

  public function update() {
    if (isset($this->entity->original)) {
      $original_left = $this->preset->left()->getHandler($this->entity->original);

      foreach ($original_left as $ref) {
        if (! $this->references->contains($ref)) {
          $this->preset->right()->getHandler($ref)->delete($this->entity);
        }
      }
    }

    $this->insert();
  }

  public function delete() {
    foreach ($this->references as $ref) {
      // Without this sanity check, bulk delete is likely to fail with
      // an EntityMalformedException.
      if ($ref) {
        $this->preset->right()->getHandler($ref)->delete($this->entity);
      }
    }
  }

} 
