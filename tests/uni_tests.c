#include "unity.h"                 // Unity framework
#include "lexical_analyser.h"      // Assume this declares `add`
#include "syntactic_analysis.h"    // Assume this declares `subtract`

FILE *file;
FILE *tempFile;

char *consts_fail[] = {
    // Příklad 1: Chybějící složená závorka
    "pub fn main() void \n"
    "    var x: i32 = 10;\n"
    "    if (x > 5) \n"
    "        x = x + 1;\n"
    "}\n",

    // Příklad 2: Špatně ukončený blok
    "pub fn main() void {\n"
    "    var x: i32 = 10;\n"
    "    if (x > 5) {\n"
    "        x = x + 1;\n"
    "}\n", // Chybí ukončení hlavní funkce

    // Příklad 3: Neexistující proměnná
    "pub fn main() void {\n"
    "    var x: i32 = y + 1;\n" // Proměnná y není definována
    "}\n",

    // Příklad 4: Špatná struktura funkce
    "pub fn main() void {\n"
    "    var x: i32 = 10;\n"
    "    var y: i32 = 20;\n"
    "    x = add(x, y);\n"
    "}\n", // Funkce add není definována

    // Příklad 5: Chybějící závorky ve výrazu
    "pub fn main() void {\n"
    "    var x: i32 = (10 + 5;\n" // Chybí uzavření závorky
    "}\n",

    // Příklad 6: Vnořený blok není ukončen
    "pub fn main() void {\n"
    "    var x: i32 = 10;\n"
    "    if (x > 5) {\n"
    "        var y: i32 = 20;\n"
    "        if (y > x) {\n"
    "            y = y + 1;\n"
    "}\n", // Vnitřní blok není uzavřen

    // Příklad 7: Špatné ukončení smyčky while
    "pub fn main() void {\n"
    "    var x: i32 = 0;\n"
    "    while (x < 10) {\n"
    "        x = x + 1;\n"
    "}\n", // Chybí ukončení hlavní funkce
    // Příklad 8: Chybí závorka při podmínce
    "pub fn main() void {\n"
    "    if (x > 5 {\n"
    "        var y: i32 = 10;\n"
    "    }\n"
    "}\n",

    // Příklad 9: Chybí složená závorka za podmínkou
    "pub fn main() void {\n"
    "    if (x > 5)\n"
    "        var y: i32 = 10;\n"
    "}\n",

    // Příklad 10: Chybí středník po příkazu
    "pub fn main() void {\n"
    "    var x: i32 = 10\n"
    "    var y: i32 = x + 5;\n"
    "}\n",

    // Příklad 11: Chybí uzavření smyčky
    "pub fn main() void {\n"
    "    while (x < 10) {\n"
    "        x = x + 1;\n"
    "}\n", // Chybí ukončení hlavního bloku

    // Příklad 12: Chybí ukončovací složená závorka funkce
    "pub fn main() void {\n"
    "    var x: i32 = 10;\n",

    // Příklad 13: Chybí typ proměnné
    "pub fn main() void {\n"
    "    var x = 10;\n"
    "}\n",

    // Příklad 14: Neúplný deklarovaný blok
    "pub fn main() void {\n"
    "    var y: i32 = 20;\n"
    "    if (y > 10) {\n"
    "        var z: i32 = y + 5;\n"
    "}\n",

    // Příklad 15: Chybí argument ve výrazu
    "pub fn main() void {\n"
    "    var x: i32 = 10 + ;\n"
    "}\n",

    // Příklad 16: Chybí jméno proměnné při deklaraci
    "pub fn main() void {\n"
    "    var : i32 = 10;\n"
    "}\n"
};


