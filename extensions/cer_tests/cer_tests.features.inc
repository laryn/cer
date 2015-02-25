<?php
/**
 * @file
 * cer_tests.features.inc
 */

/**
 * Implements hook_default_cer().
 */
function cer_tests_default_cer() {
  $items = array();
  $items['node:page:field_author*user:user:field_my_pages'] = entity_import('cer', '{
    "identifier" : "node:page:field_author*user:user:field_my_pages",
    "wrapper" : {},
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_weight" : { "und" : [ { "value" : "0" } ] },
    "cer_left" : { "und" : [ { "path" : "node:page:field_author" } ] },
    "cer_right" : { "und" : [ { "path" : "user:user:field_my_pages" } ] }
  }');
  $items['node:page:field_my_collection::field_collection_item:field_my_collection:field_author*user:user:field_my_collection::field_collection_item:field_my_collection:field_my_pages'] = entity_import('cer', '{
    "identifier" : "node:page:field_my_collection::field_collection_item:field_my_collection:field_author*user:user:field_my_collection::field_collection_item:field_my_collection:field_my_pages",
    "wrapper" : {},
    "cer_bidirectional" : { "und" : [ { "value" : "1" } ] },
    "cer_enabled" : { "und" : [ { "value" : "1" } ] },
    "cer_weight" : { "und" : [ { "value" : "0" } ] },
    "cer_left" : { "und" : [
        { "path" : "node:page:field_my_collection::field_collection_item:field_my_collection:field_author" }
      ]
    },
    "cer_right" : { "und" : [
        { "path" : "user:user:field_my_collection::field_collection_item:field_my_collection:field_my_pages" }
      ]
    }
  }');
  return $items;
}
