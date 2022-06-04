#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;  //the index of the winner in candidates
    int loser;  //the index of the loser in candidates
}
pair;

// Array of candidates
string winner;
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
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
        for(int v=0; v<candidate_count; v++)
        {
            for(int vy=0; vy<candidate_count; vy++)
            {
                preferences[v][vy]=0;
            }
        }
        record_preferences(ranks);
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
    // TODO
    for(int k=0; k<candidate_count; k++)
    {
        if(strcmp(candidates[k],name) == 0)
        {
            ranks[rank]=k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for(int i=0; i<candidate_count; i++)
    {
        for(int j=0; j<candidate_count; j++)
        {
            int firstIndex= 0;
            int secondIndex= 0;
            if(i!=j)
            {
                for(int k=0; k<candidate_count; k++)
                {
                    if(ranks[k]==i)
                    {
                        firstIndex= k;
                    }
                }
                for(int l=0; l<candidate_count; l++)
                {
                    if(ranks[l]==j)
                    {
                        secondIndex= l;
                    }
                }
                if(firstIndex>secondIndex)
                {
                    preferences[j][i]++;
                }
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for(int i=0; i<candidate_count; i++)
    {
        for(int ij=0; ij<candidate_count; ij++)
        {
            for(int ijk=0; ijk<candidate_count; ijk++)
            {
                if(preferences[ij][ijk]>preferences[ijk][ij])
                {
                    pairs[i].winner= ij;
                    pairs[i].loser=ijk;
                    pair_count ++;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for(int i=0; i<candidate_count; i++)
    {
        for(int j=i; j<candidate_count; j++)
        {
            int maxmax=0;
            if(preferences[pairs[j].winner][pairs[j].loser]-preferences[pairs[j].loser][pairs[j].winner]>maxmax)
            {
                pairs[i]=pairs[j];
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    //draw the arrows using the locked array
    int ab = pair_count;
    for(int i=0; i<pair_count; i++)
    {
        if(ab>1)
        {
            locked[pairs[i].winner][pairs[i].loser]=true;
            ab--;
        }
        else if(ab==1)
        {
            int variable=0;
            for(int j=0; j<candidate_count; j++)
            {
                if(locked[j][pairs[i].loser]==true)
                {
                    variable=1;
                    locked[pairs[i].winner][pairs[i].loser]=true;
                }
            }
            ab--;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    //the person with no arrow towards to itself is the winner
    for(int i=0; i<candidate_count; i++)
    {
        int now=0;
        for(int j=0; j<candidate_count; j++)
        {
            if(locked[j][pairs[i].winner]==true)
            {
                now++;
            }
        }
        if(now==0)
        {
            winner=candidates[i];
        }
    }
    printf("The winner is %s.\n",winner);
}