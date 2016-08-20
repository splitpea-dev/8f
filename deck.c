#include "deck.h"

/* create deck */
struct deck* createDeck(void)
{
    int i;
    struct deck* d;
    
    /* create deck object on heap */
    d = (struct deck*) malloc(sizeof(struct deck));

    /* create 52 card objects pointed to by deck model */
    for(i = 0; i < DECK_SIZE; ++i)
        d->deck_model[i] = (struct card*) malloc(sizeof(struct card));
    
    for(i = 0; i < QD; ++i)
    {
        /* hearts */
        d->deck_model[i]->value = d->deck_model[i]->points = i + 1;
        d->deck_model[i]->suit = 'H';

        /* spades */
        d->deck_model[i + QD]->value = d->deck_model[i + QD]->points = i + 1;
        d->deck_model[i + QD]->suit = 'S';

        /* diamonds */
        d->deck_model[i + (QD * 2)]->value = d->deck_model[i + (QD * 2)]->points = i + 1;
        d->deck_model[i + (QD * 2)]->suit = 'D';

        /* clubs */
        d->deck_model[i + (QD * 3)]->value = d->deck_model[i + (QD * 3)]->points = i + 1;
        d->deck_model[i + (QD * 3)]->suit = 'C';
    }

    /* correct point values for court cards and eights */
    for(i = 10; i < QD; ++i)
    {
        d->deck_model[i]->points =
            d->deck_model[i + QD]->points =
            d->deck_model[i + (QD * 2)]->points =
            d->deck_model[i + (QD * 3)]->points = 10;
    }
    d->deck_model[7]->points =
            d->deck_model[7 + QD]->points =
            d->deck_model[7 + (QD * 2)]->points =
            d->deck_model[7 + (QD * 3)]->points = 50;

    /* finally copy model deck pointers to game deck and initialize markers */
    resetDeck(d);

    return d;
}

/* destroy deck and free memory */
void destroyDeck(struct deck* d)
{
    int i;

    if(d != NULL)
    {
        /* free all card objects memory on heap */
        for(i = 0; i < DECK_SIZE; ++i)
        {
            if(d->deck_model[i] != NULL)
                free(d->deck_model[i]);
        }

        /* free deck object from heap */
        free(d);
    }    
}

void resetDeck(struct deck* d)
{
    int i;

    for(i = 0; i < DECK_SIZE; ++i)
        d->current_deck[i] = d->deck_model[i];
    d->fp = d->bf = 0;
    d->bp = DECK_SIZE - 1;
}

void shuffleDeck(struct deck* d)
{
    int r, i;
    int range;
    struct card* temp_card;

    range = ((d->bp + 1) - d->fp);
    for(i = d->fp; i < d->bp+1; ++i)
    {
        r = ((int) (rand() % range)) + d->fp;
        temp_card = d->current_deck[i];
        d->current_deck[i] = d->current_deck[r];
        d->current_deck[r] = temp_card;
    }

    /* shuffle card item positions between front marker and back marker */
}

struct card* drawCardFromDeck(struct deck* d)
{
    struct card* c = d->current_deck[d->fp];

    d->fp++;
    if(d->fp > d->bp)
    {
        d->fp = 0;
        d->bp = d->bf - 1;
        d->bf = 0;
        shuffleDeck(d);
    }
    
    return c;
}

void returnCardToDeck(struct deck* d, struct card* c)
{
    d->current_deck[d->bf] = c;
    d->bf++;
}