<?php

/**
 * @file
 * Contains the CER plugin for Taxonomy Term Reference fields.
 */
 
class CerTaxonomyTermReferenceField extends CerField {

  public function getTargetType() {
    return 'taxonomy_term';
  }
  
  public function getTargetBundles() {
    $bundles = array();

    foreach ($this->fieldInfo('settings', 'allowed_values') as $item) {
      $bundles[] = $item['vocabulary'];
    }

    return $bundles;
  }

}
