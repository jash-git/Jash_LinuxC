var SEARCH_TAB_INDEX = 3;
var highlightEnabled = document.cookie.match(/highlight=true/i);
var advancedSearchEnabled = document.cookie.match(/advsearch=true/i);
var lastQuery = null;
var lastResult = -1;

/* setups jquery ui tabs for online version (treeview+search) */
function setupPage() {
    var p = relpath + "/assets/ui_images/tabs/";
    var selectedIndex = getTabIndex();
    tabs[SEARCH_TAB_INDEX] = search_init();
    $("#tabs").tabs({
        images: { 
            "select-search": [p + "search.png", p + "search_hover.png", p + "search_active.png"],
            "select-services": [p + "topics.png", p + "topics_hover.png", p + "topics_active.png"],
            "select-libraries": [p + "libraries.png", p + "libraries_hover.png", p + "libraries_active.png"],
            "select-guide": [p + "developer_library.png", p + "developer_library_hover.png", p + "developer_library_active.png"]
        },
        tabShow: function(index) {
            if (tabs[index] == undefined)
                treeInit(index);
        },
        selected: selectedIndex
    });
    if (selectedIndex == SEARCH_TAB_INDEX)
        pageInit(selectedIndex);
}

/* gets page parameters from url, updates links accordingly, and 
   performs a search */
function pageInit(selectedIndex) {
    var m = /q=([^&]*)/.exec(location.search);
    if (m) {
        var keyword = decodeURIComponent(m[1]);
        var startat = 0;
        var params = {"tab": selectedIndex, "q": keyword};
        var maxpage;
        m = /sa=(\d+)/.exec(location.search);
        if (m) {
            startat = m[1];
            params.sa = startat;
        }
        m = /linkno=(\d+)/.exec(document.cookie);
        if (m) {
            maxpage = (Math.floor(parseInt(m[1]) / 20) + 1) * 20 + 10;
        }
        m = /sp=([^&]*)/.exec(location.search);
        if (m) {
            params.sp = m[1];
            $("#package-select").val(m[1]);
        }
        doSearch(keyword, startat, false, maxpage);
        setLinks(params);
    }
}

function search_init() {
    $("#search-field").autocomplete({
        source: function(request, response) {
            if (request.term.match(/\s+$/)) {
                $("#search-field").autocomplete("close");
                return;
            }
            var scope = $("#package-select").val();
            $.ajax({
                url: relpath + "/../suggest.php?count=10" + 
                    (scope == "all" ? "" : "&scope=" + scope),
                dataType: "json",
                data: request,
                success: response
            });
        },
        select: function(event, ui) {
            if (ui.item) {
                if (ui.item.url) {
                    clearCookies();
                    var scope = $("#package-select").val();
                    scope = scope == "all" ? "" : "&sp=" + scope;
                    var q = "?tab=" + SEARCH_TAB_INDEX + "&q=" + ui.item.value + scope;
                    location.href = ui.item.url + q;
                } else {
                    doSearch(ui.item.value);
                }
            }
        },
        html: true                
    });
    $("#search-box").keyup(function(e) {
        var keycode;
        if (window.event) 
            keycode = window.event.keyCode;
        else if (e) 
            keycode = e.which;
        if (keycode == 13) {
            resetAndSearch();
            $("#search-field").autocomplete("close");
            $("#select-search").click();
        }
    });
    $("#checkbox").attr("checked", highlightEnabled).click(function() {
        if (highlightEnabled === this.checked) return;
        highlightEnabled = this.checked;
        if (this.checked) {
            setCookie("highlight", "true");
            if (lastQuery)
                highlight(lastQuery);
        } else {
            setCookie("highlight", "false");
            removeHighlight();
        }
    });
    $("#search-button").click(function() {
        $("#select-search").click();
        resetAndSearch();
    });
    $("#package-select").change(function() {
        resetAndSearch();
    });
    $("#search-box").find("[href=#advanced-search]").click(function() {
        if ($(this).html().indexOf("Disable") < 0) {
            enableAdvancedSearch(true);
        } else {
            enableAdvancedSearch(false);
        }
        return false;
    });
    $("#search-"+SEARCH_TAB_INDEX).scroll(function() {
        if (this.scrollTop >= (this.scrollHeight-this.clientHeight-150)) {
            var m = $("#result-header").html().match("^(\\d+)");
            var total = parseInt(m[1]);
            var id = $(".search-result").last().attr("id");
            var last = parseInt(id.match("(\\d+)$")[1]);
            if (last+1 < total && last > lastResult) {
                doSearch(lastQuery, last+1, true);
                lastResult = last;
            }
        }
    });
    $("#search-button").bind("mouseover", function() {
        var src = $(this).attr("src");
        src = src.replace("normal", "hover");
        $(this).attr("src", src);
    })
    .bind("mouseout", function() {
        var src = $(this).attr("src");
        src = src.replace("hover", "normal");
        $(this).attr("src", src);
    });
    enableAdvancedSearch(advancedSearchEnabled);

    return true;
}

function doSearch(query, startat, append, maxpage) {
    query = $.trim(query).replace(/\s{2,}/g, ' ');
    if (query == '') 
        return;

    $("#search-field").attr("value", query);
    
    var params = new Object();
    params.query = query;
    params.startat = startat;
    params.scope = $("#package-select").val();
    params.maxpage = maxpage;
    
    $.get(relpath + '/../search.php', params, function(results) {
        if (append) {
            $("#search-results").append(results);
        } else {
            var m = document.cookie.match("scroll" + SEARCH_TAB_INDEX + "=(\\d+)");
            var scrollTop = m ? parseInt(m[1]) : 0;
            $("#search-results").html(results).css('display', '');
            $("#search-"+SEARCH_TAB_INDEX).attr("scrollTop", scrollTop);
        }
        $("#search-results a").click(function() {
            var s = $("#search-" + SEARCH_TAB_INDEX).attr("scrollTop") || 0;
            var m = $(this).parent().attr("id").match("(\\d+)$"); 
            if (m) {
                setCookie("linkno", m[1]);
                setCookie("scroll" + SEARCH_TAB_INDEX, s);
            }
        });
        $("#body-content").removeHighlight();
        highlight(query);
        lastQuery = query;
    });
}

function resetAndSearch() {
    var q = $("#search-field").attr("value");
    lastResult = -1;
    clearCookies();
    doSearch(q);
}

function enableAdvancedSearch(enable) {
    var $el = $("#search-box").find("[href=#advanced-search]");
    if (enable) {
        $el.html("Disable advanced search");
        $(".sidebar-tab").css("padding-top", "9em");
        $("#advanced-search").show();
        setCookie("advsearch", "true");
    } else {
        $(".sidebar-tab").css("padding-top", "6em");
        $el.html("Advanced search");
        $("#advanced-search").hide();
        setCookie("advsearch", "false");
    }
}

function highlight(query) {
    if (highlightEnabled) {
        $("#body-content").highlight(query);
        $("#search-results").highlight(query);
    }
}

function removeHighlight() {
    $("#body-content").removeHighlight();
    $("#search-results").removeHighlight();
}

function setCookie(name, value) {
    document.cookie = [name, "=", value, ";path=", cookiepath].join("");
}

function clearCookies() {
    setCookie("scroll" + SEARCH_TAB_INDEX, "");
    setCookie("linkno", "");
}

function resizeCallback(w) {
    if (w === undefined)
        w = $("#sidebar").width();
    $("#search-box").css("width", 1+parseInt(w)+"px");
}
