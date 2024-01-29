# Corresponding Entity References

CER keeps reference fields in sync. If entity Alice references entity Bob, CER
will make Bob back-reference Alice automatically, and it will continue to keep
the two in sync if either one is changed or deleted. CER does this by way of
“presets”, which are relationships you set up between reference-type fields.

By “reference-type fields”, I mean any kind of field that references an entity.
Out of the box, CER can integrate with the following field types:

- Entity Reference
- Node Reference
- User Reference
- Taxonomy Term Reference
- Profile2 (using the cer_profile2 add-on module)

CER has an object-oriented API you can use to integrate other kinds of fields,
if you need to. For more information, see cer.api.php.

## Dependencies

- Entity Plus
- Entity UI

## Installation and Usage:

- Install this module using the [official Backdrop CMS instructions](https://backdropcms.org/guide/modules)
- Usage instructions can be [viewed and edited in the Wiki](https://github.com/backdrop-contrib/cer/wiki).

# Creating Presets

CER won’t do anything until you create at least one preset. To create a preset,
visit admin/config/content/cer and click “Add a preset”.

Select the field you want to use for the left side of the preset, then click
Continue. Another select field will appear; use it to choose the field to use
for the right side of the preset. Click Save, and you’re all set!

## Things you should know

* If you have Corresponding Node References installed, CER will disable it and
take over its field relationships.

* Everything CER does, it does in a normal security context. This can lead to
unexpected behavior if you’re not aware of it. In other words, if you don’t have
the permission to view a specific node, don’t expect CER to be able to reference
it when logged in as you. Be mindful of your entity/field permissions!

* devel_generate does not play nicely with CER, especially where field
collections are concerned. The results are utterly unpredictable.

## Current Maintainers

- [Laryn Kragt Bakker](https://github.com/laryn)
- [Jason Flatt](https://github.com/oadaeh)
- Seeking co-maintainers

## Credits

- Ported to Backdrop by [Laryn Kragt Bakker](https://github.com/laryn).
- Backdrop version is supported by [Aten Design Group](https://aten.io).
- Drupal version maintained by Anybody, bmcclure, chertzog, gcb, gregcube,
  Grevil, jrglasgow, and phenaproxima.

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory
for complete text.
