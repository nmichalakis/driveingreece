<div class="row">
  <div <?php print drupal_attributes($view_attributes); ?> style="float: left;">
    <div class="row">
      <?php foreach ($records as $record): ?>
          <div <?php print drupal_attributes($record['attributes']); ?>>
             <div class="row-inner"><?php print $record['record']; ?></div>
          </div>
      <?php endforeach; ?>
    </div>
  </div>
</div>