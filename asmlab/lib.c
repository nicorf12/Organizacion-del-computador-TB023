#include "lib.h"

funcCmp_t *getCompareFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcCmp_t *)&intCmp;
        break;
    case TypeString:
        return (funcCmp_t *)&strCmp;
        break;
    case TypeCard:
        return (funcCmp_t *)&cardCmp;
        break;
    default:
        break;
    }
    return 0;
}
funcClone_t *getCloneFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcClone_t *)&intClone;
        break;
    case TypeString:
        return (funcClone_t *)&strClone;
        break;
    case TypeCard:
        return (funcClone_t *)&cardClone;
        break;
    default:
        break;
    }
    return 0;
}
funcDelete_t *getDeleteFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcDelete_t *)&intDelete;
        break;
    case TypeString:
        return (funcDelete_t *)&strDelete;
        break;
    case TypeCard:
        return (funcDelete_t *)&cardDelete;
        break;
    default:
        break;
    }
    return 0;
}
funcPrint_t *getPrintFunction(type_t t)
{
    switch (t)
    {
    case TypeInt:
        return (funcPrint_t *)&intPrint;
        break;
    case TypeString:
        return (funcPrint_t *)&strPrint;
        break;
    case TypeCard:
        return (funcPrint_t *)&cardPrint;
        break;
    default:
        break;
    }
    return 0;
}

/** Int **/

int32_t intCmp(int32_t *a, int32_t *b)
{
    if (*a == *b) return 0;
    return (*a < *b ? 1:-1);
}

void intDelete(int32_t *a)
{
    free(a);
    a = NULL;
}

void intPrint(int32_t *a, FILE *pFile)
{
    fprintf(pFile, "%i", *a);
}

int32_t *intClone(int32_t *a)
{
    int32_t* b = malloc(sizeof(int32_t));
    *b = *a;
    return b;
}

/** Lista **/

list_t *listNew(type_t t)
{
    list_t* list = (list_t*)malloc(sizeof(list_t));
    list->type = t;
    list->size = 0;
    list->first = 0;
    list->last = 0;
    return list;
}

uint8_t listGetSize(list_t *l)
{
    return l->size;
}

void *listGet(list_t *l, uint8_t i)
{
    if (i >= l->size || l->first == 0) {
        return 0;
    }
    listElem_t* current = l->first;
    for (uint8_t j = 0; j < i ; j++) {
        current = current->next;
    }
    return current->data;
}

void listAddFirst(list_t *l, void *data)
{
    listElem_t* element = (listElem_t*)malloc(sizeof(listElem_t));
    void* newData = getCloneFunction(l->type)(data);
    element->data = newData;
    element->next = l->first;
    element->prev = 0;

    if (l->size == 0) {
        l->last = element;
    }
    l->first = element;
    l->size = l->size + 1;
}

void listAddLast(list_t *l, void *data)
{
    listElem_t* element = (listElem_t*)malloc(sizeof(listElem_t));
    void* newData = getCloneFunction(l->type)(data);
    element->data = newData;
    element->next = 0;
    element->prev = l->last;

    if (l->size == 0) {
        l->first = element;
    } else {
        (l->last)->next = element;
    }
    l->last = element;
    l->size = l->size + 1;
}

list_t *listClone(list_t *l)
{
    list_t* newList = listNew(l->type);

    listElem_t* current = l->first;
    while (current != 0) {
        listAddLast(newList,current->data);
        current = current->next;
    }
    return newList;
}

void *listRemove(list_t *l, uint8_t i)
{
    if (i >= l->size) {
        return 0;
    }

    listElem_t* current = l->first;
    for (uint8_t j = 0; j < i ; j++) {
        current = current->next;
    }

    if (current->prev == 0){
        l->first = current->next;
    } else{
        (current->prev)->next = current->next;
    }

    if (current->next == 0) {
        l->last = current->prev;
    } else {
        (current->next)->prev = current->prev;
    }

    void* currentData = current->data;
    free(current);
    l->size = l->size - 1;
    return currentData;
}

void listSwap(list_t *l, uint8_t i, uint8_t j) 
{
    if (i >= l->size || j >= l->size || i == j || l->first == NULL || l->last == NULL) {
        return;
    }

    listElem_t *elementI = l->first;
    listElem_t *elementJ = l->first;

    for (uint8_t n = 0; n < i; n++) {
        elementI = elementI->next;
    }

    for (uint8_t n = 0; n < j; n++) {
        elementJ = elementJ->next;
    }

    if (elementI == NULL || elementJ == NULL) {
        return;
    }

    void *tempData = elementI->data;
    elementI->data = elementJ->data;
    elementJ->data = tempData;
}



void listDelete(list_t *l)
{
    listElem_t* current = l->first;
    listElem_t* aux;
    while (current != 0) {
        getDeleteFunction(l->type)(current->data);
        aux = current->next;
        free(current);
        current = aux;
    }
    free(l);
}

void listPrint(list_t *l, FILE *pFile)
{
    fprintf(pFile, "[");
    listElem_t* current = l->first;
    while (current != 0) {
        getPrintFunction(l->type)(current->data,pFile);
        current = current->next;
        if (current != 0) {
            fprintf(pFile, ",");
        }
    }
    fprintf(pFile, "]");
}

/** Game **/

game_t *gameNew(void *cardDeck, funcGet_t *funcGet, funcRemove_t *funcRemove, funcSize_t *funcSize, funcPrint_t *funcPrint, funcDelete_t *funcDelete)
{
    game_t *game = (game_t *)malloc(sizeof(game_t));
    game->cardDeck = cardDeck;
    game->funcGet = funcGet;
    game->funcRemove = funcRemove;
    game->funcSize = funcSize;
    game->funcPrint = funcPrint;
    game->funcDelete = funcDelete;
    return game;
}
int gamePlayStep(game_t *g)
{
    int applied = 0;
    uint8_t i = 0;
    while (applied == 0 && i + 2 < g->funcSize(g->cardDeck))
    {
        card_t *a = g->funcGet(g->cardDeck, i);
        card_t *b = g->funcGet(g->cardDeck, i + 1);
        card_t *c = g->funcGet(g->cardDeck, i + 2);
        if (strCmp(cardGetSuit(a), cardGetSuit(c)) == 0 || intCmp(cardGetNumber(a), cardGetNumber(c)) == 0)
        {
            card_t *removed = g->funcRemove(g->cardDeck, i);
            cardAddStacked(b, removed);
            cardDelete(removed);
            applied = 1;
        }
        i++;
    }
    return applied;
}
uint8_t gameGetCardDeckSize(game_t *g)
{
    return g->funcSize(g->cardDeck);
}
void gameDelete(game_t *g)
{
    g->funcDelete(g->cardDeck);
    free(g);
}
void gamePrint(game_t *g, FILE *pFile)
{
    g->funcPrint(g->cardDeck, pFile);
}