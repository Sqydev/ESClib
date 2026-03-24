// TODO: MAKE DRAWTEXTFORMAT
// NOTE: Ok. So the plan is to make this in phases. po polsku bo mi się nie chce. Że są fazy. Pierwsza faza to że robi się buffor i tam jest pierwszy prass czyli zmienienie zmiennych typu %d i %f itd jak w printf w sumie to można to spróbować zkopiować z printf bo czemu nie i 2 faza to prasowanie żeczy tui czyli kolory, spaceing, kąt itd. i pisanie do backbuffora. czyli pisze już stringa z zmiennymi podmienionymi i jak jest np. %fg to to pomija i daje foreground na kolor zaspecyfikowany

// TODO: Kiedy będziesz robił kształty w sęsie jak skończysz robić brila to zrób wersję DrawBrileRectangle czy coś takiego co by miał dokładniejszą siatkę koordynatów. Bo każda komórka ma z 6/8 możliwych kropek w brileu i by to pomogło. Ale to jak zrobisz brilea. A, i wtedy zrób jeszcze gettery dla brila(np. GetTuiBrileDimentions())

// TODO: Jak będzie ci się nudzić to zrób że DrawChar i DrawText ma faktyczny parametr fontsize i ma czcionki. i czcionka to używany charset i jak będzie ci się jeszcze bardziej nudzić to jeszcze vectory itd
