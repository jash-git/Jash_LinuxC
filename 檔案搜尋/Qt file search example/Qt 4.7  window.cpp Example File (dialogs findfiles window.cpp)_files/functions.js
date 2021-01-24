var tabs = new Array();

var TOPICS_TAB_INDEX = 0;
var LIBRARIES_TAB_INDEX = 1;
var GUIDE_TAB_INDEX = 2;
var DEFAULT_TAB_INDEX = TREEVIEWS.length >= 3 ? GUIDE_TAB_INDEX : TOPICS_TAB_INDEX; 

var cookiepath="/";         // cookie path component

function init()
{
    /* IE version check: older than 8.0 are not supported */
    var IEver = navigator.userAgent.match("MSIE [0-9]*");
    if (IEver && (document.cookie.match("oldIE=") == null)) {
        IEver = parseInt(IEver[0].slice(4));
        if (IEver<8) {
            alert("Warning! IE versions older than 8.0 are not supported.");
            document.cookie = "oldIE=1;path="+cookiepath;
        }
    }

/*
    var pos = navigator.userAgent.indexOf("NokiaN9");
    if (pos > -1 && document.cookie.match("NokiaN9=") == null) {
        document.cookie = "NokiaN9=1;path="+cookiepath;
        alert("When using the Developer Library on a Harmattan device, use two fingers to scroll. For the best viewing experience, we recommend accessing the site from your workstation.");
    }
*/
    /* Check if the web browser is a mobile version or has a small screen resolution */
    if(navigator.userAgent.match(/NokiaN9/i) || navigator.userAgent.match(/Maemo/i) || navigator.userAgent.match(/Android/i) || navigator.userAgent.match(/webOS/i) || navigator.userAgent.match(/iPhone/i) || navigator.userAgent.match(/iPod/i) || screen.width <= 650 ){
        setupMobileVersion();
    }
    /* path for cookies */
    if (location.pathname.length > 1) {
        var upcount = 0;
        prefixparts = relpath.split("/");
        for (var i in prefixparts) {
            if (prefixparts[i]=="..")
                upcount++;
        }
        
        var pathparts = location.pathname.slice(1).split("/");
        cookiepath="";
        for (var i=0; i<pathparts.length-upcount-1; i++)
            cookiepath += "/" + pathparts[i];
    }
    
    /* read build stamp */
    var oldBuild = document.cookie.match("build=[0-9-]*");
    if (oldBuild)
        oldBuild = (oldBuild[0].slice(6));
    else
        oldBuild = "";
    
    if (oldBuild != build) {//build changed -> invalidate old tree cookies
	for (var i = 0; i < TREEVIEWS.length; i++) {
            document.cookie = "tree"+i+"=;"+"path="+cookiepath;
            document.cookie = "scroll"+i+"=;"+"path="+cookiepath;
	}
    }
    
    /* write current build stamp */
    document.cookie = "build="+build+";path="+cookiepath;
    
    /* init treeview in sidebar */
    var index = getTabIndex();
    if (TOPICS_TAB_INDEX == index || LIBRARIES_TAB_INDEX == index || GUIDE_TAB_INDEX == index) {
        treeInit(index);
        if (index != DEFAULT_TAB_INDEX)
            setLinks({"tab": index});
    }

    setupPage();

    /* sidebar resizing using jquery ui */
    $("#sidebar").resizable({handles: 'e'}).bind("resize", function(event, ui) {
        var p = parseInt($("#sidebar").css("width").slice(0,-2))
                + parseInt($("#sidebar").css("padding-left").slice(0,-2))*2
                + parseInt($("#content_and_sidebar").css("padding-left").slice(0,-2))*2;
        $("#body-content").css("left", p+"px");
        document.cookie = "tabw="+parseInt($("#sidebar").css("width").slice(0,-2))+";path="+cookiepath;
	if (typeof resizeCallback == "function")
	    resizeCallback();
    });

    if (typeof resizeCallback == "function") {
	var m = document.cookie.match("tabw=(\\d+)");
	if (m)
	    resizeCallback(m[1]);
    }
    /* jquery ui tooltips */
    $("#content_and_sidebar img, #tabs img").tooltip({
        track: true,
        delay: 0,
        showURL: false,
        showBody: " - ",
        fade: 250
    });
    
    /* swipe image hover */
    $("#swipelink").bind("mouseover", function() {
        src = $("#swipeimg").attr("src");
        src_hover = src.replace("normal", "hover");
        $("#swipeimg").attr("src", src_hover);});
    
    $("#swipelink").bind("mouseout", function() {
        src_hover = $("#swipeimg").attr("src");
        src_normal = src.replace("hover", "normal");
        $("#swipeimg").attr("src", src_normal);});
    
    if (document.cookie.length>0) {// sidebar session from cookies
        
        /* tab width */
        var w = document.cookie.match("tabw=[0-9]*");
        if (w) {
            w = parseInt(w[0].slice(5));
            var p = w + parseInt($("#sidebar").css("padding-left").slice(0,-2))*2 + parseInt($("#content_and_sidebar").css("padding-left").slice(0,-2))*2;
            $("#sidebar").css("width", w+"px");
            $("#body-content").css("left", p+"px");
        }
    }
}


