******************************** Welcome to our testsuite! ***************************************

In order to test our digital receipt system we have made some text files to simulate user input.

To use our test suite:
1. Go to command line and make our extension.
2. Go to the test file you wish to test and replace <Enter Client's Email Address>
with the email address you wish to receive your receipt.
3. Run ./merchant <filename of test> in your terminal
4. Check your email address for the receipt and compare the output to the expected text file.

***************************************************************************************************

Test Cases Information:

test_take_order_01: Tests that taking order works
Expected Text File Location: ...Merchants/bhsz/test_take_order_01_expected.txt

test_edit_order_01: Tests that ordering and then editing order works by adding an extra order.
Expected Text File Location: ...Merchants/bee/test_edit_order_01_expected.txt

test_pay_by_cash : Tests paying by cash
Expected Text File Location: ...Merchants/bhsz/test_pay_by_cash_expected.txt

test_pay_by_card : Tests paying by card
Expected Text File Location: ...Merchants/bee/test_pay_by_card_expected.txt

test_pay_by_ewallet : test paying by e-wallet
Expected Text File Location: ...Merchants/bhsz/test_pay_by_ewallet_expected.txt

test_cancel_order: Tests cancelling order by taking two orders and cancelling both of them.
*No receipts should be made
Expected Text File Location: ...Merchants/bee/test_cancel_order_expected.txt