/* handles tab images: hover, selection */
(function($) {
    $.fn.extend({
        tabs: function(opts) {
            return this.each(function() {
                var selected = opts.selected || 0;
                var $active;
                $("ul", this).first().find("li").each(function() {
                    var $img = $(this).find("img").first();
		    var href = $(this).find("a").first().attr("href");
		    var index = parseInt(/#.*-(\d+)/.exec(href)[1]);
                    var imgId = $img.attr("id");
                    $img.click(function() {
                        selectTab(imgId, index, opts);
                        return false;
                    })
                    .bind("mouseenter", function() {
                        if ($img.attr("src") != opts.images[imgId][2]) 
                            $img.attr("src", opts.images[imgId][1]); 
                    })
                    .bind("mouseleave", function() {
                        if ($img.attr("src") != opts.images[imgId][2])     
                            $img.attr("src", opts.images[imgId][0]); 
                    });
                    if (index === selected)
                        $active = $img
                });
                $active.click();
            });
        }
     });

    function selectTab(active, index, o) {
        if (typeof o.tabShow == "function") {
            o.tabShow(index);
        }
        for (x in o.images) {
            if (x == active) {
                var id = $("#" + x).attr("src", o.images[x][2]).parent().attr("href");
		$(id).show();
            } else {
                var id = $("#" + x).attr("src", o.images[x][0]).parent().attr("href");
		$(id).hide();
            }
        }
    }

})(jQuery);
 
