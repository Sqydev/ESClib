- Use %zu instead of %lu becouse on alphine for example size_t is unsigned int not unsigned long so %lu breakes for alphine and isn't portable but %zu is good and portable becouse why not

- We DO NOT exit the program. NEVER

- We log like this: {
    "[ESCLIB.FUNCTIONNAME]: LOG_LEVEL: Message", so for example: "[ESCLIB.InitTui]: INFO: Detected Wayland"

    Which log level is which: {
        LOG_ERROR - Used in non loop functions(f.e not in DrawChar() but in InitTui()). Is for things that make the function return; but are 100% not intended by the user(malloc fail type things)
	    LOG_WARNING - Used in non loop functions(f.e not in DrawChar() but in InitTui()). Is for things that may or may not be intended. like: the user is lazy and doesn't want to type if to check is something is inited arleady
	    LOG_INFO - Used in non loop functions(f.e not in DrawChar() but in InitTui()). Info, just info, look at InitTui(), not too much but not too little
	    LOG_DEBUG - Used in loop functions(f.e not in InitTui() but in DrawChar()). All of the above but in looped functions so that the user of the user's program is not flooded with unnesesery information that will take MBs of disk storage for no reason. I think you get the point
    }
}
