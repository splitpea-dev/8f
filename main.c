/*  
    8F
    by Brian Puthuff
    
    8F is a simple player vs CPU game of
    Crazy Eights that can be played on the
    terminal.

    Last updated: Sun Aug 28 23:39:16 PDT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "card.h"
#include "deck.h"
#include "player.h"

/* terminal color defines */
#define ESCN  "\x1B[0m"
#define ESCR  "\x1B[31;1m"
#define ESCB  "\x1B[34;1m"

/* function declarations */
void getLine(char buffer[]);
int isValidEntry(struct player* p, char buffer[]);
void printInformation(struct player* user, struct player* cpu, struct card* show_card, char play_card_suit);
void printShowCard(struct card* show_card, char play_card_suit);
void printHand(struct player* p);
void printCard(struct card* c);
char getWildCardSuit(void);

int main(int argc, char* argv[])
{
    /* SET-UP EVERYTHING */

    /* main structures */
    struct player* user;
    struct player* cpu;
    struct player* current_player; /* just a pointer to user or cpu */
    struct deck* game_deck;
    struct card* show_card;

    /* main variables */
    int is_playing, in_session;
    int index, valid_plays, i, score;
    char input_buffer[5];
    char c, play_card_suit;

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
        /* RESET GAME */

        /* reset players */
        resetPlayer(user);
        resetPlayer(cpu);

        /* reset scores */
        resetPlayerScore(user);
        resetPlayerScore(cpu);

        /* setup deck */
        resetDeck(game_deck);
        shuffleDeck(game_deck);

        /* deal each player 8 cards */
        for(i = 0; i < 8; ++i)
        {
            addCardToPlayerHand(user, drawCardFromDeck(game_deck));
            addCardToPlayerHand(cpu, drawCardFromDeck(game_deck));
        }

        /* get current play card */
        show_card = drawCardFromDeck(game_deck);

        /* and finally */
        current_player = user;
        play_card_suit = show_card->suit;
        
        /* turn loop */
        while(in_session)
        {
            /* display game information */
            printInformation(user, cpu, show_card, play_card_suit);

            /* does player have a valid move */
            do
            {
                /* if the current player has no valid plays, player must draw */
                if((valid_plays = getValidPlays(current_player, show_card, play_card_suit)) == 0)
                {
                    if(current_player == cpu)
                        printf("CPU");
                    else
                        printf("Player");
                    printf(" hand has no valid plays!\n");
                    if(current_player == cpu)
                        printf("CPU must draw a card, press ENTER to continue.");
                    else
                        printf("Press enter to draw a card.");
                    getchar();
                    addCardToPlayerHand(current_player, drawCardFromDeck(game_deck));
                    printInformation(user, cpu, show_card, play_card_suit);
                }
            } while(valid_plays == 0);

            /* play prompt */
            if(current_player == user)
            {
                do
                {
                    /* REPEAT UNTIL PLAYER ENTERS A VALID CARD! */
                    printf("Enter card to play (eg. 6H): ");
                    getLine(input_buffer);
                    index = isValidEntry(current_player, input_buffer);
                }
                while(index < 0);

                /* once a card index is selected, we need to make that the new show card */
                show_card = playCard(current_player, index); /* played card becomes new show_card */
                if(show_card->value == 8)
                    play_card_suit = getWildCardSuit(); /* prompt user for suit */
                else
                    play_card_suit = show_card->suit;            
            }
            else
            {
                /* CPU selects best play */
                printf("CPU has chosen to play... ");
                show_card = AIPlayCard(current_player, show_card, &play_card_suit);
                printCard(show_card);
                printf(".\nPress ENTER to continue. ");
                getchar();
            }
            returnCardToDeck(game_deck, show_card); /* return old show card to deck */
            
            /* check for no more cards in current player's hand */
            if((current_player->hand_size) == 0)
            {
                in_session = 0; /* this game is over */
                current_player->score+=score;
                printInformation(user, cpu, show_card, play_card_suit);
                
                printf("The game has ended.\n");
                
                if(current_player == cpu)
                {
                    score = getPlayerHandSum(user);
                    printf("CPU wins with %d points.\n", score);
                }
                else
                {
                    score = getPlayerHandSum(cpu);
                    printf("Player wins with %d points.\n", score);
                }
                /* if player's score is WINNING SCORE, game over */
                is_playing = 0;
                

                if(current_player->score >= WINNING_SCORE)
                {
                    /* NEEDS TO BE WRITTEN */
                    /* game over for winner */
                    /* play again */
                    printf("New game (Y/N)? ");
                    c = getchar();
                    if(c == 'N' || c =='n')
                        is_playing = 0;
                }
                else
                {
                    printf("Press enter to continue playing. ");
                    getchar();

                    /* RESET GAME */

                    /* reset players but not score */
                    resetPlayer(user);
                    resetPlayer(cpu);

                    /* setup deck */
                    resetDeck(game_deck);
                    shuffleDeck(game_deck);
                    printDeck(game_deck);

                    /* deal each player 8 cards */
                    for(i = 0; i < 8; ++i)
                    {
                        addCardToPlayerHand(user, drawCardFromDeck(game_deck));
                        addCardToPlayerHand(cpu, drawCardFromDeck(game_deck));
                    }

                    /* get current play card */
                    show_card = drawCardFromDeck(game_deck);

                    /* and finally */
                    current_player = user;
                    play_card_suit = show_card->suit;
                    is_playing = 1; /* overall game running bool */
                    in_session = 1; /* current game loop bool */
                }
            }
            else
            {
                /* switch player for next turn*/
                if(current_player == cpu)
                    current_player = user;
                else
                    current_player = cpu;
            }

        }
    }

    /* free memory before exiting */
    destroyPlayer(user);
    destroyPlayer(cpu);
    destroyDeck(game_deck);

    printf("Thanks for playing! =)\n\n");
    return 0;
}


