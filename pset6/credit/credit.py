import re


def main():
    # Get user input and get digit
    num = input("Number: ")
    digit = len(num)

    # If digit invalid end function
    if digit != 13 and digit != 15 and digit != 16:
        print("INVALID\n")
        return

    # Check for Luhn's Algorithm
    if(luhns_algorithm(num)):
        # Check for card matches
        if digit == 13:
            if(re.match('4', num) != None):
                print("VISA\n")
                return
        elif digit == 15:
            if(re.match('34|37', num) != None):
                print("AMEX\n")
                return
        elif digit == 16:
            if(re.match('4', num) != None):
                print("VISA\n")
                return
            elif(re.match('51|52|53|54|55', num) != None):
                print("MASTERCARD\n")
                return
        # If nothing matches, print invalid
        else:
            print("INVALID\n")
            return


def luhns_algorithm(num):

    # Reverse String
    reverse = num[::-1]

    # Initialize checksum
    checksum = 0

    # Step 1-2
    for i in range(1, len(num), 2):
        n = int(reverse[i])
        checksum += n * 2 - 9 if n * 2 > 9 else n * 2

    # Step 3
    for i in range(0, len(num), 2):
        checksum += int(reverse[i])

    # Return result
    return True if (checksum % 10 == 0) else False


if __name__ == "__main__":
    main()