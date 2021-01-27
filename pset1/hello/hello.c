#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get Name from user
    string answer = get_string("What's your name? ");
    // Print greetings with user name
    printf("hello, %s\n", answer);
}