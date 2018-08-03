typedef struct 
{
    int Valid;
    int Frame;
    int Dirty;
    int Requested;
    int TimeStamp;
} SharedVariable;

typedef SharedVariable* PageTable;
