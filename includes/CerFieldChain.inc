<?php

/**
 * @file
 *  Contains the CerFieldChain object.
 */

/**
 * @class
 *  Encapsulates a doubly-linked list of CerField objects.
 */
class CerFieldChain implements Countable, Iterator {

  /**
   * @var array
   *  Every CerField object in the chain.
   */
  protected $chain = array();

  /**
   * Magic post-unserialization callback. Provides every field in the chain
   * with a reference to its parent (if any) and child (if any).
   */
  public function __wakeup() {
    foreach ($this->chain as $field) {
      if (isset($parent)) {
        $field->parent($parent)->child($field);
      }
      $parent = $field;
    }
  }

  /**
   * Generates an identifier for the chain.
   *
   * @return string
   */
  public function __toString() {
    $key = array();

    foreach ($this->chain as $field) {
      $key[] = $field->__toString();
    }

    return implode('::', $key);
  }

  /**
   * Convenience method. Returns a handler for this chain in the context of
   * the given entity.
   *
   * @return CerFieldChainHandler
   */
  public function getHandler($entity) {
    return new CerFieldChainHandler($this, $entity);
  }

  /**
   * Generates a human-readable label for the chain.
   *
   * @return string
   */
  public function getLabel() {
    $label = array();

    foreach ($this->chain as $field) {
      $label[] = $field->getLabel();
    }

    return implode(' &raquo; ', $label);
  }

  public function canReference(CerFieldChain $that) {
    $here = $this->end();

    $that->rewind();
    $there = $that->current();

    return ($here->getTargetType() == $there->instanceInfo('entity_type') && in_array($there->instanceInfo('bundle'), $here->getTargetBundles()));
  }

  public function addField(CerField $field, array &$completed = NULL) {
    array_unshift($this->chain, $field);

    if (isset($completed)) {
      $parents = $field->getParents();

      if ($parents) {
        foreach ($parents as $parent) {
          $branch = clone $this;
          $branch->addField($parent, $completed);
        }
      }
      else {
        $completed[] = $this;
      }
    }
  }
  
  public function end() {
    return end($this->chain);
  }
  
  /**
   * Implements Countable::count().
   */
  public function count() {
    return sizeof($this->chain);
  }

  /**
   * Implements Iterator::current().
   */
  public function current() {
    return current($this->chain);
  }
  
  /**
   * Implements Iterator::key().
   */
  public function key() {
    return $this->current()->__toString();
  }
  
  /**
   * Implements Iterator::next().
   */
  public function next() {
    next($this->chain);
  }
  
  /**
   * Implements Iterator::rewind().
   */
  public function rewind() {
    reset($this->chain);
  }

  /**
   * Implements Iterator::valid().
   */
  public function valid() {
    return ($this->current() instanceof CerField);
  }

} 
 