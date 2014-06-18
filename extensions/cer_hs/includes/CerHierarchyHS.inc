<?php

/**
 * @file
 * Contains the CerHierarchyHS class.
 */

class CerHierarchyHS extends CerHierarchy {

  public function validItem($key) {
    return array_key_exists($key, $this->data);
  }

  public function getLabel($key) {
    return $this->data[$key]['label'];
  }

  public function getLineage($item_key, array $lineage = NULL) {
    if (empty($lineage)) {
      $lineage = array($item_key);
    }

    foreach ($this->data as $key => $item) {
      if ($key == self::ROOT) {
        continue;
      }

      if (isset($item['children']) && in_array($item_key, $item['children'])) {
        array_unshift($lineage, $key);
        $this->getLineage($key, $lineage);
      }
    }

    return $lineage;
  }

  public function getOptions($item) {
    $options = array();

    if (isset($this->data[$item]['children'])) {
      foreach ($this->data[$item]['children'] as $child) {
        if (isset($this->data[$child]['label'])) {
          $options[$child] = $this->data[$child]['label'];
        }
      }
    }

    return $options;
  }

}