char *consts[] = {
    "const INP = 5;\n",
    "const vysl = factorial(INP);\n",
    "pub fn main() void\n{\n}\n",
    "const ifj = @import(\"ifj24.zig\");\n",
    "pub fn main() void\n{\nconst inp = 4;\nreturn 0;\n}\n",
    "pub fn main() void\n{\nconst inp = 4;\nreturn inp;\n}\n",
    "const inp = inp + 1;\n",
    "const inp_2 = inp - 1;\n",
    "const inp_3 = inp * 1;\n",
    "const inp_4 = inp / 1;\n",
    "const inp = ifj + inp;\n",
    "var inp_str: u8 = pub fn ifj.readstr();\n",
    "var inp_int: i32 = pub fn ifj.readi32();\n",
    "var inp_float: f64 = pub fn ifj.readf64();\n",
    "var d: f64 = ifj.i2f(val);\npub fn main() void\n{\nvar d: f64 = ifj.i2f(val);\nvar k: u8 = 25 + 5;\nvar a: i32 = 0;\nconst r = foo(param);\n}\n",
    "pub fn factorial(n : i32) i32\n{\nvar result : i32 = 0 - 1;\nif (n < 2)\n{\nresult = 1;\n}\n}",
    
};

char *consts2[] = {
    "pub fn printInput() void\n{\nvar input: u8 = ifj.readstr();\nifj.write(input);\n}\n", // Funkce bez návratové hodnoty
    "pub fn sum(a: i32, b: i32) i32 {\nreturn a + b;\n}\n", // Funkce s návratovou hodnotou
    "const PI: f64 = 3.14159;\n", // Konstantní definice
    "var array: u8 = ifj.string(\"hello\");\n", // Proměnná typu slice
    "pub fn main() void\n{\nif (array != null) {\nifj.write(\"Array is not null\");\n}\n}\n", // Podmínka s porovnáním na null
    "pub fn main() void\n{\nwhile (counter > 0) {\ncounter = counter - 1;\n}\n}\n", // While smyčka
    "pub fn main() void\n{\nif (input) |str| {\nifj.write(\"Non-null string: \");\nifj.write(str);\n}\n}\n", // Podmínka s typem zahrnujícím null
    "pub fn main() void\n{\nreturn factorial(5);\n}\n", // Návratová hodnota z funkce
    "var value: i32 = ifj.f2i(3.14);\n", // Konverze z f64 na i32
    "pub fn concatStrings(a: u8, b: u8) u8 {\nreturn ifj.concat(a, b);\n}\n", // Funkce s parametry typu slice
    "pub fn main() void\n{\nif (x < y) {\nx = x + 1;\n}\n else {\nx = y;\n}\n}\n", // If-else struktura
    "pub fn main() void\n{\nconst val = 5;\nif (x > 0) |val| {\nifj.write(val);\n}\n else {\nreturn;\n}\n}\n", // If s návratovou hodnotou a větvením
    "// This is a comment\nconst message: u8 = ifj.string(\"Hello, world!\");\n", // Komentář a definice
    "pub fn main() void \n{\nwhile (n != 0) \n{\nifj.write(n);\nn = n - 1;\n}\n}\n", // While smyčka s výpisem
    "pub fn main() void \n{\nconst a = 42;\nvar b = a + 1;\nifj.write(b);\n}\n" // Hlavní funkce
};

