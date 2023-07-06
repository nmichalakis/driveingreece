(function ($) {
Drupal.behaviors.pt_ajax_link = {
attach: function(context, settings) {

$("#gallery-close").click(function(event){
  $('#navbar').fadeIn("slow");
});



//Replace menu hrefs with ajax node hrefs
$('#navbar nav ul li').each(function( index ) {
            menu_href =  $(this).children('a').attr('href');
            menu_href = menu_href.replace('/node/','/ajaxnode/');
            $(this).children('a').attr('href',menu_href);
        });

$( "#navbar ul li a" ).once().click(function( event ) {

       $('#navbar nav ul li').each(function(index) {
          $(this).children('a').removeClass('active-menu-link');
       });
       $(this).addClass('active-menu-link');

       $('#block-system-main').empty().append('<div id="ajax-loader"></div>');
       //$('body').addClass('ajax-loaded-once');
        event.preventDefault();
         $.ajax({
                url: event.currentTarget.href ,
                success: function(data) {

                    //console.log(data);
                    //console.log(data.img);
                    var img = data.img;

                    if($('#bodypage').length > 0){
                        $('#bodypage').remove();
                    }
                    $('#superslides').superslides('destroy');
                    $('#superslides').remove();
                    if(data.oneimage){
                      $('#block-system-main').html('<div id="superslides"><ul class="slides-container"><li><img src="'+data.img+'"/></li></ul></div>');
                       reset_simple_superslides();
                    }else{
                       superslides_html = '<div id="superslides"><ul class="slides-container">';
                       $.each(data.img, function(key,imgurl) {
                       superslides_html += '<li><img src="'+imgurl+'"/></li>';
                    });
                      if(data.nid == 10){
                         superslides_html += '</ul> <nav class="slides-navigation"><a href="#" class="next">Next</a><a href="#" class="prev">Previous</a></nav></div>';
                         //$('#navbar').fadeOut('slow');
                      }else{
                      superslides_html += '</ul></div>';
                      }
                      $('#block-system-main').html(superslides_html);
                      reset_superslides();
                      }

      $('#block-system-main').waitForImages(function() {
}, function(loaded, count, success) {
   $(this).fadeIn(1200);
});

                if(data.nid != 10){
                    $('#block-system-main').append('<div id="bodypage"><div class="bodypage-inner"><div id="bginner">'+data.body[0].safe_value+'</div></div><div id="close-me"><a href="#">CLOSE | X</a></div></div>');
                    $('#bodypage').css({'top':Math.round($('#navbar').height())+'px'});
                    $('#bodypage').stop(true, true).delay(500).slideDown("slow" ,  function() {
                        });
};

$('#bginner').slimScroll({
    color: '#bf9836',
    size: '4px',
    height: '280px',
    alwaysVisible: true,
});


        $("#bodypage #close-me").once().click(function(event){
            $('#bodypage').slideUp("slow");
        });


                    /*
                    $('.main-container .view-welcome .view-content').append('<div id="slides"><div class="slides-container"><img src="'+data.img+'"/></div>');
                    $('.main-container .view-welcome .view-content').append('<div id="bodypage"><div class="bodypage-inner"><div class="scroll-pane">'+data.body[0].safe_value+'</div></div><div id="close-me"><a href="#">x</a></div></div>');
                    $('.main-container').fadeIn();
                    $('#slides').superslides();
                    //console.log('container height:'+$('#bodypage').height());
                    maxheight = $(window).height()-Math.round($('#navbar').height());

                    $('#bodypage').css({'top':Math.round($('#navbar').height())+'px'});
                    //$('.bodypage-inner').css({'height':Math.round(maxheight/2)+'px'});

                    $('#bodypage').stop(true, true).delay(500).slideDown("slow");
                        */
                     }
            }); //end of ajax call
});




function reset_superslides(){
    $('#superslides').superslides({
    animation: 'fade',
    //slide_easing: 'easeInOutCubic',
    //slide_speed: 1100,
    pagination: true,
    play: 8000,
    //hashchange: true,
    //scrollable: true
  });
}
function reset_simple_superslides(){
$('#superslides').superslides({
    animation: 'fade',
  });
}

}//call theme OK

};//call theme jqery
})(jQuery);
