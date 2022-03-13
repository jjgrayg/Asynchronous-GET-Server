window.onload = function() {
    var id = null;
    var elem = document.getElementById("banner-text");
    var opacity = 0;
    clearInterval(id);
    id = setInterval(frame, 5);
    function frame() {
        if (opacity == 1) {
        clearInterval(id);
        } else {
        opacity += .01;
        elem.style.opacity = opacity;
        }
    }
    
    accordianItems = Array.from(document.getElementById("accordian").querySelectorAll(".accordian-item"));
    activeItem = document.getElementById("1");

    accordianItems.forEach( function(item) {
        //Get the anchor tag
        var anchor = Array.from(item.getElementsByTagName("a"))
        anchor[0].addEventListener('click', function() {
            
            if (!item.classList.contains('currently-viewed')) {
                //Open accordian entry for clicked item
                item.classList.add('currently-viewed');
                var text = item.getElementsByClassName('not-viewed')
                text[0].classList.remove('not-viewed');

                //Hide the actively viewed
                if (activeItem != undefined) {
                    activeItem.classList.remove('currently-viewed');
                    var activeText = activeItem.getElementsByClassName('text');
                    activeText[0].classList.add('not-viewed');
                }

                //Change actively viewed
                activeItem = item;
            } else {
                item.classList.remove('currently-viewed');
                var text = item.getElementsByClassName('text');
                text[0].classList.add('not-viewed');
                activeItem = undefined;
            }
        })
    })
}