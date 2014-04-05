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
  
  /**
   * @override CerField::getHandler().
   */
  public function getHandler(EntityDrupalWrapper $entity) {
    return new CerFileFieldHandler($this, $entity);
  }

}

/**
 * @class
 * File fields need a little bit of special handling, because Entity API returns
 * file field values as arrays, not proper entities.
 */
class CerFileFieldHandler extends CerFieldHandler {

  /**
   * @override CerFieldHandler::__construct().
   */
  public function __construct(CerField $field, EntityDrupalWrapper $entity) {
    parent::__construct($field, $entity);

    $value = $this->value->value();

    // If there's data in the field, cast each value to an StdClass (it's normally
    // an array) and wrap it.
    if ($value) {
      if ($this->isMultiValue) {
        for ($delta = 0; $delta < sizeof($value); $delta++) {
          $value[$delta] = new EntityDrupalWrapper('file', (object) $value[$delta]);
        }
        $this->value->set($value);
      }
      else {
        $this->value = new EntityDrupalWrapper('file', (object) $value);
      }
    }
  }
  
  /**
   * @override CerFieldHandler::getIDs().
   */
  public function getIDs() {
    $IDs = array();

    if ($this->isMultiValue) {
      foreach ($this->value as $ref) {
        $IDs[] = $ref->getIdentifier();
      }
    }
    else {
      $IDs[] = $this->value->getIdentifier();
    }
    
    return array_unique(array_filter($IDs));
  }

}
