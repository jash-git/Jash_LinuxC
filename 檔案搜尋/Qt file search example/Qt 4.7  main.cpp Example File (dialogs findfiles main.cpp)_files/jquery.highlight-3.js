/*

highlight v3

Highlights arbitrary terms.

<http://johannburkard.de/blog/programming/javascript/highlight-javascript-text-higlighting-jquery-plugin.html>

MIT license.

Johann Burkard
<http://johannburkard.de>
<mailto:jb@eaio.com>

*/

jQuery.fn.highlight = function(pat) {
 function innerHighlight(node, pat, re) {
  var skip = 0;
  if (node.nodeType == 3) {
   var pos = node.data.toUpperCase().search(re);
   if (pos >= 0) {
    var spannode = document.createElement('span');
    spannode.className = 'highlight';
    var middlebit = node.splitText(pos);
    var endbit = middlebit.splitText(pat.length);
    var middleclone = middlebit.cloneNode(true);
    spannode.appendChild(middleclone);
    middlebit.parentNode.replaceChild(spannode, middlebit);
    skip = 1;
   }
  }
  else if (node.nodeType == 1 && node.childNodes && !/(script|style)/i.test(node.tagName)) {
   for (var i = 0; i < node.childNodes.length; ++i) {
       i += innerHighlight(node.childNodes[i], pat, re);
   }
  }
  return skip;
 }
 return this.each(function() {
  var p = $.trim(pat).toUpperCase(), p1;
  if (/^(['"]).*\1$/.test(p)) {// || !p.match("\\s+")) {
    p = p.replace(/^([\'\"])(.*)\1$/g , "$2");
    p1 = p.replace(/\W/, "\\W"); //highlight a bit closer to the actual search
    var regex = new RegExp("\\b"+p1+"\\b");
    innerHighlight(this, p, regex);
  } else {
    var words = p.split(" ");
    for (var i = 0; i < words.length; i++) {
      if (words[i].length > 0) {
        var regex = new RegExp("\\b"+words[i]+"\\b");
        innerHighlight(this, words[i], regex);
      }
    }
  }
 });
};

jQuery.fn.removeHighlight = function() {
 return this.find("span.highlight").each(function() {
  this.parentNode.firstChild.nodeName;
  with (this.parentNode) {
   replaceChild(this.firstChild, this);
   normalize();
  }
 }).end();
};
