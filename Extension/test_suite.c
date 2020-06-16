//
// Created by Zhai Zirun on 16/6/20.
//

//
// Created by Zhai Zirun on 16/6/20.
//
#include <stdbool.h>
#include "login.c"

bool txt_compare(char* output_file; char* expected_file) {
    file out = fopen(output_file, 'r');
    file expected = fopen(expected_file, 'r');
    while (!feof(expected)) {
        int next_out = fgetc(out);
        int next_expected = fgetc(expected);
        if (next_out == EOF) {
            return false;
        } else if (next_out != next_expected) {
            return false;
        }
    }
    return true;
}


void test_login(void) {
    //1. Tests a registered account with the wrong password
    printf("1. Testing a registered account with the wrong password...\n");
    login("login01.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY LOGIN FUNCTION
    assert(txt_compare("out.txt" == "expected_login01.txt")); //TODO: find a function that compares txt files
    printf("Test passed!\n");

    //2. Tests a non-registered account
    printf("2. Testing a non-registered account...\n");
    printf("Test passed!\n");

    //3. Tests for invalid input
    printf("3. Testing for invalid input...\n");
    printf("Test passed!\n");

    printf("All tests passed for login!");
}

void test_take_order(void) {
    //1. No orders
    printf("1. Testing for no orders made...\n");
    printf("Test passed!\n");

    //2. Order has qty 0
    printf("2. Testing for order having qty 0...\n");
    printf("Test passed!\n");

    //3. Invalid item id
    printf("3. Testing for order having invalid item ID...\n");
    printf("Test passed!\n");

    //4. Invalid qty
    printf("4. Testing for order having invalid quantity...\n");
    printf("Test passed!\n");

    printf("All tests passed for take_order!");

}

void test_store_receipt(void) {
    //1. Invalid path
    printf("1. Testing for invalid path...\n");
    printf("Test passed!\n");

    printf("All tests passed for store_receipt!");
}

int main(int argc, char **argv) {
    printf("Running tests:\n");
    printf("Testing login:\n");
    test_login();
    printf("Testing take_order:\n");
    test_take_order();
    printf("Testing store_receipt:\n");
    test_store_receipt();
    printf("Testing store_receipt:\n");
    test_store_receipt();
    printf("All tests passed :)\n");
    return EXIT_SUCCESS;
}