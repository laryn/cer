<?php

class CerUIController extends EntityDefaultUIController {

  /**
   * Overridden.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    $items[$this->path]['title'] = t('Corresponding Entity References');
    $items["{$this->path}/list"]['title'] = t('Presets');

    $this->setTitle($items["{$this->path}/add"], t('Add preset'));
    $this->setTitle($items["{$this->path}/import"], t('Import preset'));

    return $items;
  }

  /**
   * Sets a static title on a menu item.
   */
  private function setTitle(array &$item, $title) {
    $item['title'] = $title;
    unset($item['title callback'], $item['title arguments']);
  }

}
