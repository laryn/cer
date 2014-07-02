<?php

class FieldInstance {

  protected $entityType;

  protected $bundle;

  protected $name;

  protected $isBundleable;

  protected $entityTypeLabel;

  protected $bundleLabel;

  protected $cardinality;

  protected $label;

  protected $parent;

  protected $child;

  public function __construct($entity_type, $bundle, $field_name) {
    $this->entityType = $entity_type;
    $this->bundle = $bundle;
    $this->name = $field_name;

    // Get info about the entity type and bundle hosting this field instance.
    $info = entity_get_info($entity_type);
    $this->isBundleable = (boolean) $info['entity keys']['bundle'];
    $this->entityTypeLabel = $info['label'];
    $this->bundleLabel = $info['bundles'][$bundle]['label'];

    // Get global info about the field.
    $info = field_info_field($field_name);
    $this->cardinality = $info['cardinality'];

    // Finally, get info about the field instance.
    $instance = field_info_instance($entity_type, $field_name, $bundle);
    $this->label = $instance['label'];
  }

  public function __get($property) {
    return $this->{$property};
  }

  public function __toString() {
    return "{$this->entityType}:{$this->bundle}:{$this->name}";
  }

  public function parent(FieldInstance $parent = NULL) {
    if ($parent) {
      $this->parent = $parent;
    }
    return $this->parent;
  }

  public function child(FieldInstance $child = NULL) {
    if ($child) {
      $this->child = $child;
    }
    return $this->child;
  }

  public function requireParent() {
    return FALSE;
  }

  public function getParents() {
    return array();
  }

}
