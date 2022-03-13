window.onload = function() {
    var id = null;
    var elem = document.getElementById("banner-text");
    var opacity = 0;
    clearInterval(id);
    id = setInterval(frame, 5);
    function frame() {
        if (opacity >= 1) {
        clearInterval(id);
        } else {
        opacity += .01;
        elem.style.opacity = opacity;
        }
    }
}