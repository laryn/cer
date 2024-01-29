<?php

/**
 * Contains the controller class for CER's UI (i.e., preset management pages),
 * used by Entity API.
 */

class CerUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();

    $items[$this->path]['title'] = t('Corresponding Entity References');
    $items["{$this->path}/list"]['title'] = t('Presets');

    $this->setTitle($items["{$this->path}/add"], t('Add preset'));
    //$this->setTitle($items["{$this->path}/import"], t('Import preset'));

    $items["{$this->path}/manage/%entity_ui_object/toggle"] = $this->createCallback('cer_toggle_preset', 'update');
    $items["{$this->path}/manage/%entity_ui_object/invert"] = $this->createCallback('cer_invert_preset', 'create');

    // Don't provide a page for cloning a preset.
    unset($items["{$this->path}/manage/%entity_ui_object/clone"]);

    return $items;
  }

  private function createCallback($function, $operation, array $init = array()) {
    return $init + array(
      'type' => MENU_CALLBACK,
      'page callback' => $function,
      'page arguments' => array(5),
      'load arguments' => array('cer'),
      'access callback' => 'entity_access',
      'access arguments' => array($operation, 'cer'),
      'file' => 'cer.admin.inc',
      'file path' => backdrop_get_path('module', 'cer'),
    );
  }

  /**
   * Sets a static title on a menu item.
   */
  private function setTitle(array &$item, $title) {
    $item['title'] = $title;
    unset($item['title callback'], $item['title arguments']);
  }

  public function operationForm($form, &$form_state, $entity, $action) {
    switch ($action) {
      case 'delete':
        return confirm_form($form, t('Are you sure you want to delete this preset?'), $this->path, t('@left will no longer synchronize with @right.', $entity->label_variables));

      default:
        return parent::operationForm($form, $form_state, $entity, $action);
    }
  }

  public function overviewForm($form, &$form_state) {
    $form = parent::overviewForm($form, $form_state);

    $form['actions'] = array(
      'update' => array(
        '#type' => 'submit',
        '#value' => t('Save changes'),
      ),
      '#type' => 'actions',
    );

    return $form;
  }

  public function overviewFormSubmit($form, &$form_state) {
    foreach ($form_state['values']['table'] as $id => $values) {
      $preset = entity_ui_object_load($id, $this->entityType);

      $preset->wrapper->cer_enabled->set($values['cer_enabled'][LANGUAGE_NONE][0]['value']);
      $preset->wrapper->cer_bidirectional->set($values['cer_bidirectional'][LANGUAGE_NONE][0]['value']);
      $preset->wrapper->cer_weight->set($values['cer_weight'][LANGUAGE_NONE][0]['value']);

      $preset->save();
    }

    backdrop_set_message(t('The changes have been saved.'));
  }

  public function overviewTable($conditions = array()) {
    $render = array(
      '#theme' => 'table',
      '#header' => array(
        t('Left Field'),
        t('Right Field'),
        t('Status'),
        t('Enabled'),
        t('Bidirectional'),
        t('Weight'),
        t('Operations'),
      ),
      '#empty' => t('None.'),
      '#attributes' => array('id' => 'cer-overview-table'),
    );
    backdrop_add_tabledrag('cer-overview-table', 'order', 'sibling', 'cer-weight');

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $query->fieldOrderBy('cer_weight', 'value');

    $results = $query->execute();
    $entities = isset($results['cer']) ? entity_load('cer', array_keys($results['cer'])) : array();

    foreach ($entities as $entity) {
      $render['#rows'][$entity->pid] = $this->overviewTableRow($conditions, $entity->pid, $entity);
    }
    return $render;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $render_fields = field_attach_view($this->entityType, $entity, 'default');

    $render_fields['cer_left']['#label_display'] = 'inline';
    $render_fields['cer_left']['#title'] = $entity->wrapper->cer_left->chain->value()->end()->fieldTypeLabel;
    $row['data']['cer_left']['data'] = $render_fields['cer_left'];

    $render_fields['cer_right']['#label_display'] = 'inline';
    $render_fields['cer_right']['#title'] = $entity->wrapper->cer_right->chain->value()->end()->fieldTypeLabel;
    $row['data']['cer_right']['data'] = $render_fields['cer_right'];

    $row['data']['status']['data'] = array(
      '#theme' => 'entity_ui_entity_status',
      '#status' => $entity->status,
    );

    $form_fields = array();
    $form_state = form_state_defaults();
    $form_state['build_info']['form_id'] = 'cer_overview_form';
    field_attach_form($this->entityType, $entity, $form_fields, $form_state);

    unset($form_fields['cer_enabled'][LANGUAGE_NONE]['#title']);
    $row['data']['cer_enabled']['data'] = $form_fields['cer_enabled'];

    unset($form_fields['cer_bidirectional'][LANGUAGE_NONE]['#title']);
    $row['data']['cer_bidirectional']['data'] = $form_fields['cer_bidirectional'];

    unset($form_fields['cer_weight'][LANGUAGE_NONE]['#title']);
    $form_fields['cer_weight'][LANGUAGE_NONE]['#attributes']['class'][] = 'cer-weight';
    $row['data']['cer_weight']['data'] = $form_fields['cer_weight'];

    // Add in any passed additional cols.
    foreach ($additional_cols as $key => $col) {
      $row['data'][$key]['data'] = $col;
    }

    $links = array(
      'toggle' => array(
        'title' => $entity->wrapper->cer_enabled->value() ? t('disable') : t('enable'),
        'href' => "{$this->path}/manage/{$id}/toggle",
        'query' => backdrop_get_destination(),
      ),
      'edit' => array(
        'title' => t('edit'),
        'href' => "{$this->path}/manage/{$id}",
      ),
    );

    // If the preset is one-directional, expose a link to invert it.
    if (! $entity->wrapper->cer_bidirectional->value()) {
      $links['invert'] = array(
        'title' => t('invert'),
        'href' => "{$this->path}/manage/{$id}/invert",
        'query' => backdrop_get_destination(),
      );
    }

    if (entity_plus_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
      $links['revert'] = array(
        'title' => t('revert'),
        'href' => "{$this->path}/manage/{$id}/revert",
        'query' => backdrop_get_destination(),
      );
    }
    else {
      $links['delete'] = array(
        'title' => t('delete'),
        'href' => "{$this->path}/manage/{$id}/delete",
        'query' => backdrop_get_destination(),
      );
    }
    $links['export'] = array(
      'title' => t('export'),
      'href' => "{$this->path}/manage/{$id}/export",
    );

    $row['data']['operations'] = array(
      'data' => array(
        '#type' => 'dropbutton',
        '#links' =>
        $links,
      ),
    );

    $row['class'][] = 'draggable';

    return $row;
  }

}
