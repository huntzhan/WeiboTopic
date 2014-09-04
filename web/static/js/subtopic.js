
var CreateDialog = function(subtopic) {
    var dialog = $("<div id='topic-dialog' class='modal fade' tabindex='-1' role='dialog'></div>");
    dialog.load('templates/subtopic_dialog.html');
    return dialog.html()
}

$(function() {
    $('[id^="get_sub_topics"]').bind('click', function() {
        $.getJSON('/sub_topics', {
                timestamp: $(this).find('input[name="timestamp"]').val(),
                id: $(this).find('input[name="id"]').val()
        }, function(data){
            // alert(CreateDialog(data));
            alert('alert');
        });
        return false;
    });
});
