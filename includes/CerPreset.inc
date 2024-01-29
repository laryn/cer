<?php

/**
 * @file
 * Contains the entity class for CER presets.
 */

class CerPreset extends Entity {

  /**
   * @var integer
   * The preset's numeric ID in the database.
   */
  public $pid = 0;

  /**
   * @var string
   * The export identifier, in the format $this->cer_left*$this->cer_right.
   */
  public $identifier;

  /**
   * @var EntityMetadataWrapper
   * A metadata wrapper around this preset, for convenience.
   */
  public $wrapper;

  /**
   * @var integer
   * The exportable status of this preset.
   */
  public $status = 0x01;  // ENTITY_CUSTOM

  /**
   * @var string
   * The module exporting this preset.
   */
  public $module = 'cer';

  /**
   * Implements EntityInterface::id().
   */
  public function id()
  {
    return $this->identifier;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType()
  {
    return 'cer';
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri()
  {
    return array(
      'path' => 'admin/config/content/cer/manage/' . $this->identifier,
    );
  }
  /**
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'cer');
    $this->wrapper = new EntityBackdropWrapper('cer', $this);
  }

  /**
   * Overrides Entity::label().
   */
  public function label() {
    return isset($this->label_variables) ? t('@left !operator @right', $this->label_variables) : $this->defaultLabel();
  }

  /**
   * Overrides Entity::save().
   */
  public function save() {
    // Generate the export identifier automagically before saving.
    $this->identifier = $this->wrapper->cer_left->value() . '*' . $this->wrapper->cer_right->value();
    parent::save();
  }

  public function invert() {
    $init = get_object_vars($this);

    unset($init['pid'], $init['wrapper'], $init['identifier'], $init['status']);

    $buf = $init['cer_left'];
    $init['cer_left'] = $init['cer_right'];
    $init['cer_right'] = $buf;

    return entity_create('cer', $init);
  }
}
