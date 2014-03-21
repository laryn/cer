<?php

/**
 * In order to create relationships between reference fields, CER needs to know
 * about what reference fields are available, and how to handle them. That's
 * what this hook is for. It should return a numerically indexed array of
 * field instances that you want to expose to CER. Each field you want
 * to expose needs to have a field plugin class, which is simply a class
 * that extends CerField.
 *
 * By default, CER will integrate with all of the following field types:
 * entityreference, node_reference, user_reference, taxonomy_term_reference,
 * and commerce_product_reference. (See cer_cer_fields() for that.)
 */
function hook_cer_fields() {
  return array(
    array(
      // Field instance's entity type.
      'entity_type' => 'node',
      // Field instance's bundle. If the entity type has no bundles (e.g., user
      // accounts), this will be the same as the entity type.
      'bundle' => 'article',
      // Machine name of the field.
      'field_name' => 'field_related_pages',
      // The field definition class. This must be registered with the autoloader
      // and extend CerField.
      'class' => 'CerEntityReferenceField',
      // Optional: if the field is embedded in a field collection, CER will try
      // to find all 'paths' to this field instance (i.e., if the field collection
      // itself is used more than once). Generally, you won't have to worry about
      // this value. Defaults to TRUE.
      'get parents' => TRUE,
    ),
  );
}

/**
 * Alter the information gathered by hook_cer_fields().
 */
function hook_cer_fields_alter(array &$fields) {
  // NOP
}
