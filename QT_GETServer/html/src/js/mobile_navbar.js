function openMenu() {
    var linkMenu = document.getElementById("link-menu");
    if (linkMenu.style.display !== "flex") {
        linkMenu.style.display = "flex";  
        slideIn();
    } else {
        slideOut();
    }
}

function closeMenu() {
    slideOut();
}

function slideIn() {
    var linkMenu = document.getElementById("link-menu");
    var margin = linkMenu.style.marginLeft;
    margin = parseInt(margin.slice(0, -2));
    var menu = null; 
    clearInterval(menu);
    menu = setInterval(frame, 5);
    function frame() {
        if (margin === 0) {
            clearInterval(menu);
        } else {
            margin += 10;
            linkMenu.style.marginLeft = `${margin}px`;
        }
    }
}

function slideOut() {
    var linkMenu = document.getElementById("link-menu");
    var margin = linkMenu.style.marginLeft;
    margin = parseInt(margin.slice(0, -2));
    var menu = null; 
    clearInterval(menu);
    menu = setInterval(frame, 5);
    function frame() {
        if (margin === -700) {
            linkMenu.style.display = "none";
            clearInterval(menu);
        } else {
            margin -= 10;
            linkMenu.style.marginLeft = `${margin}px`;
        }
    }
}