char *consts3[] = {
    "pub fn factorial(n: i32) i32 {\nif (n <= 1) {\nreturn 1;\n}\nreturn n * factorial(n - 1);\n}\n", // Rekurzivní funkce
    "pub fn main() void {\nconst sum = 10 + 20;\n}\n", // Aritmetická operace v main
    "pub fn fibonacci(n: i32) i32 {\nif (n <= 1) {\nreturn n;\n}\nreturn fibonacci(n - 1) + fibonacci(n - 2);\n}\n", // Rekurzivní výpočet Fibonacciho čísel
    "pub fn main() void {\nvar counter: i32 = 10;\nwhile (counter > 0) {\ncounter = counter - 1;\n}\n}\n", // While smyčka v main
    "pub fn divide(a: f64, b: f64) f64 {\nif (b == 0.0) {\nreturn 0.0;\n}\nreturn a / b;\n}\n", // Dělení s kontrolou dělitele
    "pub fn main() void {\nconst largeNumber: i32 = 1000000;\nconst smallNumber: i32 = 1;\nconst difference = largeNumber - smallNumber;\n}\n", // Operace s čísly v main
    "pub fn max(a: i32, b: i32) i32 {\nif (a > b) {\nreturn a;\n} else {\nreturn b;\n}\n}\n", // Podmínka if-else
    "pub fn main() void {\nvar num: i32 = 5;\nnum = num * 2;\nnum = num + 3;\n}\n", // Modifikace proměnné v main
    "pub fn main() void {\nvar PI: f64 = 3.14159;\nvar radius: f64 = 10.0;\nconst area = PI * radius * radius;\n}\n", // Výpočet kruhové plochy v main
    "pub fn concatSlices(a: u8, b: u8) u8 {\nreturn ifj.concat(a, b);\n}\n", // Spojení řezů (slice)
    "pub fn stringLength(s: u8) i32 {\nreturn ifj.length(s);\n}\n", // Délka řezu
    "pub fn main() void {\nvar result: i32 = 1;\nvar i: i32 = 1;\nwhile (i <= 5) {\nresult = result * i;\ni = i + 1;\n}\n}\n", // Iterativní výpočet faktoriálu v main
    "pub fn power(base: f64, exp: i32) f64 {\nvar result: f64 = 1.0;\nvar i: i32 = 0;\nwhile (i < exp) {\nresult = result * base;\ni = i + 1;\n}\nreturn result;\n}\n" // Mocnina iterativně
};

char *consts_complex[] = {
    // Příklad 1: Kombinace podmínky a cyklu pro výpočet maxima
    "pub fn main() void {\n"
    "    var numbers: i32 = {1, 3, 5, 2, 9, 6};\n"
    "    var max: i32 = numbers[0];\n"
    "    var i: i32 = 1;\n"
    "    while (i < ifj.length(numbers)) {\n"
    "        if (numbers[i] > max) {\n"
    "            max = numbers[i];\n"
    "        }\n"
    "        i = i + 1;\n"
    "    }\n"
    "}\n",

    // Příklad 2: Výpočet faktoriálu kombinující iteraci a kontrolu
    "pub fn main() void {\n"
    "    var n: i32 = 5;\n"
    "    var result: i32 = 1;\n"
    "    if (n < 0) {\n"
    "        // Do nothing, faktoriál záporného čísla není definován\n"
    "    } else {\n"
    "        var i: i32 = 1;\n"
    "        while (i <= n) {\n"
    "            result = result * i;\n"
    "            i = i + 1;\n"
    "        }\n"
    "    }\n"
    "}\n",

    // Příklad 3: Výpočet Fibonacciho čísla s iterací a podmínkou
    "pub fn main() void {\n"
    "    var n: i32 = 10;\n"
    "    var a: i32 = 0;\n"
    "    var b: i32 = 1;\n"
    "    if (n == 0) {\n"
    "        a = 0;\n"
    "    } if (n == 1) {\n"
    "        a = 1;\n"
    "    } else {\n"
    "        var i: i32 = 2;\n"
    "        while (i <= n) {\n"
    "            var temp: i32 = a + b;\n"
    "            a = b;\n"
    "            b = temp;\n"
    "            i = i + 1;\n"
    "        }\n"
    "    }\n"
    "}\n",

    // Příklad 4: Vyhledání prvního výskytu hodnoty v poli
    "pub fn main() void {\n"
    "    var data: i32 = {3, 7, 2, 5, 7, 8};\n"
    "    var target: i32 = 7;\n"
    "    var foundIndex: i32 = -1;\n"
    "    var i: i32 = 0;\n"
    "    while (i < ifj.length(data)) {\n"
    "        if (data[i] == target) {\n"
    "            foundIndex = i;\n"
    "            i = ifj.length(data);\n"
    "        }\n"
    "        i = i + 1;\n"
    "    }\n"
    "}\n",

    // Příklad 5: Zanořená podmínka a cyklus
    "pub fn main() void {\n"
    "    var matrix: i32 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};\n"
    "    var sum: i32 = 0;\n"
    "    var i: i32 = 0;\n"
    "    while (i < ifj.length(matrix)) {\n"
    "        var j: i32 = 0;\n"
    "        while (j < ifj.length(matrix[i])) {\n"
    "            if (matrix[i][j] % 2 == 0) {\n"
    "                sum = sum + matrix[i][j];\n"
    "            }\n"
    "            j = j + 1;\n"
    "        }\n"
    "        i = i + 1;\n"
    "    }\n"
    "}\n",

    // Příklad 6: Komplexní operace s řetězci
    "pub fn main() void {\n"
    "    var str1: u8 = ifj.string(\"hello\");\n"
    "    var str2: u8 = ifj.string(\"world\");\n"
    "    var combined: u8 = ifj.concat(str1, str2);\n"
    "    var result: i32 = ifj.length(combined);\n"
    "    if (result > 10) {\n"
    "        result = 10;\n"
    "    }\n"
    "}\n",

    //kod ukradeny ze zadani
    "pub fn main() void {\n"
    "    var str1: u8 = ifj.string(\"Toto je text v programu jazyka IFJ24\");\n"
    "    var str2: u8 = ifj.string(\", ktery jeste trochu obohatime\");\n"
    "    str2 = ifj.concat(str1, str2);\n"
    "    var newInput: u8 = ifj.readstr();\n"
    "    var all: u8 = ifj.string(\"\");\n"
    "    while (newInput) |inpOK| {\n"
    "        var abcdefgh: u8 = ifj.string(\"abcdefgh\");\n"
    "        var strcmpResult: i32 = ifj.strcmp(inpOK, abcdefgh);\n"
    "        if (strcmpResult == 0) {\n"
    "            newInput = null;\n"
    "        } else {\n"
    "            all = ifj.concat(all, inpOK);\n"
    "            newInput = ifj.readstr();\n"
    "        }\n"
    "    }\n"
    "}\n"



};







