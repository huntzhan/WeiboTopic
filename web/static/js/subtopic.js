
var CreateDialog = function(topic_mainidea, topic_keywords, subtopic) {
    $.get('/templates/subtopic_dialog.html', function(data) {
        var dialog = $(data);
        dialog.find('#topic_mainidea').text(topic_mainidea);
        dialog.find('#topic_keywords').text(topic_keywords);
        dialog.find('#topic_keywords').text("Number of SubTopics: " + subtopic.sub_topics.length);
        var dialog_body = dialog.find('#dialog_body');
        $.each(subtopic.sub_topics, function(index, sub) {
            var head = $("<h4 id='sub_topic'></h4>").append("Sub Topic " + index.toString());
            dialog_body.append(head);
            var number_of_blogs = $("<p></p>").append("Number of blogs: <span class='label label-default'>" + sub.number_of_blogs.toString() + "</span>");
            dialog_body.append(number_of_blogs);
            var table = $("<table class='table table-striped'></table>");
            $.each(sub.blogs, function(index_blog, blog) {
                var row = $("<tr></tr>").html("<td>" + blog.keywords + ":<br>" + blog.content + "<br></td>");
                table.append(row);
            });
            dialog_body.append(table);
            dialog_body.append("<hr>");
        });
        $('#topic-dialog').html(dialog.html());
        $('#topic-dialog').modal();
        // window.location.replace('#topic-dialog');
    }, 'text');
}

$(function() {
    $('[id^="get_sub_topics"]').bind('click', function() {
        var topic_a = $(this);
        $.getJSON('/sub_topics', {
                timestamp: topic_a.find('input[name="timestamp"]').val(),
                id: topic_a.find('input[name="id"]').val()
        }, function(data){
            // alert(CreateDialog(data));
            CreateDialog(topic_a.text(), topic_a.parent().next().text(), data);
            // alert('success');
        });
        return false;
    });
});
