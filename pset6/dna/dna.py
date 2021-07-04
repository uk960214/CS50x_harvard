# Find matching dna STR
import csv
import sys


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    STRs = []
    CODIS = []

    # Open csv file and append rows to CODIS as dict, STRs as list
    with open(sys.argv[1], "r") as csvfile:
        reader = csv.DictReader(csvfile)
        STRs = reader.fieldnames[1:]
        for row in reader:
            CODIS.append(row)

    dna = ''

    # Open txt file and save dna sequence
    with open(sys.argv[2], "r") as dnafile:
        dna = dnafile.readline()

    dnaSTR = {}
    count = 0

    # Count STRs in dna and save each as key and value in dict
    for STR in STRs:
        strlen = len(STR)
        temp = 0
        i = 0
        while i < len(dna):
            if(dna[i:i + strlen] == STR):
                count += 1
                i += strlen
            else:
                temp = count if(count > temp) else temp
                count = 0
                i += 1
        dnaSTR[STR] = temp

    # Search CODIS for match
    for p in CODIS:
        if(match_str(p, dnaSTR, STRs)):
            print(p['name'])
            return

    print("No match")
    return


# Match STRs of given person to sequence
def match_str(person, dna, STRs):
    for STR in STRs:
        if(int(person[STR]) == dna[STR]):
            continue
        else:
            return False
    return True


if __name__ == "__main__":
    main()