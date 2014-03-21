<?php

/**
 * @file
 * Contains the CerUserReferenceField object.
 */
 
class CerUserReferenceField extends CerField {

  public function getTargetType() {
    return 'user';
  }

}
 