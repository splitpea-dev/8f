#include "deck.h"

/* create deck */
struct deck* createDeck(void)
{
    int i, s;
    struct deck* d;

    /* create deck object on heap */
    d = (struct deck*) malloc(sizeof(struct deck));
    d->play_deck_size = DECK_SIZE;
    d->shuffle_counter = DECK_SIZE;

    /* create 52 cards on the heap */
    for(i = 0; i < DECK_SIZE; ++i)
        d->master_deck[i] = (struct card*) malloc(sizeof(struct card));

    /* populate cards with data */
    s = 0;
    for(i = 0; i < DECK_SIZE; ++i)
    {
        /* suit based on mod 13 */
        if((i > 0) && (i % 13 == 0))
            ++s;

        /* value 1 - 13 */
        d->master_deck[i]->value = (i % 13) + 1;

        /* suit H, S, D, and C */
        if(s == 0)
            d->master_deck[i]->suit = 'H';
        else if(s == 1)
            d->master_deck[i]->suit = 'S';
        else if(s == 2)
            d->master_deck[i]->suit = 'D'; 
        else
            d->master_deck[i]->suit = 'C';

        /* points */
        if(d->master_deck[i]->value == 8)
            d->master_deck[i]->points = 50;
        else if(d->master_deck[i]->value > 9)
            d->master_deck[i]->points = 10;
        else
            d->master_deck[i]->points = d->master_deck[i]->value;
    }

    /* NOTE: The master deck is designed to hold all the
       card addresses for easy deletion when deck is destroyed. */

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
            if(d->master_deck[i] != NULL)
                free(d->master_deck[i]);
        }

        /* free deck object from heap */
        free(d);
    }    
}

void resetDeck(struct deck* d)
{
    int i;

    for(i = 0; i < DECK_SIZE; ++i)
        d->play_deck[i] = d->master_deck[i];
    d->play_deck_size = DECK_SIZE;
    d->shuffle_counter = DECK_SIZE;
}

void shuffleDeck(struct deck* d)
{
    int r, i;
    struct card* temp_card;

    for(i = 0; i < d->play_deck_size; ++i)
    {
        r = (int) (rand() % d->play_deck_size);
        temp_card = d->play_deck[i];
        d->play_deck[i] = d->play_deck[r];
        d->play_deck[r] = temp_card;
    }

    /* shuffle card item positions between front marker and back marker */
}

struct card* drawCardFromDeck(struct deck* d)
{
    int i;
    struct card* c = d->play_deck[0];

    /* card pulled from front, shift everything forward */
    for(i = 0; i < d->play_deck_size; ++i)
        d->play_deck[i] = d->play_deck[i + 1];
    d->play_deck_size--;
    d->shuffle_counter--;
    if(d->shuffle_counter == 0)
    {
        printf("shuffling deck...");
        shuffleDeck(d);
        printf("done.\n");
        d->shuffle_counter = d->play_deck_size;
    }
    return c;
}

void returnCardToDeck(struct deck* d, struct card* c)
{
    d->play_deck[d->play_deck_size] = c;
    d->play_deck_size++;
}

void printDeck(struct deck* d)
{
    int i;

    for(i = 0; i < d->play_deck_size; ++i)
        printf("%3d%c", d->play_deck[i]->value, d->play_deck[i]->suit);
    
    printf("\ndeck-size:%3d   shuffle-count:%3d\n", d->play_deck_size, d->shuffle_counter);

}