<?php
$qparams = drupal_get_query_parameters();
if (isset($qparams['field_company_name_value']) && $qparams['field_group_location_tid_i18n'] == 'All'){
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
  ->fieldCondition('field_company_name', 'value', $qparams['field_company_name_value'], '=')
  ->addMetaData('account', user_load(1));
  $result = $query->execute();
  if (isset($result['user'])) {
    global $base_url;
    drupal_goto($base_url.'/user/'.current($result['user'])->uid);
  }
}
?>
<div class="row">
  <div <?php print drupal_attributes($view_attributes); ?> style="float: left;">
    <div class="row">
      <?php foreach ($records as $key=>$record): ?>
          <div <?php print drupal_attributes($record['attributes']); ?>>
            <?php
              $user_obj = user_load($view->result[$key]->uid);
              $user_data = user_view($user_obj,'view_grid_display');
              print '<div class="row-inner">'.drupal_render($user_data).'</div>';
            ?>
          </div>
      <?php endforeach; ?>
    </div>
  </div>
</div>
