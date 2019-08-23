
//
// Program for maintaining a personal team.
//
//
// Uses a linked list to hold the team players.
//
//Author: Taswar


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//**********************************************************************
// Linked List Definitions


struct Player {
    char *first_name, *family_name;
    char position;
    int value;
    struct Player *next;  // pointer to next player
};

//**********************************************************************
// Linked List Function Declarations
//


struct Player *createNewPlayer(char first_name[], char family_name[], char position, int value);
int insertPlayer(struct Player *p, struct Player **team);
int deletePlayerByLastName(char family_name[], struct Player **team);
int deleteTeam(struct Player **team);
int printTeam(struct Player *team);
int printPlayerByLastName(char family_name[], struct Player *team);
int printPlayerWithValueLessEqualTo(int value, struct Player *team);

//**********************************************************************
// Support Function Declarations
//

void safegets (char s[], int arraySize);        // gets without buffer overflow
void familyNameDuplicate (char familyName[]);   // marker/tester friendly
void familyNameFound (char familyName[]);       //   functions to print
void familyNameNotFound (char familyName[]);    //     messages to user
void familyNameDeleted (char familyName[]);
void printTeamEmpty (void);
void printTeamTitle(void);
void printNoPlayersWithLowerValue(int value);

//**********************************************************************
// Program-wide Constants
//

const int MAX_LENGTH = 1023;
const char NULL_CHAR = '\0';
const char NEWLINE = '\n';
const char GOALKEEPER = 'G';
const char DEFENDER = 'D';
const char MIDFIELDER = 'M';
const char STRIKER = 'S';

//**********************************************************************
// Main Program
//

int main (void)
{
    const char bannerString[]
            = "Personal Team Maintenance Program.\n\n";
    const char commandList[]
            = "Commands are I (insert), D (delete), S (search by name),\n"
                    "  V (search by value), P (print), Q (quit).\n";

    // Declare linked list head.
    struct Player *team = NULL;

    // announce start of program
    printf("%s",bannerString);
    printf("%s",commandList);

    char response;
    char input[MAX_LENGTH+1];
    do
    {
        printf("\nCommand?: ");
        safegets(input,MAX_LENGTH+1);
        // Response is first char entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I')
        {
            // Insert a player entry into the linked list.
            char family_name[MAX_LENGTH + 1];
            char first_name[MAX_LENGTH + 1];
            char val_string[MAX_LENGTH + 1], *pt;
            char pos_string[MAX_LENGTH + 1], position;
            int value;

            printf("  family name: ");
            safegets(family_name, MAX_LENGTH + 1);

            printf("  first name: ");
            safegets(first_name, MAX_LENGTH + 1);

            printf("  position: ");
            safegets(pos_string, MAX_LENGTH + 1);
            position = toupper(pos_string[0]);

            printf("  value: ");
            safegets(val_string, MAX_LENGTH + 1);
            value = (int) strtol(val_string, &pt, 10);

            struct Player *newPlayer = createNewPlayer(first_name, family_name, position, value);
            int result = insertPlayer(newPlayer, &team);
            // handling error
            switch(result) {
                case -1 :
                    printf("\nPlayer doesn't belong to either wings\n");
                    free(newPlayer);
                    break;
                case -2 :
                    familyNameDuplicate(family_name);
                    free(newPlayer);
                    break;
                default :
                    ;  // do nothing
            }

        }
        else if (response == 'D')
        {
            // Delete a player from the list.
            /* What happen if there are more than one player in the team,
             * but in different wings?
             */
            char family_name[MAX_LENGTH + 1];

            printf("\nEnter family name for entry to delete: ");
            safegets(family_name, MAX_LENGTH + 1);

            int result = deletePlayerByLastName(family_name, &team);

            if (result == 0) {
                familyNameDeleted(family_name);
            } else {
                familyNameNotFound(family_name);
            }
        }
        else if (response == 'S')
        {
            // Search for a player by family name.
            char family_name[MAX_LENGTH + 1];

            printf("\nEnter family name to search for: ");

            safegets(family_name, MAX_LENGTH + 1);

            int result = printPlayerByLastName(family_name, team);

            if (result != 0)  // when all print jobs came back empty
            {
                familyNameNotFound(family_name);
            }
        }
        else if (response == 'V')
        {
            // Search for players that are worth less than or equal a value.
            int value;

            printf("\nEnter value: ");

            char val_string[MAX_LENGTH + 1], *pt;
            safegets(val_string, MAX_LENGTH + 1);
            value = (int) strtol(val_string, &pt, 10);

            int result = printPlayerWithValueLessEqualTo(value, team);

            if (result != 0)  // when all print jobs came back empty
            {
                printNoPlayersWithLowerValue(value);
            }
        }
        else if (response == 'P')
        {
            // Print the team.

            int result = printTeam(team);

            if (result != 0)  // when all print jobs came back empty
            {
                printTeamEmpty();
            }

        }
        else if (response == 'Q')
        {
            ; // do nothing, we'll catch this case below
        }
        else
        {
            // do this if no command matched ...
            printf("\nInvalid command.\n%s\n",commandList);
        }
    } while (response != 'Q');

    // Delete the whole linked list that hold the team.
    deleteTeam(&team);

    // Print the linked list to confirm deletion.
    printTeam(team);

    return 0;
}

