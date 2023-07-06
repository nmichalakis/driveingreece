(function ($) {
    Drupal.behaviors.carental = {
        attach: function (context, settings) {
// Docs at http://simpleweatherjs.com
$(window).bind("load",function(){
    manage_jquery_effects();
    append_goback_accordion_links();
});//windload ready

$(window).resize(function () {
    manage_jquery_effects();
});


function append_goback_accordion_links(){
    $('.ui-accordion .views-row').each(function(){
        var current_accordion_id = $(this).children('.ui-accordion-content').attr('id');
        var current_accordion_header = $(this).children('.ui-accordion-header').attr('id');
        console.log(current_accordion_id);
        var goback_link = '<a href="#'+current_accordion_header+'"><span class="glyphicon glyphicon-arrow-up"></span></a>';
        if ($(this).find('.agencies-inner')){
            $('#'+current_accordion_id+' .agencies-inner').append(goback_link);
        }
        else {
            $('#'+current_accordion_id+' .views-field-body .field-content').append(goback_link);
        }
    });
}


function manage_jquery_effects(){

    // var div = L.DomUtil.get('leaflet-map');
    // if (div){
    //     if (!L.Browser.touch) {
    //         L.DomEvent.disableClickPropagation(div);
    //         L.DomEvent.on(div, 'mousewheel', L.DomEvent.stopPropagation);
    //     } else {
    //         L.DomEvent.on(div, 'click', L.DomEvent.stopPropagation);
    //     }
    
    //     L.DomEvent.dragging.disable();
    // }



    if ($('.view-agencies').length > 0) {
        $('.view-agencies .grid-item .row-inner').matchHeight({});
    }

    if ($('.view-agencies-map').length > 0) {
        $('.view-agencies-map .grid-item .row-inner').matchHeight({});
    }

    if ($('.view-news').length > 0) {
        $('.view-news .grid-item .row-inner').matchHeight({});
    }

    if ($(window).width() > 767) {
        $(".view-georegions .views-row").each(function (index) {
            $(this).css('width', Math.round($(this).width() * 1.2) + 'px');
        });
    }
}



  
$(".group-association-link").once().click(function(e){
    e.preventDefault();
    var id = $(this).attr("data-group-id");
    console.log(id);
    replaceView(id);
});


function replaceView(args) {
	
    $.ajax({
      url: Drupal.settings.basePath + 'views/ajax',
      type: 'post',
      data: {
        view_name: 'agencies', //machine name of your view
        view_display_id: 'block_4', //machine name of your display id
        view_args: 'field_agency_group_location = '+ args,
      },
      dataType: 'json',
      success: function (response) {
          console.log(response);
          if (response[1] !== undefined) {
          var viewHtml = response[1].data;
          $("#content-agency-group-"+args+' .view-content').html(viewHtml);
          //$("#content-"+id+' .view-content').innerHTML=response.display;
          Drupal.attachBehaviors(); //check if you need this.
        }
        
      },
          error: function(data) {
           alert('An error occured!');
          }
    });
}

// $(".group-association-link").click(function(event){
//     event.preventDefault();
//     var id = $(this).attr("id");
//     console.log(id);
//     //  $("#content-"+id+' .view-content').fadeOut();
//         $.ajax({
//             url: Drupal.settings.basePath + 'views/ajax',
//             type: 'POST',
//             // dataType: 'json',
//             // data: 'view_name=agencies&view_display_id=block_4&view', // Pass a key/value pair.
//             data : {
//                 view_name : 'agencies',
//                 view_display_id : 'block_4',
//                 view_args : 'field_agency_group_location = '+$(this).attr('data-group-id'),
//             },
//             success: function(response) {
//                 console.log(response);
//               $("#content-"+id+' .view-content').innerHTML=response.display;
//             },
//             error: function(data) {
//              alert('An error occured!');
//             }
//           });

// });



    }//call theme OK

    }
    ;
//call theme jqery
})
    (jQuery);



