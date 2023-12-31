<?php

/**
 * @file
 * Blocks available in IP Geolocation.
 */

define('IP_GEOLOC_MAP_DIV_DEFAULT_STYLE', 'height:300px');
define('IP_GEOLOC_CURRENT_VISITOR_MAP_OPTIONS', '{"mapTypeId":"roadmap", "disableDefaultUI":true, "zoom":15, "zoomControl":true}');
define('IP_GEOLOC_RECENT_VISITORS_MAP_OPTIONS', '{"mapTypeId":"terrain", "disableDefaultUI":true, "zoom":2,  "zoomControl":true}');

define('DOC_GOOGLE_MAP_OPTIONS', 'http://code.google.com/apis/maps/documentation/javascript/reference.html#MapOptions');

/**
 * Implements hook_block_info().
 */
function ip_geoloc_block_info() {
  $blocks['current_visitor_map'] = array(
    'info' => t("Map centered on current visitor's location"),
    'cache' => DRUPAL_NO_CACHE
  );
  $blocks['recent_visitors_map'] = array(
    'info' => t('Map showing locations of @n most recent visitors', array('@n' => variable_get('ip_geoloc_recent_visitors_map_number_of_visitors', 10))),
    'cache' => DRUPAL_NO_CACHE
  );
  $blocks['address_lookup'] = array(
    'info' => t('Address lookup based on IP'),
    'cache' => DRUPAL_NO_CACHE
  );
  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function ip_geoloc_block_configure($delta = '') {
  $form = array();
  if ($delta == 'address_lookup') {
    return $form;
  }
  if ($delta == 'recent_visitors_map') {
    $form['ip_geoloc_recent_visitors_map_number_of_visitors'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of recent visitor locations to show on the map.'),
      '#default_value' => variable_get('ip_geoloc_recent_visitors_map_number_of_visitors', 10),
      '#description' => t('Must be greater than 0. Values greater than 20 are not recommended.')
    );
  }
  $form['ip_geoloc_' . $delta . '_div_style'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS style attribute(s) applied to the HTML DIV element that is placeholding the map'),
    '#default_value' => variable_get('ip_geoloc_' . $delta . '_div_style', IP_GEOLOC_MAP_DIV_DEFAULT_STYLE),
    '#description' => t('The default %default_style results in a map of 300 pixels high, with a width bounded by the element that contains it. Do not enter quotes or equal signs and make sure your quotes match.',
      array('%default_style' => IP_GEOLOC_MAP_DIV_DEFAULT_STYLE))
  );
  if ($delta == 'current_visitor_map') {
    $form['ip_geoloc_current_visitor_map_options'] = array(
      '#type' => 'textfield',
      '#size' => 120,
      '#title' => t('Map options'),
      '#default_value' => variable_get('ip_geoloc_current_visitor_map_options', IP_GEOLOC_CURRENT_VISITOR_MAP_OPTIONS),
      '#description' => t("The default %default_options produces a street map zoomed in to level 15. A list of map options can be found <a href='!google_map_docs'>here</a>. Remember to separate options with comma's, not semi-colons, and make sure your quotes match.",
            array('%default_options' => IP_GEOLOC_CURRENT_VISITOR_MAP_OPTIONS,
                  '!google_map_docs' => DOC_GOOGLE_MAP_OPTIONS))
    );
  }
  elseif ($delta == 'recent_visitors_map') {
    $form['ip_geoloc_recent_visitors_map_options'] = array(
      '#type' => 'textfield',
      '#size' => 120,
      '#title' => t('Map options'),
      '#default_value' => variable_get('ip_geoloc_recent_visitors_map_options', IP_GEOLOC_RECENT_VISITORS_MAP_OPTIONS),
      '#description' => t("The default %default_options produces a world map zoomed in to level 2. A list of map options can be found <a href='!google_map_docs'>here</a>. Remember to separate options with comma's, not semi-colons, and make sure your quotes match.",
            array('%default_options' => IP_GEOLOC_RECENT_VISITORS_MAP_OPTIONS,
                  '!google_map_docs' => DOC_GOOGLE_MAP_OPTIONS))
    );
  }
  return $form;
}

/**
 * Implements hook_block_save().
 */
function ip_geoloc_block_save($delta = '', $edit = array()) {
  if ($delta == 'address_lookup') {
    return;
  }
  variable_set('ip_geoloc_' . $delta . '_div_style', $edit['ip_geoloc_' . $delta . '_div_style']);
  if ($delta == 'current_visitor_map') {
    ip_geoloc_save_map_options('ip_geoloc_current_visitor_map_options', $edit['ip_geoloc_current_visitor_map_options']);
  }
  elseif ($delta == 'recent_visitors_map') {
    ip_geoloc_save_map_options('ip_geoloc_recent_visitors_map_options', $edit['ip_geoloc_recent_visitors_map_options']);
    variable_set('ip_geoloc_recent_visitors_map_number_of_visitors', $edit['ip_geoloc_recent_visitors_map_number_of_visitors']);
  }
}

/**
 * Implements hook_block_view().
 */
function ip_geoloc_block_view($delta = '') {
  $block = array();
  $map_style = variable_get('ip_geoloc_' . $delta . '_div_style', IP_GEOLOC_MAP_DIV_DEFAULT_STYLE);
  switch ($delta) {

    case 'current_visitor_map':
      $block['subject'] = t('Your location');
      $map_options = variable_get('ip_geoloc_current_visitor_map_options', IP_GEOLOC_CURRENT_VISITOR_MAP_OPTIONS);
      $block['content'] = ip_geoloc_output_map_current_location('ip-geoloc-block-current-visitor-map', drupal_json_decode($map_options), $map_style);
      break;

    case 'recent_visitors_map':
      $block['subject'] = t('Recent visitors');
      $how_many = variable_get('ip_geoloc_recent_visitors_map_number_of_visitors', 10);
      if ($how_many > 0) {
        $locations = ip_geoloc_get_recent_visitor_locations($how_many);
        $map_options = variable_get('ip_geoloc_recent_visitors_map_options', IP_GEOLOC_RECENT_VISITORS_MAP_OPTIONS);
        $block['content'] = empty($locations)
          ? t('No recent visitors available to map.')
          : ip_geoloc_output_map_multi_locations($locations, 'ip-geoloc-block-recent-visitors-map', drupal_json_decode($map_options), $map_style);
      }
      break;

    case 'address_lookup':
      $block['subject'] = t('Global address lookup');
      $block['content'] = drupal_get_form('ip_geoloc_address_lookup');
      break;
  }
  return $block;
}

function ip_geoloc_save_map_options($var_name, $map_options) {
  $map_options_decoded = drupal_json_decode($map_options);
  if ($map_options_decoded == NULL) {
    drupal_set_message(t("Sytax error in map options. These map options may not work: '%map_options'", array('%map_options' => $map_options)), 'warning');
  }
  variable_set($var_name, $map_options);
}

function ip_geoloc_get_recent_visitor_locations($how_many) {
  $locations = array();
  if (db_table_exists('accesslog')) {
    // A LEFT JOIN would also pick up new IP addresses that are about to be
    // inserted into the {accesslog} during execution of hook_exit().
    // However a LEFT JOIN in this query can easily make it 100 times slower
    // than the INNER JOIN used below and would only be relevant for the very
    // first click from a new IP address or in the case where the IP address was
    // removed from the {accesslog}.
    $result = db_query_range('SELECT DISTINCT ip_address, latitude, longitude, formatted_address, COUNT(a.timestamp) AS visit_count, MAX(a.timestamp) AS last_visit FROM {ip_geoloc} i INNER JOIN {accesslog} a ON i.ip_address = a.hostname GROUP BY i.ip_address ORDER BY last_visit DESC', 0, $how_many);
    $date_format = variable_get('date_format_short', 'm/d/Y - H:i');
    foreach ($result as $location) {
      if (!isset($locations[$location->ip_address])) {
        $location->visit_count = empty($location->visit_count) ? '?' : $location->visit_count;
        $location->last_visit = empty($location->last_visit) ? '?' : date($date_format, $location->last_visit);
        $locations[$location->ip_address] = $location;
      }
    }
  }
  return $locations;
}

/**
 * Generates a simple form for collecting the IP address to be reverse-geocoded
 * into a street address.
 *
 * @ingroup forms
 */
function ip_geoloc_address_lookup($form, &$form_state) {
  $last_visit = isset($form_state['storage']['last_visit']) ? $form_state['storage']['last_visit'] : '';
  $formatted_address = isset($form_state['storage']['formatted_address']) ? $form_state['storage']['formatted_address'] : '';
  if (!empty($last_visit) || !empty($formatted_address)) {
    $prefix = '<div class="ip_geoloc_address_lookup">' . $formatted_address . (empty($last_visit) ? '' : "<br/>$last_visit") . '</div>';
  }
  $form['ip_address'] = array(
    '#type' => 'textfield',
    '#title' => t('IP address'),
    '#default_value' => '',
    '#size' => 16,
    '#required' => FALSE,
    '#prefix' => isset($prefix) ? $prefix : NULL
  );
  if (user_access('administer site configuration')) {
    $form['store'] = array(
      '#type' => 'checkbox',
      '#title' => t('If found, store on IP Geolocation database for future reference.'),
      '#default_value' => FALSE
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lookup'),
    '#submit' => array('ip_geoloc_lookup')
  );
  return $form;
}

function ip_geoloc_lookup($form, &$form_state) {
  $store = !empty($form_state['values']['store']);
  $location = ip_geoloc_get_location_by_ip(trim($form_state['values']['ip_address']), TRUE, $store);
  if (db_table_exists('accesslog')) {
    $last_visit = db_query('SELECT MAX(timestamp) FROM {accesslog} WHERE hostname = :ip_address', array(':ip_address' => $location['ip_address']))->fetchField();
    if ($last_visit) {
      $date_format = variable_get('date_format_short', 'm/d/Y - H:i');
      $last_visit = date($date_format, $last_visit);
      $form_state['storage']['last_visit'] = t('<strong>Last visit:</strong> %date', array('%date' => $last_visit));
    }
    else {
    //$form_state['storage']['last_visit'] = t('There is no record of this IP address visiting this site.');
    }
  }
  $form_state['storage']['formatted_address'] = isset($location['formatted_address'])
    ? t('%ip: <strong>%address</strong>', array('%ip' => $location['ip_address'], '%address' => $location['formatted_address']))
    : '';
  $form_state['rebuild'] = TRUE; // to preserve entered values and storage array
}

