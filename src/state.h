#ifndef STATE_H_SEEN
#define STATE_H_SEEN

typedef struct StocktopState {
    int prevrow;
    int currow;
} StocktopState;

StocktopState *StocktopState_Create(void);
void StocktopState_Delete(StocktopState *state);

#endif
