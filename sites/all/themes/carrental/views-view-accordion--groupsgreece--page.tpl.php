<?php
/**
 * @file
 * Displays the items of the accordion.
 *
 * @ingroup views_templates
 *
 * Note that the accordion NEEDS <?php print $row ?> to be wrapped by an
 * element, or it will hide all fields on all rows under the first field.
 *
 * Also, if you use field grouping and use the headers of the groups as the
 * accordion headers, these NEED to be inside h3 tags exactly as below
 * (though you can add classes).
 *
 * The current div wraping each row gets two css classes, which should be
 * enough for most cases:
 *     "views-accordion-item"
 *      and a unique per row class like item-0
 */
global $language;
$link_text = 'προβολή όλων';
if ($language->language == 'en'){
  $link_text = 'view all';
}
?>
<?php if (!empty($title)): ?>
  <h3 class="<?php print $view_accordion_id; ?>">
    <?php print $title; ?>
  </h3>
<?php endif; ?>
<?php if ($use_group_header): ?><div><?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
<?php $view_result = views_get_view_result('agencies','block',$view->result[$id]->nid);?>

  <?php if (is_array($view_result) && count($view_result)>0):?>
  <div class="<?php print $classes_array[$id]; ?>">
    <?php print $row;
    $region_node = node_load($view->result[$id]->nid);

    if (isset($region_node->field_group_location[$language->language][1]['tid'])){
      $group_location_tid = $region_node->field_group_location[$language->language][1]['tid'];
    }
    else {
      $group_location_tid = $region_node->field_group_location['en'][0]['tid'];
    }

    print '<div id="content-agency-group-'.$group_location_tid.'" class="agencies-inner container">'.views_embed_view('agencies','block',$view->result[$id]->nid);
    print '</div>';
    ?>
  </div>
<?php endif; ?>
<?php endforeach; ?>
<?php if ($use_group_header): ?></div><?php endif; ?>