/* Poznamky co nefunguje - 

    některé if/else/while
    ifj.write(input); - zastaví se test při načítání ")"
    při: if/while(podminka) musí být if ve funkci jinak neprojde testama - účel? asi? nevím jsem clueless
    když chci nadefinovat počátečně konstantu tak to failne - př: const PI: f64 = 3.14159 - jednoduše vyzkoušíte změnou proměnné na const v jednom z testu;
    u u8 se tomu nelíbí závorky ([]u8), s nima to failne, to samé pro pole int ([]i32) nebo 2D pole ([][]i32) bez nich to funguje :clueless:
    fail na while (newInput) |inpOK| {\n - nevím proč, ale failuje to na |inpOK|, když to odstraním tak to projde
    prochází testy kde se používá proměnná která ale dříve nebyla definovana... třeba to není chyba v tom co se testuje ale už nemám kapacitu to zjišťovat
    dále prochází testy kde třeba nakonci chybí závorka, př: var x: i32 = (10 + 5;
    bez chybějícího středníku testy taky projdou
    testy projdou taky když chybí třeba argument ve výrazu: var x: i32 = 10 + ;\n"

*/

/*odkladove místo
    "if (inp)\n{\n}\n",
    "if (inp) | INP |\n{\n}\n",
    "else\n{\n}\n",
    "while (inp)\n{\n}\n",
    "while (inp) | INP |\n{\n}\n",

    nefungující testy ale to bylo proto že nejsou ve funkci
   


*/


int i = 0;



// Optional setup code for tests (runs before each test)
void setUp(void) {
    file = fopen("../tests/inputs/01.txt", "r");
    if(file == NULL)
        fprintf(stderr, "Usage: %s <filename>\n", "./inputs/01.txt");
    tempFile = tmpfile();
    if (tempFile == NULL)
        fprintf(stderr, "Failed to create temporary file");
    
}

// Optional teardown code for tests (runs after each test)
void tearDown(void) {
    fclose(tempFile);
    fclose(file); 
}

void test_syntactic(void) {
    TEST_ASSERT_EQUAL(1, FIRST(file)); 
}


