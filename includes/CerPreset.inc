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
  public $bidirectional = TRUE;
  
  /**
   * @var boolean
   */
  public $enabled;

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
      $this->left(CerFieldChain::unpack($preset->a));
      $this->right(CerFieldChain::unpack($preset->b));
    }
  }

  public function __get($property) {
    switch ($property) {
      case 'id':
        return MD5($this->__toString());

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

    $inverse->left($this->right());
    $inverse->right($this->left());
    
    return $inverse;
  }

  public function left(CerFieldChain $chain = NULL) {
    if ($chain) {
      $this->left = $chain;
    }
    return $this->left;
  }

  public function right(CerFieldChain $chain = NULL) {
    if ($chain) {
      $this->right = $chain;
    }
    return $this->right;
  }

}