//**********************************************************************
// Support Function Definitions

// Function to get a line of input without overflowing target char array.
void safegets (char s[], int arraySize)
{
    int i = 0, maxIndex = arraySize-1;
    char c;
    while (i < maxIndex && (c = getchar()) != NEWLINE)
    {
        s[i] = c;
        i = i + 1;
    }
    s[i] = NULL_CHAR;
}

// Function to call when user is trying to insert a family name
// that is already in the book.
void familyNameDuplicate (char familyName[])
{
    printf("\nAn entry for <%s> is already in the team!\n"
                   "New entry not entered.\n",familyName);
}

// Function to call when a player with this family name was found in the team.
void familyNameFound (char familyName[])
{
    printf("\nThe player with family name <%s> was found in the team.\n",
           familyName);
}

// Function to call when a player with this family name was not found in the team.
void familyNameNotFound (char familyName[])
{
    printf("\nThe player with family name <%s> is not in the team.\n",
           familyName);

}

// Function to call when a family name that is to be deleted
// was found in the team.
void familyNameDeleted (char familyName[])
{
    printf("\nDeleting player with family name <%s> from the team.\n",
           familyName);
}

// Function to call when printing an empty team.
void printTeamEmpty (void)
{
    printf("\nThe team is empty.\n");
}

// Function to call to print title when whole team being printed.
void printTeamTitle (void)
{
    printf("\nMy Team: \n");
}

// Function to call when no player in the team has lower or equal value to the given value
void printNoPlayersWithLowerValue(int value)
{
    printf("\nNo player(s) in the team is worth less than or equal to <%d>.\n", value);
}

//**********************************************************************
// Add your functions below this line.

// - Helper functions

// gets player by family name
struct Player *_getPlayerByLastName(char family_name[], struct Player *team)
{
    struct Player *curr = NULL;

    for(curr = team; curr != NULL; curr = curr->next) {
        if (strcmp(curr->family_name, family_name) == 0) {
            return curr;
        }
    }

    return NULL;
}


struct Player *_getLastPlayerOfPosition(char position, struct Player *team)
{
    struct Player *prev = NULL;
    struct Player *curr = NULL;
    int targetFound = 0;

    for(curr = team; curr != NULL; prev = curr, curr = curr->next) {
        if (targetFound == 0 && curr->position == position) {
            targetFound = 1;
        }

        if (targetFound == 1) {
            if (curr->position != position) {
                return prev;
            } else if (curr->next == NULL) {
                return curr;
            }
        }
    }

    return NULL;
}

//prints player details
int _printPlayer(struct Player *p)
{
    if (p != NULL) {
         printf("\n%s\n%s\n%c\n%d\n", p->family_name, p->first_name, p->position, p->value);
        return 0;
    } else {
        // when player is NULL
        return -1;
    }

}

// - Main functions
struct Player *createNewPlayer(char first_name[], char family_name[], char position, int value)
{
    struct Player *p = malloc(sizeof(struct Player));

    p->first_name = (char *) malloc(sizeof(char) * strlen(first_name));
    strcpy(p->first_name,first_name);
    p->family_name = (char *) malloc(sizeof(char) * strlen(family_name));
    strcpy(p->family_name, family_name);
    p->position = position;
    p->value = value;
    p->next = NULL;

    return p;
}

