#include "unity.h"                 // Unity framework
#include "lexical_analyser.h"      // Assume this declares `add`
#include "syntactic_analysis.h"    // Assume this declares `subtract`

FILE *file;

// Optional setup code for tests (runs before each test)
void setUp(void) {
    file = fopen("../tests/inputs/01.txt", "r");
    if(file == NULL)
    {
        fprintf(stderr, "Usage: %s <filename>\n", "./inputs/01.txt");
    }
}

// Optional teardown code for tests (runs after each test)
void tearDown(void) {
    // Code to clean up after tests, if needed
}

// Test case for addition
void test_addition(void) {
    TEST_ASSERT_EQUAL(1, FIRST(file)); 
}

// Test case for subtraction
void test_subtraction(void) {
    TEST_ASSERT_EQUAL(1, FIRST(file)); 
}

// Main test runner
int main(void) {
    UNITY_BEGIN();            // Initialize Unity test framework
    RUN_TEST(test_addition);
    // RUN_TEST(test_subtraction); 
    return UNITY_END();       // Finalize Unity and report results
}