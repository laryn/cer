<?php

/**
 * @file
 * Contains the CerHierarchyWidgetInterface.
 */

/**
 * @interface
 * Defines an object that builds a form element representing a field hierarchy.
 */
interface CerHierarchyWidgetInterface {

  /**
   * Constructs the object.
   *
   * @param CerHierarchy $hierarchy
   *  The hierarchy to be represented by the widget.
   */
  public function __construct(CerHierarchy $hierarchy);

  /**
   * Unpacks the element's submitted value, returning a field chain.
   *
   * @param mixed $value
   *  The value submitted from the form.
   *
   * @return CerFieldChain
   */
  public function unpack($value);

  /**
   * Builds the form element.
   *
   * @return array
   */
  public function build();

}
