/* EIGHT FREAK */
/* by Brian Puthuff */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"
#include "deck.h"
#include "player.h"

#define WINNING_SCORE   100
#define DECK_SIZE       52
#define QD              13
#define LIMIT           4

/* function declaration */
void getLine(char buffer[]);
int isValidEntry(struct player* p, char buffer[]);
void printInformation(struct player* p1, struct player* p2, struct card* c);
void printCard(struct card* c);

int main(int argc, char* argv[])
{
    /* set up everything */

    /* main structures */
    struct player* user;
    struct player* cpu;
    struct player* current_player;
    struct deck* game_deck;
    struct card* show_card;

    /* main variables */
    int is_playing, in_session;
    int index, valid_plays, i;
    char input_buffer[5];
    char c;

    /* create user and cpu players */
    user = createPlayer(0);
    cpu = createPlayer(1);

    /* create deck */
    game_deck = createDeck();

    /* main loop booleans */
    is_playing = 1; /* overall game running bool */
    in_session = 1; /* current game loop bool */

    /* initialize random seed */
    srand(time(NULL));

    /* program loop */
    while(is_playing)
    {
        /* initialize new game */
        current_player = user;
        resetPlayer(user);
        resetPlayer(cpu);

        /* setup deck */
        resetDeck(game_deck);
        shuffleDeck(game_deck);

        /* deal each player 8 cards */
        for(i = 0; i < 8; ++i)
        {
            addCardToPlayerHand(user, drawCardFromDeck(game_deck));
            addCardToPlayerHand(cpu, drawCardFromDeck(game_deck));
        }

        /* show card is card to play from */
        show_card = drawCardFromDeck(game_deck);

        /* display game information */
        printInformation(user, cpu, show_card);

        /* turn loop */
        while(in_session)
        {
            do
            {
                /* if the current player has no valid plays, player must draw */
                if((valid_plays = getValidPlays(current_player, show_card)) == 0)
                {
                    addCardToPlayerHand(current_player, drawCardFromDeck(game_deck));
                    printInformation(user, cpu, show_card);
                }
            } while(valid_plays == 0);
            
            /* play prompt */
            if(current_player == user)
            {
                printInformation(user, cpu, show_card);
                do
                {
                    /* REPEAT UNTIL PLAYER ENTERS A VALID CARD! */
                    /* NEEDS TO BE WRITTEN */
                    printf("Enter card to play (eg. 6H): ");
                    getLine(input_buffer);
                    index = isValidEntry(current_player, input_buffer);
                }
                while(index < 0);                    
            }
            else
            {
                /* CPU use limited AI to select best play */
                /* NEEDS TO BE WRITTEN */
                index = pickBestPlay(current_player);
            }

            /* once a card index is selected, we need to make that the new show card */
            returnCardToDeck(game_deck, show_card); /* return old show card to deck */
            show_card = playCard(current_player, 0); /* played card becomes new show_card */

            /* check for no more cards in current player's hand */
            if(current_player->hand_size == 0)
            {
                in_session = 0; /* this game is over */

                /* announce winner --- NEEDS TO BE WRITTEN */
                /* if player's score is WINNING SCORE, game over */

                if(current_player->score >= WINNING_SCORE)
                {
                    /* NEEDS TO BE WRITTEN */
                    /* game over for winner */
                    /* play again */
                    c = getchar();
                    if(c == 'N' || c =='n')
                        is_playing = 0;
                }
                else
                {
                    /* switch player for next turn*/
                    if(current_player->id == 0)
                        current_player = user;
                    else
                        current_player = cpu;
                }
            }

        }
    }

    /* free memory before exiting */
    destroyPlayer(user);
    destroyPlayer(cpu);
    destroyDeck(game_deck);

    printf("Exit.");
    return 0;
}

void getLine(char buffer[])
{
    int i;
    char c;

    i = 0;
    do
    {
        c = getchar();
        if(c != '\n')
            buffer[i] = c;
        ++i;
    } while((c != '\n') && (i < LIMIT - 1));
    buffer[LIMIT - 1] = '\0'; /* just sayin' */
}

int isValidEntry(struct player* p, char buffer[])
{
    int value, i;
    char suit;
    
    /* value evaluation */
    if(buffer[0] == '1')
    {
        if(buffer[1] == '0')
                value = 10;
    }
    else if((buffer[0] == 'J') || (buffer[0] == 'j'))
        value = 11;
    else if((buffer[0] == 'Q') || (buffer[0] == 'q'))
        value = 12;
    else if((buffer[0] == 'K') || (buffer[0] == 'k'))
        value = 13;
    else if((buffer[0] == 'A') || (buffer[0] == 'a'))
        value = 1;
    else if((buffer[0] >= '2') && (buffer[0] <= '9'))
        value = buffer[0] - '0';   
    else
        return -1; /* failed on value entry */

    if(value == 10)
        i = 2;
    else
        i = 1;
        
    if(buffer[i] == 'H' || buffer[i] == 'h')
        suit = 'H';
    else if(buffer[i] == 'S' || buffer[i] == 's')
        suit = 'S';
    else if(buffer[i] == 'D' || buffer[i] == 'd')
        suit = 'D';
    else if(buffer[i] == 'C' || buffer[i] == 'c')
        suit = 'C';
    else
        return -1;

    for(i = 0; i < p->hand_size; ++i)
    {
        if((p->hand[i]->value == value) && (p->hand[i]->suit == suit))
        {
            printf("value is a match! (%d)\n", i);
            return i;
        }
    }

    return -1;
}

void printInformation(struct player* p1, struct player* p2, struct card* c)
{
    int i;

    printf("WINNING SCORE: %d\n", WINNING_SCORE);
    printf("PLAYER: %6d\n", p1->score);
    printf("CPU: %6d\n\n", p2->score);
    printf("SHOW CARD: ");

    /* display show card */

    printCard(c);
    printf("\n");

    /* print cpu's hand --- DEBUG ONLY*/
    printf("CPU's Hand:\n");
    for(i = 0; i < p2->hand_size; ++i)
    {
        printCard(p2->hand[i]);
        if(i < p2->hand_size - 1)
            printf(" ");
        else
            printf("\n");
    }
    printf("CPU's Valid Plays: %d\n", getValidPlays(p2, c));

    /* print players hand*/
    printf("Player's Hand:\n");
    for(i = 0; i < p1->hand_size; ++i)
    {
        printCard(p1->hand[i]);
        if(i < p1->hand_size - 1)
            printf(" ");
        else
            printf("\n");
    }
    printf("Player's Valid Plays: %d\n", getValidPlays(p1, c));

}

/* print a card's value based on its structure data */
void printCard(struct card* c)
{
    switch(c->value)
    {
        case 1:
            printf("A");
            break;
        case 10:
            printf("10");
            break;
        case 11:
            printf("J");
            break;
        case 12:
            printf("Q");
            break;
        case 13:
            printf("K");
            break;
        default:
            printf("%c", c->value + '0');
            break;
    }
    printf("%c", c->suit);
}