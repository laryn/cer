<?php

/**
 * @file
 * Contains the CerFieldHandler object.
 */
 
abstract class CerFieldHandler implements Countable, SeekableIterator {

  /**
   * @var CerField
   */
  protected $field;

  /**
   * @var object
   */
  protected $entity;

  /**
   * @var integer
   */
  protected $delta = 0;

  /**
   * @var array
   */
  protected $items;

  /**
   * @var string
   */
  protected $language = LANGUAGE_NONE;

  abstract public function contains($entity);

  abstract public function add($entity);

  abstract public function delete($entity);

  public function __construct(CerField $field, $entity) {
    $this->field = $field;

    if ($field->fieldInfo('translatable')) {
      $language = entity_language($field->instanceInfo('entity_type'), $entity);
      
      if (in_array($language, $field->languages())) {
        $this->language = $language;
      }
    }
    $this->items = (array) @$entity->{ $field->fieldInfo('field_name') }[$this->language];

    $this->entity = $entity;
  }

  protected function validate($entity) {
    $cardinality = $this->field->fieldInfo('cardinality');

    if ($cardinality == FIELD_CARDINALITY_UNLIMITED) {
      return TRUE;
    }
    else {
      if ($cardinality > $this->count()) {
        return TRUE;
      }
      else {
        $this->logError('Cannot add reference to !that_link from !field_label on !this_link because there are no more slots available.', $entity);
        return FALSE;
      }
    }
  }

  protected function write() {
    $entity_type = $this->field->instanceInfo('entity_type');
    list($entityID, ,) = entity_extract_IDs($entity_type, $this->entity);

    $this->entity->{ $this->field->fieldInfo('field_name') }[$this->language] = $this->items;

    field_attach_presave($entity_type, $this->entity);
    field_attach_update($entity_type, $this->entity);

    entity_get_controller($entity_type)->resetCache((array) $entityID);

    $entities = entity_load($entity_type, (array) $entityID);
    $this->__construct($this->field, $entities[$entityID]);

    $this->rewind();
  }

  protected function logError($message, $entity = NULL) {
    $variables = array(
      '!field_name' => $this->field->fieldInfo('field_name'),
      '!field_type' => $this->field->fieldTypeInfo('label'),
      '!field_label' => $this->field->instanceInfo('label'),
    );

    $variables['!this_type'] = $this->field->instanceInfo('entity_type');
    $variables['!this_label'] = entity_label($variables['!this_type'], $this->entity);

    $uri = entity_uri($variables['!this_type'], $this->entity);
    $variables['!this_uri'] = $uri['path'];
    $variables['!this_link'] = l($variables['!this_label'], $uri['path'], $uri['options']);

    if ($entity) {
      $variables['!that_type'] = $this->field->getTargetType();
      $variables['!that_label'] = entity_label($variables['!that_type'], $entity);

      $uri = entity_uri($variables['!that_type'], $entity);
      $variables['!that_uri'] = $uri['path'];
      $variables['!that_link'] = l($variables['!that_label'], $uri['path'], $uri['options']);
    }

    watchdog('cer', $message, $variables, WATCHDOG_ERROR);

    if (variable_get('cer_debug', FALSE)) {
      drupal_set_message(t($message, $variables), 'error');
    }
  }

  public function count() {
    return sizeof($this->items);
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
 