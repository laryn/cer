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
  public $pid;

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
   * Overrides Entity::__construct().
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'cer');
    $this->wrapper = new EntityDrupalWrapper('cer', $this);
  }

  /**
   * Overrides Entity::label().
   */
  public function label() {
    if ($this->pid) {
      $fields = field_attach_view('cer', $this, 'default');
      return render($fields['cer_left'][0]) . ' <> ' . render($fields['cer_right'][0]);
    }
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
    // Disguise this as a new preset by clearing the ID and setting $this->is_new.
    $this->pid = NULL;
    $this->is_new = TRUE;

    // Regenerate the identifier and reinitialize the wrapper.
    $this->__construct();

    // Swap the left and right fields.
    $buffer = $this->wrapper->cer_left->value();
    $this->wrapper->cer_left->set($this->wrapper->cer_right->value());
    $this->wrapper->cer_right->set($buffer);

    return $this;
  }
}
