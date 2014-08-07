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
    return t('@left !operator @right', $this->labelVariables());
  }

  /**
   * Gets variables used in the label. The only reason this is a separate method
   * is because the variables are needed in other contexts, like the main
   * preset management page.
   */
  public function labelVariables($key = NULL) {
    // I kinda doubt this is very performant, but for now it'll do.
    $fields = field_attach_view('cer', $this, 'default');

    $variables = array(
      '@left' =>
        render($fields['cer_left'][0]),
      '@right' =>
        render($fields['cer_right'][0]),
      '!operator' =>
        $this->wrapper->cer_bidirectional->value() ? '<>' : '>',
    );

    return isset($key) ? $variables[$key] : $variables;
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
