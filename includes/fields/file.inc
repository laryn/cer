<?php

/**
 * @file
 * Contains the CER plugin for File and Image fields.
 */
 
class CerFileField extends CerField {

  public function getTargetType() {
    return 'file';
  }

  /**
   * @override CerField::getTargetBundles().
   */
  public function getTargetBundles() {
    return ($this->fieldInfo('type') == 'image' ? array('image') : parent::getTargetBundles());
  }

}
