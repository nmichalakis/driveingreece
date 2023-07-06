(function ($) {
Drupal.behaviors.xlfa_ajax_link = {
attach: function(context, settings) {


  $('.tmlink').click(function() {
								
								var targetOffset = $('#block-block-23 .block-content').offset().top - 50;
 
                $('html,body').animate({scrollTop: targetOffset}, 1000);

   // This function will get exceuted after the ajax request is completed successfully
    var updateData = function(data) {

      $('#block-block-23 .block-content').html(data.nodemap);
    }
    $.ajax({
      type: 'POST',
      url: this.href, // Which url should be handle the ajax request. This is the url defined in the <a> html tag
      success: updateData, // The js function that will be called upon success request
      dataType: 'json', //define the type of data that is going to get back from the server
      data: 'js=1' //Pass a key/value pair
    });
    return false; 
      


      });

}//call theme OK

};//call theme jqery
})(jQuery);