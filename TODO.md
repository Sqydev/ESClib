// TODO: Make optional option to have log file. YOu init by doing InitLogging(char* path); and like replase all the printf loging by TraceLog(something); so you know, logs are normal

// TODO: Make input from the compositor(asking the compositor so sudo isn't needed and focus works) and make termios input type have 2 modes, if DATA.input.typewritermode is true than it does what it does now. and if it's false than do ncurses hack so whait after first input and than read in like 15 or something like that ms delays so the input is delayed but nearly like true thing

// TODO: Make DrawRecPro lines

// TODO: Make lines in DrawCircle work

// TODO: Make aspect ratio correction optional by adding argument to each draw fn: bool aCorrection or bool aspectCorrettion or something like that. And add lines argument to DrawRecPro

// Make makefile make different obj files for tester and shit

// TODO: Kiedy będziesz robił kształty w sęsie jak skończysz robić brila to zrób wersję DrawBrileRectangle czy coś takiego co by miał dokładniejszą siatkę koordynatów. Bo każda komórka ma z 6/8 możliwych kropek w brileu i by to pomogło. Ale to jak zrobisz brilea. A, i wtedy zrób jeszcze gettery dla brila(np. GetTuiBrileDimentions())

// TODO: Jak będzie ci się nudzić to zrób że DrawChar i DrawText ma faktyczny parametr fontsize i ma czcionki. i czcionka to używany charset i jak będzie ci się jeszcze bardziej nudzić to jeszcze vectory itd

// TODO: Make optional shaders support

// TODO: Fix makefile

// TODO: Fix DrawRec(for sure with 🔥 but it could also be with normal chars) Beying wierd with 0, 0 origin

// TODO: Revrite DrawRecPro and make it understandable

// TODO: Add alpha channel to the colors

// TODO: Make not aspectRatiofied Draws have completly different and faster logic
