// var opacity = 0;
// window.onload = function() {
//     var id = null;
//     var banner = document.getElementById("banner-text");
//     var source = document.getElementById("source-tutorial");
//     var right_click = document.getElementById("right-click");
//     var right_click_img = document.getElementById("right-click-img");
//     var source_page = document.getElementById("source-page");
//     var source_page_img = document.getElementById("source-page-img");
//     var description = document.getElementById("description");
//     var offset = 350;
//     clearInterval(id);
//     id = setInterval(frame, 5);
//     function frame() {
//         if (opacity >= 1) {
//             clearInterval(id);
//         } else {
//             opacity += .01;
//             offset -= 3.5;
//             banner.style.opacity = opacity;
//             source.style.opacity = opacity;
//             right_click.style.left = offset + "px";
//             source_page_img.style.left = offset + "px";
//             right_click_img.style.right = offset + "px";
//             source_page.style.right = offset + "px";
//             description.style.marginTop = offset + "px";
//         }
//     }
// }