var currentNodes = new Array(); //tree nodes corresponding to the open page (per tab)

/* inits treeview in tab <index> */
function treeInit(index) {
    var focus;
    var tree = new YAHOO.widget.TreeView("treeview-" + index);
    
    //init root node
    rootData = TREEVIEWS[index][0]
    
    var obj = {label: rootData[0], 
               href: relpath + "/" + rootData[1] + (index != DEFAULT_TAB_INDEX ? "?tab="+index : "")};
    if (rootData[2] != null) {
        obj.title = rootData[2];
    } else {
        obj.title = rootData[0];
    }
    var rootNode = new YAHOO.widget.TextNode(obj, tree.getRoot(), false);
    rootNode.expanded = rootData[3];
    
    openCurrentNode(index, tree, useTreeCookie);
    
    if (useTreeCookie) {
        treeCookieRead(index, tree);
    }
    tree.setDynamicLoad(function(treeNode, fnLoadComplete) {
        loadNodeData(treeNode, index);
        fnLoadComplete();
    });
    tree.render();
    
    if (useTreeCookie) {
        tree.subscribe("expandComplete", function(node) {
            treeCookieWrite(index, node, false);
        });
        tree.subscribe("collapseComplete", function(node) {
            treeCookieWrite(index, node, true);
        });
        
        $("#treeview-"+index).scroll(function (event) {
            scrollWrite(index);
        });
        setTimeout(function(){scrollRead(index, tree, currentNodes[index]);}, 20);
    }
    tabs[index] = tree;
}


/* expand node corresponding to current page */
function openCurrentNode(index, tree, useTreeCookie) {
    //current page from js array embedded in html
    var branch = treecodes[index];
    var node = tree.getRoot();
    var n = 0;
    for (var j in branch) {
        node = node.children[parseInt(branch[j])];
        if (!node.dynamicLoadComplete && n < branch.length-1) {
            loadNodeData(node, index);
            node.expanded = true;
            node.dynamicLoadComplete = true;
            if (useTreeCookie)
                treeCookieWrite(index, node, false);
        }
        if (n == branch.length-1) {
            node.highlight();
        }
        n++;        
    }
    currentNodes[index] = node;

}

/* tree cookie handling functions */

/* reads tree cookie and expands those tree nodes that are saved in it */
function treeCookieRead(index, tree) {
    var cookiename = "tree" + index;
    var cookie = document.cookie.match(cookiename+"=[^;]*");
    var branches = []
    if (cookie) {
        cookie = cookie[0].slice(cookiename.length+1); //contents
        if (cookie) {
            branches = cookie.split("/").slice(0,-1);
        }
    }
    
    /* expand all branches saved in the cookie */
    for (var i=0; i<branches.length; i++) {
        var branch = branches[i].split("-");
        var node = tree.getRoot();
        for (var j in branch) {
            node = node.children[parseInt(branch[j])];
            if (!node)
                break
            if (!node.dynamicLoadComplete) {
                loadNodeData(node, index);
                node.expanded = true;
                node.dynamicLoadComplete = true;
            }
        }
    }
}

