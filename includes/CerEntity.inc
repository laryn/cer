<?php

class CerEntity extends EntityDrupalWrapper {

  protected $rootEntityType;

  protected $rootEntity;

  protected $chain = array();

  public function __construct($entity_type, $entity) {
    if (! $this->chain) {
      parent::__construct($entity_type, $entity);
      $this->chain[] = $entity_type . ':' . $this->getBundle() . ':%';
    }

    $this->rootEntityType = $entity_type;
    $this->rootEntity = $entity;

    if ($entity instanceof FieldCollectionItemEntity) {
      $this->chain[] = $this->host_entity->type() . ':' . $this->host_entity->getBundle() . ':' . $this->getBundle();
      $this->__construct($this->host_entity->type(), $this->host_entity->value());
    }
  }

  public function __toString() {
    return implode('::', array_reverse($this->chain));
  }

  public function depth() {
    return (sizeof($this->chain) - 1);
  }

  public function root() {
    return new CerEntity($this->rootEntityType, $this->rootEntity);
  }

}