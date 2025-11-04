TODO:
- Readme
- Name
- Do hidecursor && show cursor
- Make it so ClearChar and ClearLine takes an int HOW_MUCH argument
- FIX GETKEY AND KEYS MACROS
- Look for printfs that can be replaced with simple putchar
- End making it into 1 file
- Do CloseTui
- Make #define to enums
- Make #ifdefs better especialy in GetKey()(cuz you can do only one in begining and not do 2 long switches for win and lin. just optim it)
- Clean .c to fit .h
- Make more Is fuctions, especialy for cursor functions for example: IsCursorHidden/Locked, Idea to how to find ones todo, look at bools in CORE
- Find better ways to do Move and Set Cursor fns, cuz there for sure is better way to do same thing that I did when i was tired, and check if it works
- FAST: GOTO DBP
- Rewrite tester after making begindrawing && end 
- Make disable modes in closetui
- MAKE GETKEY RETURN KEY THAT'S PRESSED NOT LAST KEY THAT'S PRESSED
- MAKE PRINTF INTO WRITEINTOBACKBUFFOR();
- Find what makes getkey and just LoopTest break(it's propably becouse most functions doesn't write to backbuffer yet(still do printf)) way to give vars to WritetoTui: {
    just make char* and do sprintf magic;
}
- See what make's DisableBufferMode not work
- Ok, fixed flicering in test(it was ClearBg) NOW CHECK WHAT IS STILL TO REPAIR

REMAMBER:
- Remamber to make sleep to make fps when making lib

DBP:
- Ok, so you'll need to make it work on doubble buffor(not tripple) how to:
- Ok, so every function accualy prints to char* and when EndDrawing() is called you just printf conntents of this char*
- Chatgbt summery of my thouths {
# Double Buffering Plan (DBP) dla TUI

## 1. Idea
- Używamy **dwóch buforów**: 
  - `frontBuffer` – reprezentuje to, co aktualnie widoczne jest w terminalu.
  - `backBuffer` – miejsce, gdzie zapisujemy wszystkie operacje rysowania w bieżącej klatce.
- Funkcje typu `DrawChar()`, `FillScreen()`, `SetCursorPosition()`, itp. **nie drukują od razu do terminala**, tylko zapisują wynik do `backBuffer`.
- Bufor to najprościej tablica `char` lub struktura `char + kolor` (RGBA).

## 2. Przepływ w pętli rysowania

```c
BeginDrawing();             // Przygotowuje backBuffer do nowej klatki

DrawChar(x, y, 'A', RED);   // Zapisuje do backBuffer, nie printf

EndDrawing();               // Porównuje backBuffer z frontBuffer i wypisuje różnice do terminala

3. Swap Buffers

    Po EndDrawing():

        backBuffer staje się frontBufferem (lub jego zawartość jest kopiowana).

        backBuffer czyści się do rysowania kolejnej klatki.

    Dzięki temu terminal aktualizuje się płynnie i bez migotania.

4. Dlaczego double, a nie triple buffer

    Triple buffer w OpenGL pozwala CPU rysować w trzecim buforze podczas gdy GPU renderuje drugi, co unika stalling.

    W TUI nie mamy GPU ani asynchronicznego renderowania, więc double buffer w pełni wystarcza:

        Jeden bufor do rysowania.

        Jeden bufor do porównania i wysłania zmian do terminala.

5. Dodatkowe uwagi

    Każda funkcja rysująca powinna tylko zapisywać do backBuffer.

    EndDrawing() decyduje, które zmiany wysłać do terminala (np. różnice w znakach lub kolorach).

    Można śledzić currentCursorPosition i kolor, aby wysyłać minimalne sekwencje ESC, co optymalizuje wydajność.
}

This one is mine not chatgbt's
- AND REMEMBER: {
EVERY RESULT IS SIMPLY ADDED TO THE BACKBUFFER.  
FRONTBUFFER IS JUST THE TERMINAL.  

For example:  
- If I call `ClearScreen()`, it will only write to the backbuffer: "\033[2J\033[H"  
- If I then call `HideCursor()`, it will add to the backbuffer: "\033[?25l"  

When `EndDrawing()` is triggered, it will simply `printf` the contents of the backbuffer to the terminal.

ALSO: USE WRITE SYSCALL AND WINDOWS'S WRITE VERSION INSTEAD OF PRINTF IN PRINTING BACKBUFFOR. BACKBUFFOR SHOULD BE IN CORE STRUCT
}