void getLine(char buffer[])
{
    int i;
    
    fgets(buffer, 5, stdin);
    for(i = 0; i < 5; ++i)
        buffer[i] = toupper(buffer[i]);
}


int isValidEntry(struct player* p, char buffer[])
{
    int value, i;
    char suit;
    
    /* value evaluation */
    if(buffer[0] == '1')
    {
        /* see if 10 was entered */
        if(buffer[1] == '0')
                value = 10;
    }
    else if(buffer[0] == 'J')
        value = 11;
    else if(buffer[0] == 'Q')
        value = 12;
    else if(buffer[0] == 'K')
        value = 13;
    else if(buffer[0] == 'A')
        value = 1;
    else if((buffer[0] >= '2') && (buffer[0] <= '9'))
        value = buffer[0] - '0';   
    else
        return -1; /* failed on value entry */

    /* if first two chars were 1 and 0 then suit should be at buffer[2] otherwise buffer[1] */
    if(value == 10)
        i = 2;
    else
        i = 1;
        
    /* check for valid suit */
    if(buffer[i] != 'H' && buffer[i] != 'S' && buffer[i] != 'D' && buffer[i] != 'C')
        return -1;  /* failed on suit entry */
    else
        suit = buffer[i];

    /* finally, ensure card entry is actually in the hand */
    for(i = 0; i < p->hand_size; ++i)
    {
        if((p->hand[i]->value == value) && (p->hand[i]->suit == suit))
            return i;
    }

    return -1; /* if a valid index wasn't returned, something failed. */
}


void printInformation(struct player* user, struct player* cpu, struct card* show_card, char play_card_suit)
{
    /* clear screen and display scores */
    system("clear");
    printf("WINNING SCORE:        %6d\n", WINNING_SCORE);
    printf("PLAYER: %6d   ", user->score);
    printf("CPU: %6d\n\n", cpu->score);

    /* display show card */
    printShowCard(show_card, play_card_suit);
    printf("\n");

    /* display number of cards cpu has */
    printf("CPU's hand has %d card", cpu->hand_size);
    if(cpu->hand_size == 1)
        printf(".\n");
    else
        printf("s.\n");
    printHand(cpu);

    /* display player card/hand info */
    printf("Player's hand has %d cards (%d valid play", user->hand_size, getValidPlays(user, show_card, play_card_suit));
    if(getValidPlays(user, show_card, play_card_suit) == 1)
        printf(").\n");
    else
        printf("s).\n");    
    printHand(user);
    printf("\n");
}


void printShowCard(struct card* show_card, char play_card_suit)
{
    printf("PLAY >>> ");
    printCard(show_card);
    if(play_card_suit != show_card->suit)
    {
        printf(" --- SPECIAL SUIT: ");
        if(play_card_suit == 'H' || play_card_suit == 'D')
            printf(ESCR);
        else
            printf(ESCB);   
        printf("%c", play_card_suit);
        printf(ESCN);
        printf("\n");
    }
    else
        printf("\n");
}


void printHand(struct player* p)
{
    int i;

    /* print players hand*/
    for(i = 0; i < p->hand_size; ++i)
    {
        /* if CPU, just show card backs '###' */
        if(p->id == 0)
            printCard(p->hand[i]);
        else
            printf("###");
        
        if(i < p->hand_size - 1)
            printf(" ");
        else
            printf("\n");
    }
}


void printCard(struct card* c)
{
    switch(c->value)
    {
        case 1:
            printf(" A");
            break;
        case 10:
            printf("10");
            break;
        case 11:
            printf(" J");
            break;
        case 12:
            printf(" Q");
            break;
        case 13:
            printf(" K");
            break;
        default:
            printf(" %c", (c->value + '0'));
            break;
    }
    if(c->suit == 'H' || c->suit == 'D')
        printf(ESCR);
    else
        printf(ESCB);
    printf("%c", c->suit);
    printf(ESCN);
}


char getWildCardSuit()
{
    char c;
    do
    {
        printf("Enter wildcard suit (");
        printf(ESCR "H" ESCN ", ");
        printf(ESCB "S" ESCN ", ");
        printf(ESCR "D" ESCN ", or ");
        printf(ESCB "C" ESCN "): ");
        c = getchar();
        c = toupper(c);
        getchar(); /* enter */
    }
    while((c != 'H') && (c != 'S') && (c != 'D') && (c != 'C'));
    return c;
}
