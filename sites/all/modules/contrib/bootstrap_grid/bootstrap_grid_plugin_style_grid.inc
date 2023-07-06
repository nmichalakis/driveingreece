<?php

/**
 * @file
 * Contains the grid style plugin.
 */

/**
 * Style plugin to render each item in a grid cell.
 *
 * @ingroup views_style_plugins
 */
class bootstrap_grid_plugin_style_grid extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['view-xs-columns'] = array('default' => '12');
    $options['record-xs-columns'] = array('default' => '12');
    $options['view-xs-offset'] = array('default' => '0');
//    $options['record-xs-offset'] = array('default' => '0');

    $options['view-sm-columns'] = array('default' => '12');
    $options['record-sm-columns'] = array('default' => '6');
    $options['view-sm-offset'] = array('default' => '0');
//    $options['record-sm-offset'] = array('default' => '0');

    $options['view-md-columns'] = array('default' => '12');
    $options['record-md-columns'] = array('default' => '4');
    $options['view-md-offset'] = array('default' => '0');
//    $options['record-md-offset'] = array('default' => '0');

    $options['view-lg-columns'] = array('default' => '12');
    $options['record-lg-columns'] = array('default' => '3');
    $options['view-lg-offset'] = array('default' => '0');
//    $options['record-lg-offset'] = array('default' => '0');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(
      0 => 0,
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
      10 => 10,
      11 => 11,
      12 => 12,
    );

    $sizes = array(
      'xs' => t('Extra Small Devices'),
      'sm' => t('Small Devices'),
      'md' => t('Medium Devices'),
      'lg' => t('Large Devices'),
    );


    foreach($sizes as $index => $size) {
      $form['prefix-' . $index]['#markup'] = "<div style=\"display: inline-block; width: 100%; margin-bottom: 20px; border-top: 1px solid #cfcfcf; padding-top: 20px; \">";

      $form['prefix-settings' . $index]['#markup'] = "<div style=\"float: left\">";
      $form['prefix-title' . $index]['#markup'] = "<h3>" . $size . "</h3>";

      $form['view-' . $index . '-columns'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Columns for View'),
        '#default_value' => $this->options['view-' . $index . '-columns'],
        '#required' => TRUE,
        '#element_validate' => array('views_element_validate_integer'),
      );

      if($index != 'xs') {
        $form['view-' . $index . '-offset'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#title' => t('Offset for View: '),
          '#default_value' => $this->options['view-' . $index . '-offset'],
          '#required' => TRUE,
          '#element_validate' => array('views_element_validate_integer'),
        );
      }

      $form['record-' . $index . '-columns'] = array(
        '#type' => 'select',
        '#title' => t('Columns Per Record: '),
        '#options' => $options,
        '#required' => TRUE,
        '#element_validate' => array('views_element_validate_integer'),
        '#default_value' => $this->options['record-' . $index . '-columns'],
      );

      $form['record-' . $index . '-trigger'] = array(
        '#type' => 'button',
        '#attributes' => array(
          'style' => array('margin-top: 15px')
        ),
        '#value' => t('Show Me Preview'),
      );
      // This is just a mess in views...
      views_ui_add_ajax_trigger($form, 'trigger', array('options', 'style_options'));

      $form['suffix-settings' . $index]['#markup'] = "</div>";
      $mapping = array(
        'view_cols' => 'view-' . $index . '-columns',
        'view_offset' => 'view-' . $index . '-offset',
        'record_cols' => 'record-' . $index . '-columns',
        'record_offset' => 'record-' . $index . '-offset',
      );

      $settings = array(
        'device' => $index,
      );

      foreach($mapping as $var => $index) {
        $settings[$var] = $this->options[$index];
        if(isset($form_state['input']['style_options'][$index])) {
          $settings[$var] = $form_state['input']['style_options'][$index];
        }
      }


      $form[$index]['visual']['visual-' . $index] = array(
        '#markup' => theme('bootstrap_grid_preview', $settings),
      );

      $form['suffix-' . $index]['#markup'] = "</div>";
    }
  }
}
