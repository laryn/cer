<?php

/**
 * @file
 * Contains a CER field plugin for referencing Profile2 entities. These references
 * are not actually fields at all, so this plugin is really more of a bridge to fool
 * CER into thinking that Profile2 is a reference field.
 */

class CerProfile2Field extends CerField implements CerEntityContainerInterface {

  protected $profileType;

  public function __construct($entity_type, $bundle, $field) {
    parent::__construct($entity_type, $bundle, $field);
    $this->profileType = subStr($field, 9);
  }

  public function getTargetType() {
    return 'profile2';
  }

  public function getTargetBundles() {
    return (array) $this->profileType;
  }

  public function fieldTypeInfo() {
    $info = array(
      'label' => t('Profile2'),
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  public function fieldInfo() {
    $info = array(
      'field_name' => $this->field,
      'cardinality' => 1,
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  public function instanceInfo() {
    $info = array(
      'entity_type' => $this->entityType,
      'bundle' => $this->bundle,
      'field_name' => $this->field,
      'label' => profile2_get_types($this->profileType)->label,
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * Implements CerEntityContainerInterface::createInnerEntity().
   */
  public function createInnerEntity(EntityDrupalWrapper $owner) {
    // @todo
  }

}