/* Updates stored tree cookie (that stores the state of open/closed branches 
   in the tree)
     <collapse> = true -> remove from cookie, otherwise add to it */
function treeCookieWrite(index, node, collapse) {
    var cookiename = "tree" + index;

    /* index to this node in the tree */
    var tree_index = [];
    var indexNode = node;
    var branch="";
    while (indexNode.parent) {
        var sibling_i = 0;
        while (indexNode.previousSibling) {
            indexNode = indexNode.previousSibling;
            sibling_i++;
        }
        branch = sibling_i + "-" + branch;
        indexNode = indexNode.parent;
    }
    if (branch[branch.length-1] == "-")
        branch = branch.slice(0,-1)
    
    if (!branch)
        return;
    
    if (node.children.length == 0) {//leaf node -> no saving
        return;
    }
    
    if (collapse) {
        var cookie = document.cookie.match(cookiename+"=[^;]*");
        if (cookie) {
            cookie = cookie[0].slice(cookiename.length+1);
            finding1 = cookie.match("^"+branch+"/");
            finding2 = cookie.match("/"+branch+"/");
            if (finding1) {
                cookie = cookie.replace(finding1[0], "");
            } else if (finding2) {
                cookie = cookie.replace(finding2[0].slice(1), "");
            } 
            
            document.cookie = cookiename+"="+cookie + ";path="+cookiepath;
        } 
        node.collapseAll();
    } else {
        
        var cookie = document.cookie.match(cookiename+"=[^;]*");
        var insertPos = 0;
        if (cookie) {
            cookie = cookie[0].slice(cookiename.length+1);
            if (cookie == "")
                var branches = "";
            else
                var branches = cookie.split("/");
            for (var i=0; i < branches.length-1; i++) {
                if (branches[i] == branch) {
                    return; //already added -> return
                } else {
                    if (branches[i].length <= branch.length)
                        insertPos += branches[i].length+1;
                    else //insertion position found (length ordering)
                        break;
                }
            }
        }
            
        if (!cookie || (cookie.length + branch.length<4040)) {
            if (insertPos >= 2) {
                var part1 = cookie.slice(0, insertPos);
                var part2 = cookie.slice(insertPos);
                cookie = part1 + branch + "/" + part2;
            } else
                cookie = branch + "/";
            document.cookie = cookiename + "=" + cookie + ";path="+cookiepath;
        }
    }
    scrollWrite(index);
}


/* gets stored sidebar scrollbar position from cookie and sets as current pos */
function scrollRead(index, tree, pageNode) {
    if (tree.locked) {//wait for dynamic tree load
        setTimeout(function(){scrollRead(index, tree, pageNode);}, 20);
        return;
    }
    
    /* read scroll cookie */
    var scrollcookiename = "scroll" + index;
    var scroll = document.cookie.match(scrollcookiename+"=[^;]+");
    var scrollPos = 0;
    if (scroll) {
        scroll = scroll[0].slice(scrollcookiename.length + 1).split("/");
        var scrollMaxCurr = $("#treeview-"+index).attr("scrollHeight");
        if (scrollMaxCurr == parseInt(scroll[1]))
            scrollPos = parseInt(scroll[0]);
    }
    
    /* find the current element into view if not there */
    if (pageNode) {
        var nodePos = pageNode.getEl().offsetTop;
        var nodeH = pageNode.getEl().offsetHeight;
        var scrollVisH = parseInt($("#sidebar").attr("offsetHeight"));
        var scrollH = parseInt($("#treeview-"+index).attr("scrollHeight"));
        
        if (nodePos > scrollPos + scrollVisH || nodePos + nodeH < scrollPos) {
            scrollPos = nodePos - Math.floor(scrollVisH/2);
            if (scrollPos + scrollVisH > scrollH)
                scrollPos = scrollH - scrollVisH;
            if (scrollPos < 0)
                scrollPos = 0;
            $("#treeview-" + index).attr("scrollTop", scrollPos);
        }
    }
    
    /* finally actually set the scroll if needed */
    if (scrollPos > 0)
        $("#treeview-" + index).attr("scrollTop", scrollPos);
    $("#treeview-" + index).attr("scrollTop", scrollPos);
}

