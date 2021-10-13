# Arduino_rfid

| Pôvodný kód je v zložke „Kod“

## Nastavenie editora:
1. Na https://www.arduino.cc/en/software si stiahnite najnovšiu verziu editora Arduino IDE pre váš operačný systém. (Nie je potrebná žiadna donácia, stačí zakliknúť "Just download")
2. Na https://github.com/MatoXsvk/Arduino_rfid stlačte zelene tlačidlo "Code" a dajte "Download ZIP".
3. Po stiahnutí rozbaľte daný súbor
4. Prejdite do zložky "Kod" a otvorte "Kod.ino"
5. Na paneli editora rozkliknite "Projekt" > "Zahrnúť knižnice" > "Pridať .zip knižnicu" a vyberte .zip súbor priložený k projektu (rfid-master.zip)
6. Na hormnom paneli rozkliknite "Nástroje" a za "Dosku" zadajte "Arduino UNO"
7. Po pripojení dosky arduino, rozkliknite na paneli " Nástroje " > "Port" a vyberte možnosť, za ktorou je (Arduino UNO) (pripadne jednu z možností)


## Nahrávanie programu:
1. Pripojte dosku arduino k počítaču pomocou kábla USB-B
2. Na arduino doske vypojte kábel z 8. pinu!
3. V ľavom hornom rohu editora Arduino stlačte tlačidlo lupy
4. Stlačte tlačidlo šípky aby sa kod nahral na dosku Arduino
5. Čip, ktorý chcete pridať, priložte k čítačke
6. Skopírujte informáciu za vetou 'UID tag:' medzi úvodzovkami a vložte ju za ostatné kódy vo forme "XX XX XX XX" a oddeľujte ich čiarkou. Ak doposiaľ nie je uložený žiadny kód, tak ju vložte medzi kučeravé zátvorky: {}
7. Uložte, nahrajte kód tlačidlom s šípkou vľavo hore v editore arduino.
8. Znovu zapojte 8. pin
