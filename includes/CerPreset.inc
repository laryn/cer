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
   * @var string
   */
  protected $id;

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
        return MD5($this->__toString());

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

  public function __toString() {
    return "{$this->left}*{$this->right}";
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
