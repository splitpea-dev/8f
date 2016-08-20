#ifndef DECK_H
#define DECK_H

#include <stdio.h>
#include <stdlib.h>
#include "card.h"


#define DECK_SIZE       52
#define QD              13

/* deck data structure */
struct deck
{
    struct card* deck_model[DECK_SIZE];
    struct card* current_deck[DECK_SIZE];
    int fp, bp, bf; 
};

/* deck function declarations */
struct deck* createDeck(void);
void destroyDeck(struct deck* d);
void resetDeck(struct deck* d);
void shuffleDeck(struct deck* d);
struct card* drawCardFromDeck(struct deck* d);
void returnCardToDeck(struct deck* d, struct card* c);

#endif
