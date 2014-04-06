<?php

/**
 * @file
 *  Contains CerPresetHandler.
 */

/**
 * @class
 * Contains the logic for performing CER operations on a single entity, 
 * using a single preset.
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

  /**
   * Process an entity insert. This loops through the referenced entity $IDs and
   * adds a reference to this entity if the reference doesn't already have one.
   */
  public function insert(array $IDs = array()) {
    // If no IDs were passed in, use the current reference set.
    $IDs = ($IDs ? $IDs : $this->refIDs);

    // Get this entity's ID right now, so we don't have to keep calling
    // $this->entity->cer->owner->getIdentifier(). Hooray for micro-optimization!
    $myID = $this->entity->cer->owner->getIdentifier();

    foreach ($this->load( $IDs ) as $ref) {
      $handler = $this->right->getHandler( $ref );

      // Only create the backreference if the reference doesn't already reference
      // this entity.
      if (! in_array($myID, $handler->getIDs())) {
        $handler->add( $this->entity->cer->owner );
      }
    }
  }

  /**
   * Process an entity update. This could be either a normal update done by a user,
   * or a bulk update.
   */
  public function update() {
    // Get the previous set of reference IDs. $entity->cer->original will return either
    // $entity->original, if it exists, or the current entity. So, if this is a bulk
    // update, $originalIDs will be identical to $this->refIDs.
    $originalIDs = $this->left->getHandler( $this->entity->cer->original )->getIDs();

    // If there are any references that were in the previous set but not the current
    // set, delete those backreferences. Under normal circumstances, there will be
    // nothing to delete during a bulk update, since the previous set and current
    // set should be identical.
    $deleted = array_diff($originalIDs, $this->refIDs);
    if ($deleted) {
      $this->delete($deleted);
    }

    // If the previous set is identical to the current set, we'll be processing
    // all existing references (see the first line of $this->insert()).
    $added = array_diff($this->refIDs, $originalIDs);
    $this->insert($added);
  }

  /**
   * Process an entity delete. Loops through the referenced entity IDs and clears
   * their references to this entity.
   */
  public function delete(array $IDs = array()) {
    // As with $this->insert(), we can process a specific set of references or
    // everything in the current set.
    $IDs = ($IDs ? $IDs : $this->refIDs);

    foreach ($this->load( $IDs ) as $ref) {
      $this->right->getHandler( $ref )->delete( $this->entity->cer->owner );
    }
  }
  
  /**
   * Loads referenced entities.
   *
   * @param array $IDs
   *  Array of entity IDs to load.
   *
   * @return array
   *  The requested entities, wrapped by EntityDrupalWrapper. If nothing could be
   *  loaded, an empty array is returned.
   */
  protected function load(array $IDs) {
    $refs = array();

    $entity_type = $this->right->end()->instanceInfo('entity_type');
    $entities = entity_load($entity_type, $IDs);
    foreach ($entities as $entity) {
      $entity = new EntityDrupalWrapper($entity_type, $entity);

      if ($this->right->end()->instanceInfo('bundle') == $entity->getBundle()) {
        $refs[] = $entity;
      }
    }
    
    return $refs;
  }

} 
