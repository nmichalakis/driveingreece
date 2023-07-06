<div class="device <?php print $class ?>">

<div style="width: 310px; display: inline-block;">
  <div class="display">
  <div class="col-<?php print $device; ?>-offset-<?php print $view_offset;?> col-<?php print $device; ?>-<?php print $view_cols;?>">
  <h3>Preview</h3>

  <?php for($j = 1; $j<= 3; $j++): ?>
    <?php for($i = 1; $i<= $records; $i++): ?>
      <div class="col-<?php print $device; ?>-<?php print $record_cols;?>">
           <div class="inner">
             <div class="grid-item">

             </div>
           </div>
      </div>
    <?php endfor; ?>
  <?php endfor; ?>
  </div>
  </div>
</div>
</div>