<?php

require_once 'bootstrap_grid_plugin_style_grid.inc';

function bootstrap_grid_views_plugins() {
 
  return array(
    'module' => 'bootstrap_grid',
    'style' => array(
      'bootstrap_grid' => array(
        'title' => t('Bootstrap Grid'),
        'help' => t('Display the results as bootstrap responsive grid.'),
        'handler' => 'bootstrap_grid_plugin_style_grid',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'parent' => 'list',
        'path' => drupal_get_path('module', 'bootstrap_grid'),
        'theme' => 'bootstrap_grid_view',
        'theme path' => drupal_get_path('module', 'bootstrap_grid'),
      ),
    ),
  );
}

