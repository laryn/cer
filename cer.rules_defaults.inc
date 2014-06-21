<?php

/**
 * Implements hook_default_rules_configuration().
 */
function cer_default_rules_configuration() {
  $items = array();

  $items['rules_disable_cer_preset'] = entity_import('rules_config', '{ "rules_disable_cer_preset" : {
      "LABEL" : "Disable CER preset",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "preset" : { "label" : "Preset", "type" : "cer" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "preset:cer-enabled" ], "value" : 0 } },
        { "entity_save" : { "data" : [ "preset" ] } }
      ]
    }
  }');
  $items['rules_enable_cer_preset'] = entity_import('rules_config', '{ "rules_enable_cer_preset" : {
      "LABEL" : "Enable CER preset",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "preset" : { "label" : "Preset", "type" : "cer" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "preset:cer-enabled" ], "value" : 1 } },
        { "entity_save" : { "data" : [ "preset" ] } }
      ]
    }
  }');
  $items['rules_make_cer_preset_bidirectional'] = entity_import('rules_config', '{ "rules_make_cer_preset_bidirectional" : {
      "LABEL" : "Make CER preset bidirectional",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "preset" : { "label" : "Preset", "type" : "cer" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "preset:cer-bidirectional" ], "value" : 1 } },
        { "entity_save" : { "data" : [ "preset" ] } }
      ]
    }
  }');
  $items['rules_make_cer_preset_one_directional'] = entity_import('rules_config', '{ "rules_make_cer_preset_one_directional" : {
      "LABEL" : "Make CER preset one-directional",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "preset" : { "label" : "Preset", "type" : "cer" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "preset:cer-bidirectional" ], "value" : 0 } },
        { "entity_save" : { "data" : [ "preset" ] } }
      ]
    }
  }');
  return $items;
}
