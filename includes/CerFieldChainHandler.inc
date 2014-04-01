<?php

/**
 * @file
 * Contains the CerFieldChainHandler object.
 */

class CerFieldChainHandler {

  protected $chain;

  protected $entity;

  protected $handlers;

  public function __construct(CerFieldChain $chain, EntityDrupalWrapper $entity) {
    $this->chain = $chain;
    $this->entity = $entity;

    $chain->__wakeup();
    $chain->seek($entity->cer->depth->value());

    $field = $chain->current();

    if ($field->child()) {
      $this->handlers = new RecursiveIteratorIterator(new CerEndPointIterator($field, $entity));
    }
    else {
      $this->handlers = array( $field->getHandler($entity) );
    }
  }

  public function getIDs() {
    $IDs = array();

    foreach ($this->handlers as $handler) {
      $IDs = array_merge($handler->getIDs(), $IDs);
    }

    return array_unique($IDs);
  }

  public function add(EntityDrupalWrapper $entity) {
    $owner = $this->entity;

    foreach ($this->chain as $field) {
      if ($field instanceof CerEntityContainerInterface) {
        $items = $field->getHandler($owner);

        if (sizeof($items) == 0) {
          $owner = $field->createInnerEntity($owner);
        }
        else {
          $items->seek(-1);
          $owner = $items->current();
        }
      }
    }

    $field->getHandler($owner)->add($entity);
  }

  public function delete(EntityDrupalWrapper $entity) {
    foreach ($this->handlers as $handler) {
      $handler->delete($entity);
    }
  }

}
