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
void printInformation(struct player* p1, struct player* p2, struct card* c, char wild_card_suit);
void printShowCard(struct card* c, char wild_card_suit);
void printHand(struct player* p);
void printCard(struct card* c);
char getWildCardSuit(void);

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
        play_card_suit = show_card->suit;
        
        /* display game information */
        printInformation(user, cpu, show_card, play_card_suit);

        /* turn loop */
        while(in_session)
        {
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
                        printf("Press enter to draw a card.\n");
                    getchar();
                    addCardToPlayerHand(current_player, drawCardFromDeck(game_deck));
                    printHand(current_player);
                }
            } while(valid_plays == 0);


            printInformation(user, cpu, show_card, play_card_suit);

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
                    play_card_suit = getWildCardSuit();
                else
                    play_card_suit = show_card->suit;            
            }
            else
            {
                /* CPU use limited AI to select best play */
                /* NEEDS TO BE WRITTEN */
                printf("AI check.\n");
                show_card = AIPlayCard(current_player, show_card, &play_card_suit);
            }
            returnCardToDeck(game_deck, show_card); /* return old show card to deck */
            /* check for no more cards in current player's hand */
            if((current_player->hand_size) == 0)
            {
                in_session = 0; /* this game is over */
                printf("The game has ended.");
                if(current_player == cpu)
                    printf("CPU wins with %d points.\n", getPlayerScore(user));
                else
                    printf("Player wins with %d points.\n", getPlayerScore(cpu));
                /* if player's score is WINNING SCORE, game over */
                is_playing = 0;
                
                if(current_player->score >= WINNING_SCORE)
                {
                    /* NEEDS TO BE WRITTEN */
                    /* game over for winner */
                    /* play again */
                    c = getchar();
                    if(c == 'N' || c =='n')
                        is_playing = 0;
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

void printInformation(struct player* p1, struct player* p2, struct card* c, char wild_card_suit)
{
    /*
    printf("WINNING SCORE: %d\n", WINNING_SCORE);
    printf("PLAYER: %6d\n", p1->score);
    printf("CPU: %6d\n\n", p2->score);
    printf("SHOW CARD: ");
    */

    /* display show card */
    printShowCard(c, wild_card_suit);

    /* print cpu's hand --- DEBUG ONLY */
    printf("CPU's hand has %d cards:\n", p2->hand_size);
    printHand(p2);
    
    printf("Player's hand has %d cards (%d valid plays):\n", p1->hand_size, getValidPlays(p1, c, wild_card_suit));
    printHand(p1);
    printf("\n");

}

/*print show card */
void printShowCard(struct card* c, char wild_card_suit)
{
    printf("PLAY >>> ");
    printCard(c);
    if(wild_card_suit != c->suit)
        printf(" --- SPECIAL SUIT: %c\n", wild_card_suit);
    else
        printf("\n");
}

/*print player's hand */
void printHand(struct player* p)
{
    int i;

    /* print players hand*/
    for(i = 0; i < p->hand_size; ++i)
    {
        /*
        if(p->id == 0)*/
            printCard(p->hand[i]);
        /* else
            printf("###");*/
        if(i < p->hand_size - 1)
            printf(" ");
        else
            printf("\n");
    }
}

/* print a card's value based on its structure data */
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
            printf(" %c", c->value + '0');
            break;
    }
    printf("%c", c->suit);
}

char getWildCardSuit()
{
    char c;
    do
    {
        printf("Enter wildcard suit (H, S, D, or C): ");
        c = getchar();
        getchar(); /* enter */
    }
    while((c != 'H' && c != 'h') && (c != 'S' && c != 's') && (c != 'D' && c != 'd') && (c != 'C' && c != 'c'));

    return c;
}