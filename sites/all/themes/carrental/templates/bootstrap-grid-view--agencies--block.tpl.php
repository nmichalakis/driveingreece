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
