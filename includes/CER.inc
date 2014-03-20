<?php

/**
 * @file
 *  Contains the CER object.
 */

/**
 * @class
 *  Encapsulates the logic to load and iterate over all presets relating
 *  to a given entity.
 */
class CER implements Iterator {

  /**
   * @var array
   *  The entity whose references will be processed. In preset terminology,
   *  this is the 'left' entity.
   */
  protected $entity;

  /**
   * @var integer
   *  Index of the current preset.
   */
  protected $index = 0;

  /**
   * @var array
   *  All loaded CerPreset objects.
   */
  protected $presets = array();

  public function __construct($entity_type, $entity) {
    if (defined('MAINTENANCE_MODE')) {
      drupal_set_message(t('Refusing to initialize CER when global MAINTENANCE_MODE flag is set.'), 'warning');
      return;
    }

    $this->entity = new CerEntity($entity_type, $entity);

    $baseQuery = db_select('cer')->fields('cer')->condition('enabled', 1);

    $a = clone $baseQuery;
    $result = $a
      ->condition('a', $this->entity->__toString(), 'LIKE')
      ->execute();

    foreach ($result as $row) {
      $this->presets[] = new CerPreset($row);
    }

    $b = clone $baseQuery;
    $result = $b
      ->condition('b', $this->entity->__toString(), 'LIKE')
      ->condition('bidirectional', 1)
      ->execute();

    foreach ($result as $row) {
      $preset = new CerPreset($row);
      $this->presets[] = $preset->invert();
    }
  }

  /**
   * Implements Iterator::current().
   */
  public function current() {
    return new CerPresetHandler($this->presets[$this->index], $this->entity->value());
  }

  /**
   * Implements Iterator::key().
   */
  public function key() {
    return $this->presets[$this->index]->__toString();
  }

  /**
   * Implements Iterator::next().
   */
  public function next() {
    $this->index++;
  }

  /**
   * Implements Iterator::rewind().
   */
  public function rewind() {
    $this->index = 0;
  }

  /**
   * Implements Iterator::valid().
   */
  public function valid() {
    return ($this->index < sizeof($this->presets) && $this->presets[$this->index] instanceof CerPreset);
  }

}
