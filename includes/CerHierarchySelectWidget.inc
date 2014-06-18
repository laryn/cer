<?php

class CerHierarchySelectWidget implements CerHierarchyWidgetInterface {

  protected $data;

  public function __construct(CerHierarchy $hierarchy) {
    $this->data = $hierarchy->data;
  }

  public function unpack($value) {
    return CerFieldChain::unpack($value);
  }

  public function build() {
    return array(
      '#type' => 'select',
      '#options' => $this->options(),
    );
  }

  protected function options($key = CerHierarchy::ROOT, $parent = NULL, $depth = -1) {
    $options = array();

    $item = $this->data[$key];

    if (isset($item['label'])) {
      $options[$key] = str_repeat('-', $depth) . $item['label'];
    }

    if (isset($item['children'])) {
      foreach ($item['children'] as $child) {
        $options = array_merge($options, $this->options($child, $key, $depth + 1));
      }
    }
    
    return $options;
  }

}
