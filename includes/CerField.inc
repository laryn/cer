<?php

/**
 * @file
 * Contains the base class for CER field plugins.
 *
 * A field plugin tells CER how to interact with fields of a certain type. If a particular
 * field type integrates with Entity API, its CER plugin can be as simple as extending
 * CerField and implementing the getTargetType() method.
 *
 * @todo
 * More info about extending CerFieldHandler to further customize field plugins.
 */

/**
 * @class
 * Represents a single field instance.
 */
abstract class CerField {

  /**
   * @var array
   * The plugin definition.
   */
  protected $plugin;

  /**
   * @var string
   *  The entity type this instance is bound to.
   */
  protected $entityType;
  
  /**
   * @var string
   *  The bundle this instance is bound to.
   */
  protected $bundle;

  /**
   * @var string
   *  The internal name of the field.
   */
  protected $name;

  /**
   * @var CerField
   *  The field instance which contains this one, if any.
   */
  protected $parent;

  /**
   * @var CerField
   *  Field instance contained by this one, if any.
   */
  protected $child;

  /**
   * @var boolean
   */
  protected $isBundleable;

  /**
   * @var string
   */
  protected $entityTypeLabel;

  /**
   * @var string
   */
  protected $bundleLabel;

  /**
   * @var string
   * The field instance's label.
   */
  protected $label;

  /**
   * @var integer
   */
  protected $cardinality;

  /**
   * @var string
   */
  protected $fieldTypeLabel;

  /**
   * @var array
   */
  protected $settings;

  /**
   * Gets the type of entity that can be referenced by this field, e.g. 'node'.
   *
   * @return string
   */
  abstract public function getTargetType();

  /**
   * Constructor. Pretty self-explanatory!
   */
  public function __construct(array $plugin) {
    $this->plugin = $plugin;
    list ($this->entityType, $this->bundle, $this->name) = explode(':', $plugin['identifier']);

    // Get info about the entity type and bundle hosting this field instance.
    $info = entity_get_info($this->entityType);
    $this->isBundleable = (boolean) $info['entity keys']['bundle'];
    $this->entityTypeLabel = $info['label'];
    $this->bundleLabel = $info['bundles'][$this->bundle]['label'];

    // Get global info about the field.
    $info = field_info_field($this->name);
    $this->cardinality = $info['cardinality'];

    // Store a copy of the field settings for convenience. At the time of this
    // writing, this is needed by the Entity Reference, Node Reference,
    // and Term Reference plugins.
    $this->settings = $info['settings'];

    // Get info about the field type.
    $type = field_info_field_types($info['type']);
    $this->fieldTypeLabel = $type['label'];

    // Finally, get info about the field instance.
    $instance = field_info_instance($this->entityType, $this->name, $this->bundle);
    $this->label = $instance['label'];
  }

  /**
   * Generates an identifier for this field instance.
   * 
   * @return string
   */
  public function __toString() {
    return "{$this->entityType}:{$this->bundle}:{$this->name}";
  }

  /**
   * Magic getter allowing read-only access to protected properties.
   */
  public function __get($property) {
    return $this->{$property};
  }

  /**
   * Returns a CerFieldHandler subclass instance for the given entity.
   *
   * @param object $entity
   *  The entity to be wrapped by the handler.
   *
   * @return CerFieldHandler
   */
  public function getHandler(EntityDrupalWrapper $entity) {
    return new $this->plugin['handler']($this, $entity);
  }

  /**
   * Returns the bundles that this field instance can reference.
   *
   * @return array
   */
  public function getTargetBundles() {
    $info = entity_get_info($this->getTargetType());
    return array_keys($info['bundles']);
  }

  /**
   * Gets or sets the parent of this field.
   *
   * @param CerField
   *  (optional) The field to set as this one's parent.
   *
   * @return CerField
   */
  public function parent(CerField $parent = NULL) {
    if ($parent) {
      $this->parent = $parent;
    }
    return $this->parent;
  }

  /**
   * Gets or sets the child of this field.
   *
   * @param CerField
   *  (optional) The field to set as this one's child.
   *
   * @return CerField
   */
  public function child(CerField $child = NULL) {
    if ($child) {
      $this->child = $child;
    }
    return $this->child;
  }

  /**
   * Generates a human-readable label for this field instance. By default,
   * this is in the form of Entity Type >> Bundle >> Label.
   *
   * @return string
   */
  public function getLabel() {
    $label = '';

    if (! $this->plugin['require parent']) {
      $label .= "{$this->entityTypeLabel} &raquo; ";

      if ($this->isBundleable) {
        $label .= "{$this->bundleLabel} &raquo; ";
      }
    }    
    $label .= $this->label;

    return $label;
  }

  /**
   * Returns the export pipe for this field when exporting to Features. Anything
   * this particular field needs in order to be properly exported, it should add
   * to the pipe. The full export definition is passed by reference so dependencies
   * can be added.
   *
   * @return array
   */
  public function getExportPipe(array &$export) {
    $pipe = array();

    $pipe['field_base'][] = $this->name;
    $pipe['field_instance'][] = "{$this->entityType}-{$this->bundle}-{$this->name}";

    return $pipe;
  }

  /**
   * Returns information about a particular field plugin, by its identifier, or all
   * available plugins (i.e., defined by hook_cer_fields()) if no identifier is given.
   * The aggregated result of hook_cer_fields() is statically cached.
   */
  public static function getPluginInfo($identifier = NULL) {
    $info = &drupal_static(__METHOD__);

    if (! isset($info)) {
      $info = module_invoke_all('cer_fields');

      foreach ($info as $key => &$field) {
        $field += array(
          'identifier' =>
            $key,
          'parents' =>
            array(),
          'require parent' =>
            FALSE,
          'handler' =>
            'CerFieldHandler',
        );
      }
      drupal_alter('cer_fields', $info);
    }

    return ($identifier ? (isset($info[$identifier]) ? $info[$identifier] : NULL) : $info);
  }

  /**
   * Returns a single field plugin instance, by its identifier. All plugin instances
   * are statically cached.
   *
   * @param string $identifier
   *  The plugin's identifier, in the format entity_type:bundle:field_name.
   *
   * @return CerField
   *
   * @throws Exception if there's no plugin for the given identifier. Why so harsh, you
   * ask? Because CerFieldChain::unpack() utterly depends on being able to get plugin
   * instances for every field in the chain, and if it can't, it could result in myriad
   * weird and serious problems. Throwing an exception will head that off at the pass.
   */
  public static function getPlugin($identifier) {
    $instances = &drupal_static(__METHOD__);
    
    if (! isset($instances[$identifier])) {
      list ($entity_type, $bundle, $field) = explode(':', $identifier);

      $info = self::getPluginInfo($identifier);
      if ($info) {
        $instances[$identifier] = new $info['class']($info);
      }
      else {
        throw new Exception(t('Cannot get instance of invalid plugin %identifier.', array('%identifier' => $identifier)));
      }
    }

    return $instances[$identifier];
  }

}
