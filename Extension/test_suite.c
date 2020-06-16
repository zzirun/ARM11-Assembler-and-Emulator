//
// Created by Zhai Zirun on 16/6/20.
//

void test_login(void) {
    //1. Tests a registered account
    //2. Tests a registered account with the wrong password
    //3. Tests a non-registered account
    //4. Tests for invalid input
}

void test_take_order(void) {
    //1. No orders
    //2. Order has qty 0
    //3. Invalid item id
    //4. Invalid qty

}

void test_store_receipt(void) {
    //1. Invalid path
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