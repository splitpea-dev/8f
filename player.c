#include "player.h"

/* constructor */
struct player* createPlayer(int id)
{
    struct player* p = (struct player*) malloc(sizeof(struct player));
    p->id = id;
    resetPlayer(p);
    resetPlayerScore(p);
    return p;
}

/* destructor */
void destroyPlayer(struct player* p)
{
    if(p != NULL)
        free(p);
}

/* initialize all but id and score */
void resetPlayer(struct player* p)
{
    p->hand_size = 0;
    p->hand_index = 0;
    p->point_sum = 0;
}

/* reset score */
void resetPlayerScore(struct player* p)
{
    p->score = 0;
}

void addCardToPlayerHand(struct player* p, struct card* c)
{
    p->hand[p->hand_size] = c;
    p->hand_index++;
    p->hand_size++;
    p->point_sum+= c->points;
}

struct card* playCard(struct player* p, int index)
{
    int i;
    struct card* c;

    /* return value */
    c = p->hand[index];

    /* copy left */
    for(i = index; i < p->hand_size; ++i)
        p->hand[i] = p->hand[i+1];
    p->hand_size--;

    return c;
}

int pickBestPlay(struct player* p)
{
    /* NEEDS TO BE WRITTEN */
    return 0;
}

int getValidPlays(struct player* p, struct card* c, char wild_card_suit)
{
    int i;
    int plays;

    plays = 0;
    if(wild_card_suit == c->suit)
    {
        for(i = 0; i < p->hand_size; ++i)
        {
            if((p->hand[i]->value == c->value) || (p->hand[i]->suit == c->suit) || (p->hand[i]->value == 8))
                plays++;
        }
    }
    else
    {
        for(i = 0; i < p->hand_size; ++i)
        {
            if((p->hand[i]->suit == wild_card_suit) || (p->hand[i]->value == 8))
                plays++;
        }
    }
    return plays;
}

int hasWildCard(struct player* p)
{
	int i;
	for(i = 0; i < p->hand_size; ++i)
		if(p->hand[i]->value == 8)
			return i;

	return -1;
}