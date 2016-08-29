#ifndef CARD_H
#define CARD_H

/*  since this file is included in all the key
    structure and function files, including main
    these defines will are placed here. */

#define WINNING_SCORE   100
#define DECK_SIZE       52
#define QD              13
#define LIMIT           4

/* card data structure */
struct card
{
    int value;
    char suit;
    int points;
};

#endif