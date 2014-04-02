<?php

/**
 * Page callback listing all CER presets.
 */
function cer_preset_list() {
  $table = array(
    '#theme' => 'table',
    '#header' => array(
      t('Active'),
      t('Left Field'),
      t('Right Field'),
      t('Bidirectional'),
      t('Operations'),
    ),
  );

  ctools_include('export');
  foreach (ctools_export_crud_load_all('cer') as $preset) {
    $operations = array(
      'links' => array(
        'toggle' => array(
          'title' => ($preset->enabled ? t('disable') : t('enable')),
          'href' => "admin/config/content/cer/presets/toggle/{$preset->id}",
        ),
        'invert' => array(
          'title' => t('invert'),
          'href' => "admin/config/content/cer/presets/invert/{$preset->id}",
        ),
        'delete' => array(
          'title' => t('delete'),
          'href' => "admin/config/content/cer/presets/delete/{$preset->id}",
        ),
      ),
    );

    $table['#rows'][] = array(
      ($preset->enabled ? '&#10003;' : ''),
      sprintf('<strong>%s</strong>: %s', $preset->left()->end()->fieldTypeInfo('label'), $preset->left()->getLabel()),
      sprintf('<strong>%s</strong>: %s', $preset->right()->end()->fieldTypeInfo('label'), $preset->right()->getLabel()),
      ($preset->bidirectional ? '&#10003;' : ''),
      theme('links__ctools_dropbutton', $operations),
    );
  }

  return (isset($table['#rows']) ? $table : t('No presets have been created yet.'));
}

function cer_add_preset(array $form, array &$form_state) {
  drupal_set_title(t('Add a CER preset'));

  if (! module_exists('hierarchical_select')) {
    drupal_set_message(t('You must install the <a href="http://www.drupal.org/project/hierarchical_select">Hierarchical Select</a> module in order to create CER presets.'), 'error');
    return $form;
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array(
      '_cer_add_preset_cancel',
    ),
    '#weight' => 5,
  );

  // Either create a new hierarchy to choose the left field, or load it from
  // the cache if the form is being rebuilt.
  $left = CerFieldHierarchy::get('cer_left');

  // If the hierarchy is empty, add every available field chain to it.
  if (sizeof($left) == 0) {
    foreach (CerFieldChain::collectAll() as $chain) {
      $left->addChain($chain);
    }
  }

  // If the hierarchy now has data, generate a hierarchical_select field and add
  // it to the form. Otherwise, fail with an error message.
  if (sizeof($left) > 0) {
    $form['left'] = $left->getHierarchicalSelect(array('save_lineage' => TRUE));
    $form['left']['#required'] = TRUE;
    $form['left']['#title'] = t('Choose a field...');
  }
  else {
    drupal_set_message(t('There are no reference fields available.'), 'warning');
    return $form;
  }

  // One the user has chosen a left field and clicked Continue, we can find the
  // possible right fields. (@see _cer_add_preset_continue())
  if (isset($form_state['storage']['cer']['left'])) {
    // Fetch the left field chain from storage, and get its full identifier
    // and a regex we can use to match it against right fields.
    $left = $form_state['storage']['cer']['left'];
    $left_identifier = $left->__toString();
    $left_re = $left->regex();

    // Create, or get from the cache, a hierarchy for the right field chain.
    $right = CerFieldHierarchy::get('cer_right');

    // Loop through every possible field chain and test it against the left
    // chain's regex. If it matches, we can add the chain to the right hierarchy.
    foreach (CerFieldChain::collectAll() as $chain) {
      if (preg_match($left_re, $chain->__toString()) && preg_match($chain->regex(), $left_identifier)) {
        $right->addChain($chain);
      }
    }

    // If there's data in the right hierarchy, we have corresponding fields to choose from,
    // so display the other hierarchical_select field and advanced options. Otherwise,
    // display an error and start over.
    if (sizeof($right) > 0) {
      $form['left'] = array(
        '#type' => 'markup',
        '#markup' => sprintf('%s', $left->getLabel()),
      );

      $form['right'] = $right->getHierarchicalSelect(array('save_lineage' => TRUE));
      $form['right']['#title'] = t('Choose a corresponding field...');

      $form['advanced'] = array(
        '#type' => 'fieldset',
        '#title' => t('Advanced'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['advanced']['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enabled'),
        '#default_value' => TRUE,
      );
      $form['advanced']['bidirectional'] = array(
        '#type' => 'checkbox',
        '#title' => t('Bidirectional'),
        '#default_value' => TRUE,
        '#description' => t('If checked, a change to either field will trigger a reaction on the other field. Otherwise, only a change to the first field will trigger a reaction on the other field.'),
      );

      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Preset'),
      );
    }
    else {
      drupal_set_message(t('There are no fields which can correspond with !left.', array('!left' => $left->getLabel())), 'warning');
    }
  }
  else {
    $form['actions']['continue'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
      '#submit' => array(
        '_cer_add_preset_continue',
      ),
    );
  }

  return $form;
}

