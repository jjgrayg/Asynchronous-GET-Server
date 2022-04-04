/*
    
    Main file for initializing and running the server

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/
    
#include "server.h"

int main(int, char**) {
    Server srv(80, true, false);
    srv.run();
	return 0;
}