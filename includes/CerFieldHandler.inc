<?php

/**
 * @file
 * Contains CerFieldHandler.
 */
 
/**
 * @class
 * Handles low-level operations for a single field on a single entity. Exposes
 * methods to add, delete and check for references. This will also iterate over
 * the references, returning each one in a CerEntity wrapper.
 */
class CerFieldHandler implements Countable, SeekableIterator {

  /**
   * @var CerField
   */
  protected $field;

  /**
   * @var EntityDrupalWrapper
   */
  protected $entity;

  /**
   * @var EntityMetadataWrapper
   */
  protected $value;

  /**
   * @var integer
   */
  protected $delta = 0;
  
  /**
   * @var boolean
   */
  protected $isMultiValue;

  public function __construct(CerField $field, EntityDrupalWrapper $entity) {
    $this->field = $field;
    $this->entity = $entity;
    $this->value = $entity->{ $field->fieldInfo('field_name') };
    $this->isMultiValue = ($this->value instanceof EntityListWrapper);
  }

  public function contains(EntityDrupalWrapper $entity) {
    $entityID = $entity->getIdentifier();
    
    if ($this->isMultiValue) {
      foreach ($this->value as $ref) {
        if ($entityID == $ref->getIdentifier()) {
          return TRUE;
        }
      }
      return FALSE;
    }
    else {
      return ($entityID == $this->value->getIdentifier());
    }
  }

  public function add(EntityDrupalWrapper $entity) {
    if ($this->validate($entity)) {
      $this->write();
    }
  }

  public function delete(EntityDrupalWrapper $entity) {
    $entityID = $entity->getIdentifier();

    if ($this->isMultiValue) {
      foreach ($this->value as $delta => $ref) {
        if ($entityID == $ref->getIdentifier()) {
          $this->value[$delta]->set(NULL);
        }
      }
    }
    elseif ($entityID == $this->value->getIdentifier()) {
      $this->value->set(NULL);
    }

    $this->write();
  }
  
  protected function validate(EntityDrupalWrapper $entity) {
    // Before we do anything else, check that the field has enough space to add the
    // reference. If there isn't, bail out so we don't blindly overwrite existing
    // field data.
    if ($this->checkCardinality()) {
      // Keep the previous value so we can restore it if validation fails.
      $prev_value = $this->value->value();

      if ($this->isMultiValue) {
        $value = $this->value->value();
        $value[] = $entity->value();
        $this->value->set($value);
      }
      else {
        $this->value->set( $entity->value() );
      }

      // Leverage the Field Attach API to validate the reference.
      try {
        // Only validate this field.
        field_attach_validate($this->entity->type(), $this->entity->value(), array('field_name' => $this->field->fieldInfo('field_name')));

        return TRUE;
      }
      catch (FieldValidationException $e) {
        foreach ($e->errors as $field) {
          foreach ($field as $language) {
            foreach ($language as $error) {
              $this->logError($error, $entity);
            }
          }
        }
        $this->value->set($prev_value);
      }
    }
    else {
      $this->logError('Cannot add reference to !that_link from !field_label on !this_link because there are no more slots available.', $entity);
    }

    return FALSE;
  }

  protected function checkCardinality() {
    $cardinality = $this->field->fieldInfo('cardinality');

    if ($cardinality == FIELD_CARDINALITY_UNLIMITED) {
      return TRUE;
    }
    else {
      return ($cardinality > $this->count());
    }
  }

  protected function write() {
    $entity_type = $this->entity->type();
    $entityID = $this->entity->getIdentifier();
    $entity = $this->entity->value();

    field_attach_presave($entity_type, $entity);
    field_attach_update($entity_type, $entity);

    entity_get_controller($entity_type)->resetCache((array) $entityID);

    $entities = entity_load($entity_type, (array) $entityID);
    $this->__construct($this->field, new CerEntity($entity_type, $entities[$entityID]));

    $this->rewind();
  }

  protected function logError($message, EntityDrupalWrapper $entity = NULL) {
    $variables = array(
      '!field_name' => $this->field->fieldInfo('field_name'),
      '!field_type' => $this->field->fieldTypeInfo('label'),
      '!field_label' => $this->field->instanceInfo('label'),
    );

    $variables['!this_type'] = $this->entity->type();
    $variables['!this_label'] = $this->entity->label();

    $variables['!this_url'] = $this->entity->url->value();
    $variables['!this_link'] = '<a href="' . $variables['!this_url'] . '">' . $variables['!this_label'] . '</a>';

    if ($entity) {
      $variables['!that_type'] = $entity->type();
      $variables['!that_label'] = $entity->label();

      $variables['!that_url'] = $entity->url->value();
      $variables['!that_link'] = '<a href="' . $variables['!that_url'] . '">' . $variables['!that_label'] . '</a>';
    }

    watchdog('cer', $message, $variables, WATCHDOG_ERROR);

    if (variable_get('cer_debug', FALSE)) {
      drupal_set_message(t($message, $variables), 'error');
    }
  }

  public function count() {
    if ($this->isMultiValue) {
      return sizeof($this->value);
    }
    else {
      return ($this->value->value() ? 1 : 0);
    }
  }

  public function seek($position) {
    $length = $this->count();

    if ($position < 0) {
      $position += $length;
    }

    if ($position >= 0 && $position < $length) {
      $this->delta = $position;
    }
    else {
      throw new OutOfBoundsException(t('Cannot seek to invalid position.'));
    }
  }

  public function current() {
    $entity = ($this->isMultiValue ? $this->value[$this->delta] : $this->value);

    return new CerEntity($this->field->getTargetType(), $entity->value());
  }

  public function key() {
    return $this->delta;
  }

  public function next() {
    $this->delta++;
  }
  
  public function rewind() {
    $this->delta = 0;
  }

  public function valid() {
    return ($this->delta < $this->count());
  }

}
 