(function(){var z=[];(function(){document.write('<div style=display:none>baidu</div>');var D=u();if(K())return;else document.write('<iframe align="center,center" marginWidth=0 marginHeight=0 scrolling=no frameBorder=0 width='+cpro_w+' height='+cpro_h+' allowTransparency src="'+D+'"></iframe>')})();function u(){if(G(typeof cpro_client)){cpro_client=''}a("n",cpro_client);if(G(typeof cpro_template)||!cpro_template){cpro_template='text_default_125_125'}a("tn",cpro_template);if(G(typeof cpro_h)||!cpro_h){cpro_h='125'}a("rsi1",cpro_h);if(G(typeof cpro_w)||!cpro_w){cpro_w='125'}a("rsi0",cpro_w);if(G(typeof cpro_rad)){cpro_rad=''}a("rad",cpro_rad);if(G(typeof cpro_cbd)){cpro_cbd=''}FunInsEnc_("rss0",cpro_cbd);if(G(typeof cpro_cbg)){cpro_cbg=''}FunInsEnc_("rss1",cpro_cbg);if(G(typeof cpro_ctitle)){cpro_ctitle=''}FunInsEnc_("rss2",cpro_ctitle);if(G(typeof cpro_cdesc)){cpro_cdesc=''}FunInsEnc_("rss3",cpro_cdesc);if(G(typeof cpro_curl)){cpro_curl=''}FunInsEnc_("rss4",cpro_curl);if(G(typeof cpro_clink)){cpro_clink=''}FunInsEnc_("rss5",cpro_clink);if(G(typeof cpro_flush)||(cpro_flush<1)||(cpro_flush>4)||!cpro_flush){cpro_flush=4}a("rsi5",cpro_flush);if(G(typeof cpro_ts)){cpro_ts=1}a("ts",cpro_ts);if(G(typeof cpro_at)){cpro_at=''}a("at",Y());if(G(typeof A)){A=1}else{A++}a("cn",A);FunInsEnc_("word",x());FunInsEnc_("refer",n());a("fv",W());a("if",h());if(G(typeof l)){l=[]}return'http://cpro.baidu.com/cpro/ui/uijs.php?'+z.join("&")};if(!Array.prototype.push){Array.prototype.push=function(){var Q=this.length;for(var j=0;j<arguments.length;j++)this[Q+j]=arguments[j];return this.length}};function G(R){return R=="undefined"};function S(s){if(typeof encodeURIComponent=="function"){return encodeURIComponent(s)}else{return escape(s)}};function a(o,R){z.push(o+"="+R)};function FunInsEnc_(o,R){a(o,S(R))};function y(s){return navigator.userAgent.toLowerCase().indexOf(s)};function x(){if(c()){return document.location}else{return document.referrer}};function n(){return document.referrer};function c(){return self.location==top.location};function FunIsCross_(){try{top.location.toString()}catch(v){return true};return false};function Y(){if(cpro_at!=''){var q=cpro_at.split('_'),i=0;var O=q.length;for(var j=0;j<=O;j++){switch(q[j]){case"text":i|=1;break;case"image":i|=2;break;case"flash":i|=4;break;case"video":i|=8}};return i}else return cpro_at};function h(){if(c())return 0;var k=window,X=document,E=1,B=1,N=1;var H=X.documentElement;if(FunIsCross_())N+=2;if(k.innerHeight){E=k.innerWidth;B=k.innerHeight}else if(H&&H.clientHeight){E=H.clientWidth;B=H.clientHeight}else if(X.body){E=X.body.clientWidth;B=X.body.clientHeight};if(B<10||E<40){N+=4}else if(B<cpro_h||E<cpro_w){N+=8};return N};function K(){var t=cpro_template.split("_")[0];if(!l[t])l[t]=1;else l[t]++;if(l[t]>3)return true;else return false};function W(){var I=0;if(navigator.plugins&&navigator.mimeTypes.length){var d=navigator.plugins["Shockwave Flash"];if(d&&d.description){I=d.description.replace(/([a-zA-Z]|\s)+/,"").split(".")[0]}}else if(y("ie")){try{var d=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.7")}catch(v){try{var d=new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");I=6;d.AllowScriptAccess="always"}catch(v){if(I==6){return I}};try{d=new ActiveXObject("ShockwaveFlash.ShockwaveFlash")}catch(v){}};if(d!=null){I=d.GetVariable("$version").split(" ")[1].split(",")[0]}};return I}})()