<?php

/**
 * Implements hook_default_rules_configuration().
 */
function cer_default_rules_configuration() {
  $items = array();

  $items['cer_disable_preset'] = entity_import('rules_config', '{ "cer_disable_preset" : {
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
  $items['cer_enable_preset'] = entity_import('rules_config', '{ "cer_enable_preset" : {
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
  $items['cer_make_preset_bidirectional'] = entity_import('rules_config', '{ "cer_make_preset_bidirectional" : {
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
  $items['cer_make_preset_one_directional'] = entity_import('rules_config', '{ "cer_make_preset_one_directional" : {
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
  $items['cer_invert_preset'] = entity_import('rules_config', '{ "cer_invert_preset" : {
      "LABEL" : "Invert a CER preset",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "original" : { "label" : "Original preset", "type" : "cer" } },
      "IF" : [
        { "data_is" : { "data" : [ "original:cer-bidirectional" ], "value" : 0 } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "cer",
              "param_cer_weight" : [ "original:cer-weight" ],
              "param_cer_left" : [ "original:cer-right" ],
              "param_cer_right" : [ "original:cer-left" ]
            },
            "PROVIDE" : { "entity_created" : { "inverse" : "Inverse" } }
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "original" ]
    }
  }');
  $items['cer_replace_preset_with_inverse'] = entity_import('rules_config', '{ "cer_replace_preset_with_inverse" : {
      "LABEL" : "Replace a CER preset with its inverse",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CER" ],
      "REQUIRES" : [ "rules" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : { "original" : { "label" : "Original preset", "type" : "cer" } },
      "ACTION SET" : [
        { "component_cer_invert_preset" : {
            "USING" : { "original" : [ "original" ] },
            "PROVIDE" : { "original" : { "inverse" : "Inverted preset" } }
          }
        },
        { "entity_save" : { "data" : [ "inverse" ] } },
        { "entity_delete" : { "data" : [ "original" ] } }
      ]
    }
  }');
  return $items;
}
