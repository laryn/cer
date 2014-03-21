<?php

/**
 * @file
 * Contains the CerEndPointIterator object.
 */

class CerEndPointIterator implements RecursiveIterator {

  protected $field;

  protected $handler;

  public function __construct(CerField $field, EntityDrupalWrapper $entity) {
    $this->field = $field;
    $this->handler = $field->getHandler($entity);
  }

  public function current() {
    return $this->field->child()->getHandler($this->handler->current());
  }

  public function key() {
    return $this->handler->key();
  }

  public function next() {
    $this->handler->next();
  }

  public function rewind() {
    $this->handler->rewind();
  }

  public function valid() {
    return $this->handler->valid();
  }

  public function hasChildren() {
    return ($this->field->child() instanceof CerFieldCollectionField);
  }

  public function getChildren() {
    return new CerEndPointIterator($this->field->child(), $this->handler->current());
  }

}
