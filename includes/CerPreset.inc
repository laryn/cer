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
  public $left;

  /**
   * @var CerFieldChain
   */
  public $right;
  
  /**
   * @var boolean
   */
  public $bidirectional = TRUE;

  /**
   * @var boolean
   */
  public $enabled = TRUE;

  public function __get($property) {
    switch ($property) {
      case 'id':
        return MD5("{$this->left}*{$this->right}");
      
      case 'a':
        return $this->left;

      case 'b':
        return $this->right;

      default:
        break;
    }
  }

  public function __set($property, $value) {
    switch ($property) {
      case 'a':
        if ($value) {
          $this->left = CerFieldChain::unpack($value);
        }
        break;

      case 'b':
        if ($value) {
          $this->right = CerFieldChain::unpack($value);
        }
        break;
        
      default:
        break;
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
