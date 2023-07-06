<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */

//Try to see if there are nodes attachhed on this
// dpm($row->taxonomy_term_data_name);
$tax_nodes = taxonomy_select_nodes($row->tid);
// dpm($tax_nodes);
// foreach ($tax_nodes as $key => $tax_node) {
//   $node = node_load($tax_node);
//   drupal_set_message($node->title);
// }
if (is_array($tax_nodes) && count($tax_nodes)>0 ){
  $output = str_replace('</a>', '<div class="region-wball"></div></a>',$output);
}
else {
  $output = '<div class="region-nolink">'.strip_tags($output) . '<div class="region-wball"></div></div>';
}

print $output;
?>
