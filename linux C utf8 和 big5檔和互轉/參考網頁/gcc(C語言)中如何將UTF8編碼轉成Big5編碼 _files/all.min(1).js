var headlines=headlines||{};var domain_append="";var server_name=document.location.host;var nodes=server_name.split(".");if(nodes[nodes.length-1]=="pixnet"){domain_append="."+nodes[nodes.length-3]+"."+nodes[nodes.length-2]+"."+nodes[nodes.length-1]}headlines.main_site="www.7headlines.com"+domain_append;!function($){headlines.initAutoLoad=function(more_btn,scroll_target){var scroll_event;if(!more_btn.length){return}more_btn.click(function(e){e.preventDefault();var more_btn=$(this);if(more_btn.is(".clicked")){return}more_btn.addClass("clicked");$.get(more_btn.attr("href"),function(data){more_btn.removeClass("clicked");if(data.error){return}var target_dom;if(more_btn.attr("data-target")){if("item-stream-hot"==more_btn.attr("data-target")){target_dom=$("#item-stream-hot");target_dom.isotope("insert",$(data.boxes.join(" ")))}else{target_dom=$(more_btn.attr("data-target"));for(var i=0;i<data.boxes.length;i++){target_dom.append($(data.boxes[i]))}}}else{target_dom=$("#item-stream");target_dom.isotope("insert",$(data.boxes.join(" ")))}if("undefined"!==typeof data.next_link){more_btn.attr("href",data.next_link)}else{more_btn.remove();scroll_target.unbind("scroll",scroll_event)}$(window).trigger("footprintlist-init")},"json")});scroll_event=function(){if(!more_btn.length){return}var scroll_top=scroll_target.scrollTop();var total_height="undefined"!==typeof scroll_target[0].scrollHeight?scroll_target[0].scrollHeight:$(document).height();var container_height=scroll_target.height();if(total_height-scroll_top<2*container_height){more_btn.click();return false}if("index"==more_btn.data("action")){more_btn.click();more_btn.removeAttr("data-action");return false}};var isScrolling=false;var scrolling_checker=function(){if(!isScrolling){setTimeout(scrolling_checker,100);return}isScrolling=false;scroll_event();setTimeout(scrolling_checker,100)};scrolling_checker();scroll_target.bind("scroll",function(){isScrolling=true});scroll_event()};headlines.initKeywordPopup=function(){$("#tag-filter-ul").delegate("li.tag .delete","click",function(e){e.preventDefault();var li_dom=$(this).parent("li.tag");var keyword=li_dom.attr("data-keyword");$.post("/keyword/like?keyword="+encodeURIComponent(keyword)+"&like=0",{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);return}li_dom.remove()},"json")}).delegate("li.tag .like","click",function(e){e.preventDefault()});$("#keyword_like_form").submit(function(e){e.preventDefault();var form=$(this);form.prop("disabled",true);var keyword=form.find('input[name="keyword"]').val();$.post(form.attr("action")+"?keyword="+encodeURIComponent(keyword)+"&like=1",{sToken:headlines.sToken},function(ret){form.prop("disabled",false);if(ret.error){alert(ret.message);return}form.find('input[name="keyword"]').val("");if(ret.created_at){$("#tag-filter-ul").append($("#tag-filter-li-tmpl").html().replace(/\$\{keyword\}/g,$("<span>").text(ret.keyword).html()).replace(/\$\{link\}/g,$("<span>").text(ret.link).html()))}},"json")})};headlines.getInternetExplorerVersion=function(){var rv=-1;if("Microsoft Internet Explorer"==navigator.appName){var ua=navigator.userAgent;var re=new RegExp("MSIE ([0-9]{1,}[.0-9]{0,})");if(re.exec(ua)!=null){rv=parseFloat(RegExp.$1)}}return rv};headlines.checkVersion=function(){var ver=headlines.getInternetExplorerVersion();if(ver>-1){if(ver<8){var userAgent=window.navigator.userAgent.toLowerCase();var error_page="http://"+headlines.main_site+"/error/browsererror?useragent="+encodeURIComponent(userAgent);if(error_page!=document.location.href){document.location.href=error_page}}}};headlines.checkVersion();headlines.popupLoader=function(url,done_cb){headlines.popupLoaderInterrupt=false;$("#popup-wrapper .popup").load(url+"?nowpage="+encodeURIComponent(location.pathname+location.search),null,function(response){if(headlines.popupLoaderInterrupt){return}if(!response){return}headlines.showbox();if("undefined"!==typeof done_cb){done_cb()}});return false};headlines.showbox=function(){$("#dimmer").show();$("#popup-wrapper").show();$("html").css("overflow","hidden");$(".list-box ul").bind("touchstart",function(e){var startTopScroll=this.scrollTop;if(startTopScroll<=0){this.scrollTop=1}if(startTopScroll+this.offsetHeight>=this.scrollHeight){this.scrollTop=this.scrollHeight-this.offsetHeight-1}});$(".close-popup, .modal-close").click(headlines.hidebox)};headlines.hidebox=function(){$("#dimmer").hide();$("#popup-wrapper").hide();$("html").css("overflow","auto");$(".list-box ul").unbind("touchstart")};headlines.showsetsnsbox=function(){var url="/user/setsns";headlines.popupLoader(url)};headlines.subscribe_collection_done=function(data,btn){if("set"==data.action){btn.addClass("btn-done").attr("href",btn.attr("data-removelink")).find("span").text(btn.attr("data-wording-subscribed"))}else{btn.removeClass("btn-done").removeClass("btn-cancel").attr("href",btn.attr("data-addlink")).find("span").text(btn.attr("data-wording-subscribe"))}};headlines.subscribe_streambox_collection_done=function(data,btn){btn.hide()};headlines.follow_user_done=function(data,btn){if("add"==data.action){btn.addClass("btn-done").attr("href",btn.attr("data-removelink")).find("span").text(btn.attr("data-wording-followed"))}else{btn.removeClass("btn-done").removeClass("btn-cancel").attr("href",btn.attr("data-addlink")).find("span").text(btn.attr("data-wording-follow"))}};headlines.follow_user_all_done=function(data,btn){var id=btn.data("id");if("add"==data.action){$(".id-"+id).addClass("btn-done").attr("href",btn.data("removelink")).find("span").text(btn.data("wordingFollowed"))}else{$(".id-"+id).removeClass("btn-done").removeClass("btn-cancel").attr("href",btn.data("addlink")).find("span").text(btn.data("wordingFollow"))}};headlines.delete_collection_done=function(data,btn){var id=btn.attr("id").split("_")[2];var item=$("#collection_"+id);item.remove();headlines.hidebox()};headlines.article_like_done=function(){$(".like-box").hide()};headlines.getNotificationCount=function(){$.get("/notification/count",function(ret){$(".notifications").toggleClass("read",!ret);$(".notifications span").text(ret);setTimeout(headlines.getNotificationCount,1e4)},"json")};headlines.shareToMailCount=function(url){$.post(url,{sToken:headlines.sToken})};headlines.showPlaceholder=function(){placeholderTest=document.createElement("input");if(!("placeholder"in placeholderTest)){$("input[placeholder],textarea[placeholder]").each(function(){var input=$(this);if(!$(input).val())$(input).val(input.attr("placeholder")).css({color:"#AAA"});$(input).focus(function(){if(input.val()==input.attr("placeholder")){input.val("").css({color:"#000"})}});$(input).blur(function(){if(input.val()==""||input.val()==input.attr("placeholder")){input.val(input.attr("placeholder")).css({color:"#AAA"})}})})}};$(function(){headlines.hidebox();$(".dimmer").click(function(){headlines.hidebox()});if($(".notifications span").length){headlines.getNotificationCount()}$(window).bind("reload-footprintlist",function(e,article_id){var footprintlist=$(".footprint-"+article_id);if(!footprintlist.length){return false}var liststyle=footprintlist.attr("data-attr").split("_")[0];var share_cnt=footprintlist.attr("data-attr").split("_")[1];if(window.opener&&!window.opener.closed&&"undefined"!==typeof window.opener.location.href){opener.$(window.opener).trigger("reload-footprintlist",article_id)}$.get("/article/getfootprintlist",{article_id:article_id,style:liststyle,share_cnt:share_cnt},function(data){if(data.error){alert(data.message);return false}footprintlist.parents(".item-shares").removeClass("no-footprints");var new_list=data.list;footprintlist.replaceWith(new_list);$(".item[id="+article_id+"]").find(".contribution-detail").html(data.action_list);var share_stats=$("#share-stats-"+article_id);if(share_stats.length){var footprint_count_text=data.footprint_count_text;share_stats.find("span").text(footprint_count_text)}return false})});$(window).bind("reload-message",function(e,article_id){$.get("/article/getfootprintlist",{article_id:article_id},function(data){if(data.error){alert(data.message);return false}$(".contribution-detail").html(data.action_list);return false})});$(window).bind("footprintlist-init",function(e){if($("ul[class^=share-people] > li.subscribed").length>0){$("ul[class^=share-people] > li:first-child.subscribed").click()}return false});if($("#article_counter_tag").length){headlines.article_counter_tag=$("#article_counter_tag").val()}$("body").delegate(".login-button","click",function(e){e.preventDefault();window.open($(this).attr("data-login-link"),"login","height=500,width=650,scrollbars=1,resizable=0").focus()});$("body").delegate(".btn.btn-follow.btn-done","mouseover",function(){$(this).removeClass("btn-done");$(this).addClass("btn-cancel");$("> span",this).text($(this).attr("data-wording-cancel_follow"))});$("body").delegate(".btn.btn-follow.btn-cancel","mouseout",function(){$(this).removeClass("btn-cancel");$(this).addClass("btn-done");$("> span",this).text($(this).attr("data-wording-followed"))});$("body").delegate(".like-keyword-button","click",function(e){e.preventDefault();var btn=$(this);var keyword=btn.attr("data-keyword");$.post("/keyword/like?keyword="+encodeURIComponent(keyword)+"&like="+(btn.is(".btn-cancel")||btn.is(".btn-done")?0:1),{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);return}if(ret.created_at){btn.addClass("btn-done").find("span").text(btn.attr("data-wording-liked"))}else{btn.removeClass("btn-cancel").removeClass("btn-done").find("span").text(btn.attr("data-wording-like"))}},"json")}).delegate(".like-keyword-button","mouseover",function(e){var btn=$(this);if(btn.is(".btn-done")){btn.removeClass("btn-done").addClass("btn-cancel").find("span").text(btn.attr("data-wording-unlike"))}}).delegate(".like-keyword-button","mouseout",function(e){var btn=$(this);if(btn.is(".btn-cancel")){btn.addClass("btn-done").removeClass("btn-cancel").find("span").text(btn.attr("data-wording-liked"))}});$("body").delegate(".func-post-link","click",function(e){e.preventDefault();var btn=$(this);var post_link=false!==btn.attr("href")&&"undefined"!==typeof btn.attr("href")?btn.attr("href"):btn.attr("data-link");if(btn.attr("data-confirm")){if(!confirm(btn.attr("data-confirm"))){return}}$.post(post_link,{sToken:headlines.sToken},function(data){if(data.error){if("undefined"!==typeof data.message){alert(data.message)}if(data.need_login){document.location="/signup/?next="+encodeURIComponent(location.pathname+location.search)}return}if(false!==btn.attr("data-callback-url")&&"undefined"!==typeof btn.attr("data-callback-url")){document.location=btn.attr("data-callback-url")}else if(false!==btn.attr("data-callback")&&"undefined"!==typeof btn.attr("data-callback")){var func=eval(btn.attr("data-callback"));func(data,btn)}else{location.reload()}},"json")});$("body").delegate(".btn.btn-subscribe.btn-done","mouseover",function(){$(this).removeClass("btn-done");$(this).addClass("btn-cancel");$("> span",this).text($(this).attr("data-wording-cancel_subscribe"))});$("body").delegate(".btn.btn-subscribe.btn-cancel","mouseout",function(){$(this).removeClass("btn-cancel");$(this).addClass("btn-done");$("> span",this).text($(this).attr("data-wording-subscribed"))});$("#set_sns_btn").click(function(e){e.preventDefault();headlines.showsetsnsbox()});$("body").delegate(".popup-loader","click",function(e){e.preventDefault();var url;if("undefined"!==typeof $(this).attr("href")&&false!==$(this).attr("href")){url=$(this).attr("href")}else{url=$(this).attr("data-href")}headlines.popupLoader(url)});if($(".popover").length){$("body").click(function(e){if($(e.target).is(".button-share")||$(e.target).parents().is(".button-share")){return}$(".popover").hide()})}$("#title .select-box").hide();$("body").click(function(e){$("#title .select-box").hide();$(".notification-box").hide();$(".navbar-user-dropdown").hide()});$("#channel_select_btn").click(function(e){$("#title .select-box").toggle();e.cancelBubble=true;e.stopPropagation()});$("body").delegate("a[name=channel_subscribe_btn]","click",function(e){e.cancelBubble=true;e.stopPropagation();var channel_id=$(this).attr("id").split("_")[2];var btn=$(this);$.post("/channel/subscribechannel",{sToken:headlines.sToken,channel_id:channel_id},function(data){if(data.error){alert(data.message);return false}btn.addClass("btn-done");$("> span",btn).text(btn.attr("data-wording-subscribed"));btn.attr("name","channel_subscribe_done_btn");return false},"json")});$("body").delegate("a[name=channel_subscribe_done_btn]","click",function(){var channel_id=$(this).attr("id").split("_")[2];var btn=$(this);$.post("/channel/deletechannel",{sToken:headlines.sToken,channel_id:channel_id},function(data){if(data.error){alert(data.message);return false}btn.removeClass("btn-done");btn.removeClass("btn-cancel");$("> span",btn).text(btn.attr("data-wording-subscribe"));btn.attr("name","channel_subscribe_btn");return false},"json")});$(".like-button").click(function(e){var button=$(this);var article_id=parseInt(document.location.pathname.match(/\/article\/show\/(\d+)/)[1],10);$.post("/article/like/"+article_id+"/"+(button.hasClass("like-yes")?"yes":"no"),{sToken:headlines.sToken},function(ret){$(".like-box").hide()})});$(".article-author-like").click(function(e){e.preventDefault();var a_dom=$(this);var author=a_dom.attr("data-author");var is_liked=0;$.post("/author/like/"+author+"?like="+(is_liked?0:1),{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);return}a_dom.remove()},"json")});$(".article-keyword-like").click(function(e){e.preventDefault();var a_dom=$(this);var keyword=a_dom.attr("data-keyword");var is_liked=a_dom.is(".liked");$.post("/keyword/like?keyword="+encodeURIComponent(keyword)+"&like="+(is_liked?0:1),{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);return}if(ret.created_at){a_dom.addClass("liked")}else{a_dom.removeClass("liked")}},"json")});$(".share-to-sns, .share-to-mail").click(function(e){if($(this).hasClass("share-to-mail")){headlines.shareToMailCount($(this).data("incSharedUrl"))}window.open($(this).attr("href"),"share","height=500,width=650,scrollbars=1,resizable=0").focus();e.preventDefault()});$(".share-cancel").click(function(e){window.close();e.preventDefault()});$("#share_form :button.send").click(function(e){var share_type=$("input[name=share_type]").val();var url="";switch(share_type){case"article":url="/article/sharearticle/";break;case"collection":url="/collection/sharecollection/";break;case"story":url="/story/sharestory/";break}$.post(url,jQuery("#share_form").serialize(),function(data){if(data.error){alert(data.message);return false}if("article"==share_type){var article_id=$("#share_form input[name=article_id]").val();if(window.opener.opener&&!window.opener.opener.closed){opener.$(window.opener).trigger("reload-footprintlist",article_id)}}window.close();return false});e.preventDefault()});$("#share_set_sns_btn").click(function(e){window.close();window.opener.headlines.showsetsnsbox();e.preventDefault()});$(".oauth-cancel").click(function(e){var confirm_message=$(this).attr("data-confirm-message");if(confirm(confirm_message)){var service=$(this).attr("data-service");$.post("/signup/deleteassociate?type="+encodeURIComponent(service),{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);return false}window.close()},"json");return false}return false});if($("#channel-featured-ul").length){$("#channel-featured-ul li a.func-left-link").mouseover(function(e){e.preventDefault();$("#channel-featured-ul li").removeClass("active");$(this).parents("li").addClass("active");$("#channel-featured-ul div.item-stretch").hide();$(this).parents("li").find(".item-stretch").show()});$("#channel-featured-ul li a.func-left-link:first").mouseover()}jQuery("body").delegate("#collect_submit","click",function(e){var article_id=$("#collect_article").val();var footprintlist=$(".footprint-"+article_id);var has_opener=false;if(window.opener&&!window.opener.closed){has_opener=true}if(jQuery("textarea[name=note]").val()==jQuery("textarea[name=note]").attr("placeholder"))jQuery("textarea[name=note]").val("");var post_data=jQuery("#collect_form").serializeArray();$.post("/collection/additem",post_data,function(data){if(data.error){alert(data.message);return false}if("true"!=$("#bin").attr("data-is-move")&&data.show_iphone5_alert_msg){alert('恭喜您取得"喀嚓iPhone5夠讚"的抽獎資格囉！')}if(data.show_clip_tutorial){headlines.tutorial.startClipTutorial()}headlines.hidebox();$(window).trigger("reload-footprintlist",article_id);headlines.popupLoader("/article/cliprecommend/"+article_id);return false},"json");return false});jQuery("body").delegate("#collect_delete_submit","click",function(){$.post("/collection/deleteitem",jQuery("#collect_edit_form").serialize(),function(data){if(data.error){alert(data.message);return false}location.reload();return false},"json");return false});jQuery("body").delegate("#collect_edit_submit","click",function(){$.post("/collection/edititem",jQuery("#collect_form").serialize(),function(data){if(data.error){alert(data.message);return false}location.reload();return false},"json");return false});jQuery("body").delegate("a.select-box","click",function(e){jQuery(".list-box").show()});jQuery("body").delegate(".list-box li[name=collec_list]","click",function(e){var collection_id=jQuery(this).attr("id").split("_")[1];var collection_name=jQuery("> a",this).html();jQuery("#collect_form .select-box").html(collection_name);jQuery("input[name=set_collection_select]").val(collection_id);jQuery(".list-box").hide();return false});jQuery("body").delegate("a.category-box","click",function(e){jQuery(".category-box-sub").show()});jQuery("body").delegate(".category-box-sub li","click",function(e){var channel_id=jQuery(this).attr("id").split("_")[1];var channel_name=jQuery("> a",this).text();jQuery("a.category-box").text(channel_name);jQuery("input[name=collection_category]").val(channel_id);jQuery(".category-box-sub").hide();return false});jQuery("body").delegate("#add_collection","click",function(){if(jQuery("input[name=collection_name]").val()==jQuery("input[name=collection_name]").attr("placeholder"))jQuery("input[name=collection_name]").val("");if(jQuery("textarea[name=collection_description]").val()==jQuery("textarea[name=collection_description]").attr("placeholder"))jQuery("textarea[name=collection_description]").val("");var collection_name=jQuery("input[name=collection_name]").val();var channel_id=jQuery("input[name=collection_category]").val();var channel_name=jQuery(".category-box").text();var select_box_content="<span class='sub-cat'>"+channel_name+"</span>"+jQuery("<span></span>").text(collection_name).html();$.post("/collection/addcollection",jQuery("#collect_form").serialize(),function(data){if(data.error){alert(data.message);return false}if(data.collection_id){var collection_id=data.collection_id;jQuery("input[name=set_collection_select]").val(collection_id);jQuery("#collect_form .select-box").html(select_box_content);jQuery(".list-box").hide();jQuery(".category-box-sub").hide()}return false},"json");return false});jQuery("body").delegate(".item .share-stats, .item .add-button, .first-action .contribution","click",function(){$(this).parents(".item").find(".expanded").fadeOut();$(this).parents(".item").find(".folded").fadeIn();return false});jQuery("body").delegate(".item .close","click",function(){$(this).parents(".item").find(".folded").fadeOut();$(this).parents(".item").find(".expanded").fadeIn();return false});$("body").delegate(".invite","click",function(e){e.preventDefault();window.open($(this).attr("data-invite-link"),"invite","height=500,width=650,scrollbars=1,resizable=0").focus()});headlines.initAutoLoad($("#more-btn"),$(window));headlines.initAutoLoad($("#hot-more-btn"),$(window));headlines.initAutoLoad($("#ticker-more-btn"),$(window));$("body").delegate(".give-contribution .button-white","click",function(e){e.preventDefault();var $this=$(this),$b=$this.find("b.emoticon-count");var emoticon_id=$this.data("emoticon-id");var article_id=$this.closest(".item").attr("id");if($this.data("article-id")){article_id=$this.data("article-id")}var checked=$this.hasClass("checked");var count=parseInt($b.text());if(!checked){var url="/article/addemoticon/"+article_id+"/"+emoticon_id;$this.addClass("checked");$b.text(count+1)}else{var url="/article/deleteemoticon/"+article_id+"/"+emoticon_id;$this.removeClass("checked");$b.text(count-1)}$.post(url,{sToken:headlines.sToken},function(ret){if(ret.error){alert(ret.message);if(ret.need_login){document.location.href="/signup/?next="+encodeURIComponent(location.pathname+location.search)}return}$(window).trigger("reload-footprintlist",article_id);if($this.data("article-id")){$(window).trigger("reload-message",article_id)}$b.text(ret.count)})});$("body").delegate(".submit_message","click",function(e){var $this=$(this);var $input_message=$this.siblings("input[name=message]");var message=$input_message.val();if(0==message.trim().length){return}var article_id=$this.closest(".item").attr("id");var in_article=false;if($this.siblings("input[name=articleid]").val()){in_article=true;article_id=$this.siblings("input[name=articleid]").val()}var url="/article/addmessage/"+article_id;var data={sToken:headlines.sToken,message:message};var placeholder_original=$input_message.attr("placeholder");$.ajax({type:"post",url:url,data:data,dataType:"json",beforeSend:function(){$input_message.val("").attr("placeholder","留言送出中...")},success:function(ret){$input_message.attr("placeholder",placeholder_original);if(ret.error){alert(ret.message);if(ret.need_login){document.location.href="/signup/?next="+encodeURIComponent(location.pathname+location.search)}return}$(window).trigger("reload-footprintlist",article_id);if(in_article){$(window).trigger("reload-message",article_id)}}})});$("body").delegate("form[name=post_message]","submit",function(){$(this).find(".submit_message").click();return false});$("body").delegate("ul[class^=share-people] > li","click",function(e){var $this=$(this);var id=$this.data("footprint-id");if(!id){return}if($this.hasClass("active")){return}$.get("/article/getfootprintcontent/"+id,function(data){if(data.error){return false}var content=data.content;var $target_div=$this.closest(".item").find(".item-reference-content");$this.closest(".item").find(".item-edit").hide();$this.closest(".item").find(".item-reference").show();$target_div.html(content).parent().removeClass("item-reference-category");$target_div.parent().show();if(!data.subscribed){$target_div.siblings("a").show().attr("href","/collection/subscribeset?collection_id="+$this.data("collection-id"))}else{$target_div.siblings("a").hide().data("href","")}$this.siblings().removeClass("active");$this.addClass("active");return false});e.preventDefault()});$("body").delegate(".thumb-change-btn","click",function(e){var $this=$(this);var target_link=$this.data("link");$.get(target_link,function(data){var thumb_items=$("#tmpl-thumb-select-item").render(data.image_urls);$(".thumb-select").html(thumb_items);$(".thumb-select").show()});e.preventDefault()});$("body").delegate(".thumb-select > a","click",function(e){var thumb_src=$(this).data("src");$(".thumb-select").hide();if(thumb_src==$("input[name=collection_thumb]").val()){return false}$("input[name=collection_thumb]").val(thumb_src);$("img.article-thumb").fadeOut(500,function(){$("img.article-thumb").attr("src","http://imageproxy.pimg.tw/zoomcrop?width=150&height=150&url="+thumb_src);$("img.article-thumb").fadeIn(500)});e.preventDefault()});$("#search-category-id").change(function(){$('#search-form input[name="category_id"]').val($(this).val());$("#search-form").submit()});$("body").delegate("#curation-popup-submit","click",function(e){e.preventDefault();$.post("/curation/adddraft",jQuery("#curation_publish_popup_form").serialize(),function(data){if(data.error){alert(data.message);return false}$.cookie("goback_url",location.pathname+location.search,{path:"/"});var draft_id=data.draft_id;document.location="/curation/"+draft_id+"/";return false},"json");return false});$("body").delegate("#curation_collection_filter","change",function(e){var $this=$(this);$.get("/curation/getfeaturedcollection/"+$this.val(),function(data){var collection_list=$("#tmpl-magazines-select-list-li").render(data.collections);$("#curation_magazines_select_list").html(collection_list);return false});return false});$("body").delegate("#curation_publish_popup_form","keypress",function(e){var code=e.keyCode||e.which;if(code==13){e.preventDefault();var val=$("#curation_search_collection_val").val();$.get("/curation/searccollection?q="+val,function(data){var collection_list=$("#tmpl-magazines-select-list-li").render(data.collections);$("#curation_magazines_select_list").html(collection_list)});return false}});$("body").delegate(".submit-no","click",function(e){var id=$(this).data("object-id");var item_li=$(this).parent().parent();$.post("/curation/denysubmit/"+id,{sToken:headlines.sToken},function(data){if(data.error){alert(data.message);return false}item_li.hide("slow",function(){item_li.remove()});var sub_num=$(".submissions-num").text();$(".submissions-num").text(sub_num-1)});return false});$("body").delegate(".submit-yes","click",function(e){var id=$(this).data("object-id");var item_li=$(this).parent().parent();$.post("/curation/acceptsubmit/"+id,{sToken:headlines.sToken},function(data){if(data.error){alert(data.message);return false}item_li.hide("slow",function(){item_li.remove()});var sub_num=$(".submissions-num").text();$(".submissions-num").text(sub_num-1)});return false});$("body").delegate(".notifications","click",function(e){e.cancelBubble=true;e.stopPropagation();$(".notification-box").toggle();$(".navbar-user-dropdown").hide()});$("body").delegate(".user-avatar","click",function(e){e.cancelBubble=true;e.stopPropagation();$(".navbar-user-dropdown").toggle();$(".notification-box").hide()});$("body").delegate(".transform-admin","click",function(e){var $this=$(this);var admin_id=$this.data("admin-id");$.get("/user/transformadmin/"+admin_id,function(data){if(data.error){alert(data.message)}document.location="/";return false});return false});$("body").delegate(".curation-edit","click",function(e){var $this=$(this);var box=$this.parent().parent();box.find(".item-reference").hide();box.find(".item-edit").show();return false});$("body").delegate(".curation-delete","click",function(e){var $this=$(this);var url=$this.attr("href");var message=$this.data("confirm");if(!confirm(message)){return false}$.post(url,{sToken:headlines.sToken},function(data){if(data.error){alert(data.message)}location.reload();return false});return false})})}(jQuery);!function($){$.fn.autogrow=function(options){return this.filter("textarea").each(function(){var self=this;var $self=$(self);var minHeight=$self.height();var noFlickerPad=$self.hasClass("autogrow-short")?0:parseInt($self.css("lineHeight"));var shadow=$("<div></div>").css({position:"absolute",top:-1e4,left:-1e4,width:$self.width(),fontSize:$self.css("fontSize"),fontFamily:$self.css("fontFamily"),fontWeight:$self.css("fontWeight"),lineHeight:$self.css("lineHeight"),resize:"none"}).appendTo(document.body);var update=function(){var times=function(string,number){for(var i=0,r="";i<number;i++)r+=string;return r};var val=self.value.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/&/g,"&amp;").replace(/\n$/,"<br/>&nbsp;").replace(/\n/g,"<br/>").replace(/ {2,}/g,function(space){return times("&nbsp;",space.length-1)+" "});shadow.css("width",$self.width());shadow.html(val);$self.css("height",Math.max(shadow.height()+noFlickerPad,minHeight))};$self.change(update).keyup(update).keydown(update);$(window).resize(update);update()})}}(jQuery);