<?php

class CerUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();

    $items[$this->path]['title'] = t('Corresponding Entity References');
    $items["{$this->path}/list"]['title'] = t('Presets');

    $this->setTitle($items["{$this->path}/add"], t('Add preset'));
    $this->setTitle($items["{$this->path}/import"], t('Import preset'));
 
    // Don't provide a page for cloning a preset.
    unset($items["{$this->path}/manage/%entity_object/clone"]);

    return $items;
  }

  /**
   * Sets a static title on a menu item.
   */
  private function setTitle(array &$item, $title) {
    $item['title'] = $title;
    unset($item['title callback'], $item['title arguments']);
  }

  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = array(
      t('Enabled'),
      t('Left Field'),
      t('Right Field'),
      t('Bidirectional'),
      t('Status'),
      array(
        'data' => t('Operations'),
        'colspan' => $this->operationCount(),
      ),
    );

    return array_merge($header, $additional_header);
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $fields = field_attach_view($this->entityType, $entity, 'default');

    $row[] = $entity->wrapper->cer_enabled->value() ? '&#10003;' : '';
    $row[] = render($fields['cer_left']);
    $row[] = render($fields['cer_right']);
    $row[] = $entity->wrapper->cer_bidirectional->value() ? '&#10003;' : '';
    $row[] = array(
      'data' => array(
        '#theme' => 'entity_status',
        '#status' => $entity->{$this->statusKey},
      ),
    );

    // Add in any passed additional cols.
    foreach ($additional_cols as $col) {
      $row[] = $col;
    }

    $row[] = l(t('edit'), "{$this->path}/manage/{$id}");

    if (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
      $row[] = l(t('revert'), "{$this->path}/manage/{$id}/revert", array('query' => drupal_get_destination()));
    }
    else {
      $row[] = l(t('delete'), "{$this->path}/manage/{$id}/delete", array('query' => drupal_get_destination()));
    }

    $row[] = l(t('export'), "{$this->path}/manage/{$id}/export");

    return $row;
  }

}
