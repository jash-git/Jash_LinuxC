var gapi=window.gapi=window.gapi||{};gapi._bs=new Date().getTime();(function(){var aa=encodeURIComponent,k=window,ba=Object,p=document,da=Array,ea=parseInt,q=String,fa=decodeURIComponent;function ga(a,b){return a.type=b}
var ha="appendChild",r="push",s="test",ia="shift",ja="exec",ka="width",u="replace",la="getElementById",ma="concat",na="charAt",oa="JSON",y="indexOf",pa="nodeName",qa="match",ra="readyState",z="createElement",A="setAttribute",sa="type",ta="bind",ua="getTime",va="getElementsByTagName",B="substr",C="toString",D="length",E="prototype",F="split",G="location",wa="hasOwnProperty",H="style",xa="removeChild",I="call",J="getAttribute",ya="protocol",K="charCodeAt",L="href",M="substring",za="action",N="apply",
Aa="attributes",O="parentNode",Ba="update",Ca="height",P="join",Da="toLowerCase",Ea=function(a,b,c){return a[I][N](a[ta],arguments)},Fa=function(a,b,c){if(!a)throw Error();if(2<arguments[D]){var d=da[E].slice[I](arguments,2);return function(){var c=da[E].slice[I](arguments);da[E].unshift[N](c,d);return a[N](b,c)}}return function(){return a[N](b,arguments)}},Ga=function(a,b,c){Ga=Function[E][ta]&&-1!=Function[E][ta][C]()[y]("native code")?Ea:Fa;return Ga[N](null,arguments)};
Function[E].bind=Function[E][ta]||function(a,b){if(1<arguments[D]){var c=da[E].slice[I](arguments,1);c.unshift(this,a);return Ga[N](null,c)}return Ga(this,a)};var Q=k,R=p,Ha=Q[G],Ia=function(){},Ja=/\[native code\]/,S=function(a,b,c){return a[b]=a[b]||c},Ka=function(a){for(var b=0;b<this[D];b++)if(this[b]===a)return b;return-1},La=function(a){a=a.sort();for(var b=[],c=void 0,d=0;d<a[D];d++){var e=a[d];e!=c&&b[r](e);c=e}return b},Ma=/&/g,Na=/</g,Oa=/>/g,Pa=/"/g,Qa=/'/g,Ra=function(a){return q(a)[u](Ma,"&amp;")[u](Na,"&lt;")[u](Oa,"&gt;")[u](Pa,"&quot;")[u](Qa,"&#39;")},T=function(){var a;if((a=ba.create)&&Ja[s](a))a=a(null);else{a={};for(var b in a)a[b]=
void 0}return a},U=function(a,b){return ba[E][wa][I](a,b)},Sa=function(a){if(Ja[s](ba.keys))return ba.keys(a);var b=[],c;for(c in a)U(a,c)&&b[r](c);return b},V=function(a,b){a=a||{};for(var c in a)U(a,c)&&(b[c]=a[c])},Ta=function(a){return function(){Q.setTimeout(a,0)}},Ua=function(a,b){if(!a)throw Error(b||"");},W=S(Q,"gapi",{});var X=function(a,b,c){var d=RegExp("([#].*&|[#])"+b+"=([^&#]*)","g");b=RegExp("([?#].*&|[?#])"+b+"=([^&#]*)","g");if(a=a&&(d[ja](a)||b[ja](a)))try{c=fa(a[2])}catch(e){}return c},Va=/^([^?#]*)(\?([^#]*))?(\#(.*))?$/,Wa=function(a){a=a[qa](Va);var b=T();b.R=a[1];b.g=a[3]?[a[3]]:[];b.l=a[5]?[a[5]]:[];return b},Xa=function(a){return a.R+(0<a.g[D]?"?"+a.g[P]("&"):"")+(0<a.l[D]?"#"+a.l[P]("&"):"")},Ya=function(a,b){var c=[];if(a)for(var d in a)if(U(a,d)&&null!=a[d]){var e=b?b(a[d]):a[d];c[r](aa(d)+"="+
aa(e))}return c},Za=function(a,b,c,d){a=Wa(a);a.g[r][N](a.g,Ya(b,d));a.l[r][N](a.l,Ya(c,d));return Xa(a)},$a=function(a,b){var c="";2E3<b[D]&&(c=b[M](2E3),b=b[M](0,2E3));var d=a[z]("div"),e=a[z]("a");e.href=b;d[ha](e);d.innerHTML=d.innerHTML;b=q(d.firstChild[L]);d[O]&&d[O][xa](d);return b+c},ab=/^https?:\/\/[^\/%\\?#\s]+\/[^\s]*$/i;var bb=function(a,b,c,d){if(Q[c+"EventListener"])Q[c+"EventListener"](a,b,!1);else if(Q[d+"tachEvent"])Q[d+"tachEvent"]("on"+a,b)},eb=function(a){var b=cb;if("complete"!==R[ra])try{b()}catch(c){}db(a)},db=function(a){if("complete"===R[ra])a();else{var b=!1,c=function(){if(!b)return b=!0,a[N](this,arguments)};Q.addEventListener?(Q.addEventListener("load",c,!1),Q.addEventListener("DOMContentLoaded",c,!1)):Q.attachEvent&&(Q.attachEvent("onreadystatechange",function(){"complete"===R[ra]&&c[N](this,arguments)}),
Q.attachEvent("onload",c))}},fb=function(a){for(;a.firstChild;)a[xa](a.firstChild)},gb={button:!0,div:!0,span:!0};var Y;Y=S(Q,"___jsl",T());S(Y,"I",0);S(Y,"hel",10);var hb=function(a){return Y.dpo?Y.h:X(a,"jsh",Y.h)},ib=function(a){var b=S(Y,"sws",[]);b[r][N](b,a)},jb=function(a){var b=S(Y,"PQ",[]);Y.PQ=[];var c=b[D];if(0===c)a();else for(var d=0,e=function(){++d===c&&a()},f=0;f<c;f++)b[f](e)},lb=function(a){return S(S(Y,"H",T()),a,T())};var mb=S(Y,"perf",T()),nb=S(mb,"g",T()),ob=S(mb,"i",T());S(mb,"r",[]);T();T();var pb=function(a,b,c){var d=mb.r;"function"===typeof d?d(a,b,c):d[r]([a,b,c])},qb=function(a,b,c){nb[a]=!b&&nb[a]||c||(new Date)[ua]();pb(a)},sb=function(a,b,c){b&&0<b[D]&&(b=rb(b),c&&0<c[D]&&(b+="___"+rb(c)),28<b[D]&&(b=b[B](0,28)+(b[D]-28)),c=b,b=S(ob,"_p",T()),S(b,c,T())[a]=(new Date)[ua](),pb(a,"_p",c))},rb=function(a){return a[P]("__")[u](/\./g,"_")[u](/\-/g,"_")[u](/\,/g,"_")};var tb=T(),ub=[],vb=function(a){throw Error("Bad hint"+(a?": "+a:""));};ub[r](["jsl",function(a){for(var b in a)if(U(a,b)){var c=a[b];"object"==typeof c?Y[b]=S(Y,b,[])[ma](c):S(Y,b,c)}if(b=a.u)a=S(Y,"us",[]),a[r](b),(b=/^https:(.*)$/[ja](b))&&a[r]("http:"+b[1])}]);var wb=/^(\/[a-zA-Z0-9_\-]+)+$/,xb=/^[a-zA-Z0-9\-_\.!]+$/,yb=/^gapi\.loaded_[0-9]+$/,zb=/^[a-zA-Z0-9,._-]+$/,Db=function(a,b,c,d){var e=a[F](";"),f=tb[e[ia]()],g=null;f&&(g=f(e,b,c,d));if(b=g)b=g,c=b[qa](Ab),d=b[qa](Bb),b=!!d&&1===d[D]&&Cb[s](b)&&!!c&&1===c[D];b||vb(a);return g},Gb=function(a,b,c,d){a=Eb(a);yb[s](c)||vb("invalid_callback");b=Fb(b);d=d&&d[D]?Fb(d):null;var e=function(a){return aa(a)[u](/%2C/g,",")};return[aa(a.S)[u](/%2C/g,",")[u](/%2F/g,"/"),"/k=",e(a.version),"/m=",e(b),d?"/exm="+
e(d):"","/rt=j/sv=1/d=1/ed=1",a.G?"/am="+e(a.G):"",a.H?"/rs="+e(a.H):"","/cb=",e(c)][P]("")},Eb=function(a){"/"!==a[na](0)&&vb("relative path");for(var b=a[M](1)[F]("/"),c=[];b[D];){a=b[ia]();if(!a[D]||0==a[y]("."))vb("empty/relative directory");else if(0<a[y]("=")){b.unshift(a);break}c[r](a)}a={};for(var d=0,e=b[D];d<e;++d){var f=b[d][F]("="),g=fa(f[0]),h=fa(f[1]);2==f[D]&&g&&h&&(a[g]=a[g]||h)}b="/"+c[P]("/");wb[s](b)||vb("invalid_prefix");c=Hb(a,"k",!0);d=Hb(a,"am");a=Hb(a,"rs");return{S:b,version:c,
G:d,H:a}},Fb=function(a){for(var b=[],c=0,d=a[D];c<d;++c){var e=a[c][u](/\./g,"_")[u](/-/g,"_");zb[s](e)&&b[r](e)}return b[P](",")},Hb=function(a,b,c){a=a[b];!a&&c&&vb("missing: "+b);if(a){if(xb[s](a))return a;vb("invalid: "+b)}return null},Cb=/^https?:\/\/[a-z0-9_.-]+\.google\.com(:\d+)?\/[a-zA-Z0-9_.,!=\-\/]+$/,Bb=/\/cb=/g,Ab=/\/\//g,Ib=function(){var a=hb(Ha[L]);if(!a)throw Error("Bad hint");return a};
tb.m=function(a,b,c,d){(a=a[0])||vb("missing_hint");return"https://apis.google.com"+Gb(a,b,c,d)};var Jb=decodeURI("%73cript"),Kb=function(a,b){for(var c=[],d=0;d<a[D];++d){var e=a[d];e&&0>Ka[I](b,e)&&c[r](e)}return c},Mb=function(a){"loading"!=R[ra]?Lb(a):R.write("<"+Jb+' src="'+encodeURI(a)+'"></'+Jb+">")},Lb=function(a){var b=R[z](Jb);b[A]("src",a);b.async="true";(a=R[va](Jb)[0])?a[O].insertBefore(b,a):(R.head||R.body||R.documentElement)[ha](b)},Nb=function(a,b){var c=b&&b._c;if(c)for(var d=0;d<ub[D];d++){var e=ub[d][0],f=ub[d][1];f&&U(c,e)&&f(c[e],a,b)}},Pb=function(a,b){Ob(function(){var c;
c=b===hb(Ha[L])?S(W,"_",T()):T();c=S(lb(b),"_",c);a(c)})},Rb=function(a,b){var c=b||{};"function"==typeof b&&(c={},c.callback=b);Nb(a,c);var d=a?a[F](":"):[],e=c.h||Ib(),f=S(Y,"ah",T());if(f["::"]&&d[D]){for(var g=[],h=null;h=d[ia]();){var l=h[F]("."),l=f[h]||f[l[1]&&"ns:"+l[0]||""]||e,v=g[D]&&g[g[D]-1]||null,m=v;v&&v.hint==l||(m={hint:l,K:[]},g[r](m));m.K[r](h)}var n=g[D];if(1<n){var w=c.callback;w&&(c.callback=function(){0==--n&&w()})}for(;d=g[ia]();)Qb(d.K,c,d.hint)}else Qb(d||[],c,e)},Qb=function(a,
b,c){a=La(a)||[];var d=b.callback,e=b.config,f=b.timeout,g=b.ontimeout,h=null,l=!1;if(f&&!g||!f&&g)throw"Timeout requires both the timeout parameter and ontimeout parameter to be set";var v=S(lb(c),"r",[]).sort(),m=S(lb(c),"L",[]).sort(),n=[][ma](v),w=function(a,b){if(l)return 0;Q.clearTimeout(h);m[r][N](m,t);var d=((W||{}).config||{})[Ba];d?d(e):e&&S(Y,"cu",[])[r](e);if(b){sb("me0",a,n);try{Pb(b,c)}finally{sb("me1",a,n)}}return 1};0<f&&(h=Q.setTimeout(function(){l=!0;g()},f));var t=Kb(a,m);if(t[D]){var t=
Kb(a,v),x=S(Y,"CP",[]),ca=x[D];x[ca]=function(a){if(!a)return 0;sb("ml1",t,n);var b=function(b){x[ca]=null;w(t,a)&&jb(function(){d&&d();b()})},c=function(){var a=x[ca+1];a&&a()};0<ca&&x[ca-1]?x[ca]=function(){b(c)}:b(c)};if(t[D]){var kb="loaded_"+Y.I++;W[kb]=function(a){x[ca](a);W[kb]=null};a=Db(c,t,"gapi."+kb,v);v[r][N](v,t);sb("ml0",t,n);b.sync||Q.___gapisync?Mb(a):Lb(a)}else x[ca](Ia)}else w(t)&&d&&d()};var Ob=function(a){if(Y.hee&&0<Y.hel)try{return a()}catch(b){Y.hel--,Rb("debug_error",function(){try{k.___jsl.hefn(b)}catch(a){throw b;}})}else return a()};W.load=function(a,b){return Ob(function(){return Rb(a,b)})};var Sb=function(a){var b=k.___jsl=k.___jsl||{};b[a]=b[a]||[];return b[a]},Tb=function(a){var b=k.___jsl=k.___jsl||{};b.cfg=!a&&b.cfg||{};return b.cfg},Ub=function(a){return"object"===typeof a&&/\[native code\]/[s](a[r])},Vb=function(a,b){if(b)for(var c in b)b[wa](c)&&(a[c]&&b[c]&&"object"===typeof a[c]&&"object"===typeof b[c]&&!Ub(a[c])&&!Ub(b[c])?Vb(a[c],b[c]):b[c]&&"object"===typeof b[c]?(a[c]=Ub(b[c])?[]:{},Vb(a[c],b[c])):a[c]=b[c])},Wb=function(a){if(a&&!/^\s+$/[s](a)){for(;0==a[K](a[D]-1);)a=
a[M](0,a[D]-1);var b;try{b=k[oa].parse(a)}catch(c){}if("object"===typeof b)return b;try{b=(new Function("return ("+a+"\n)"))()}catch(d){}if("object"===typeof b)return b;try{b=(new Function("return ({"+a+"\n})"))()}catch(e){}return"object"===typeof b?b:{}}},Xb=function(a){Tb(!0);var b=k.___gcfg,c=Sb("cu");if(b&&b!==k.___gu){var d={};Vb(d,b);c[r](d);k.___gu=b}var b=Sb("cu"),e=p.scripts||p[va]("script")||[],d=[],f=[];f[r][N](f,Sb("us"));for(var g=0;g<e[D];++g)for(var h=e[g],l=0;l<f[D];++l)h.src&&0==
h.src[y](f[l])&&d[r](h);0==d[D]&&0<e[D]&&e[e[D]-1].src&&d[r](e[e[D]-1]);for(e=0;e<d[D];++e)d[e][J]("gapi_processed")||(d[e][A]("gapi_processed",!0),(f=d[e])?(g=f.nodeType,f=3==g||4==g?f.nodeValue:f.textContent||f.innerText||f.innerHTML||""):f=void 0,(f=Wb(f))&&b[r](f));a&&(d={},Vb(d,a),c[r](d));d=Sb("cd");a=0;for(b=d[D];a<b;++a)Vb(Tb(),d[a]);d=Sb("ci");a=0;for(b=d[D];a<b;++a)Vb(Tb(),d[a]);a=0;for(b=c[D];a<b;++a)Vb(Tb(),c[a])},Z=function(a){if(!a)return Tb();a=a[F]("/");for(var b=Tb(),c=0,d=a[D];b&&
"object"===typeof b&&c<d;++c)b=b[a[c]];return c===a[D]&&void 0!==b?b:void 0},Yb=function(a,b){var c=a;if("string"===typeof a){for(var d=c={},e=a[F]("/"),f=0,g=e[D];f<g-1;++f)var h={},d=d[e[f]]=h;d[e[f]]=b}Xb(c)};var Zb=function(){var a=k.__GOOGLEAPIS;a&&(a.googleapis&&!a["googleapis.config"]&&(a["googleapis.config"]=a.googleapis),S(Y,"ci",[])[r](a),k.__GOOGLEAPIS=void 0)};var $b=k.console,ac=function(a){$b&&$b.log&&$b.log(a)};var bc=function(){return!!Y.oa},cc=function(){};var $=S(Y,"rw",T()),dc=function(a){for(var b in $)a($[b])},ec=function(a,b){var c=$[a];c&&c.state<b&&(c.state=b)};var fc;var gc=/^https?:\/\/(?:\w|[\-\.])+\.google\.(?:\w|[\-:\.])+(?:\/[^\?\#]*)?\/u\/(\d)\//,hc=/^https?:\/\/(?:\w|[\-\.])+\.google\.(?:\w|[\-:\.])+(?:\/[^\?\#]*)?\/b\/(\d{10,})\//,ic=function(a){var b=Z("googleapis.config/sessionIndex");null==b&&(b=k.__X_GOOG_AUTHUSER);if(null==b){var c=k.google;c&&(b=c.authuser)}null==b&&(a=a||k[G][L],b=X(a,"authuser")||null,null==b&&(b=(b=a[qa](gc))?b[1]:null));return null==b?null:q(b)},jc=function(a){var b=Z("googleapis.config/sessionDelegate");null==b&&(b=(a=(a||k[G][L])[qa](hc))?
a[1]:null);return null==b?null:q(b)};var kc=function(){};var lc=function(){this.b=[];this.n=[];this.N=[];this.k=[];this.k[0]=128;for(var a=1;64>a;++a)this.k[a]=0;this.reset()};(function(){function a(){}a.prototype=kc[E];lc.Z=kc[E];lc.prototype=new a})();lc[E].reset=function(){this.b[0]=1732584193;this.b[1]=4023233417;this.b[2]=2562383102;this.b[3]=271733878;this.b[4]=3285377520;this.o=this.i=0};
var mc=function(a,b,c){c||(c=0);var d=a.N;if("string"==typeof b)for(var e=0;16>e;e++)d[e]=b[K](c)<<24|b[K](c+1)<<16|b[K](c+2)<<8|b[K](c+3),c+=4;else for(e=0;16>e;e++)d[e]=b[c]<<24|b[c+1]<<16|b[c+2]<<8|b[c+3],c+=4;for(e=16;80>e;e++){var f=d[e-3]^d[e-8]^d[e-14]^d[e-16];d[e]=(f<<1|f>>>31)&4294967295}b=a.b[0];c=a.b[1];for(var g=a.b[2],h=a.b[3],l=a.b[4],v,e=0;80>e;e++)40>e?20>e?(f=h^c&(g^h),v=1518500249):(f=c^g^h,v=1859775393):60>e?(f=c&g|h&(c|g),v=2400959708):(f=c^g^h,v=3395469782),f=(b<<5|b>>>27)+f+
l+v+d[e]&4294967295,l=h,h=g,g=(c<<30|c>>>2)&4294967295,c=b,b=f;a.b[0]=a.b[0]+b&4294967295;a.b[1]=a.b[1]+c&4294967295;a.b[2]=a.b[2]+g&4294967295;a.b[3]=a.b[3]+h&4294967295;a.b[4]=a.b[4]+l&4294967295};lc[E].update=function(a,b){void 0===b&&(b=a[D]);for(var c=b-64,d=0,e=this.n,f=this.i;d<b;){if(0==f)for(;d<=c;)mc(this,a,d),d+=64;if("string"==typeof a)for(;d<b;){if(e[f]=a[K](d),++f,++d,64==f){mc(this,e);f=0;break}}else for(;d<b;)if(e[f]=a[d],++f,++d,64==f){mc(this,e);f=0;break}}this.i=f;this.o+=b};var nc=function(){this.p=new lc};nc[E].reset=function(){this.p.reset()};var uc=function(){var a;oc?(a=new Q.Uint32Array(1),pc.getRandomValues(a),a=Number("0."+a[0])):(a=qc,a+=ea(rc[B](0,20),16),rc=sc(rc),a/=tc+Math.pow(16,20));return a},pc=Q.crypto,oc=!1,vc=0,wc=0,qc=1,tc=0,rc="",xc=function(a){a=a||Q.event;var b=a.screenX+a.clientX<<16,b=b+(a.screenY+a.clientY),b=(new Date)[ua]()%1E6*b;qc=qc*b%tc;0<vc&&++wc==vc&&bb("mousemove",xc,"remove","de")},sc=function(a){var b=new nc;a=unescape(aa(a));for(var c=[],d=0,e=a[D];d<e;++d)c[r](a[K](d));b.p[Ba](c);a=b.p;b=[];d=8*a.o;
56>a.i?a[Ba](a.k,56-a.i):a[Ba](a.k,64-(a.i-56));for(c=63;56<=c;c--)a.n[c]=d&255,d/=256;mc(a,a.n);for(c=d=0;5>c;c++)for(e=24;0<=e;e-=8)b[d]=a.b[c]>>e&255,++d;a="";for(c=0;c<b[D];c++)a+="0123456789ABCDEF"[na](Math.floor(b[c]/16))+"0123456789ABCDEF"[na](b[c]%16);return a},oc=!!pc&&"function"==typeof pc.getRandomValues;oc||(tc=1E6*(screen[ka]*screen[ka]+screen[Ca]),rc=sc(R.cookie+"|"+R[G]+"|"+(new Date)[ua]()+"|"+Math.random()),vc=Z("random/maxObserveMousemove")||0,0!=vc&&bb("mousemove",xc,"add","at"));var yc=function(){var a=Y.onl;if(!a){a=T();Y.onl=a;var b=T();a.e=function(a){var d=b[a];d&&(delete b[a],d())};a.a=function(a,d){b[a]=d};a.r=function(a){delete b[a]}}return a},zc=function(a,b){var c=b.onload;return"function"===typeof c?(yc().a(a,c),c):null},Ac=function(a){Ua(/^\w+$/[s](a),"Unsupported id - "+a);yc();return'onload="window.___jsl.onl.e(&#34;'+a+'&#34;)"'},Bc=function(a){yc().r(a)};var Cc={allowtransparency:"true",frameborder:"0",hspace:"0",marginheight:"0",marginwidth:"0",scrolling:"no",style:"",tabindex:"0",vspace:"0",width:"100%"},Dc={allowtransparency:!0,onload:!0},Ec=0,Fc=function(a){Ua(!a||ab[s](a),"Illegal url for new iframe - "+a)},Gc=function(a,b,c,d,e){Fc(c.src);var f,g=zc(d,c),h=g?Ac(d):"";try{f=a[z]('<iframe frameborder="'+Ra(q(c.frameborder))+'" scrolling="'+Ra(q(c.scrolling))+'" '+h+' name="'+Ra(q(c.name))+'"/>')}catch(l){f=a[z]("iframe"),g&&(f.onload=function(){f.onload=
null;g[I](this)},Bc(d))}for(var v in c)a=c[v],"style"===v&&"object"===typeof a?V(a,f[H]):Dc[v]||f[A](v,q(a));(v=e&&e.beforeNode||null)||e&&e.dontclear||fb(b);b.insertBefore(f,v);f=v?v.previousSibling:b.lastChild;c.allowtransparency&&(f.allowTransparency=!0);return f};var Hc=/^:[\w]+$/,Ic=/:([a-zA-Z_]+):/g,Jc=function(a,b){if(!fc||Z("oauth-flow/authAware")){var c=ic()||"0",d=jc(),e;e=ic(void 0)||c;var f=jc(void 0),g="";e&&(g+="u/"+e+"/");f&&(g+="b/"+f+"/");e=g||null;f=Z("oauth-flow/authAware")?"isLoggedIn":"googleapis.config/signedIn";(g=(f=!1===Z(f))?"_/im/":"")&&(e="");var h=Z("iframes/:socialhost:"),l=Z("iframes/:im_socialhost:");fc={socialhost:h,ctx_socialhost:f?l:h,session_index:c,session_delegate:d,session_prefix:e,im_prefix:g}}return fc[b]||""};var Kc={"\b":"\\b","\t":"\\t","\n":"\\n","\f":"\\f","\r":"\\r",'"':'\\"',"\\":"\\\\"},Lc=function(a){var b,c,d;b=/[\"\\\x00-\x1f\x7f-\x9f]/g;if(void 0!==a){switch(typeof a){case "string":return b[s](a)?'"'+a[u](b,function(a){var b=Kc[a];if(b)return b;b=a[K]();return"\\u00"+Math.floor(b/16)[C](16)+(b%16)[C](16)})+'"':'"'+a+'"';case "number":return isFinite(a)?q(a):"null";case "boolean":case "null":return q(a);case "object":if(!a)return"null";b=[];if("number"===typeof a[D]&&!a.propertyIsEnumerable("length")){d=
a[D];for(c=0;c<d;c+=1)b[r](Lc(a[c])||"null");return"["+b[P](",")+"]"}for(c in a)!/___$/[s](c)&&a[wa](c)&&"string"===typeof c&&(d=Lc(a[c]))&&b[r](Lc(c)+":"+d);return"{"+b[P](",")+"}"}return""}},Mc=function(a){if(!a)return!1;if(/^[\],:{}\s]*$/[s](a[u](/\\["\\\/b-u]/g,"@")[u](/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g,"]")[u](/(?:^|:|,)(?:\s*\[)+/g,"")))try{return eval("("+a+")")}catch(b){}return!1},Nc=!!k[oa]&&!!k[oa].parse&&!!k[oa].stringify,Oc=function(a){try{return k[oa].parse(a)}catch(b){return!1}},
Pc=Nc?k[oa].stringify:Lc,Qc=Nc?Oc:Mc;var Rc=function(a){var b;a[qa](/^https?%3A/i)&&(b=fa(a));return $a(p,b?b:a)},Sc=function(a){a=a||"canonical";for(var b=p[va]("link"),c=0,d=b[D];c<d;c++){var e=b[c],f=e[J]("rel");if(f&&f[Da]()==a&&(e=e[J]("href"))&&(e=Rc(e))&&null!=e[qa](/^https?:\/\/[\w\-\_\.]+/i))return e}return k[G][L]};var Tc={callback:-1,clientid:1,cookiepolicy:1,openidrealm:-1,requestvisibleactions:1,scope:1},Uc=!1,Vc=T();var Wc={post:!0},Xc={style:"position:absolute;top:-10000px;width:450px;margin:0px;border-style:none"},Yc="onPlusOne _ready _close _open _resizeMe _renderstart oncircled drefresh erefresh".split(" "),Zc=S(Y,"WI",T()),$c=function(a,b,c){var d,e;d=e=a;"plus"==a&&b[za]&&(e=a+"_"+b[za],d=a+"/"+b[za]);(e=Z("iframes/"+e+"/url"))||(e=":socialhost:/:session_prefix:_/widget/render/"+d);d=$a(R,e[u](Ic,Jc));var f="iframes/"+a+"/params/";e={};V(b,e);var g=Z("lang")||Z("gwidget/lang");g&&(e.hl=g);Wc[a]||(e.origin=
k[G].origin||k[G][ya]+"//"+k[G].host);e.exp=Z(f+"exp");if(f=Z(f+"location"))for(g=0;g<f[D];g++){var h=f[g];e[h]=Q[G][h]}switch(a){case "plus":case "follow":f=e[L];g=b[za]?void 0:"publisher";f=(f="string"==typeof f?f:void 0)?Rc(f):Sc(g);e.url=f;delete e[L];break;case "plusone":e.url=b[L]?Rc(b[L]):Sc();f=b.db;g=Z();null==f&&g&&(f=g.db,null==f&&(f=g.gwidget&&g.gwidget.db));e.db=f||void 0;f=b.ecp;g=Z();null==f&&g&&(f=g.ecp,null==f&&(f=g.gwidget&&g.gwidget.ecp));e.ecp=f||void 0;delete e[L];break;case "signin":e.url=
Sc()}Y.ILI&&(e.iloader="1");delete e["data-onload"];delete e.rd;e.gsrc=Z("iframes/:source:");f=Z("inline/css");"undefined"!==typeof f&&0<c&&f>=c&&(e.ic="1");f=/^#|^fr-/;c={};for(var l in e)U(e,l)&&f[s](l)&&(c[l[u](f,"")]=e[l],delete e[l]);l="q"==Z("iframes/"+a+"/params/si")?e:c;if(!Uc){f=p[va]("meta");for(g=0;g<f[D];++g)if(h=f[g].name[Da](),0==h.lastIndexOf("google-signin-",0)){var h=h[M](14),v=f[g].content;Tc[h]&&v&&(Vc[h]=v)}if(k.self!==k.top){var f=p[G][C](),m;for(m in Tc)0<Tc[m]&&(g=X(f,m,""))&&
(Vc[m]=g)}Uc=!0}m=T();V(Vc,m);for(var n in m)!U(m,n)||U(e,n)||U(c,n)||(l[n]=m[n]);n=[][ma](Yc);(m=Z("iframes/"+a+"/methods"))&&"object"===typeof m&&Ja[s](m[r])&&(n=n[ma](m));for(var w in b)U(b,w)&&/^on/[s](w)&&("plus"!=a||"onconnect"!=w)&&(n[r](w),delete e[w]);delete e.callback;c._methods=n[P](",");return Za(d,e,c)},ad=["style","data-gapiscan"],cd=function(a){for(var b=T(),c=0!=a[pa][Da]()[y]("g:"),d=0,e=a[Aa][D];d<e;d++){var f=a[Aa][d],g=f.name,h=f.value;0<=Ka[I](ad,g)||c&&0!=g[y]("data-")||"null"===
h||"specified"in f&&!f.specified||(c&&(g=g[B](5)),b[g[Da]()]=h)}a=a[H];(c=bd(a&&a[Ca]))&&(b.height=q(c));(a=bd(a&&a[ka]))&&(b.width=q(a));return b},bd=function(a){var b=void 0;"number"===typeof a?b=a:"string"===typeof a&&(b=ea(a,10));return b},ed=function(){var a=Y.drw;dc(function(b){if(a!==b.id&&4!=b.state){var c=b.id,d=b[sa],e=b.url;b=b.userParams;var f=R[la](c);if(f){var g=$c(d,b,0);g?(f=f[O],e[u](/\#.*/,"")[u](/(\?|&)ic=1/,"")!==g[u](/\#.*/,"")[u](/(\?|&)ic=1/,"")&&(b.dontclear=!0,b.rd=!0,b.ri=
!0,ga(b,d),dd(f,b),(d=$[f.lastChild.id])&&(d.oid=c),ec(c,4))):delete $[c]}else delete $[c]}})},fd=function(){};var gd,hd,id,jd,kd,ld=/(?:^|\s)g-((\S)*)(?:$|\s)/,md={plusone:!0,autocomplete:!0,profile:!0};gd=S(Y,"SW",T());hd=S(Y,"SA",T());id=S(Y,"SM",T());jd=S(Y,"FW",[]);kd=null;
var od=function(a,b){nd(void 0,!1,a,b)},nd=function(a,b,c,d){qb("ps0",!0);c=("string"===typeof c?p[la](c):c)||R;var e;e=R.documentMode;if(c.querySelectorAll&&(!e||8<e)){e=d?[d]:Sa(gd)[ma](Sa(hd))[ma](Sa(id));for(var f=[],g=0;g<e[D];g++){var h=e[g];f[r](".g-"+h,"g\\:"+h)}e=c.querySelectorAll(f[P](","))}else e=c[va]("*");c=T();for(f=0;f<e[D];f++){g=e[f];var l=g,h=d,v=l[pa][Da](),m=void 0;l[J]("data-gapiscan")?h=null:(0==v[y]("g:")?m=v[B](2):(l=(l=q(l.className||l[J]("class")))&&ld[ja](l))&&(m=l[1]),
h=!m||!(gd[m]||hd[m]||id[m])||h&&m!==h?null:m);h&&(md[h]||0==g[pa][Da]()[y]("g:")||0!=Sa(cd(g))[D])&&(g[A]("data-gapiscan",!0),S(c,h,[])[r](g))}if(b)for(var n in c)for(b=c[n],d=0;d<b[D];d++)b[d][A]("data-onload",!0);for(var w in c)jd[r](w);qb("ps1",!0);if((n=jd[P](":"))||a)try{W.load(n,a)}catch(t){ac(t);return}if(pd(kd||{}))for(var x in c){a=c[x];w=0;for(b=a[D];w<b;w++)a[w].removeAttribute("data-gapiscan");qd(x)}else{d=[];for(x in c)for(a=c[x],w=0,b=a[D];w<b;w++)e=a[w],rd(x,e,cd(e),d,b);sd(n,d)}},
td=function(a){var b=S(W,a,{});b.go||(b.go=function(b){return od(b,a)},b.render=function(b,d){var e=d||{};ga(e,a);return dd(b,e)})},ud=function(a){gd[a]=!0},vd=function(a){hd[a]=!0},wd=function(a){id[a]=!0};var qd=function(a,b){var c=S(Y,"watt",T())[a];b&&c?(c(b),(c=b.iframeNode)&&c[A]("data-gapiattached",!0)):W.load(a,function(){var c=S(Y,"watt",T())[a],e=b&&b.iframeNode;e&&c?(c(b),e[A]("data-gapiattached",!0)):(0,W[a].go)(e&&e[O])})},pd=function(){return!1},sd=function(){},rd=function(a,b,c,d,e,f){switch(xd(b,a,f)){case 0:a=id[a]?a+"_annotation":a;d={};d.iframeNode=b;d.userParams=c;qd(a,d);break;case 1:var g;if(b[O]){for(var h in c)if(U(c,h)&&c[h]&&"object"===typeof c[h]&&(!c[h][C]||c[h][C]===ba[E][C]||
c[h][C]===da[E][C]))try{var l=Pc(c[h]);600>l[D]?c[h]=l:delete c[h]}catch(v){delete c[h]}l=!0;c.dontclear&&(l=!1);delete c.dontclear;cc();h=$c(a,c,e);f={allowPost:1,attributes:Xc};f.dontclear=!l;e={};e.userParams=c;e.url=h;ga(e,a);var m;c.rd?m=b:(m=p[z]("div"),b[A]("data-gapistub",!0),m[H].cssText="position:absolute;width:450px;left:-10000px;",b[O].insertBefore(m,b));e.siteElement=m;m.id||(b=m,S(Zc,a,0),l="___"+a+"_"+Zc[a]++,b.id=l);b=T();b[">type"]=a;V(c,b);l=h;c=m;h=f||{};b=h[Aa]||{};Ua(!h.allowPost||
!b.onload,"onload is not supported by post iframe");f=b=l;Hc[s](b)&&(f=Z("iframes/"+f[M](1)+"/url"),Ua(!!f,"Unknown iframe url config for - "+b));l=$a(R,f[u](Ic,Jc));b=c.ownerDocument||R;m=0;do f=h.id||["I",Ec++,"_",(new Date)[ua]()][P]("");while(b[la](f)&&5>++m);Ua(5>m,"Error creating iframe id");m={};var n={};V(h.queryParams||{},m);V(h.fragmentParams||{},n);var w=h.pfname,t=T();t.id=f;t.parent=b[G][ya]+"//"+b[G].host;var x=X(b[G][L],"parent"),w=w||"";!w&&x&&(x=X(b[G][L],"id",""),w=X(b[G][L],"pfname",
""),w=x?w+"/"+x:"");t.pfname=w;V(t,n);(t=X(l,"rpctoken")||m.rpctoken||n.rpctoken)||(t=n.rpctoken=h.rpctoken||q(Math.round(1E8*uc())));h.rpctoken=t;x=b[G][L];t=T();(w=X(x,"_bsh",Y.bsh))&&(t._bsh=w);(x=hb(x))&&(t.jsh=x);h.hintInFragment?V(t,n):V(t,m);l=Za(l,m,n,h.paramsSerializer);n=T();V(Cc,n);V(h[Aa],n);n.name=n.id=f;n.src=l;h.eurl=l;if((h||{}).allowPost&&2E3<l[D]){m=Wa(l);n.src="";n["data-postorigin"]=l;l=Gc(b,c,n,f);-1!=navigator.userAgent[y]("WebKit")&&(g=l.contentWindow.document,g.open(),n=g[z]("div"),
t={},x=f+"_inner",t.name=x,t.src="",t.style="display:none",Gc(b,n,t,x,h));n=(h=m.g[0])?h[F]("&"):[];h=[];for(t=0;t<n[D];t++)x=n[t][F]("=",2),h[r]([fa(x[0]),fa(x[1])]);m.g=[];n=Xa(m);m=b[z]("form");m.action=n;m.method="POST";m.target=f;m[H].display="none";for(f=0;f<h[D];f++)n=b[z]("input"),ga(n,"hidden"),n.name=h[f][0],n.value=h[f][1],m[ha](n);c[ha](m);m.submit();m[O][xa](m);g&&g.close();g=l}else g=Gc(b,c,n,f,h);e.iframeNode=g;e.id=g[J]("id");g=e.id;c=T();c.id=g;c.userParams=e.userParams;c.url=e.url;
ga(c,e[sa]);c.state=1;$[g]=c;g=e}else g=null;g&&((e=g.id)&&d[r](e),qd(a,g))}},xd=function(a,b,c){if(a&&1===a.nodeType&&b){if(c)return 1;if(id[b]){if(gb[a[pa][Da]()])return(a=a.innerHTML)&&a[u](/^[\s\xa0]+|[\s\xa0]+$/g,"")?0:1}else{if(hd[b])return 0;if(gd[b])return 1}}return null},dd=function(a,b){var c=b[sa];delete b[sa];var d=("string"===typeof a?p[la](a):a)||void 0;if(d){var e={},f;for(f in b)U(b,f)&&(e[f[Da]()]=b[f]);e.rd=1;(f=!!e.ri)&&delete e.ri;var g=[];rd(c,d,e,g,0,f);sd(c,g)}else ac("string"===
"gapi."+c+".render: missing element "+typeof a?a:"")};S(W,"platform",{}).go=od;var pd=function(a){for(var b=["_c","jsl","h"],c=0;c<b[D]&&a;c++)a=a[b[c]];b=hb(Ha[L]);return!a||0!=a[y]("n;")&&0!=b[y]("n;")&&a!==b},sd=function(a,b){yd(a,b)},cb=function(a){nd(a,!0)},zd=function(a,b){for(var c=b||[],d=0;d<c[D];++d)a(c[d]);for(d=0;d<c[D];d++)td(c[d])};ub[r](["platform",function(a,b,c){kd=c;b&&jd[r](b);zd(ud,a);zd(vd,c._c.annotation);zd(wd,c._c.bimodal);Zb();Xb();if("explicit"!=Z("parsetags")){ib(a);var d;c&&(a=c.callback)&&(d=Ta(a),delete c.callback);eb(function(){cb(d)})}}]);W._pl=!0;var Ad=function(a){a=(a=$[a])?a.oid:void 0;if(a){var b=R[la](a);b&&b[O][xa](b);delete $[a];Ad(a)}},fd=function(a,b,c){if(c[ka]&&c[Ca]){n:{c=c||{};if(bc()){var d=b.id;if(d){var e;e=(e=$[d])?e.state:void 0;if(1===e||4===e)break n;Ad(d)}}(e=a.nextSibling)&&e[J]&&e[J]("data-gapistub")&&(a[O][xa](e),a[H].cssText="");e=c[ka];var f=c[Ca],g=a[H];g.textIndent="0";g.margin="0";g.padding="0";g.background="transparent";g.borderStyle="none";g.cssFloat="none";g.styleFloat="none";g.lineHeight="normal";g.fontSize=
"1px";g.verticalAlign="baseline";a=a[H];a.display="inline-block";g=b[H];g.position="static";g.left=0;g.top=0;g.visibility="visible";e&&(a.width=g.width=e+"px");f&&(a.height=g.height=f+"px");c.verticalAlign&&(a.verticalAlign=c.verticalAlign);d&&ec(d,3)}b["data-csi-wdt"]=(new Date)[ua]()}};var Bd=/^\{h\:'/,Cd=/^!_/,Dd="",yd=function(a,b){function c(){bb("message",d,"remove","de")}function d(d){var g=d.data,h=d.origin;if(Ed(g,b)){var l=e;e=!1;l&&qb("rqe");Fd(a,function(){l&&qb("rqd");c();for(var a=S(Y,"RPMQ",[]),b=0;b<a[D];b++)a[b]({data:g,origin:h})})}}if(0!==b[D]){Dd=X(Ha[L],"pfname","");var e=!0;bb("message",d,"add","at");Rb(a,c)}},Ed=function(a,b){a=q(a);if(Bd[s](a))return!0;var c=!1;Cd[s](a)&&(c=!0,a=a[B](2));if(!/^\{/[s](a))return!1;var d=Qc(a);if(!d)return!1;var e=d.f;if(d.s&&
e&&-1!=Ka[I](b,e)){if("_renderstart"===d.s||d.s===Dd+"/"+e+"::_renderstart")c=d.a&&d.a[c?0:1],d=R[la](e),ec(e,2),c&&d&&fd(d[O],d,c);return!0}return!1},Fd=function(a,b){Rb(a,b)};var Gd=function(a,b){this.A=a;var c=b||{};this.P=c.V;this.w=c.domain;this.B=c.path;this.Q=c.W},Hd=/^[-+/_=.:|%&a-zA-Z0-9@]*$/,Id=/^[A-Z_][A-Z0-9_]{0,63}$/;Gd[E].write=function(a,b){if(!Id[s](this.A))throw"Invalid cookie name";if(!Hd[s](a))throw"Invalid cookie value";var c=this.A+"="+a;this.w&&(c+=";domain="+this.w);this.B&&(c+=";path="+this.B);var d="number"===typeof b?b:this.P;if(0<=d){var e=new Date;e.setSeconds(e.getSeconds()+d);c+=";expires="+e.toUTCString()}this.Q&&(c+=";secure");p.cookie=c};
Gd.iterate=function(a){for(var b=p.cookie[F](/;\s*/),c=0;c<b[D];++c){var d=b[c][F]("="),e=d[ia]();a(e,d[P]("="))}};var Jd=function(a){this.T=a},Kd={};Jd[E].write=function(a){Kd[this.T]=a};Jd.iterate=function(a){for(var b in Kd)Kd[wa](b)&&a(b,Kd[b])};var Ld="https:"===k[G][ya],Md=Ld||"http:"===k[G][ya]?Gd:Jd,Nd=function(a){var b=a[B](1),c="",d=k[G].hostname;if(""!==b){c=ea(b,10);if(isNaN(c))return null;b=d[F](".");if(b[D]<c-1)return null;b[D]==c-1&&(d="."+d)}else d="";return{c:"S"==a[na](0),domain:d,d:c}},Od=function(a){if(0!==a[y]("GCSC"))return null;var b={v:!1};a=a[B](4);if(!a)return b;var c=a[na](0);a=a[B](1);var d=a.lastIndexOf("_");if(-1==d)return b;var e=Nd(a[B](d+1));if(null==e)return b;a=a[M](0,d);if("_"!==a[na](0))return b;d="E"===c&&
e.c;return!d&&("U"!==c||e.c)||d&&!Ld?b:{v:!0,c:d,U:a[B](1),domain:e.domain,d:e.d}},Pd=function(a){if(!a)return[];a=a[F]("=");return a[1]?a[1][F]("|"):[]},Qd=function(a){a=a[F](":");return{q:a[0][F]("=")[1],L:Pd(a[1]),Y:Pd(a[2]),X:Pd(a[3])}},Rd=function(){var a,b=null;Md.iterate(function(c,d){if(0===c[y]("G_AUTHUSER_")){var e=Nd(c[M](11));if(!a||e.c&&!a.c||e.c==a.c&&e.d>a.d)a=e,b=d}});if(null!==b){var c;Md.iterate(function(b,d){var e=Od(b);e&&e.v&&e.c==a.c&&e.d==a.d&&(c=d)});if(c){var d=Qd(c),e=d&&
d.L[Number(b)],d=d&&d.q;if(e)return{M:b,O:e,q:d}}}return null};var Sd=function(a){this.F=a};Sd[E].j=0;Sd[E].D=2;Sd[E].F=null;Sd[E].t=!1;Sd[E].J=function(){this.t||(this.j=0,this.t=!0,this.C())};Sd[E].C=function(){this.t&&(this.F()?this.j=this.D:this.j=Math.min(2*(this.j||this.D),120),k.setTimeout(Ga(this.C,this),1E3*this.j))};for(var Td=0;64>Td;++Td);var Ud=null,bc=function(){return Y.oa=!0},cc=function(){Y.oa=!0;var a=Rd();(a=a&&a.M)&&Yb("googleapis.config/sessionIndex",a);Ud||(Ud=S(Y,"ss",new Sd(Vd)));a=Ud;a.J&&a.J()},Vd=function(){var a=Rd(),b=a&&a.O||null,c=a&&a.q;Rb("auth",{callback:function(){var a={client_id:c,session_state:b};Q.gapi.auth.checkSessionState(a,function(b){var c=a.session_state,g=Z("isLoggedIn");b=c&&b||!c&&!b;g!=b&&(Yb("isLoggedIn",b),cc(),ed())})}});return!0};qb("bs0",!0,k.gapi._bs);qb("bs1",!0);delete k.gapi._bs;})();
gapi.load("plusone",{callback:window["gapi_onload"],_c:{"jsl":{"ci":{"client":{"cors":false},"plus_layer":{"isEnabled":false},"enableMultilogin":false,"isLoggedIn":false,"iframes":{"additnow":{"methods":["launchurl"],"url":"https://apis.google.com/additnow/additnow.html?bsv\u003do"},"person":{"url":":socialhost:/:session_prefix:_/widget/render/person?usegapi\u003d1\u0026bsv\u003do"},"plus_followers":{"params":{"url":""},"url":":socialhost:/_/im/_/widget/render/plus/followers?usegapi\u003d1\u0026bsv\u003do"},"signin":{"methods":["onauth"],"params":{"url":""},"url":":socialhost:/:session_prefix:_/widget/render/signin?bsv\u003do"},"commentcount":{"url":":socialhost:/:session_prefix:_/widget/render/commentcount?bsv\u003do"},"page":{"url":":socialhost:/:session_prefix:_/widget/render/page?usegapi\u003d1\u0026bsv\u003do"},"hangout":{"url":"https://talkgadget.google.com/:session_prefix:talkgadget/_/widget?bsv\u003do"},"plus_circle":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/widget/plus/circle?usegapi\u003d1\u0026bsv\u003do"},"card":{"url":":socialhost:/:session_prefix:_/hovercard/card?bsv\u003do"},"evwidget":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/events/widget?bsv\u003do"},"zoomableimage":{"url":"https://ssl.gstatic.com/microscope/embed/?bsv\u003do"},"follow":{"url":":socialhost:/:session_prefix:_/widget/render/follow?usegapi\u003d1\u0026bsv\u003do"},"shortlists":{"url":"?bsv\u003do"},"plus":{"methods":["onauth"],"url":":socialhost:/u/:session_index:/_/pages/badge?usegapi\u003d1\u0026bsv\u003do"},":socialhost:":"https://apis.google.com","post":{"params":{"url":""},"url":":socialhost:/:im_prefix:_/widget/render/post?bsv\u003do"},"community":{"url":":socialhost:/:session_prefix:_/widget/render/community?usegapi\u003d1\u0026bsv\u003do"},"rbr_s":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/widget/render/recobarsimplescroller?bsv\u003do"},"autocomplete":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/widget/render/autocomplete?bsv\u003do"},"plus_share":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/+1/sharebutton?plusShare\u003dtrue\u0026usegapi\u003d1\u0026bsv\u003do"},":source:":"3p","savetowallet":{"url":"https://clients5.google.com/s2w/o/savetowallet?bsv\u003do"},"panoembed":{"url":"https://ssl.gstatic.com/pano/embed/?bsv\u003do"},"rbr_i":{"params":{"url":""},"url":":socialhost:/:session_prefix:_/widget/render/recobarinvitation?bsv\u003do"},"appcirclepicker":{"url":":socialhost:/:session_prefix:_/widget/render/appcirclepicker?bsv\u003do"},":im_socialhost:":"https://plus.googleapis.com","savetodrive":{"methods":["save"],"url":"https://drive.google.com/savetodrivebutton?usegapi\u003d1\u0026bsv\u003do"},":signuphost:":"https://plus.google.com","plusone":{"preloadUrl":["https://ssl.gstatic.com/s2/oz/images/stars/po/Publisher/sprite4-a67f741843ffc4220554c34bd01bb0bb.png"],"params":{"count":"","size":"","url":""},"url":":socialhost:/:session_prefix:_/+1/fastbutton?usegapi\u003d1\u0026bsv\u003do"},"comments":{"methods":["scroll","openwindow"],"params":{"location":["search","hash"]},"url":":socialhost:/:session_prefix:_/widget/render/comments?bsv\u003do"},"ytsubscribe":{"url":"https://www.youtube.com/subscribe_embed?usegapi\u003d1\u0026bsv\u003do"}},"isPlusUser":false,"debug":{"host":"https://apis.google.com","reportExceptionRate":0.05,"rethrowException":false},"deviceType":"desktop","inline":{"css":1},"lexps":[102,98,99,79,109,45,17,118,115,81,95,122,61,30],"oauth-flow":{"improveToastUi":false,"authAware":true,"usegapi":false,"disableOpt":true,"authUrl":"https://accounts.google.com/o/oauth2/auth","proxyUrl":"https://accounts.google.com/o/oauth2/postmessageRelay","toastCfg":"1000:3000:1000"},"report":{"host":"https://apis.google.com","rate":0.001,"apis":["iframes\\..*","gadgets\\..*","gapi\\.appcirclepicker\\.*","gapi\\.client\\..*"]},"csi":{"rate":0.01},"googleapis.config":{}},"h":"m;/_/scs/apps-static/_/js/k\u003doz.gapi.zh_TW.4SW1XZPfHis.O/m\u003d__features__/am\u003dAQ/rt\u003dj/d\u003d1/rs\u003dAItRSTPXCMA16NeWbzQJ4lDEFRw6jGm0jg","u":"https://apis.google.com/js/plusone.js","hee":true,"fp":"4e1326f06547e8b3a0f4bd7c5f21e2390bde065f","dpo":false},"platform":["additnow","comments","commentcount","community","follow","page","panoembed","person","plus","plusone","post","savetodrive","shortlists","ytsubscribe","zoomableimage","savetowallet","hangout"],"fp":"4e1326f06547e8b3a0f4bd7c5f21e2390bde065f","annotation":["interactivepost","recobar","autocomplete","profile"],"bimodal":["signin"]}});