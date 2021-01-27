#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get card number input from user
    long card_no;
    do
    {
        card_no = get_long("Number: ");
    }
    while (card_no < 1);

    // Get digits of card number
    int digits = 1;
    long copy_for_dig = card_no;
    while (copy_for_dig > 10)
    {
        digits ++;
        copy_for_dig /= 10;
    }


    // Initiate copy of the card number for processing
    long copy_for_step_1 = card_no / 10;
    long copy_for_step_2 = card_no;


    // Step 1 - Checksum part 1
    long sum = 0;
    do
    {
        long num =  copy_for_step_1 % 10;
        long timesTwo = num * 2;
        if (timesTwo > 9)
        {
            sum += (timesTwo / 10) + (timesTwo % 10);
        }
        else
        {
            sum += timesTwo;
        }
        copy_for_step_1 /= 100;
    }
    while (copy_for_step_1 > 1);


    // Step 2 = Checksum part 2
    do
    {
        long num =  copy_for_step_2 % 10;
        sum += num;
        copy_for_step_2 /= 100;
    }
    while (copy_for_step_2 > 1);


    // Get first two digits of card number
    long first_two_digits = card_no;

    while (first_two_digits > 100)
    {
        first_two_digits /= 10;
    };


    // Identify the card

    // Check if the sum's modulo 10 is congruent to 0
    if (sum % 10 == 0)
    {
        // Check digit of input
        switch (digits)
        {
            // Check for VISA with 13 digits
            case 13:
                if (first_two_digits > 39 && first_two_digits < 50)
                {
                    printf("VISA\n");
                    break;
                }
                else
                {
                    printf("INVALID\n");
                    break;
                }
            // Check for AMEX with 15 digits
            case 15:
                if (first_two_digits == 34 || first_two_digits == 37)
                {
                    printf("AMEX\n");
                    break;
                }
                else
                {
                    printf("INVALID\n");
                    break;
                }
            // Check for VISA or MASTERCARD with 16 digits
            case 16:
                if (first_two_digits > 39 && first_two_digits < 50)
                {
                    printf("VISA\n");
                    break;
                }
                if (first_two_digits > 50 && first_two_digits < 56)
                {
                    printf("MASTERCARD\n");
                    break;
                }
                else
                {
                    printf("INVALID\n");
                    break;
                }
            // If the digits aren't 13, 15, or 16, print invalid
            default:
                printf("INVALID\n");
        }
    }
    // If the sum's modulo 10 isn't congruent to 0, print invalid
    else
    {
        printf("INVALID\n");
    }
}