void test_consts_fail(void) {
    fprintf(tempFile, "%s", consts_fail[i]);
    printf("\033[32m%s\033[0m\n", consts_fail[i]);
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile));  
}

void test_consts(void) {
    fprintf(tempFile, "%s", consts[i]);
    printf("\033[32m%s\033[0m\n", consts[i]);
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile));  
}

void test_consts2(void) {
    fprintf(tempFile, "%s", consts2[i]);
    printf("\033[32m%s\033[0m\n", consts2[i]);
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile));  
}

void test_consts3(void) {
    fprintf(tempFile, "%s", consts3[i]);
    printf("\033[32m%s\033[0m\n", consts3[i]);
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile));  
}

void test_consts_complex(void) {
    fprintf(tempFile, "%s", consts_complex[i]);
    printf("\033[32m%s\033[0m\n", consts_complex[i]);
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile));  
}

/*void test_synt_import(void) {
    // Write to the temporary file
    fprintf(tempFile, "const ifj = @import(\"ifj24.zig\");\n");
    // Move the file pointer to the beginning for reading
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}
void test_synt_const(void) {
    fprintf(tempFile, "const inp = ifj.readi32();\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}

void test_synt_const2(void) {
    fprintf(tempFile, "const vysl = factorial(INP);\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}

void test_synt_function(void) {
    fprintf(tempFile, "pub fn main() void\n{\n}\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}

void test_synt_condition(void) {
    fprintf(tempFile, "if (inp) | INP |\n{\n}\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}

void test_synt_condition_else(void) {
    fprintf(tempFile, "else\n{\n}\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}


void test_synt_factorial(void) {
    fprintf(tempFile, "const vysl = factorial(INP);\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}*/

/*void test_synt_syntatic2(void) {
    fprintf(tempFile, "ifj.write(\"Faktorial nelze spocitat!\n\");\n");
    rewind(tempFile);
    TEST_ASSERT_EQUAL(1, FIRST(tempFile)); 
}*/


// Main test runner
int main(void) {
    UNITY_BEGIN();            // Initialize Unity test framework
    //RUN_TEST(test_syntactic);
    //RUN_TEST(test_synt_import);
    //RUN_TEST(test_synt_const); 
    //RUN_TEST(test_synt_const2);
    //RUN_TEST(test_synt_function);
    //RUN_TEST(test_synt_condition);
    //RUN_TEST(test_synt_condition_else);
    //RUN_TEST(test_synt_factorial);
    int delka0 = sizeof(consts_fail) / sizeof(consts_fail[0]);
    int delka = sizeof(consts) / sizeof(consts[0]);
    int delka2 = sizeof(consts2) / sizeof(consts2[0]);
    int delka3 = sizeof(consts3) / sizeof(consts3[0]);
    int delka4 = sizeof(consts_complex) / sizeof(consts_complex[0]);
    

    printf("vyber testy které chceš spustit - 0-4 (0 pro test invalid syntax): ");

    int volba;
    scanf("%d", &volba);
    printf("volba: %d\n", volba);

    if(volba == 0){
        while(i < delka0){
            printf("\n\ntest: %d\n", i+1);
            RUN_TEST(test_consts_fail);
            i++;
        }
    }

    if(volba == 1){
        while(i < delka){
            printf("\n\ntest: %d\n", i+1);
            RUN_TEST(test_consts);
            i++;
        }
    }
    else if(volba == 2){
        while(i < delka2){
            printf("\n\ntest: %d\n", i+1);
            RUN_TEST(test_consts2);
            i++;
        }
    }
    else if(volba == 3){
        while(i < delka3){
            printf("\n\ntest: %d\n", i+1);
            RUN_TEST(test_consts3);
            i++;
        }
    }
    else if(volba == 4){
        while(i < delka4){
            printf("\n\ntest: %d\n", i+1);
            RUN_TEST(test_consts_complex);
            i++;
        }
    }
    else{
        printf("špatná volba");
    }


    
    //RUN_TEST(test_synt_syntatic2);
    return UNITY_END();       // Finalize Unity and report results
}