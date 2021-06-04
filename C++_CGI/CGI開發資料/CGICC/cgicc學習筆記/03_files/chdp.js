function chdp(objid){//change display
            var obj=eval(objid);          
            if (obj.style.display == "none"){
                obj.style.display='block'; }             
            else{
                obj.style.display='none';}              
        }