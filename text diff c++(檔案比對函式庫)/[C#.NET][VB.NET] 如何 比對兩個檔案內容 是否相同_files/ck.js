

//������J
setTimeout('InitCKEditor()',700);

function InitCKEditor() {

    //��X�����Ҧ��� TextArea
    var textarea = document.getElementsByTagName('textarea');

    if(location.href.indexOf('EditPosts.aspx') != -1)
    {
        //�Y�b�s��峹�������A��ck�]���M�峹���D�P�e
        var txt = document.getElementById('ctl00_pageContent_Editor_Edit_txbTitle');
    
        _width = (txt.style.width == '') ? txt.offsetWidth : txt.style.width;       
    }
    
    for(var i = 0; i < textarea.length;i++)
    {      
        if (location.href.indexOf('EditPosts.aspx') != -1) {

            CKEDITOR.replace(textarea[i].id,
            {
                toolbar: _toolbarset,
                width:  _width            
            });           
           
            //�s��峹�������A�u�M�Ĥ@�� TextArea
            break;
        }
        else {

            if (textarea[i].className.indexOf('customcss') != -1)
                continue;

            CKEDITOR.replace(textarea[i].id,
            {
                toolbar: _toolbarset,
                //width:  _width
                width: textarea[i].style.width
            });
        }
    }   
}
  