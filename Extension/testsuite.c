//
// Created by Ivy Tam on 16/06/2020.
//
void test_edit_order(void) {
    //1. Remove 1 item
    printf("1. Testing for 1 item to be removed...\n");
    edit_order("edit_order_01.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "expected_edit_order_01.txt"));
    printf("Test passed!\n");

    //2. Modify Quantity of an item
    printf("2. Testing for quantity of item to be modified...\n");
    edit_order("edit_order02.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "expected_edit_order_02.txt"));
    printf("Test passed!\n");

    //3. Add items
    printf("3. Testing to add items to order...\n");
    login("edit_order03.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "expected_edit_order_03.txt"));
    printf("Test passed!\n");

    printf("All tests passed for edit_order!");

}

void test_pay_order(void) {
    //1. Pay by cash
    printf("1. Testing for paying with cash..\n");
    pay("pay_order_01.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "expected_pay_order_01.txt"));
    printf("Test passed!\n");

    //2. Pay by card
    printf("2. Testing for paying by card...\n");
    pay("pay_order02.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "pay_edit_order_02.txt"));
    printf("Test passed!\n");

    //3. Pay by e-wallet
    printf("3. Testing for paying by e-wallet...\n");
    pay("pay_order03.txt", "out.txt"); // TODO: MAKE THE FILE and MODIFY TAKE_ORDER FUNCTION
    assert(txt_compare("out.txt", "expected_pay_order_03.txt"));
    printf("Test passed!\n");

    printf("All tests passed for pay_order!");

}

//test adding menu?
//test forgetting password?
