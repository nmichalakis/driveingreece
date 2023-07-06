<?php

/**
 * @file
 * template.php
 */
function carrental_preprocess_html(&$variables) {
  global $site_name;
	drupal_add_css('http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=latin,greek', array('type' => 'external'));

  $cpath = current_path();
  $params = drupal_get_query_parameters();
  if ($cpath == 'agencies' && isset($params['field_group_location_tid_i18n']) && isset($translated_term->name)){
    $tax = taxonomy_term_load($params['field_group_location_tid_i18n']);
    $translated_term = i18n_taxonomy_localize_terms($tax);
    $variables['head_title'] = $variables['head_title_array']['name'] .' / '.t('Agencies').' / '.($translated_term->name);
  }

	$theme_path = drupal_get_path('theme', 'carrental');
    drupal_add_js($theme_path . '/carental.js',array('type' => 'file', 'scope' => 'footer', 'weight' => 54));
}


function carrental_links__locale_block(&$vars) {
  foreach($vars['links'] as $language => $langInfo) {
    $abbr = $langInfo['language']->language;
    $name = $langInfo['language']->name;
    $vars['links'][$language]['title'] = substr($name,0,2);
    $vars['links'][$language]['html'] = TRUE;
  }
  $content = theme_links($vars);
  return $content;
}

/**
* Implements theme_menu_link().
* Adds menu description under main menu
*/
function carrental_menu_link(array $variables) {
  $element = $variables['element'];

  $sub_menu = '';
  $element['#localized_options']['html'] = TRUE;

  if ($element['#href'] =='google.com'){
  }
  if ($element['#href'] =='user'){

  if (user_is_logged_in()){
      $element['#href'] = 'user/logout';
      $element['#title'] = 'Logout';
    }
  }
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  if ($element['#original_link']['menu_name'] == "main-menu" && isset($element['#localized_options']['attributes']['title'])){
    $element['#title'] .= '<span class="description">' . $element['#localized_options']['attributes']['title'] . '</span>';
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}




function carrental_preprocess_page(&$vars, $hook) {
  if (isset($vars['node']->type)) {
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
  }

  global $language;
  if (current_path() == 'news'){
    if ($language->language == 'el'){
      drupal_set_title('Τα νέα μας');
    }
    else {
      drupal_set_title('Our news');
    }
  }
}
