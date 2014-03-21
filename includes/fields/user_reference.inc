<?php

/**
 * @file
 * Contains the CER plugin for User Reference fields.
 */
 
class CerUserReferenceField extends CerField {

  public function getTargetType() {
    return 'user';
  }

}
 