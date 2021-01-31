#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
// Additional function to check for cycles
bool check_cycle(int loser, int init);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Check if input name matches one of the candidate names
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // If match is found, record rank in ranks array
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Record preference, starting from rank 1, loop until second to last
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // For each rank, every name that ranks lower is recorded as less preferred
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Compare preference for each opposite pair
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // If the score is not tied, add 1 to pair count value, record winner and loser to pair array
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                k++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                k++;
            }
        }
    }
    // Record pair count
    pair_count = k;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Assign variable to hold values while swapping pairs
    pair temp;
    int swaps;

    // Swap and sort until no more swap is needed
    do
    {
        swaps = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            // Swap places if i is smaller than i + 1
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                swaps ++;
            }
        }
    }
    while (swaps != 0);
    return;
}

// Check if locking this pair creates a cycle
bool check_cycle(int loser, int init)
{
    // If the inital value becomes a loser at some point, return true
    if (loser == init)
    {
        return true;
    }

    // Check all candidates, if the loser input is winner to any other candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            // If so, recursively check for cycle, until ultimately, it reaches true or false
            if (check_cycle(i, init))
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Only lock pairs if check cycle returns true
        if (check_cycle(pairs[i].loser, pairs[i].winner) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Set an indicator to determine whether to proceed or not
        bool winner_not_found = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                // If locked as loser, i is not winner, thus winner is not found
                winner_not_found = true;
                // Break out of the nested loop
                break;
            }
        }
        // If during the loop, the winner has been found and the boolean never set to true, print the current candidate
        if (!winner_not_found)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        else
        {
            continue;
        }
    }
}

