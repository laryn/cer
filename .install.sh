#!/bin/bash

echo "Installing Drupal..."
drush si -y -q --db-url=mysql://vlad:wibble@localhost/vladdb
drush -y -q en admin_menu_toolbar cer devel entityreference features field_collection node_reference user_reference profile2 views_ui
