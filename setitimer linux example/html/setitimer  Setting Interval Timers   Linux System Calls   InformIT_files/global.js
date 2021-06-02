// the global.js include line MUST fall AFTER the jquery include for this logic to work
// the following logic ensures that flash objects appear below dropdown menus
$(document).ready(function () {
 $('embed').each(function () {
   //$(this).attr('wmode','transparent').attr('style',"position:relative;z-index:1;");
   //$(this).before('<param value="transparent" name="wmode" />');
 });

 $('object').each(function () {
	if($(this).attr('id'))
	{
	  //jquery clone won't work due to IE bug returning innerHTML so use straight DOM
	  var flash = document.getElementById($(this).attr('id'));
	  var param = document.createElement('param');
	  param.name = 'wmode';
	  param.value = 'transparent';
	  flash.appendChild(param);
	  // Commented the following two lines out so the pushdown ads would work
	  //var clone = flash.cloneNode(true);
	  //flash.parentNode.replaceChild(clone, flash);
	}
 }); 
	//jQueryUI dialog box
	$('.dialogBox').dialog({ 
		autoOpen: false,
		resizable: false,
		width: 300,
		height: 'auto'
		});				
	//jQueryUI dialog box opener
	$('.dialogOpener').click(function(event) {
		var dlg = $(this).attr('href');
		$(dlg).dialog('option','position',[(event.clientX-250),(event.clientY+15)]);
		$(dlg).css({position: 'relative', 'margin-left': '0px', clip: 'auto'});
		$(dlg).dialog('open');
		// We want a different width for smaller displays
		var cw = ((document.documentElement.clientWidth)-10);
		if (document.documentElement.clientWidth > 600) {
			$(dlg).dialog('option', 'width', 300);
		} else {			
			$(dlg).dialog('option', 'width', cw); 
		}
		// Remove annoying focus from first link in dialog
		$(dlg).find('a').first().blur();
		return false;
	});
    //jQueryUI dialog fancyZoom replacement
	// Create dialog box
	$('.dialogZoom').dialog({ 
		dialogClass: 'zoomy',
		autoOpen: false,
		resizable: false,
		draggable: false,
		width: 'auto',
		height: 'auto',
		show: 'fade',
		hide: 'fade',
		close: function(event, ui) {
			// Flush/reload content. Needed for IE issues with Flash
			var c = "#" + $(this).find('.flash-replaced').attr('id');  
			var r = $(c).html();
			$(c).html('');
			$(c).html(r);					
			}						
	});
	//Open dialog box					
	$('.zoomOpener').click(function(event) {
		var dlg = $(this).attr('href');
		//$(dlg).dialog('option','position',[event.clientX,event.clientY]);
		$(dlg).css({position: 'relative', 'margin-left': '0px', clip: 'auto'});
		$(dlg).dialog('open');
		// Set size after load, to accommodate IE6
		$(dlg).dialog('option', 'width', ($(dlg).children().width()+20)); 
		// Remove annoying focus from first link in dialog
		$(dlg).find('a').first().blur();
		return false;
	});
	
	if($('.layoutSearchColumn #columnTwo .results .advertisement').children().size() > 0) {
		$('.layoutSearchColumn #columnTwo .results .advertisement').css('margin-bottom', '1em');
	};
	
	// jquery.equalheights.js
	// Set all columns to the same height.
	$("#content").equalHeights();
	// Set all items with class 'sib' to equal height
	$(".sib").equalHeights();	
});

// hook for individual pages to execute logic in the body onLoad event
	function windowOnLoad()
	{
		if(window.pageOnLoad != null)
			pageOnLoad();
	}

// SonOfASuckerfish Dropdown - IE6 and below
// http://www.htmldog.com/articles/suckerfish/dropdowns
sfHover = function() {
	var sfEls = document.getElementById("nav").getElementsByTagName("li");
	for (var i=0; i<sfEls.length; i++) {
		sfEls[i].onmouseover=function() {
			this.className+=" sfhover";
		}
		sfEls[i].onmouseout=function() {
			this.className=this.className.replace(new RegExp(" sfhover\\b"), "");
		}
	}
}
if (window.attachEvent) window.attachEvent("onload", sfHover);

// Global Nav search box clear
	function checkClear(searchInput,defaultPhrase) {
	  if ( searchInput.value == defaultPhrase) searchInput.value = "";
	  	searchInput.className+=" focus";
	}

/* Open a Popup Window Based on MM_openBrWindow v2.0 */
/* ONLY for legacy links */
	function openBrWindow(theURL,winName,features) {
  	window.open(theURL,winName,features);
	}

/* Open Popup Window */
/* ONLY for use in article & sample chpater content */
function popUp(pPage) {
  window.open(pPage,'popWin','resizable=yes,scrollbars=yes,width=800,height=600,toolbar=no');
}

// Tab Widget
// Used for BSS, Product Widget, author info widget
function tabWidget(layer,myID) {
		var countDIV = 1;
		var countLI = 1
		if (document.getElementById) {
			navRoot = document.getElementById(myID);
			for (i=0; i<navRoot.childNodes.length; i++) {
				node = navRoot.childNodes[i];
				if (node.nodeName == "DIV" && (node.className == "container on" || node.className == "container")) {
					node.className=(countDIV == layer)?"container on":"container";
					countDIV++;
				}
				if (node.nodeName == "UL" && node.className == "tabs") {
					for (j=0; j<node.childNodes.length; j++) {
						nodelett = node.childNodes[j];
						if (nodelett.nodeName == "LI") {
							nodelett.className=(countLI == layer)?"selected":"";
							countLI++;
						}
					}
				}
			}
		}
	}

// Used on Blog Comments
// Strip HTML Tags (form) script - By JavaScriptKit.com (http://www.javascriptkit.com)
// For this and over 400+ free scripts, visit JavaScript Kit - http://www.javascriptkit.com/
// This notice must stay intact for use
function stripHTML(){
	var re= /<\S[^><]*>/g
	for (i=0; i<arguments.length; i++)
	arguments[i].value=arguments[i].value.replace(re, "")
}

// Style switcher
// Use to show/hide layers
function showme(id, newClass) {
	identity=document.getElementById(id);
	identity.className=newClass;
}

function execSearch(searchTerm)
{
	document.forms['homesearchform'].query.value = searchTerm;
	//fix for IE6, also works for Firefox, IE7
	setTimeout("document.forms['homesearchform'].submit()",10);
}

function jumpMenu(targ,selObj,restore){ //v3.0
  eval(targ+".location='"+selObj.options[selObj.selectedIndex].value+"'");
  if (restore) selObj.selectedIndex=0;
}

// Nav for narrow screens
$(".js #siteNav ul").click(function () {
	$("#siteNav ul").toggleClass("show");
	if ($("#siteNav ul").hasClass("show")) {
			$("#siteNav").siblings().one("click", function () {
			$("#siteNav ul").removeClass("show");
		})
	}	
});

// Move nav dropdown offscreen when content, header or footer are clicked. This is our hack for touch devices until a more comprehensive solution is implemented
$("#siteNav").siblings().click(function() {
	$('#nav li ul').css({'left': '-999em'});
});

