<?php

/**
 * @file
 * Contains the CerPreset object.
 */

/**
 * @class
 *  Encapsulates a single CER preset, which consists of two CerFieldChain objects.
 */
class CerPreset {

  /**
   * @var string
   */
  protected $id;

  /**
   * @var CerFieldChain
   */
  protected $left;

  /**
   * @var CerFieldChain
   */
  protected $right;
  
  /**
   * @var boolean
   */
  protected $bidirectional = TRUE;
  
  /**
   * @var boolean
   */
  protected $enabled;

  /**
   * Constructs a CerPreset object.
   *
   * @param $preset
   *  A row from the {cer} database table.
   */
  public function __construct(StdClass $preset = NULL) {
    if ($preset) {
      $this->id = $preset->id;
      $this->bidirectional = $preset->bidirectional;
      $this->enabled = $preset->enabled;
      $this->left(unserialize($preset->a_chain));
      $this->right(unserialize($preset->b_chain));
    }
  }

  public function __get($property) {
    switch ($property) {
      case 'id':
      case 'bidirectional':
      case 'enabled':
        return $this->{$property};

      default:
        break;
    }
  }

  public function __set($property, $value) {
    switch ($property) {
      case 'bidirectional':
      case 'enabled':
        $this->{$property} = (boolean) $value;
        
      default:
        break;
    }
  }

  public function __toString() {
    return "{$this->left}*{$this->right}";
  }

  /**
   * Writes this preset to the database. Only new presets, without an ID,
   * can be written.
   *
   * @param $enabled
   *  Whether or not to enable the preset by default.
   *
   * @return mixed
   *  NULL if the preset was not written, otherwise the result of drupal_write_record().
   */
  public function write($enabled = TRUE) {
    if (empty($this->id)) {
      module_invoke_all('cer_preset_create', $this);

      $preset = new StdClass();

      $preset->id = MD5($this->__toString());
      $preset->a_chain = $this->left();
      $preset->a = $preset->a_chain->__toString();
      $preset->b_chain = $this->right();
      $preset->b = $preset->b_chain->__toString();
      $preset->bidirectional = (int) $this->bidirectional;
      $preset->enabled = $this->enabled = (int) $enabled;

      $written = drupal_write_record('cer', $preset);
      if ($written) {
        $this->id = $preset->id;
      }

      return $written;
    }
  }

  public function toggle() {
    if ($this->id) {
      $fields = array(
        'enabled' => ($this->enabled ? 0 : 1),
      );

      db_update('cer')->fields($fields)->condition('id', $this->id)->execute();

      module_invoke_all('cer_preset_toggle', $this);
    }
  }

  public function delete() {
    if ($this->id) {
      module_invoke_all('cer_preset_delete', $this);

      ctools_include('export');
      ctools_export_crud_delete('cer', $this);
    }
  }

  public function invert() {
    $inverse = new CerPreset();
    
    $inverse->left($this->right());
    $inverse->right($this->left());
    
    return $inverse;
  }

  public function left(CerFieldChain $chain = NULL) {
    if ($chain && empty($this->left)) {
      $this->left = $chain;
    }
    return $this->left;
  }

  public function right(CerFieldChain $chain = NULL) {
    if ($chain && empty($this->right)) {
      $this->right = $chain;
    }
    return $this->right;
  }

}
