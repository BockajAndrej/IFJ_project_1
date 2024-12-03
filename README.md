# IFJ - Team project 01

## Clone
### Všetko
Potrebne byt v priecinku ktory obsahuje dany submodul
```
git clone --recurse-submodules https://github.com/BockajAndrej/IFJ-project01
git clone --recurse-submodules https://github.com/jansko03/IFJ24-tests
```
### Len submodule
```
git submodule update --init --recursive
```
> Unity test framework


## Prostředí
Linux
> Podporované operační systémy

## Autoři
### Tým: Tým xbockaa00
| xlogin | Meno | Pracoval na | |
| --- | --- | --- | --- |
| xbockaa00 | Andrej Bočkaj | Syntakticky analyzator, Generator kodu |
| xfiloja00 | Jakub Fiľo | Lexykalny analyzator, Semanticky analyzator|
| xglvacp00 | Pavel Glvač | Symtable, Abstraktny syntakticky strom |
| xkubicj00 | Jan Kubíček | Dokumentacia, Testy |


## Zig
ic24int - referencni interpret ciloveho jazyka IFJcode24
verze 2024-11-21, 64-bitova

Prubezne doplnovane informace najdete na Moodle predmetu IFJ na strance "Interpret cíloveho kodu IFJcode24".

CHANGES:
  * 2024-09-29:
    * zverejnena prvni verze interpretu (upravena kompatibilita s jazykem Zig hlavne u instrukci READ a WRITE (uprava znamenka v exponentu v hexadec. vypisu, nil vypisuje jako null), podpora 64-bitoveho int)
  * 2024-11-21:
    * zruseni zapomenuteho ladiciho vypisu pri nacitani nevalidniho desetinneho cisla zacinajiciho cislici

### Pouzitie
./ic24int ../src/main < ./prog/00.zig > ./output/OUTPUT.out 

overenie:
echo $?