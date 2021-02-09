//
// Adds some text where the cursor is.
//
// Works in IE and Mozilla 1.3b+
// In other browsers, it simply adds the text at the end of the current text
//
function addText( input, insText ) 
{
	input.focus();
	if( input.createTextRange ) {
		document.selection.createRange().text += insText;
 	} 
 	else if( input.setSelectionRange ) {
		var len = input.selectionEnd;
   		input.value = input.value.substr( 0, len ) + insText + input.value.substr( len );
   		input.setSelectionRange(len+insText.length,len+insText.length);
 	} 
 	else { 
	 	input.value += insText; 
	}
}

//
// Wraps the current selection of a textbox around with the given 'v' parameter.
// 
// Works in IE and Mozilla 1.3b+
// In other browsers, it simply adds an empty pair of tags at the end of the current
// text.
//
function wrapSelection(ta, v) 
{
	if( document.selection ) {
		// for IE
		var str = document.selection.createRange().text;
		ta.focus();
		var sel = document.selection.createRange();
		sel.text = "<" + v + ">" + str + "</" + v + ">";
    }
	else {
		// browsers other than IE
		var s = ta;
		if( s.selectionEnd ) {
			// Mozilla 1.3b+ 
			var s1 = (s.value).substring(0,s.selectionStart)
			var s2 = (s.value).substring(s.selectionEnd,s.textLength)
			selection = (s.value).substring(s.selectionStart, s.selectionEnd)
			s.value = s1 + '<' + v + '>' + selection + '</' + v + '>' + s2
		}
		else {
			// everything else
			s.value += '<' + v + '></' + v + '>';
		}
	}
	
	return;
}

//
// Asks for a url and adds a link to the textbox, at the same point where the cursor is.
//
// Uses insertText to insert the text so it has the same limitations as insertText has
//
function insertLink(ta) {
	var s  = ta;
	var my_link = prompt('Enter URL:', 'http://')
	if (my_link != null) {
		addText(ta, my_link);
	}
	
	return;
}    

////////////////
/// buttons
///////////////
function mouseover(el) {
  el.className = "button_raised";
}

function mouseout(el) {
  el.className = "button";
}

function mousedown(el) {
  el.className = "button_pressed";
}

function mouseup(el) {
  el.className = "button_raised";
}