<?php

/**
 * @file
 * Contains the CerHierarchy object.
 */

class CerHierarchy implements Countable {

  /**
   * @var string
   *  Cache ID for this hierarchy.
   */
  protected $cacheID;
  
  /**
   * @var array
   *  The flattened hierarchy data.
   */
  protected $data = array();

  const ROOT = 'root';

  /**
   * Constructor.
   */
  public function __construct($cacheID) {
    $this->cacheID = $cacheID;
  }

  public function __get($property) {
    return $this->{$property};
  }

  /**
   * Returns a form element for this hierarchy.
   *
   * @return array
   */
  public function build() {
    $class = variable_get('cer_hierarchy_widget', 'CerHierarchySelectWidget');
    $controller = new $class($this);
    
    $element = $controller->build();
    $element['#controller'] = $controller;

    return $element;
  }

  /**
   * Writes this hierarchy into the cache.
   */
  public function cache() {
    cache_set($this->cacheID, $this, 'cache', CACHE_TEMPORARY);
  }

  /**
   * Deletes this hierarchy from the cache.
   */
  public function clean() {
    cache_clear_all($this->cacheID, 'cache');
  }

  public function add($item_key, $label = NULL, $parent = CerHierarchy::ROOT) {
    if (!array_key_exists($item_key, $this->data)) {
      $this->data[$item_key]['label'] = isset($label) ? $label : $item_key;
    }

    if (!isset($this->data[$parent]['children'])) {
      $this->data[$parent]['children'] = array();
    }

    if (!in_array($item_key, $this->data[$parent]['children'])) {
      $this->data[$parent]['children'][] = $item_key;
    }
  }

  public function addField(CerField $field) {
    $bundle_key = "{$field->entityType}:{$field->bundle}";

    if ($field->entityInfo('entity keys', 'bundle')) {
      $this->add($field->entityType, $field->entityInfo('label'));
      $this->add($bundle_key, $field->entityInfo('bundles', $field->bundle, 'label'), $field->entityType);
    }
    else {
      $this->add($bundle_key, $field->entityInfo('label'));
    }

    $field_key = "{$bundle_key}:{$field->name}";
    $this->add($field_key, $field->label, $bundle_key);
  }

  public function addChain(CerFieldChain $chain) {
    $parents = array();

    foreach ($chain as $field) {
      if ($field->plugin['require parent']) {
        $parent_key = implode('::', $parents);
        $field_key = "{$parent_key}::{$field}";

        $this->add($field_key, $field->label, $parent_key);
      }
      else {
        $this->addField($field);
      }

      $parents[] = $field->__toString();
    }
  }

  /**
   * Returns a cached FieldHierarchy object, or a new instance if there is no cached
   * instance with the given cache ID.
   */
  public static function get($cacheID) {
    if ($c = cache_get($cacheID)) {
      return $c->data;
    }
    else {
      $class = __CLASS__;
      return new $class($cacheID);
    }
  }

  /**
   * @implements Countable::count().
   */  
  public function count() {
    return sizeof($this->data);
  }

}
