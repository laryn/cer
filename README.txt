ACKNOWLEDGEMENTS

As this is the next evolution of Corresponding Node References,
I would like to say thanks for all the work done over on Corresponding Node
References.

DESCRIPTION

CER syncs the entity reference between two entity types which have an entity
reference to each other, so double editing entities is no longer needed. If one
entity has a reference, the other entity also receives a reference to the saved
entity if it is referenced in that entity.

DEPENDENCIES

- Drupal core 7.15 or better
- Entity API

In order to create presets (relationships between reference fields), Hierarchical
Select is required. If it isn't installed, the preset creation form will complain
and refuse to appear. Everything else will work properly, though.

EXAMPLE

Entity type A has an entity reference to entity type B and entity type B has an
entity reference to entity type A. When you create entity X of type A and
reference it to entity Y of type B entity Y will also receive an update in its
entity reference field pointing to entity X.

KNOWN ISSUES N' STUFF

- This is an EXPERIMENTAL version of CER's 2.x-dev branch! It has been almost totally
  rewritten in order to support field collections. This approach also brings native
  support for Node Reference, User Reference, and Commerce Product Reference fields
  in addition to Entity Reference, and provides a rudimentary object-oriented API
  for customizing CER's behavior.
  
- That being the case, I HIGHLY recommend that you test this update on a clone of your
  site, and not on your production (or even dev) site before using it in your normal
  environment!
  
- The rewrite completely changed the way that presets are stored in the database, so
  this version DOES NOT support exporting to Features. This is a regression from the
  normal version of CER 2.x. If anyone out there knows the Features API well, a patch 
  for this would be most welcome. Otherwise, I'll get to it soon enough.
  
- You must run upgrade.php after updating to this version of CER, because CER's database
  table structure has changed. And be sure to clear your caches! This update introduces
  many new classes that need to be registered with the autoloader.
  
- Now that CER natively supports node reference fields, it will take over Corresponding
  Node References when installed/updated, then deactivate it. All active CNR presets will
  be automatically converted to CER presets.

- The frequent CerExceptions that plagued the previous version of CER 2.x are gone (rejoice)!
  When CER encounters an error, it will silently log the error to Watchdog with detailed
  information about exactly which entities and fields failed to work, and why. So if something
  is failing for no apparent reason, the first thing to do is check the logs.
  
- It's important to understand that CER does NOT necessarily operate in the administrative
  context. To do so would potentially introduce a gaping security vulnerability. CER only
  has as much privilege as the currently logged-in (or not logged-in) user. This applies
  to all levels of CER's operation, from loading referenceable entities to modifying
  field values. Carefully consider your entity-level and field-level permissions, especially
  if you're using modules which control access to entities and/or fields!
  
- Although translatable fields are supported, it may be a bit flaky due to the ambiguities
  inherent in language handling.
  
- devel_generate does not play nicely with CER, especially where field collections are concerned.

MAINTAINER

Questions, comments, etc. should be directed to phenaproxima (djphenaproxima@gmail.com).