from cs50 import get_string

# Get string from user
p = get_string("Text: ")

# Initialize count
l = 0
w = 1
s = 0

# Count letters, words, sentences
for i in range(len(p)):
    if p[i].isalpha():
        l += 1
    elif ord(p[i]) == 32:
        w += 1
    elif ord(p[i]) == 33 or ord(p[i]) == 46 or ord(p[i]) == 63:
        s += 1

# Calculate Colman-Liau index
index = round(0.0588 * (l / w * 100) - 0.296 * (s / w * 100) - 15.8)

# Print result
if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {index}")