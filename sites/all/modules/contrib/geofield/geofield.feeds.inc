<?php

/**
 * @file
 * Provides integration with Feeds module (http://drupal.org/project/feeds)
 */

/**
 * Implements hook_feeds_parser_sources_alter().
 *
 * @see geofield_feeds_combined_source()
 */
function geofield_feeds_parser_sources_alter(&$sources, $content_type) {
  $sources['geofield'] = array(
    'name' => t('Geofield (combined)'),
    'description' => t('All geographic information from the item.'),
    'callback' => 'geofield_feeds_combined_source',
  );
}

/**
 * Callback; Provides a source combining geo information from items.
 *
 * Currently handles geo output from:
 *  - simplepie 1.3
 *  - common syndication parser (feeds 7.x-2.x)
 *
 * @param $source
 *   The FeedsSource object being imported.
 * @param $result
 *   The FeedsParserResult object being mapped from.
 * @param $key
 *   The key specified in the $sources array in
 *   hook_feeds_parser_sources_alter().
 *
 * @return
 *   The value to be extracted from the source.
 *
 * @see geofield_feeds_parser_sources_alter()
 */
function geofield_feeds_combined_source($source, FeedsParserResult $result, $key) {
  $values = array();
  $item = $result->currentItem();

  // Simplepie 1.3 output
  if (isset($item['location_latitude'])) {
    // Lon X; lat Y
    foreach ($item['location_latitude'] as $key => $lat) {
      $point = array('lat' => $lat, 'lon' => $item['location_longitude'][$key]);
      $values[] = geofield_compute_values($point, 'latlon');
    }
  }
  // Common Syndication Parser
  elseif (isset($item['geolocations'][0]) && is_a($item['geolocations'][0], 'FeedsGeoTermElement')) {
    // Presently Common Syndication Parser is just parsing points?
    // and is creating FeedsGeoTermElements, which is possibly not so useful.
    // Maybe better if we could read access the original item, or add in to the item parsing?
    foreach ($item['geolocations'] as $geolocation) {
      $point = array('lat' => $geolocation->lat, 'lon' => $geolocation->lon);
      $values[] = geofield_compute_values($point, 'latlon');
    }
  }
  return $values;
}


/**
 * Implements hook_feeds_node_processor_targets_alter().
 */
function geofield_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'geofield') {
      $targets[$info['field_name'] . ':wkt'] = array(
        'name' => t($instance['label'] . ' WKT'),
        'callback' => 'geofield_set_target_wkt',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':lat'] = array(
        'name' => t($instance['label'] . ' Latitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':lon'] = array(
        'name' => t($instance['label'] . ' Longitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':left'] = array(
        'name' => t($instance['label'] . ' Left Latitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':top'] = array(
        'name' => t($instance['label'] . ' Top Longitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':right'] = array(
        'name' => t($instance['label'] . ' Right Latitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':bottom'] = array(
        'name' => t($instance['label'] . ' Bottom Longitude'),
        'callback' => 'geofield_set_target_simple',
        'real_target' => $info['field_name'],
      );
      $targets[$info['field_name'] . ':combined'] = array(
        'name' => t($instance['label'] . ' (combined)'),
        'callback' => 'geofield_set_target_combined',
        'real_target' => $info['field_name'],
      );
    }
  }
}

/**
 * Example callback specified in hook_feeds_processor_targets_alter().
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $values
 *   The values to populate the target with.
 *
 */
function geofield_set_target_simple($source, $entity, $target, $values) {
  list($field_name, $sub_field) = explode(':', $target, 2);

  if (!is_array($values)) {
    $values = array($values);
  }

  $field = isset($entity->$field_name) ? $entity->$field_name : array(LANGUAGE_NONE => array());
  $delta = 0;
  foreach ($values as $value) {
    $field[LANGUAGE_NONE][$delta][$sub_field] = $value;
    $delta++;
  }

  // Compute all the geofield values for each field value.
  foreach ($field[LANGUAGE_NONE] as $delta => $value) {
    if (!empty($value['lat']) && !empty($value['lon'])) {
      $field[LANGUAGE_NONE][$delta] = geofield_compute_values($value, GEOFIELD_INPUT_LAT_LON);
    }
    elseif (!empty($value['top']) && !empty($value['right']) && !empty($value['bottom']) && !empty($value['left'])) {
      $value = array('geom' => $value);
      $field[LANGUAGE_NONE][$delta] = geofield_compute_values($value, GEOFIELD_INPUT_BOUNDS);
    }
  }

  $entity->$field_name = $field;
}

/**
 * Feeds processor target callback from the already combined source.
 *
 * @see geofield_feeds_parser_sources_alter()
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $values
 *   The values to populate the target with.
 *
 */
function geofield_set_target_combined($source, $entity, $target, $values) {
  $field_name = substr($target, 0, strpos($target, ':'));
  _geofield_set_target($source, $entity, $field_name, $values);
}

/**
 * Feeds processor target callback for WKT source.
 */
function geofield_set_target_wkt($source, $entity, $target, $values) {
  $field_name = substr($target, 0, strpos($target, ':'));

  if (!is_array($values)) {
    $values = array($values);
  }

  $geofield_values = array();

  foreach ($values as $wkt) {
    $field = array('geom' => $wkt);
    $geofield_values[] = geofield_compute_values($field, 'wkt');
  }

  _geofield_set_target($source, $entity, $field_name, $geofield_values);
}

/**
 * Helper function to set values and respect ordinality of field.
 *
 * Based on _field_feeds_set_target(). But type set, more keys than just value.
 *
 * @param $source
 *   A FeedsSource object.
 * @param $entity
 *   The entity to map to.
 * @param $target
 *   The target key on $entity to map to.
 * @param $values
 *   The value to map. MUST be an array.
 */
function _geofield_set_target($source, $entity, $target, $values) {
  if (empty($values)) {
    return;
  }

  // Iterate over all values.
  $i = 0;

  $field = isset($entity->$target) ? $entity->$target : array(LANGUAGE_NONE => array());
  foreach ($values as $value) {
    // Check if field value is empty.
    if(empty($value)){
      continue;
    }
    if (is_array($value) || is_object($value)) {
      $field[LANGUAGE_NONE][$i] = $value;
    }
    $i++;
  }

  $entity->{$target} = $field;
}
