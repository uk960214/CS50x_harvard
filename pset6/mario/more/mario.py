# Ask user for valid input
while True:
    n = input("Height: ")
    try:
        val = int(n)
        if val > 0 and val < 9:
            break
    except ValueError:
        continue

# Loop and draw blocks
for i in range(int(n)):
    print(" " * (int(n) - i - 1) + "#" * (i + 1) + "  " + "#" * (i + 1))