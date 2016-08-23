#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "card.h"

#define DECK_SIZE       52

/* player data structure */
struct player
{
    int id;
    struct card* hand[DECK_SIZE];
    int hand_size;
    int hand_index;
    int point_sum;
    int score;
};

/* player function declarations */
struct player* createPlayer(int id);
void destroyPlayer(struct player* p);
void resetPlayer(struct player* p);
void resetPlayerScore(struct player* p);
void addCardToPlayerHand(struct player* p, struct card* c);
struct card* playCard(struct player* p, int index);
int pickBestPlay(struct player* p);
int getValidPlays(struct player* p, struct card* c, char wild_card_suit);
int hasWildCard(struct player* p);
struct card* AIPlayCard(struct player*p, struct card* c, char* wild_suit_mod);
void copyLeft(struct player* p, int index);

#endif