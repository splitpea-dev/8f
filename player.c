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
}

struct card* playCard(struct player* p, int index)
{
    struct card* c;

    /* return value */
    c = p->hand[index];

    /* copy left */
    copyLeft(p, index);
    return c;
}

int pickBestPlay(struct player* p)
{
    /* NEEDS TO BE WRITTEN */
    return 0;
}

int getValidPlays(struct player* p, struct card* c, char play_card_suit)
{
    int i;
    int plays;

    plays = 0;
    for(i = 0; i < p->hand_size; ++i)
    {
        if((p->hand[i]->value == c->value) || (p->hand[i]->suit == play_card_suit) || (p->hand[i]->value == 8))
            plays++;
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

struct card* AIPlayCard(struct player*p, struct card* c, char* play_card_suit)
{
	int index;
	int max, h_sum, s_sum, d_sum, c_sum, i;
	char s;
    struct card* out;

	index = hasWildCard(p); /* does the CPU have any eights? */
	if(index > -1)
	{
        printf("CPU has wild card to play!]\n");
		/* we will play the 8 but we need to choose a suit */
		max = h_sum = s_sum = d_sum = c_sum = 0;
		for(i = 0; i < p->hand_size; ++i)
		{		
			if((p->hand[i]->suit == 'H') && (p->hand[i]->value !=8))
				h_sum += p->hand[i]->points;
			if((p->hand[i]->suit == 'S') && (p->hand[i]->value !=8))
				s_sum += p->hand[i]->points;
			if((p->hand[i]->suit == 'D') && (p->hand[i]->value !=8))
				d_sum += p->hand[i]->points;
			if((p->hand[i]->suit == 'C') && (p->hand[i]->value !=8))
				c_sum += p->hand[i]->points;
		}

        /* suit with highest point total */ 
		max = h_sum;
		s = 'H';
		if(s_sum > max)
		{
			max = s_sum;
			s ='S';
		}
		if(d_sum > max)
		{
			max = d_sum;
			s ='D';
		}
		if(c_sum > max)
		{
			max = c_sum;
			s ='C';
		}
		*play_card_suit = s;
		out = p->hand[index];
        copyLeft(p, index);
	}
    else
    {
        max = 0;
        printf("CPU has no wild card to play!]\n");
	    /* if no 8 (aka wild card), play best card */
	    for(i = 0; i < p->hand_size; ++i)
        {
		    if(p->hand[i]->suit == (*play_card_suit))
            {
			    if(p->hand[i]->points > max)
		    	{
				    max = p->hand[i]->points;
				    index = i;
			    }
            }
        }
        if(index < 0)
        {
            printf("CPU had no same suit.\n");
	        /* if no 8 (aka wild card), play best card */
            i = 0;
	        while(index < 0 && i < p->hand_size)
            {
                if(p->hand[i]->value == c->value)
                    index = i;
                ++i;
            }    
        }    

        printf("CPU index chosen: %d.\n", index);

	    *play_card_suit = p->hand[index]->suit;
	    out = p->hand[index];
        copyLeft(p, index);
    }

    return out;
}

void copyLeft(struct player* p, int index)
{
    int i;

    /* copy left */
    for(i = index; i < p->hand_size; ++i)
        p->hand[i] = p->hand[i+1];
    p->hand_size--;
}

int getPlayerHandSum(struct player* p)
{
    int i, sum;

    sum = 0;
    for(i = 0; i < p->hand_size; ++i)
        sum+= p->hand[i]->points;
    
    return sum;
}