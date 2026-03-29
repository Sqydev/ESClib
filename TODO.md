// TODO: Fix colors mixing by the terminal {
	// NOTE: Rec that is $'s of color yellow and background blue BUT with roundness 5 BUT with 0 DEG
 	DrawRectanglePro("$", 40, 55, 40, 10, 0, 0, &(Color){ 255, 255, 0, 1 }, &(Color){ 0, 0, 255, 1 }, 0, 5);

    Here like it draws $ as white becouse 255, 255, 0 + 0, 0, 255 -> 255, 255, 255
}

// TODO: MAKE DRAWTEXTFORMAT

// TODO: Fix Vecrot typeo to Vector

// Make makefile make different obj files for tester and shit

// TODO: Kiedy będziesz robił kształty w sęsie jak skończysz robić brila to zrób wersję DrawBrileRectangle czy coś takiego co by miał dokładniejszą siatkę koordynatów. Bo każda komórka ma z 6/8 możliwych kropek w brileu i by to pomogło. Ale to jak zrobisz brilea. A, i wtedy zrób jeszcze gettery dla brila(np. GetTuiBrileDimentions())

// TODO: Jak będzie ci się nudzić to zrób że DrawChar i DrawText ma faktyczny parametr fontsize i ma czcionki. i czcionka to używany charset i jak będzie ci się jeszcze bardziej nudzić to jeszcze vectory itd

// TODO: Make optional shaders support

// TODO: Fix makefile
