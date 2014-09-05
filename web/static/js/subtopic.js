
var CreateDialog = function(subtopic) {
    var dialog;
    // $.get('../../templates/subtopic_dialog.html', function(data) {
    //         alert(data)
    // }, 'text');
    $.get('/templates/subtopic_dialog.html', function(data) {
        // alert(data);
        $('#topic-dialog').html(data);
        $('#topic-dialog').modal();
        // window.location.replace('#topic-dialog');
    }, 'text');
}

$(function() {
    $('[id^="get_sub_topics"]').bind('click', function() {
        $.getJSON('/sub_topics', {
                timestamp: $(this).find('input[name="timestamp"]').val(),
                id: $(this).find('input[name="id"]').val()
        }, function(data){
            // alert(CreateDialog(data));
            CreateDialog(data);
            // alert('success');
        });
        return false;
    });
});
