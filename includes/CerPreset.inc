<?php

/**
 * @file
 * Contains the CerPreset object.
 */

/**
 * @class
 *  Encapsulates a single CER preset, which consists of two CerFieldChain objects.
 */
class CerPreset extends StdClass {

  /**
   * This is always computed by __get(). But it needs to be here so that
   * property_exists($preset, 'id') will return TRUE, which is how
   * drupal_write_record() finds record properties.
   */
  protected $id;

  /**
   * @var integer
   * Set by the CTools Export API.
   */
  protected $export_type;

  /**
   * @var boolean
   * Set by hook_cer_default_presets().
   */
  protected $disabled = FALSE;

  /**
   * @var CerFieldChain
   */
  protected $a;

  /**
   * @var CerFieldChain
   */
  protected $b;

  /**
   * @var boolean
   */
  protected $bidirectional = TRUE;

  /**
   * @var integer
   */
  protected $weight = 0;
  
  /**
   * @var boolean
   * This flag is only applicable to presets that live in the database. (@see $this->enabled())
   */
  protected $enabled = TRUE;

  public function __get($property) {
    switch ($property) {
      case 'id':
        return MD5("{$this->left}*{$this->right}");

      case 'left':
        return $this->a;

      case 'right':
        return $this->b;

      // This constant is used by ctools_export_set_object_status().
      case 'table':
        return 'cer';

      default:
        return $this->{$property};
    }
  }

  public function __set($property, $value) {
    switch ($property) {
      case 'a':
      case 'left':
        $this->setChain($this->a, $value);
        break;

      case 'b':
      case 'right':
        $this->setChain($this->b, $value);
        break;

      case 'bidirectional':
      case 'disabled':
      case 'enabled':
        $this->{$property} = (boolean) $value;
        break;

      case 'export_type':
      case 'weight':
        $this->{$property} = (int) $value;
        break;

      default:
        break;
    }
  }

  public function __isset($property) {
    return property_exists($this, $property);
  }

  protected function setChain(&$property, $chain) {
    if ($chain instanceof CerFieldChain) {
      $property = $chain;
    }
    elseif (is_string($chain) && $chain) {
      $property = CerFieldChain::unpack($chain);
    }
  }

  public function enabled() {
    ctools_include('export');

    if ($this->export_type & EXPORT_IN_DATABASE) {
      return $this->enabled;
    }
    else {
      $id = $this->__get('id');
      $status = variable_get('default_cer', array());

      return (isset($status[$id]) ? $status[$id] : !$this->disabled);
    }
  }

  public function invert() {
    ctools_include('export');

    $inverse = ctools_export_crud_new('cer', FALSE);

    $inverse->left = $this->right;
    $inverse->right = $this->left;
    $inverse->bidirectional = $this->bidirectional;
    
    return $inverse;
  }

}
