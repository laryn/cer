<?php

/**
 * Implements hook_cer_fields().
 */
function cer_profile2_cer_fields() {
  $fields = array();
  
  foreach (array_keys(profile2_get_types()) as $profile_type) {
    $fields["user:user:profile2_{$profile_type}"]['class'] = 'CerProfile2Field';
  }
  
  return $fields;
}

/**
 * Implements hook_cer_fields_alter().
 */
function cer_profile2_cer_fields_alter(array &$fields) {
  foreach (array_keys($fields) as $identifier) {
    list ($entity_type, $bundle) = explode(':', $identifier);

    if ($entity_type == 'profile2') {
      $fields[$identifier]['parents'][] = "user:user:profile2_{$bundle}";
      $fields[$identifier]['require parent'] = TRUE;
    }
  }
}