/**
 * Submit handler. Stores the selected left field in $form_state['storage'] and rebuilds
 * the form so the user can choose the right field.
 */
function _cer_add_preset_continue(array $form, array &$form_state) {
  $form_state['storage']['cer']['left'] = CerFieldChain::unpack(end($form_state['values']['left']));  
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler. Deletes the cer_left and cer_right hierarchies from the cache and
 * sets the redirect path to the main administration page.
 */
function _cer_add_preset_cancel(array $form, array &$form_state) {
  CerFieldHierarchy::get('cer_left')->clean();
  CerFieldHierarchy::get('cer_right')->clean();

  $form_state['redirect'] = 'admin/config/content/cer';
}

function cer_add_preset_submit(array $form, array &$form_state) {
  $preset = new CerPreset();

  $preset->left($form_state['storage']['cer']['left']);
  $preset->right(CerFieldChain::unpack(end($form_state['values']['right'])));

  $preset->enabled = $form_state['values']['enabled'];
  $preset->bidirectional = $form_state['values']['bidirectional'];

  if ($preset->write()) {
    drupal_set_message(t('The preset was saved successfully.'));
  }
  else {
    drupal_set_message(t('An error occurred while trying to save the preset.'), 'error');
  }

  _cer_add_preset_cancel($form, $form_state);
}

/**
 * Page callback to toggle a preset from the main administration page.
 */
function cer_preset_toggle(CerPreset $preset) {
  $preset->toggle();
  drupal_goto('admin/config/content/cer');
}

/**
 * Page callback to invert a preset from the main administration page.
 */
function cer_preset_invert(CerPreset $original) {
  $inverse = $original->invert();
  $inverse->write($original->enabled);  // Preserve the original's status
  // There's no point in having two presets that are inverses of each other,
  // so delete the original.
  $original->delete();

  drupal_goto('admin/config/content/cer');
}

/**
 * Confirmation form to delete a preset.
 */
function cer_delete_preset(array $form, array &$form_state, CerPreset $preset) {
  $question = t('<i>!left</i> will no longer correspond with <i>!right</i>.', array(
    '!left' => $preset->left()->getLabel(),
    '!right' => $preset->right()->getLabel(),
  ));

  return confirm_form($form, t('Are you sure you want to delete this preset?'), 'admin/config/content/cer', $question, t('Delete'));
}

function cer_delete_preset_submit(array $form, array &$form_state) {
  $form_state['build_info']['args'][0]->delete();

  drupal_set_message(t('The preset was deleted.'));
  $form_state['redirect'] = 'admin/config/content/cer';
}

/**
 * Allows batch updating of existing entities.
 */
function cer_bulk_update_form(array $form, array &$form_state) {
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#required' => TRUE,
    '#options' => array(),
    '#description' => t('Select the entity type that you want to update.'),
  );
  foreach (entity_get_info() as $type => $class) {
    $form['type']['#options'][$type] = $class['label'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * The update form. Allows bulk updating of current entities.
 */
function cer_bulk_update_form_submit($form, &$form_state) {
  $batch = array(
    'finished' => 'cer_batch_update_existing_finished',
    'title' => t('Processing'),
    'init_message' => t('Preparing to update corresponding entity references for existing entities...'),
    'progress_message' => t('Processing entities...'),
    'error_message' => t('Corresponding entity references - existing entity update has encountered an error.'),
    'operations' => array(),
  );

  $entity_type = $form_state['values']['type'];
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  $result = $query->execute();

  if (isset($result[$entity_type])) {
    foreach (array_keys($result[$entity_type]) as $entity_id) {
      $batch['operations'][] = array('cer_processing_entity', array('bulk_update', $entity_id, $entity_type));
    }
  }

  batch_set($batch);
}
