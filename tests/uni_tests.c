#include "unity.h"                 // Unity framework
#include "lexical_analyser.h"      // Assume this declares `add`
#include "syntactic_analysis.h"    // Assume this declares `subtract`

FILE *file;
FILE *tempFile;

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

void test_synt_import(void) {
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


// Main test runner
int main(void) {
    UNITY_BEGIN();            // Initialize Unity test framework
    // RUN_TEST(test_syntactic);
    RUN_TEST(test_synt_import);
    RUN_TEST(test_synt_const); 
    return UNITY_END();       // Finalize Unity and report results
}