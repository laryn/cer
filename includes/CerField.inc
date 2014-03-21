<?php

/**
 * @file
 *  Contains the CerField object.
 */

/**
 * @class
 *  Encapsulates a single field instance.
 */
abstract class CerField {

  /**
   * @var string
   *  The entity type this instance is bound to.
   */
  protected $entityType;
  
  /**
   * @var string
   *  The bundle this instance is bound to.
   */
  protected $bundle;
  
  /**
   * @var string
   *  The internal name of the field.
   */
  protected $field;
  
  /**
   * @var CerField
   *  The field instance which contains this one, if any.
   */
  protected $parent;
  
  /**
   * @var CerField
   *  Field instance contained by this one, if any.
   */
  protected $child;
  
  abstract public function getTargetType();

  /**
   * Constructor. Pretty self-explanatory!
   */
  public function __construct($entityType, $bundle, $field) {
    $this->entityType = $entityType;
    $this->bundle = $bundle;
    $this->field = $field;
  }

  /**
   * Generates an identifier for this field instance.
   * 
   * @return string
   */
  public function __toString() {
    return "{$this->entityType}:{$this->bundle}:{$this->field}";
  }

  /**
   * Magic pre-serialization callback.
   */
  public function __sleep() {
    return array('entityType', 'bundle', 'field');
  }

  /**
   * Returns a CerFieldHandler subclass instance for the given entity.
   *
   * @param object $entity
   *  The entity to be wrapped by the handler.
   *
   * @return CerFieldHandler
   */
  public function getHandler(EntityDrupalWrapper $entity) {
    return new CerFieldHandler($this, $entity);
  }

  /**
   * Returns the bundles that this field instance can reference.
   *
   * @return array
   */
  public function getTargetBundles() {
    $info = entity_get_info($this->getTargetType());
    return array_keys($info['bundles']);
  }

  /**
   * Check if this field is instantiated on a field collection.
   *
   * @return boolean
   */
  public function inFieldCollection() {
    return ($this->instanceInfo('entity_type') == 'field_collection_item');
  }
  
  /**
   * Gets or sets the parent of this field.
   *
   * @param CerField
   *  (optional) The field to set as this one's parent.
   *
   * @return CerField
   */
  public function parent(CerField $parent = NULL) {
    if ($parent) {
      $this->parent = $parent;
    }
    return $this->parent;
  }

  /**
   * Gets or sets the child of this field.
   *
   * @param CerField
   *  (optional) The field to set as this one's child.
   *
   * @return CerField
   */
  public function child(CerField $child = NULL) {
    if ($child) {
      $this->child = $child;
    }
    return $this->child;
  }

  /**
   * Gets information about this field type.
   *
   * @param [string...]
   *  Array keys to fetch, or nothing to return all field type info.
   *
   * @return mixed
   */
  public function fieldTypeInfo() {
    $info = field_info_field_types($this->fieldInfo('type'));
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * Gets information about the entity type this field instance is bound to.
   *
   * @param [string...]
   *  Array keys to fetch, or nothing to fetch all entity type info.
   *
   * @return mixed
   */
  public function entityInfo() {
    $info = entity_get_info($this->entityType);
    return drupal_array_get_nested_value($info, func_get_args());
  }
  
  /**
   * Gets information about the global field definition.
   *
   * @param [string...]
   *  Array keys to fetch, or nothing to fetch all field info.
   *
   * @return mixed
   */
  public function fieldInfo() {
    $info = field_info_field($this->field);
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * Gets information about the field instance definition.
   *
   * @param [...]
   *  The array keys to fetch, or nothing to fetch all instance info.
   *
   * @return mixed
   */
  public function instanceInfo() {
    $info = field_info_instance($this->entityType, $this->field, $this->bundle);
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * Gets the language codes this field is available in (will always contain at
   * least LANGUAGE_NONE).
   *
   * @return array
   */
  public function languages() {
    return field_available_languages($this->entityType, $this->fieldInfo());
  }

  /**
   * Gets the parents of this field instance. Modules should implement
   * hook_query_cer_field_parents_alter() and set up query conditions,
   * as well as the 'class' metadata containing the name of the CerField
   * subclass which will wrap the parent field(s).
   *
   * @return array
   *  Array of CerField objects, or empty if there are no parents.
   */
  public function getParents() {
    $parents = array();

    $query = db_select('field_config_instance', 'fci')
      ->fields('fci', array(
        'entity_type', 'bundle', 'field_name',
      ))
      ->addTag('cer_field_parents')
      ->addMetaData('field', $this);

    $result = $query->execute();

    if ($class = $query->getMetaData('class')) {
      foreach ($result as $row) {
        $parents[] = new $class($row->entity_type, $row->bundle, $row->field_name);
      }
    }

    return $parents;
  }

  /**
   * Generates a human-readable label for this field instance. By default,
   * this is in the form of Entity Type >> Bundle >> Label.
   *
   * @return string
   */
  public function getLabel() {
    $label = '';

    if (! $this->inFieldCollection()) {
      $label .= $this->entityInfo('label');
      $label .= ' &raquo; ';

      if ($this->entityInfo('entity keys', 'bundle')) {
        $label .= $this->entityInfo('bundles', $this->bundle, 'label');
        $label .= ' &raquo; ';
      }
    }    
    $label .= $this->instanceInfo('label');

    return $label;
  }

}
