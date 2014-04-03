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
   * @var boolean
   */
  protected $enabled = TRUE;

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
      case 'enabled':
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

  public function invert() {
    ctools_include('export');
    $inverse = ctools_export_crud_new('cer', FALSE);

    $inverse->enabled = $this->enabled;
    $inverse->bidirectional = $this->bidirectional;

    $inverse->left = $this->right;
    $inverse->right = $this->left;
    
    return $inverse;
  }

}
