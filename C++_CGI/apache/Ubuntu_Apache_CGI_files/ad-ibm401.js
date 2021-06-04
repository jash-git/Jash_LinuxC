//down 11.29
var cloudad_type = 'ibm401';
var cloudad_urls = [
'http://ad.csdn.net/adsrc/ibm-caq4-leadership-downloadhomepage-728-90-1030.swf'
];
var cloudad_clks = [
'http://e.cn.miaozhen.com/r.gif?k=1003502&p=3y2bl0&rt=2&o=http://www.ibm.com/midmarket/cn/zh/papaginos.shtml?csr=apch_cyl4_20121017_1350444164877&ck=csdn&cmp=245tg&ct=245tg12w&cr=csdn&cm=b&csot=-&ccy=cn&cpb=-&cd=2012-10-16&cot=a&cpg=lits&cn=mm_bao(papa_gino)&csz=728*90'
];

var can_swf = (function () {
    if (document.all) swf = new ActiveXObject('ShockwaveFlash.ShockwaveFlash');
    else if (navigator.plugins) swf = navigator.plugins["Shockwave Flash"];
    return !!swf;
})();

function cloudad_show() {
    var rd = Math.random();
    var ad_url, log_url;
    if (rd < 1 && can_swf) {
        ad_url = cloudad_urls[0];

        log_url = 'http://ad.csdn.net/log.ashx';
        log_url += '?t=view&adtype=' + cloudad_type + '&adurl=' + encodeURIComponent(ad_url);
        cloudad_doRequest(log_url, true);
    }
    if (rd < 0.003) {
        ad_url = cloudad_clks[0];

        log_url = 'http://ad.csdn.net/log.ashx';
        log_url += '?t=click&adtype=' + cloudad_type + '&adurl=' + encodeURIComponent(ad_url);
        cloudad_doRequest(log_url, true);
    }
}

function cloudad_doRequest(url, useFrm) {
    var e = document.createElement(useFrm ? "iframe" : "img");

    e.style.width = "1px";
    e.style.height = "1px";
    e.style.position = "absolute";
    e.style.visibility = "hidden";

    if (url.indexOf('?') > 0) url += '&r_m=';
    else url += '?r_m=';
    url += new Date().getMilliseconds();
    e.src = url;

    document.body.appendChild(e);
}

setTimeout(function () {
    cloudad_show();
}, 1000);
