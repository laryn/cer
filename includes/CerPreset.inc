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
   * Used only by the CTools Export API.
   */
  public $export_type;
  
  /**
   * CTools Export API will set this in hook_cer_default_presets().
   */
  public $disabled = FALSE;

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

  public function __get($property) {
    switch ($property) {
      case 'id':
        return MD5("{$this->left}*{$this->right}");

      case 'a':
      case 'left':
        return $this->a;

      case 'b':
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
        $this->{$property} = (boolean) $value;
        break;
        
      default:
        break;
    }
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
    $id = $this->__get('id');
    $status = variable_get('default_cer', array());

    // The canonical status of the preset lives in the default_cer variable. But, if
    // this preset isn't mentioned there, the status is determined by whether or not
    // this preset has been overridden. If it has, it's disabled by default; if not,
    // it's the inverse of $this->disabled.
    ctools_include('export');
    return (isset($status[$id]) ? $status[$id] : ($this->export_type & EXPORT_IN_DATABASE ? FALSE : (! $this->disabled)));
  }

  public function invert() {
    ctools_include('export');

    $inverse = ctools_export_crud_new('cer', FALSE);

    $inverse->bidirectional = $this->bidirectional;

    $inverse->left = $this->right;
    $inverse->right = $this->left;
    
    return $inverse;
  }

}