int insertPlayer(struct Player *p, struct Player **team)
{
    struct Player *player = _getPlayerByLastName(p->family_name, *team);  // Look for player w/ the same last name

    if (player != NULL) {
        return -2;  // if found player w/ same last name, return error code -2 (duplicate family name)
    }

    // G -> D -> M -> S
    struct Player *gLast = _getLastPlayerOfPosition(GOALKEEPER, *team);
    struct Player *dLast = _getLastPlayerOfPosition(DEFENDER, *team);
    struct Player *mLast = _getLastPlayerOfPosition(MIDFIELDER, *team);
    struct Player *sLast = _getLastPlayerOfPosition(STRIKER, *team);

    if (p->position == GOALKEEPER) {
        if (gLast == NULL) {  // if there's no G in the team, it will be the first node of the list
            p->next = *team;
            *team = p;
        } else {  // when found, attach it to the last of the goalkeeper list
            struct Player *restOfTeam = gLast->next;
            p->next = restOfTeam;
            gLast->next = p;
        }
        return 0;
    } else if (p->position == DEFENDER) {
        if (dLast == NULL) {  // if there's no D in the team
            if (gLast != NULL) {  // fallback: look for G
                struct Player *restOfTeam = gLast->next;
                p->next = restOfTeam;
                gLast->next = p;
            } else {
                p->next = *team;
                *team = p;
            }
        } else {
            struct Player *restOfTeam = dLast->next;
            p->next = restOfTeam;
            dLast->next = p;
        }
        return 0;
    } else if (p->position == MIDFIELDER) {
        if (mLast == NULL) {  // if no M in the team
            if (dLast != NULL) {  // fallback 1: look for D
                struct Player *restOfTeam = dLast->next;
                p->next = restOfTeam;
                dLast->next = p;
            } else if (gLast != NULL) {  // fallback 2. look for G
                struct Player *restOfTeam = gLast->next;
                p->next = restOfTeam;
                gLast->next = p;
            } else {
                p->next = *team;
                *team = p;
            }
        } else {
            struct Player *restOfTeam = mLast->next;
            p->next = restOfTeam;
            mLast->next = p;
        }
        return 0;
    } else if (p->position == STRIKER) {
        if (sLast == NULL) {  // if no M in the team
            if (mLast != NULL) {  // fallback 1: look for M
                struct Player *restOfTeam = mLast->next;
                p->next = restOfTeam;
                mLast->next = p;
            } else if (dLast != NULL) {  // fallback 2: look for D
                struct Player *restOfTeam = dLast->next;
                p->next = restOfTeam;
                dLast->next = p;
            } else if (gLast != NULL) {  // fallback 3. look for G
                struct Player *restOfTeam = gLast->next;
                p->next = restOfTeam;
                gLast->next = p;
            } else {
                p->next = *team;
                *team = p;
            }
        } else {
            struct Player *restOfTeam = sLast->next;
            p->next = restOfTeam;
            sLast->next = p;
        }
        return 0;
    }

    return -1;
}


int deletePlayerByLastName(char family_name[], struct Player **team)
{
    struct Player *prev = NULL;
    struct Player *curr = *team;

    while (curr != NULL) {
        if (strcmp(curr->family_name, family_name) == 0) {
            if (prev == NULL) {
                *team = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr);
            return 0;
        }

        prev = curr;
        curr = curr->next;
    }

    return -1;  // when no player with this last name is found return error code -1 (family name not found)


}

int deleteTeam(struct Player **team)
{
    struct Player *curr = *team;
    struct Player *next = NULL;

    while(curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }

    *team = NULL;

    return 0;
}

int printTeam(struct Player *team)
{
    if (team == NULL)
    {
        printTeamEmpty();

        return -1;
    }
    else
    {
        printTeamTitle();

        struct Player *curr = NULL;

        for(curr = team; curr != NULL; curr = curr->next)
        {
            _printPlayer(curr);
        }

        return 0;
    }

}

int printPlayerByLastName(char family_name[], struct Player *team)
{
    struct Player *p = _getPlayerByLastName(family_name, team);
    if (p != NULL)
    {
        familyNameFound(family_name);
        _printPlayer(p);
        return 0;
    }
    else  // can't find the player
    {
        return -1;
    }

}

int printPlayerWithValueLessEqualTo(int value, struct Player *team)
{
    struct Player *curr = NULL;
    int result = -1;

    for(curr = team; curr != NULL; curr = curr->next)
    {
        if (curr->value <= value)
        {
            result = _printPlayer(curr);  // if found any player that matches, set the return value to 0
        }
    }

    return result;
}
