var DISQUS=function(e){var h=e.DISQUS||{};h.AssertionError=function(b){this.message=b};h.AssertionError.prototype.toString=function(){return"Assertion Error: "+(this.message||"[no message]")};h.assert=function(b,c,f){if(!b)if(f)e.console&&e.console.log("DISQUS assertion failed: "+c);else throw new h.AssertionError(c);};var c=[];h.define=function(b,n){typeof b==="function"&&(n=b,b="");for(var f=b.split("."),a=f.shift(),d=h,m=(n||function(){return{}}).call({overwrites:function(a){a.__overwrites__=!0;
return a}},e);a;)d=d[a]?d[a]:d[a]={},a=f.shift();for(var g in m)m.hasOwnProperty(g)&&(!m.__overwrites__&&d[g]!==null&&h.assert(!d.hasOwnProperty(g),"Unsafe attempt to redefine existing module: "+g,!0),d[g]=m[g],c.push(function(a,d){return function(){delete a[d]}}(d,g)));return d};h.use=function(b){return h.define(b)};h.cleanup=function(){for(var b=0;b<c.length;b++)c[b]()};return h}(window);
DISQUS.define(function(e,h){var c=e.DISQUS,b=e.document,n=b.getElementsByTagName("head")[0]||b.body,f={running:!1,timer:null,queue:[]};c.defer=function(a,d){function b(){var a=f.queue;if(a.length===0)f.running=!1,clearInterval(f.timer);for(var d=0,c;c=a[d];d++)c[0]()&&(a.splice(d--,1),c[1]())}f.queue.push([a,d]);b();if(!f.running)f.running=!0,f.timer=setInterval(b,100)};c.each=function(a,d){var b=a.length,c=Array.prototype.forEach;if(isNaN(b))for(var e in a)a.hasOwnProperty(e)&&d(a[e],e,a);else if(c)c.call(a,
d);else for(c=0;c<b;c++)d(a[c],c,a)};c.extend=function(a){c.each(Array.prototype.slice.call(arguments,1),function(d){for(var b in d)a[b]=d[b]});return a};c.serializeArgs=function(a){var d=[];c.each(a,function(a,b){a!==h&&d.push(b+(a!==null?"="+encodeURIComponent(a):""))});return d.join("&")};c.isString=function(a){return Object.prototype.toString.call(a)==="[object String]"};c.serialize=function(a,d,b){d&&(a+=~a.indexOf("?")?a.charAt(a.length-1)=="&"?"":"&":"?",a+=c.serializeArgs(d));if(b)return d=
{},d[(new Date).getTime()]=null,c.serialize(a,d);d=a.length;return a.charAt(d-1)=="&"?a.slice(0,d-1):a};c.require=function(a,d,e,g,f){function h(a){if(a.type=="load"||/^(complete|loaded)$/.test(a.target.readyState))g&&g(),t&&clearTimeout(t),c.bean.remove(a.target,l,h)}var i=b.createElement("script"),l=i.addEventListener?"load":"readystatechange",t=null;i.src=c.serialize(a,d,e);i.async=!0;i.charset="UTF-8";(g||f)&&c.bean.add(i,l,h);f&&(t=setTimeout(function(){f()},2E4));n.appendChild(i);return c};
c.requireStylesheet=function(a,d,e){var g=b.createElement("link");g.rel="stylesheet";g.type="text/css";g.href=c.serialize(a,d,e);n.appendChild(g);return c};c.requireSet=function(a,b,e){var g=a.length;c.each(a,function(a){c.require(a,{},b,function(){--g===0&&e()})})};c.injectCss=function(a){var d=b.createElement("style");d.setAttribute("type","text/css");a=a.replace(/\}/g,"}\n");e.location.href.match(/^https/)&&(a=a.replace(/http:\/\//g,"https://"));d.styleSheet?d.styleSheet.cssText=a:d.appendChild(b.createTextNode(a));
n.appendChild(d)}});
DISQUS.define("JSON",function(){function e(a){return a<10?"0"+a:a}function h(b){a.lastIndex=0;return a.test(b)?'"'+b.replace(a,function(a){var b=g[a];return typeof b==="string"?b:"\\u"+("0000"+a.charCodeAt(0).toString(16)).slice(-4)})+'"':'"'+b+'"'}function c(a,b){var l,e,g,f,i=d,k,o=b[a];o&&typeof o==="object"&&typeof o.toJSON==="function"&&!n&&(o=o.toJSON(a));typeof p==="function"&&(o=p.call(b,a,o));switch(typeof o){case "string":return h(o);case "number":return isFinite(o)?String(o):"null";case "boolean":case "null":return String(o);
case "object":if(!o)return"null";d+=m;k=[];if(Object.prototype.toString.apply(o)==="[object Array]"){f=o.length;for(l=0;l<f;l+=1)k[l]=c(l,o)||"null";g=k.length===0?"[]":d?"[\n"+d+k.join(",\n"+d)+"\n"+i+"]":"["+k.join(",")+"]";d=i;return g}if(p&&typeof p==="object"){f=p.length;for(l=0;l<f;l+=1)e=p[l],typeof e==="string"&&(g=c(e,o))&&k.push(h(e)+(d?": ":":")+g)}else for(e in o)Object.hasOwnProperty.call(o,e)&&(g=c(e,o))&&k.push(h(e)+(d?": ":":")+g);g=k.length===0?"{}":d?"{\n"+d+k.join(",\n"+d)+"\n"+
i+"}":"{"+k.join(",")+"}";d=i;return g}}var b={},n=!1;if(typeof Date.prototype.toJSON!=="function")Date.prototype.toJSON=function(){return isFinite(this.valueOf())?this.getUTCFullYear()+"-"+e(this.getUTCMonth()+1)+"-"+e(this.getUTCDate())+"T"+e(this.getUTCHours())+":"+e(this.getUTCMinutes())+":"+e(this.getUTCSeconds())+"Z":null},String.prototype.toJSON=Number.prototype.toJSON=Boolean.prototype.toJSON=function(){return this.valueOf()};var f=/[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
a=/[\\\"\x00-\x1f\x7f-\x9f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,d,m,g={"\u0008":"\\b","\t":"\\t","\n":"\\n","\u000c":"\\f","\r":"\\r",'"':'\\"',"\\":"\\\\"},p;b.stringify=function(a,b,l){var e;m=d="";if(typeof l==="number")for(e=0;e<l;e+=1)m+=" ";else typeof l==="string"&&(m=l);if((p=b)&&typeof b!=="function"&&(typeof b!=="object"||typeof b.length!=="number"))throw Error("JSON.stringify");return c("",{"":a})};b.parse=function(a,b){function d(a,
l){var c,e,g=a[l];if(g&&typeof g==="object")for(c in g)Object.hasOwnProperty.call(g,c)&&(e=d(g,c),e!==void 0?g[c]=e:delete g[c]);return b.call(a,l,g)}var l,a=String(a);f.lastIndex=0;f.test(a)&&(a=a.replace(f,function(a){return"\\u"+("0000"+a.charCodeAt(0).toString(16)).slice(-4)}));if(/^[\],:{}\s]*$/.test(a.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g,"@").replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,"]").replace(/(?:^|:|,)(?:\s*\[)+/g,"")))return l=eval("("+a+")"),
typeof b==="function"?d({"":l},""):l;throw new SyntaxError("JSON.parse");};var k={a:[1,2,3]},i,l;if(Object.toJSON&&Object.toJSON(k).replace(/\s/g,"")==='{"a":[1,2,3]}')i=Object.toJSON;typeof String.prototype.evalJSON==="function"&&(k='{"a":[1,2,3]}'.evalJSON(),k.a&&k.a.length===3&&k.a[2]===3&&(l=function(a){return a.evalJSON()}));(function(){var a=[1,2,3];typeof a.toJSON==="function"&&(a=a.toJSON(),n=!(a&&a.length===3&&a[2]===3))})();if(n||!i||!l)return{stringify:b.stringify,parse:b.parse};return{stringify:i,
parse:l}});
DISQUS.define(function(){function e(b){for(a=1;b=h.shift();)b()}var h=[],c,b=document,n=b.documentElement,f=n.doScroll,a=/^loade|c/.test(b.readyState),d;b.addEventListener&&b.addEventListener("DOMContentLoaded",c=function(){b.removeEventListener("DOMContentLoaded",c,!1);e()},!1);f&&b.attachEvent("onreadystatechange",c=function(){/^c/.test(b.readyState)&&(b.detachEvent("onreadystatechange",c),e())});d=f?function(b){self!=top?a?b():h.push(b):function(){try{n.doScroll("left")}catch(a){return setTimeout(function(){d(b)},50)}b()}()}:
function(b){a?b():h.push(b)};return{domready:d}});
DISQUS.define("Events",function(){var e=/\s+/,h={on:function(c,b,h){var f,a;if(!b)return this;c=c.split(e);for(f=this._callbacks||(this._callbacks={});a=c.shift();)a=f[a]||(f[a]=[]),a.push(b),a.push(h);return this},off:function(c,b,h){var f,a,d;if(!(a=this._callbacks))return this;if(!c&&!b&&!h)return delete this._callbacks,this;for(c=c?c.split(e):_.keys(a);f=c.shift();)if(!(d=a[f])||!b&&!h)delete a[f];else for(f=d.length-2;f>=0;f-=2)b&&d[f]!==b||h&&d[f+1]!==h||d.splice(f,2);return this},trigger:function(c){var b,
h,f,a,d,m,g;if(!(h=this._callbacks))return this;g=[];c=c.split(e);a=1;for(d=arguments.length;a<d;a++)g[a-1]=arguments[a];for(;b=c.shift();){if(m=h.all)m=m.slice();if(f=h[b])f=f.slice();if(f){a=0;for(d=f.length;a<d;a+=2)f[a].apply(f[a+1]||this,g)}if(m){b=[b].concat(g);a=0;for(d=m.length;a<d;a+=2)m[a].apply(m[a+1]||this,b)}}return this}};h.bind=h.on;h.unbind=h.off;return h});
DISQUS.define(function(e){function h(){throw Error(Array.prototype.join.call(arguments," "));}function c(a,b,d){if(a.addEventListener)a.addEventListener(b,d,!1);else if(a.attachEvent)a.attachEvent("on"+b,d);else throw Error("No event support.");}var b=e.document,n=DISQUS.use("JSON"),f={},a={},d=0;if(!(DISQUS.version&&DISQUS.version()==="2")){c(e,"message",function(b){var d,c;for(c in a)if(Object.prototype.hasOwnProperty.call(a,c)&&b.origin==a[c].origin){d=!0;break}if(d)switch(d=n.parse(b.data),(c=
a[d.sender])||h("Message from our server but with invalid sender UID:",d.sender),d.scope){case "host":c.trigger(d.name,d.data);break;case "global":DISQUS.trigger(d.name,d.data);break;default:h("Message",d.scope,"not supported. Sender:",b.origin)}},!1);c(e,"hashchange",function(){DISQUS.trigger("window.hashchange",{hash:e.location.hash})},!1);c(e,"resize",function(){DISQUS.trigger("window.resize")},!1);var m=function(){DISQUS.trigger("window.scroll")};(function(a,b,d,e){var g=(new Date).getTime();
c(a,b,function(){var a=(new Date).getTime();a-g>=e&&(g=a,d())})})(e,"scroll",m,250);(function(a,b,d,e){var g;c(a,b,function(a){g&&clearTimeout(g);g=setTimeout(function(){d(a)},e)})})(e,"scroll",m,300);c(b,"click",function(){DISQUS.trigger("window.click")});m=function(){this.uid=d++;f[this.uid]=this};DISQUS.extend(m.prototype,DISQUS.Events);m.prototype.destroy=function(){delete f[this.uid]};DISQUS.extend(m,{listByKey:function(){var a={},b;for(b in f)Object.prototype.hasOwnProperty.call(f,b)&&(a[b]=
f[b]);return a},list:function(){var a=[],b;for(b in f)Object.prototype.hasOwnProperty.call(f,b)&&a.push(f[b]);return a},get:function(a){if(Object.prototype.hasOwnProperty.call(f,a))return f[a];return null}});var g=function(a){a=a||{};this.isReady=!1;this.uid=a.uid||d++;this.elem=null;this.styles={};this.role=a.role||"application"};g.prototype.load=function(){var a=this.elem=b.createElement("iframe");a.setAttribute("id","dsq"+this.uid);a.setAttribute("data-disqus-uid",this.uid);a.setAttribute("allowTransparency",
"true");a.setAttribute("frameBorder","0");a.setAttribute("role",this.role);for(var d in this.styles)this.styles.hasOwnProperty(d)&&(a.style[d]=this.styles[d])};g.prototype.destroy=function(){this.elem&&this.elem.parentNode.removeChild(this.elem)};var p=function(b){var d=this;g.call(d,b);d.listeners={};d.origin=b.origin;d.target=b.target;d.container=b.container;d.styles=DISQUS.extend({width:"100%",border:"none",overflow:"hidden",display:"none"},b.styles||{});a[d.uid]=d;d.on("ready",function(){d.isReady=
!0})};DISQUS.extend(p.prototype,DISQUS.Events);p.prototype.load=function(a){g.prototype.load.call(this);var d=this.elem;d.setAttribute("width","100%");d.setAttribute("src",this.target+"#"+this.uid);c(d,"load",function(){d.style.display="";a&&a()});(b.getElementById(this.container)||b.body).appendChild(d);this.elem=d};p.prototype.sendMessage=function(a,b){var d=function(a,b,d){return function(){d.elem.contentWindow.postMessage(a,b)}}(n.stringify({scope:"client",data:{eventName:a,data:b}}),this.origin,
this);if(this.isReady)d();else this.on("ready",d)};p.prototype.getPosition=function(){for(var a=this.elem,b=0,d=0;a;)b+=a.offsetLeft,d+=a.offsetTop,a=a.offsetParent;return{top:d,left:b}};p.prototype.inViewport=function(a){var a=a||this.getPosition(),a=a.top,b=a+this.elem.offsetHeight,d=e.pageYOffset;return!(a>d+e.innerHeight||b<d)};p.prototype.destroy=function(){this.off();g.prototype.destroy.call(this)};var k=function(a){g.call(this,a);this.contents=a.contents;this.container=a.container;this.styles=
{width:"100%",border:"none",overflow:"hidden"};a.styles=a.styles||{};for(var b in a.styles)a.styles.hasOwnProperty(b)&&(this.styles[b]=a.styles[b])};k.prototype.load=function(){g.prototype.load.call(this);var a=this.elem;a.setAttribute("scrolling","no");(b.getElementById(this.container)||b.body).appendChild(a);this.element=a;this.window=a.contentWindow;try{this.window.document.open()}catch(d){a.src="javascript:var d=document.open();d.domain='"+b.domain+"';void(0);"}this.document=this.window.document;
this.document.write(this.contents);this.document.close();if(a=this.document.body){var c=this.element.style;c.height=c.minHeight=c.maxHeight=a.offsetHeight+"px"}};k.prototype.exec=function(a){a.call(this,this.window,this.document)};k.prototype.hide=function(){var a=this.element.style.display;if(a!=="none")this._display=a;this.element.style.display="none"};k.prototype.show=function(){this.element.style.display=this._display||"block"};k.prototype.click=function(a){c(this.document.body,"click",function(b){a(b)})};
k.prototype.destroy=g.prototype.destroy;var i=DISQUS.extend({},DISQUS.Events);return{IFRAME:"__widget_iframe__",log:function(a){var d=b.getElementById("messages");if(d){var c=b.createElement("p");c.innerHTML=a;d.appendChild(c)}},version:function(){return"2"},on:i.on,off:i.off,trigger:i.trigger,Channel:p,Sandbox:k,App:m}}});
DISQUS.define("next.publisher",function(e){function h(a,b,c){var g,c=c||b;if(a===f)return"";e.getComputedStyle?g=f.defaultView.getComputedStyle(a,null).getPropertyValue(b):a.currentStyle&&(g=a.currentStyle[b]?a.currentStyle[b]:a.currentStyle[c]);return g=="transparent"||g===""||g=="rgba(0, 0, 0, 0)"?h(a.parentNode,b,c):g||null}function c(a){function b(a){a=Number(a).toString(16);return a.length==1?"0"+a:a}if(a.substr(0,1)==="#")return a;var c=/.*?rgb\((\d+),\s*(\d+),\s*(\d+)\)/.exec(a);if(!c||c.length!==
4)return"";var a=b(c[1]),e=b(c[2]),c=b(c[3]);return"#"+a+e+c}function b(a,b,c,e){DISQUS.isString(b)&&(b=f.createElement(b));var n=null;b.style.visibility="hidden";a.appendChild(b);n=h(b,c,e);a.removeChild(b);return n}function n(a){return a.toLowerCase().replace(/^\s+|\s+$/g,"").replace(/['"]/g,"")}var f=e.document;return{getContrastYIQ:function(a){a.match("^rgb")&&(a=c(a).substr(1));var b=parseInt(a.substr(0,2),16),e=parseInt(a.substr(2,2),16),a=parseInt(a.substr(4,2),16);return(b*299+e*587+a*114)/
1E3},colorToHex:c,getElementStyle:b,getAnchorColor:function(a){var d=f.createElement("a");d.href=+new Date;return b(a,d,"color")},normalizeFontValue:n,isSerif:function(a){for(var a=b(a,"span","font-family","fontFamily").split(","),d={courier:1,times:1,"times new roman":1,georgia:1,palatino:1,serif:1},c,e=0;e<a.length;e++)if(c=n(a[e]),d.hasOwnProperty(c))return!0;return!1}}});
DISQUS.define(function(){function e(e){DISQUS.App.call(this);this.switches={};var c={target:e.useSSL?"https://securecdn.disqus.com/1359671309/build/next-switches/client_ssl.html":"http://mediacdn.disqus.com/1359671309/build/next-switches/client.html",container:e.container};c.origin=e.useSSL?"https://securecdn.disqus.com":"http://mediacdn.disqus.com";this.frame=new DISQUS.Channel(c);var b=this;this.frame.load(function(){b.frame.elem.style.display="none"})}e.prototype=DISQUS.extend({fetch:function(e){var c=
this,e=e||{},b=e.success;delete e.success;this.frame.on("switches.received",function(e){c.switches=e;DISQUS.trigger("switches.changed",e);b&&b(e)});this.frame.sendMessage("fetch",e)},enabled:function(e){return this.switches[e]?this.switches[e]:!1}},DISQUS.App.prototype);return{Switches:function(h){return new e(h)}}});
DISQUS.define(function(e){var h=function(c){DISQUS.App.call(this);this.settings=c;this.frame=null};DISQUS.extend(h.prototype,DISQUS.App.prototype);h.prototype.init=function(){var c=this.settings,b="http://disqus.com/embed/profile/",h="http://disqus.com";c.useSSL&&(b="https://disqus.com/embed/profile/",h="https://disqus.com");var f=this.frame=new DISQUS.Channel({origin:h,target:DISQUS.serialize(b,{f:c.forum}),container:c.container,uid:this.uid,styles:{height:"100%",position:"fixed",top:0,left:0}});
f.on("ready",function d(){f.off("ready",d);f.sendMessage("init",{referrer:e.location.href});this.trigger("loading.init")},this);f.on("close",function(){this.frame.elem.style.display="none";e.focus()},this);f.load();this.trigger("loading.start")};h.prototype.showProfile=function(c){var b=this.frame;if(!b.isReady)return void b.on("ready",function f(){b.off("ready",f);this.showProfile(c)},this);b.sendMessage("showProfile",c);b.elem.style.display="block"};h.prototype.destroy=function(){DISQUS.App.prototype.destroy.call(this)};
return{Profile:function(c){return new h(c)}}});
DISQUS.define("backplane",function(){var e;try{localStorage.setItem("disqus.localStorageTest","disqus"),localStorage.removeItem("disqus.localStorageTest"),e=!0}catch(h){e=!1}var c=function(b){this.frame=b;this.credentials="unset";var c=this;typeof Backplane==="function"&&typeof Backplane.version==="string"&&typeof Backplane.subscribe==="function"&&e&&Backplane(function(){c.initialize()})};DISQUS.extend(c.prototype,{frameEvents:{invalidate:"clearCredentials"},initialize:function(){var b=this;DISQUS.each(this.frameEvents,
function(c,e){b.frame.on("backplane."+e,typeof c==="function"?c:b[c],b)});this.credentialsFromLocalStorage()&&this.frame.sendMessage("login",{backplane:this.credentials});this.subscribe()},subscribe:function(){var b=this;Backplane.subscribe(function(c){var e=b.handlers[c.type];e&&e.call(b,c)})},handlers:{"identity/login":function(b){var c=b.messageURL,b=b.channel;this.credentials!=="unset"&&this.credentials!==null&&this.credentials.channel===b&&this.credentials.messageUrl===c||(this.setCredentials(b,
c),this.frame.sendMessage("login",{backplane:this.getCredentials()}))}},credentialsFromLocalStorage:function(){var b=localStorage.getItem("disqus.backplane.channel"),c=localStorage.getItem("disqus.backplane.messageUrl");this.setCredentials(b,c,!0);return this.credentials},setCredentials:function(b,c,e){if(!b||!c)return void this.clearCredentials();e||(localStorage.setItem("disqus.backplane.channel",b),localStorage.setItem("disqus.backplane.messageUrl",c));this.credentials={channel:b,messageUrl:c}},
getCredentials:function(){if(this.credentials!=="unset")return this.credentials;return this.credentialsFromLocalStorage()},clearCredentials:function(b){b=b||{};this.credentials=null;localStorage.removeItem("disqus.backplane.channel");localStorage.removeItem("disqus.backplane.messageUrl");if(b.redirectUrl)window.location=b.redirectUrl}});return{BackplaneIntegration:c}});
DISQUS.define(function(e,h){function c(a,b,c,d,e,f){return'<img width="'+a+'" height="'+b+'" alt="'+d+'" src="data:image/'+c+";base64,"+e+'"'+(f?'style="'+f+'"':"")+"/>"}function b(a){for(var b=DISQUS.App.list(),c=0,e=b.length,b=b[c];c<e;c++)b instanceof d&&a(b)}var n=e.document,f=["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","TkSuQmCC"],a=["R0lGODlhEAALAPQAAP///z2LqeLt8dvp7u7090GNqz2LqV+fuJ/F1IW2ycrf51aatHWs","waXJ14i4ys3h6FmctUCMqniuw+vz9eHs8fb5+meku+Tu8vT4+cfd5bbT3tbm7PH2+AAA","AAAAAAAAACH/C05FVFNDQVBFMi4wAwEAAAAh/hpDcmVhdGVkIHdpdGggYWpheGxvYWQu","aW5mbwAh+QQJCwAAACwAAAAAEAALAAAFLSAgjmRpnqSgCuLKAq5AEIM4zDVw03ve27if","DgfkEYe04kDIDC5zrtYKRa2WQgAh+QQJCwAAACwAAAAAEAALAAAFJGBhGAVgnqhpHIeR","vsDawqns0qeN5+y967tYLyicBYE7EYkYAgAh+QQJCwAAACwAAAAAEAALAAAFNiAgjoth",
"LOOIJAkiGgxjpGKiKMkbz7SN6zIawJcDwIK9W/HISxGBzdHTuBNOmcJVCyoUlk7CEAAh","+QQJCwAAACwAAAAAEAALAAAFNSAgjqQIRRFUAo3jNGIkSdHqPI8Tz3V55zuaDacDyIQ+","YrBH+hWPzJFzOQQaeavWi7oqnVIhACH5BAkLAAAALAAAAAAQAAsAAAUyICCOZGme1rJY","5kRRk7hI0mJSVUXJtF3iOl7tltsBZsNfUegjAY3I5sgFY55KqdX1GgIAIfkECQsAAAAs","AAAAABAACwAABTcgII5kaZ4kcV2EqLJipmnZhWGXaOOitm2aXQ4g7P2Ct2ER4AMul00k","j5g0Al8tADY2y6C+4FIIACH5BAkLAAAALAAAAAAQAAsAAAUvICCOZGme5ERRk6iy7qpy","HCVStA3gNa/7txxwlwv2isSacYUc+l4tADQGQ1mvpBAAIfkECQsAAAAsAAAAABAACwAA","BS8gII5kaZ7kRFGTqLLuqnIcJVK0DeA1r/u3HHCXC/aKxJpxhRz6Xi0ANAZDWa+kEAA7",
"AAAAAAAAAAAA"],d=function(a){DISQUS.App.call(this);this.settings=a;this.indicators={north:null,south:null};this._boundGlobalEvents=[];this.frame=null};DISQUS.extend(d.prototype,DISQUS.App.prototype);d.prototype.init=function(){function b(a,c,d){j.on("affiliateLink",function(b){var e=DISQUS.vglnk.$;if(!e)return void j.sendMessage("affiliateLink");e.request(a+"/click",{format:"jsonp",out:b.url,key:c,loc:j.target,subId:d},{fn:function(a){return function(b){var c={token:a};if(b)c.url=b;j.sendMessage("affiliateLink",
c)}}(b.token),timeout:DISQUS.vglnk.opt("click_timeout")})})}function d(a,b){k._boundGlobalEvents.push(a);DISQUS.on(a,b,k)}var k=this,i=k.settings,l="http://disqus.com/embed/comments/",m="http://disqus.com";i.useSSL&&(l="https://disqus.com/embed/comments/",m="https://disqus.com");var q={f:i.forum,t_i:i.identifier,t_u:i.url||e.location.href,t_s:i.slug,t_t:i.title,s_o:i.sortOrder,c:i.useConman||h};if(i.notSupported)q.n_s=1;var r=DISQUS.isString(i.container)?n.getElementById(i.container):i.container,
j=k.frame=new DISQUS.Channel({origin:m,target:DISQUS.serialize(l,q),container:i.container,uid:this.uid});if(i.notSupported)j.styles.height="500px";var u=!1,s,v;if(!i.notSupported)s=n.createElement("div"),s.innerHTML=c(71,17,"png","DISQUS",f.join(""))+c(16,11,"gif","...",a.join(""),"margin:0 0 3px 5px"),r.appendChild(s);l=function(){var a=j.getPosition(),b=e.pageYOffset,c=e.innerHeight,d=j.inViewport(a);d?(u=!0,j.sendMessage("window.scroll",{frameOffset:a,pageOffset:b,height:c}),j.sendMessage("window.inViewport")):
u&&!d&&(u=!1,j.sendMessage("window.scrollOffViewport"))};j.on("ready",function o(a){j.off("ready",o);s&&s.parentNode===r&&r.removeChild(s);e.clearTimeout(v);var b={themeUrl:i.themeUrl,permalink:i.permalink,anchorColor:i.anchorColor,referrer:e.location.href,colorScheme:i.colorScheme,language:i.language,typeface:i.typeface,remoteAuthS3:i.remoteAuthS3,apiKey:i.apiKey,sso:i.sso,parentWindowHash:e.location.hash};if(e.navigator.userAgent.match(/(iPad|iPhone|iPod)/))b.width=j.elem.offsetWidth;j.inViewport()&&
j.sendMessage("window.inViewport");k.clientData=a;j.sendMessage("init",b);k.trigger("loading.init")});j.on("resize",function(a){j.elem.style.height=a.height+"px"});j.on("reload",function(){e.location.reload()});j.on("reset",function(){DISQUS.reset({reload:!0})});j.on("posts.paginate",function(){k.trigger("posts.paginate")});j.on("posts.create",function(a){k.trigger("posts.create",{id:a.id,text:a.raw_message})});j.on("scrollTo",function(a){var b=j.getPosition(),b=a.relative==="window"?a.top:b.top+
a.top;(a.force||!(b>e.pageYOffset&&b<e.pageYOffset+e.innerHeight))&&e.scrollTo(0,b)});j.on("fakeScroll",l);j.on("realtime.init",function(a){for(var b=["north","south"],c,d,e=0;e<b.length;e++)d=b[e],c=new DISQUS.Sandbox({uid:"-indicator-"+d,container:k.settings.container,contents:a[d].contents,styles:a[d].styles}),c.load(),c.hide(),function(a){c.click(function(){j.sendMessage("realtime.click",a)})}(d),k.indicators[d]=c});j.on("realtime.showNorth",function(a){var b=k.indicators.north;b.document.getElementById("message").innerHTML=
a;b.show()});j.on("realtime.hideNorth",function(){k.indicators.north.hide()});j.on("realtime.showSouth",function(a){var b=k.indicators.south;b.document.getElementById("message").innerHTML=a;b.show()});j.on("realtime.hideSouth",function(){k.indicators.south.hide()});j.on("mainViewRendered",function(){DISQUS.trigger("lounge:mainViewRendered");k.trigger("loading.done")});j.on("profile.show",function(a){if(!k.profile)k.profile=DISQUS.Profile({container:i.container,useSSL:i.useSSL,forum:i.forum}),k.profile.init();
k.profile.showProfile(a.username)});j.on("loadLinkAffiliator",function(a){j.off("loadLinkAffiliator");if(!e.vglnk_self&&!e.vglnk&&!function(){for(var a in e)if(a.indexOf("skimlinks")===0||a.indexOf("skimwords")===0)return!0;return!1}()){var c=a.apiUrl,d=a.key,f=String(a.id);if(!(a.clientUrl==null||c==null||d==null||a.id==null))DISQUS.define("vglnk",function(){return{api_url:c,key:d,sub_id:f}}),e.vglnk_self="DISQUS.vglnk",DISQUS.require(a.clientUrl),DISQUS.defer(function(){return DISQUS.vglnk.opt},
function(){j.sendMessage("affiliationOptions",{timeout:DISQUS.vglnk.opt("click_timeout")})}),b(c,d,f)}});j.on("loadBackplane",function(){j.off("loadBackplane");k.backplane=new DISQUS.backplane.BackplaneIntegration(j)});v=e.setTimeout(function(){(new Image).src=DISQUS.serialize("//juggler.services.disqus.com/stat.gif",{event:"slow_embed"});s.innerHTML+='<p>DISQUS seems to be taking longer than usual. <a href="#" onclick="DISQUS.reset({reload: true}); return false;">Reload</a>?</p>'},1E4);j.load(function(){i.notSupported?
(j.elem.setAttribute("height","500px"),j.elem.setAttribute("scrolling","yes"),j.elem.setAttribute("horizontalscrolling","no"),j.elem.setAttribute("verticalscrolling","yes")):(j.elem.setAttribute("scrolling","no"),j.elem.setAttribute("horizontalscrolling","no"),j.elem.setAttribute("verticalscrolling","no"))});d("window.hashchange",function(a){j.sendMessage("window.hashchange",a.hash)});d("window.resize",function(){j.sendMessage("window.resize")});d("window.scroll",l);d("window.click",function(){j.sendMessage("window.click")});
d("switches.changed",function(a){j.sendMessage("switches.changed",a)});k.trigger("loading.start")};d.prototype.destroy=function(){var a=this.indicators;this.off();if(this._boundGlobalEvents.length)DISQUS.off(this._boundGlobalEvents.join(" "),null,this),this._boundGlobalEvents=null;this.frame&&this.frame.destroy();if(a.north)a.north.destroy(),a.north=null;if(a.south)a.south.destroy(),a.south=null;DISQUS.App.prototype.destroy.call(this)};var m=function(a){return new d(a)};DISQUS.extend(m,{listByKey:function(){var a=
{};b(function(b){a[b.uid]=b});return a},list:function(){var a=[];b(function(b){a.push(b)});return a},get:function(a){a=DISQUS.App.get(a);return a instanceof d&&a}});return{Lounge:m}});
(function(e,h,c){function b(){function a(b){var b=b.getAttribute?b.getAttribute("src"):b.src,c=[/(https?:)\/\/(www\.)?disqus\.com\/forums\/([\w_\-]+)/i,/(https?:)\/\/(www\.)?([\w_\-]+)\.disqus\.com/i,/(https?:)\/\/(www\.)?dev\.disqus\.org\/forums\/([\w_\-]+)/i,/(https?:)\/\/(www\.)?([\w_\-]+)\.dev\.disqus\.org/i],d=c.length;if(!b||b.substring(b.length-8)!="embed.js")return null;for(var e=0;e<d;e++){var f=b.match(c[e]);if(f&&f.length&&f.length==4)return p=f[1]||null,f[3]}return null}for(var b=h.getElementsByTagName("script"),
c=b.length-1;c>=0;c--){var d=a(b[c]);if(d!==null)return d}return null}function n(){if(e.location.protocol==="https:")return!0;p===c&&b();return p==="https:"}function f(){for(var a=h.getElementsByTagName("h1"),b=h.title,d=b.length,e=b,f=0.6,g=0;g<a.length;g++)(function(a){var a=a.textContent||a.innerText,g;if(!(a===null||a===c)){g=0;for(var h=Array(b.length),i=0;i<=b.length;i++){h[i]=Array(a.length);for(var j=0;j<=a.length;j++)h[i][j]=0}for(i=0;i<b.length;i++)for(j=0;j<a.length;j++)b[i]==a[j]&&(h[i+
1][j+1]=h[i][j]+1,h[i+1][j+1]>g&&(g=h[i+1][j+1]));g/=d;g>f&&(f=g,e=a)}})(a[g]);return e}function a(){h.getElementById(m).innerHTML="";var a=i.page;if(!e.postMessage||!e.JSON)r=!0;if(e.navigator.appName==="Microsoft Internet Explorer"&&(!h.documentMode||h.documentMode<8))r=!0;a={container:m,forum:l,sortOrder:"popular",permalink:k,useSSL:n(),language:i.language,typeface:d.isSerif(g)?"serif":"sans-serif",anchorColor:d.getAnchorColor(g),colorScheme:128<d.getContrastYIQ(d.getElementStyle(g,"span","color"))?
"dark":"light",url:a.url||e.location.href.replace(/#.*$/,""),title:a.title||f(),slug:a.slug,category:a.category_id,identifier:a.identifier,apiKey:a.api_key,remoteAuthS3:a.remote_auth_s3,sso:i.sso,themeUrl:e.disqus_theme_root_url,useConman:e.disqus_demo,notSupported:r};q=DISQUS.Lounge(a);var b={onReady:"loading.done",onNewComment:"posts.create",onPaginate:"posts.paginate"};DISQUS.each(i.callbacks,function(a,c){b[c]&&DISQUS.each(a,function(a){q.on(b[c],a)})});q.init()}var d=DISQUS.use("next.publisher"),
m=e.disqus_container_id||"disqus_thread",g=h.getElementById(m),p,k=function(){var a=e.location.hash;return(a=a&&a.match(/comment\-([0-9]+)/))&&a[1]}(),i={page:{url:c,title:c,slug:c,category_id:c,identifier:c,language:c,api_key:c,remote_auth_s3:c,author_s3:c,developer:c},strings:c,sso:{},callbacks:{preData:[],preInit:[],onInit:[],afterRender:[],onReady:[],onNewComment:[],preReset:[],onPaginate:[]}};DISQUS.each(["developer","shortname","identifier","url","title","category_id","language","slug"],function(a){var b=
e["disqus_"+a];typeof b!=="undefined"&&(i.page[a]=b)});var l=e.disqus_shortname||b(),l=l.toLowerCase();if(typeof e.disqus_config==="function")try{e.disqus_config.call(i)}catch(t){}var q,r=!1;a();if(!r){var j=DISQUS.Switches({container:m,useSSL:n()});j.fetch({data:{forum:l}});DISQUS.domready(function(){if(h.getElementsByClassName){var a=h.getElementsByClassName("dsq-brlink");a&&a.length&&a[0].parentNode.removeChild(a[0])}});DISQUS.request={get:function(a,b,c){DISQUS.require(a,b,c)}};DISQUS.reset=function(b){b=
b||{};if(typeof b.config==="function")try{b.config.call(i)}catch(c){}q&&(q.destroy(),q=null);b.reload&&(a(),DISQUS.trigger("switches.changed",j.switches))}}})(this,this.document);