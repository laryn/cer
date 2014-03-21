<?php

/**
 * @file
 * Contains the CerFieldChainHandler object.
 */

class CerFieldChainHandler implements IteratorAggregate {

  protected $chain;

  protected $entity;

  protected $handlers;

  public function __construct(CerFieldChain $chain, CerEntity $entity) {
    $this->chain = $chain;
    $this->entity = $entity;

    $chain->__wakeup();
    $chain->seek($entity->depth());

    $field = $chain->current();

    if ($chain->recursible()) {
      $this->handlers = new RecursiveIteratorIterator(new CerEndPointIterator($field, $entity));
    }
    else {
      $this->handlers = array( $field->getHandler($entity) );
    }
  }

  /**
   * Implements IteratorAggregate::getIterator().
   */
  public function getIterator() {
    $iterator = new AppendIterator();

    foreach ($this->handlers as $handler) {
      $iterator->append($handler);
    }

    return $iterator;
  }

  public function contains(CerEntity $entity) {
    foreach ($this->handlers as $handler) {
      if ($handler->contains($entity)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  public function addIfNotContains(CerEntity $entity) {
    if (! $this->contains($entity)) {
      $this->add($entity);
    }
  }

  public function add(CerEntity $entity) {
    $owner = $this->entity;

    foreach ($this->chain as $field) {
      if ($field instanceof CerFieldCollectionField) {
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

  public function delete(CerEntity $entity) {
    foreach ($this->handlers as $handler) {
      $handler->delete($entity);
    }
  }

}
