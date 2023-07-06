(function ($) {
Drupal.behaviors.pt_ajax_link = {
attach: function(context, settings) {


$("#close-gallery").click(function(event){
    $('#snavigator').fadeOut('1200');
    $('#navbar').fadeIn('1200');
    $('.footer').fadeIn('1200');
    $(this).fadeOut('1800');
});



      //Replace menu hrefs with ajax node hrefs
      $('#navbar .navbar-collapse ul li').each(function( index ) {
            menu_href =  $(this).children('a').attr('href');
            menu_href = menu_href.replace('/node/','/ajaxnode/');
            $(this).children('a').attr('href',menu_href);
            $(this).children('a').attr('id', 'menu-link-'+index);
            $(this).children('a').addClass('active-menu-link-'+index);
        });

       $( "#navbar .navbar-collapse ul li a" ).one().click(function( event ) {
              event.preventDefault();
                              $('#superslides .slides-pagination').fadeOut();

              $('#superslides').superslides('stop');

              $('#navbar .navbar-collapse ul li').each(function( index ) {
                    $(this).children('a').removeClass('is-active-link');
              });
              $(this).addClass('is-active-link');
              if($(this).hasClass('active-menu-link-3')){
                $('#web-loader').fadeIn('1200');
                $('#bodypage').slideUp("slow", function() {
                $('#navbar, .footer').fadeOut('800');
                $('#close-gallery, #snavigator').delay(800).fadeIn();
                });

              }else{

                $('#close-gallery').fadeOut();
                $('#snavigator').fadeOut();
                $('#navbar').fadeIn();


              }



              //$('#superslides').superslides('stop');



         $.ajax({
                url: event.currentTarget.href ,
                 beforeSend: function(msg){

                },
                success: function(data) {

                  if(data.nid != 16){

                    //contact page
                    if(data.nid == 11){
                      var safe_html = '<h2>'+data.ntitle+'</h2><div class="left-contact-block">'+data.body[0].safe_value+'</div><div class="right-contact-block">'+data.cblock_html+'</div>';
                    }else{
                      var safe_html = '<div class="col-md-12">'+data.body[0].safe_value+'</div>';
                    }


                            $('#bodypage').slideUp("slow", function() {
                            $('#block-system-main #bodypage').html('<div class="bodypage-inner"><div id="bginner">'+safe_html+'</div></div><div id="close-me"><a href="#">CLOSE | X</a></div>');
                            $('#bodypage').css({'top':Math.round($('#navbar').height())+'px'});

                              $("#close-me").click(function(event){
                              $('#bodypage').slideUp('slow');
                          });



                    var window_width = $( window ).width();
                    if(window_width > 640){
                        var scrollheight = 320;
                      }else{
                        var scrollheight = 200;

                    }


                    //Skip Contact Page slimScroller
                    if (data.nid != 11){
                      $('#bginner').slimScroll({
                          color: '#bf9836',
                          size: '5px',
                          height: scrollheight+'px',
                          alwaysVisible: true,
                      });
                      $('body').removeClass('page-contact');
                    }else{
                       $('body').addClass('page-contact');
                    }


                            });


                       /*
                      //Reset Gallery to front page gallery node
                      if($('body').hasClass('gallery-loaded')){
                          var img = data.img;
                          var superslides_html = '<ul class="slides-container">';
                           $.each(data.img, function(key,imgurl) {
                           superslides_html += '<li><img src="'+imgurl+'"/></li>';
                              });
                           superslides_html += '</ul>';
                           $('#superslides').superslides('destroy');
                           $('#superslides').html(superslides_html);
                           $('#superslides .slides-pagination').remove();
                           reset_superslides();
                           $('body').removeClass('gallery-loaded');
                      }
                      */
                           $('#bodypage').slideDown("slow", function() {
                            //$('#superslides').superslides('start');
                           });

                         //   $('#superslides .slides-pagination').fadeIn('slow');

                }else{
                  $('body').addClass('gallery-loaded');

                  if(!$('body').hasClass('gallery-loaded-once')){
                  //$('#superslides .slides-pagination').fadeOut('fast');
                   $.each(data.img, function(key,imgurl) {
                       $('#superslides .slides-control ul').append('<li><img src="'+imgurl+'"/></li>');
                       $('#superslides').superslides('update');
                          });
                    $('#superslides .slides-pagination a').each(function( index ) {
                      $(this).attr('href','#'+(index+1));
                      $(this).html((index+1));
                    });
                    $('#superslides').superslides('update');
                    $('body').addClass('gallery-loaded-once');
                  }
                                      $('#slides').superslides('animate',3);

                  /*
                  var img = data.img;
                  var superslides_html = '<ul class="slides-container">';
                       $.each(data.img, function(key,imgurl) {
                       superslides_html += '<li><img src="'+imgurl+'"/></li>';
                          });
                       superslides_html += '</ul><div class="container-slides-navigation"><nav class="slides-navigation"><a href="#" class="next">Next</a><a href="#" class="prev">Previous</a></nav></div>';
              */
                      // $('#superslides').superslides('destroy');
                      $('#web-loader').fadeOut(1400, function(){

                     });
                }



                     }
            }); //end of ajax call
});




function reset_superslides(){
    $('#superslides').superslides({
    animation: 'fade',
    slide_easing: 'easeInOutCubic',
    //slide_speed: 1100,
    pagination: true,
    play: 7000,
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