/* writes sidebar scrollbar position */
function scrollWrite(index) {
    var scroll = $("#treeview-"+index).attr("scrollTop");
    var scrollMax = $("#treeview-"+index).attr("scrollHeight");
    var cookiename = "scroll" + index;
    document.cookie = cookiename+"="+scroll+"/"+scrollMax+";path="+cookiepath;
}

/* returns tab in url if there is one, default tab otherwise */
function getTabIndex() {
    var q = /tab=(\d+)/.exec(location.href);
    return q && q[1] >= 0 && q[1] <= 3 ? parseInt(q[1]) : DEFAULT_TAB_INDEX;
}

/* modifies links on a page so that they contain page parameters
   (currently: the current tab and possibly a search keyword) */
function setLinks(params) {
    var parameters;
    for (p in params) {
        if (!parameters) {
            parameters = ["?", p, "=", params[p]].join("");
        } else {
            parameters = [parameters, "&", p, "=", params[p]].join("");
        }
    }
    var re = new RegExp("^[a-zA-Z]*:");
    $("#body-content a").each(function() {
            var href = this.getAttribute("href");
            if (href && !re.test(href)) {
                var pos = href.indexOf("#");
                if (pos > -1) {
                    var hash = href.substring(pos);
                    href = href.substring(0, pos);
                    href = href + parameters + hash;
                } else {
                    href = href + parameters;
                }
                this.setAttribute("href", href);
            }
    });
}

function makeNodeUrl(href, query) {
    var absolutePattern = /:/; //found in absolute URLs
    var parts = href.split("#", 2);
    if (parts.length == 2) {
        if (parts[0].search(absolutePattern)>=0) {
            return [parts[0], query, "#", parts[1]].join("");
        } else {
            return [relpath, "/", parts[0], query, "#", parts[1]].join("");
        }
    } else {
        if (href.search(absolutePattern)>=0) {
            return [href, query].join("");
        } else {
            return [relpath, "/", href, query].join("");
        }
    }
}

/* dynamically loads <treeNode> to tree in tab <index> */
function loadNodeData(treeNode, index) {
    /* index to this node in the tree */
    var tree_index = [];
    var indexNode = treeNode;
    
    while (indexNode.parent) {
        var sibling_i = 0;
        while (indexNode.previousSibling) {
            indexNode = indexNode.previousSibling
            sibling_i++;
        }
        tree_index.push(sibling_i);
        indexNode = indexNode.parent;
    }
    
    /* get node data from TREEVIEWS (treedata.js) */
    var data = TREEVIEWS[index][tree_index.pop()]
    while (tree_index.length > 0) {
        data = data[4][tree_index.pop()];
    }
    
    if (!data) {
        return false;
    }

    childData = data[4];
    if (!childData) {
        return false;
    }
    var query = index != DEFAULT_TAB_INDEX ? "?tab=" + index : "";
    for (var i in childData) {
        var node = childData[i];
        var obj = { label: node[0], href: makeNodeUrl(node[1], query) };
        if (node[2] != null) {
            obj.title = node[2];
        } else {
            obj.title = node[0];
        }
        var tmpNode = new YAHOO.widget.TextNode(obj, treeNode, false);
        tmpNode.focus = function(){return true;}
        if (node[4] == null || node[4].length == 0) {
            tmpNode.isLeaf = true;
        }
        tmpNode.expanded = node[3];
    }
    return true;
}

function setupMobileVersion() {
    $("#content_and_sidebar").css("position", "relative");
    $("#sidebar").css("overflow", "auto");
    $("#sidebar").css("width", "290px");

    $("#body-content").css("position", "relative");
    $("#body-content").css("left", "320px");

    resizeMobileVersion();
    $(window).resize(resizeMobileVersion);
}

function resizeMobileVersion() {
    var w = window.innerWidth - 380;
    $("#body-content").width(w);
}
