<?php

/**
 * @file
 * Contains the CerHierarchyHSWidget class.
 */

/**
 * @class
 * Builds a Hierarchical Select form element representing a field hierarchy.
 */
class CerHierarchyHSWidget implements CerHierarchyWidgetInterface {

  protected $hierarchy;

  /**
   * Implements CerHierarchyWidgetInterface::__construct().
   */
  public function __construct(CerHierarchy $hierarchy) {
    $this->hierarchy = $hierarchy;
  }

  /**
   * Implements CerHierarchyWidgetInterface::unpack().
   */
  public function unpack($value) {
    if (is_array($value)) {
      return CerFieldChain::unpack(end($value));
    }
  }

  /**
   * Implements CerHierarchyWidgetInterface::build().
   */
  public function build() {
    $defaults = array(
      'params' => cer_hs_hierarchical_select_params(),
      // The rest of this mirrors Hierarchical Select's defaults. However,
      // if I don't explicity specify them here, Hierarchical Select will
      // generate a lot of annoying 'undefined index' notices.
      'enforce_deepest' => FALSE,
      'entity_count' => FALSE,
      'require_entity' => FALSE,
      'resizable' => TRUE,
      'level_labels' => array(
        'status' => FALSE,
      ),
      'dropbox' => array(
        'status' => FALSE,
      ),
      'editability' => array(
        'status' => FALSE,
      ),
      'render_flat_select' => FALSE,
    );

    $options = array_merge_recursive($defaults, $options);

    $this->hierarchy->cache();
    $options['module'] = 'cer';
    $options['params']['id'] = $this->hierarchy->cacheID;
    $options['save_lineage'] = TRUE;  // Things break without this.

    return array(
      '#type' => 'hierarchical_select',
      '#config' => $options,
    );
  }

}
