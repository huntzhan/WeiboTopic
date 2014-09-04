
$(function() {
    $('[id^="get_sub_topics"]').bind('click', function() {
        $.getJSON('/sub_topics', {
                timestamp: $(this).find('input[name="timestamp"]').val(),
                id: $(this).find('input[name="id"]').val()
        }, function(data){
            alert("success");
        });
        return false;
    });
});
