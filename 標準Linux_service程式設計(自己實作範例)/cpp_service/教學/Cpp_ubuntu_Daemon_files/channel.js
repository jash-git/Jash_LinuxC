$(document).ready(function(){

//顶部导航鼠标滑动效果
$('.X_main_top_js .s1').hover(
		 function(){
			 $(this).find('a').attr('class','hot');
			 $(this).find('ul').show();
			 }
			 ,
		function(){
			 $(this).find('a').removeAttr('class');
			 $(this).find('ul').hide();
			 }
		 )

});