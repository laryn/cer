<?php

/**
 * @file
 * Contains a CER field plugin for referencing Profile2 entities. These references
 * are not actually fields at all, so this plugin is really more of a bridge to fool
 * CER into thinking that Profile2 is a reference field.
 */

class CerProfile2Field extends CerField implements CerEntityContainerInterface {

  protected $profileType;

  /**
   * @override CerField::__construct().
   */
  public function __construct($entity_type, $bundle, $field) {
    parent::__construct($entity_type, $bundle, $field);

    // Load the Profile2 type information
    $this->profileType = profile2_get_types(subStr($field, 8));
  }

  /**
   * Implements CerField::getTargetType().
   */
  public function getTargetType() {
    // In effect, this "field" will "point to" Profile2 entities.
    return 'profile2';
  }

  /**
   * @override CerField::getTargetBundles().
   */
  public function getTargetBundles() {
    // This field can only "reference" one type of profile. There'll be
    // a separate instance of this plugin for each Profile2 type.
    return (array) $this->profileType->type;
  }

  /**
   * @override CerField::fieldTypeInfo().
   */
  public function fieldTypeInfo() {
    // Profile2 'fields' aren't really fields, so I'll only provide the information
    // that I know is used by CER's core.
    $info = array(
      'label' => t('Profile2'),
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * @override CerField::fieldInfo().
   */
  public function fieldInfo() {
    $info = array(
      'field_name' => $this->field,
      // Each user account can only have one profile of each type.
      'cardinality' => 1,
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * @override CerField::instanceInfo().
   */
  public function instanceInfo() {
    $info = array(
      'label' => $this->profileType->label,
    );
    return drupal_array_get_nested_value($info, func_get_args());
  }

  /**
   * @override CerField::getExportPipe().
   */
  public function getExportPipe(array &$export) {
    $export['dependencies']['cer_profile2'] = 'cer_profile2';

    // Make Features export the Profile2 type, so that cer_profile2_cer_fields()
    // will be able to find it.
    return array(
      'profile2_type' => array(
        $this->profileType->type,
      ),
    );
  }

  /**
   * Implements CerEntityContainerInterface::createInnerEntity().
   */
  public function createInnerEntity(EntityDrupalWrapper $owner) {
    $init = array(
      'user' => $owner->value(),
      'type' => $this->profileType,
    );

    // Create a blank profile for this user, and save it. The "reference" is implied
    // by the existence of the profile.
    $profile = profile2_create($init);
    $profile->save();

    return $profile;
  }

}
