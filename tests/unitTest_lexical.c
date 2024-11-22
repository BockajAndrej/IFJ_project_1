#include "unity.h"                 // Unity framework
#include "lexical_analyser.h"      // Assume this declares `add`

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

void test_general(void) {
    Token token;
    // Print header for clarity
    printf("Token Type\tValue\t\tKeyword Value\n");
    printf("----------------------------------------------------------\n");

    do
    {
        token = get_token(file);
        print_token(token);

        dynamic_string_free(&token.value.valueString);
    } while (token.type != TOKEN_EOF); // Continue until EOF
}



// Main test runner
int main(void) {
    UNITY_BEGIN();            // Initialize Unity test framework
    RUN_TEST(test_general);
    return UNITY_END();       // Finalize Unity and report results
}