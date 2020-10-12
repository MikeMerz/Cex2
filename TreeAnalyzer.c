// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <ctype.h>
// ------------------------------ cost define ------------------------------
#define LEGAL_SIGNS " \n\r"
#define USAGE_ERROR "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"
#define MAX_LINE_SIZE 1024
#define LEGAL_FORMAT "%[^\n]"
#define FAILURE -1
#define INLVALID_OUTPUT "Invalid input\n"
#define LEAF "-"
#define LEGAL_KEY_VALUE "%[0-9]"
#define LEGAL_FORMAT_KEY "%[0-9]^\n\r"
#define VALID_NUMBER_ARGS 4
#define INFINITY -1
#define MINIMUM_FORMAT_OUTPUT "Length of Minimal Branch: %d\n"
#define MAXIMUM_FORMAT_OUTPUT "Length of Maximal Branch: %d\n"
#define DIAMETER_FORMAT_OUTPUT "Diameter Length: %d\n"
#define ROOT_FORMAT_OUTPUT "Root Vertex: %d\n"
#define VERTI_FORMAT_OUTPUT "Vertices Count: %d\n"
#define SHORT_PATH "Shortest Path Between "
#define FIRST_ARGU "%d and "
#define SECOND_ARGU "%d: "
#define SECOND_FORMAT_OUTPRINT "%d "
#define OUTPRINT_WITH_NEWLINE "%d\n"
#define EDGES_FORMAT_OUTPRINT "Edges Count: %d\n"
#define NUMBER_SPACES_ALLOWED 1
// ------------------------------ structs -----------------------------

/**
 * Struct for a single Node
 */
typedef struct Node
{
    int key;
    int numOfSons;
    int dis;
    int pre;
    struct Node *father;
    struct Node **sons;
} Node;
/**
 * Struct for a single Tree
 */
typedef struct Tree
{
    int numOfVortex;
    Node *arrayOfNodes;
} Tree;
// ------------------------------ functions -----------------------------
/**
 * Checks if the given line are valid.
 * @param tree 
 * @param First line of the txt file
 * @return 1 if the input is valid 0 otherwise
 */
int checkInput(Tree *tree , char *line);

/**
 * 
 * @param Amount of argu given
 * @return 0 if the amount is incorrect 1 otherwise
 */
int checkFirstInput(int argc);

/**
 * 
 * @param Arguments given 
 * @param First line of txt
 * @return 1 if valid 0 otherwise
 */
int checkValidVor(char *argv[] , char *line);

/**
 * Init the main tree
 * @param Number of Vortex
 * @return Returns the pointer of the tree
 */
Tree *startTree(int n);

/**
 * BFS search algorithem in order to find the longest and shortest path
 * @param Main tree 
 * @param Node at the end of the path 
 */
void BFS(Tree *tree , Node v);

/**
 * Prints all information needed
 * @param Main tree
 * @param First Node the path starts from 
 * @param Last Node the path end with
 */
void printPath(Tree *tree , Node u , Node v);

/**
 * Find the longest and shortest paths
 * @param Main tree 
 */
void highLowPath(Tree *tree);

/**
 * Frees all the nodes and the main tree
 * @param Main tree
 */
void freeTree(Tree *tree);

/**
 * Prints the root of the tree
 * @param Main tree 
 */
void printRoot(Tree *tree);

/**
 * Prints the amount of Vertices the tree is made of
 * @param Main tree
 */
void printVerCount(Tree *tree);

/**
 * Outpus the correct error and frees all the memory and closes the file
 * @param Main tree 
 */
void closeProgram(Tree *tree);

/**
 * Outputs the correct error and closes the file (before tree is init)
 * @param Main text file
 * @return EXIT_FAILURE
 */
int closeBeforeInit(FILE *file);

/**
 * Outputs the correct error and closes the file (after tree is init)
 * @param Main file
 * @param Main tree 
 * @return EXIT_FAILURE
 */
int finishAndExit(FILE *file , Tree *tree);

/**
 * Prints all of the information about the tree
 * @param U and V Nodes
 * @param Main  tree 
 */
void printAllInformation(char **argv , Tree *tree);

/**
 * Creates the memory needed for the sons and changes information as needed
 * @param Main Tree
 * @param Key of Father 
 * @param Number of sons 
 */
void loadInfo(Tree *tree , int lineCounter , int curDeg);

/**
 * Checks if the line given is valid
 * @param Main tree
 * @param Legal signs 
 * @param Number of Sons 
 * @param Char to check 
 * @return -1 if line is not valid number of sons otherwise
 */
int checkSingleLine(Tree *tree , char *legalBetween , int curDeg ,
                    char *token);

/**
 * Creates the pointer between the sons and fathers
 * @param Main file 
 * @param Main Tree 
 * @param Legal signs 
 * @param Key value 
 * @param Son to check 
 * @return 0 if successful -1 otherwise
 */
int singleLineConnect(FILE *file , Tree *tree , char *legalBetween , int lineCounter ,
                      char *token);

/**
 * Function to print all the information free the memory and close the file if tree was valid
 * @param U and V values 
 * @param Main file
 * @param Main tree 
 */
void endRun(char **argv , FILE *file , Tree *tree);

int main(int argc , char *argv[])
{
    FILE *file;
    Tree *tree;
    char legalBetween[] = LEGAL_SIGNS;
    char line[MAX_LINE_SIZE];
    int lineCounter = 0;
    if (checkFirstInput(argc) == 0)
    {
        fprintf(stderr , USAGE_ERROR);
        return EXIT_FAILURE;
    }
    file = fopen(argv[1] , "r");
    if (file == NULL) // checks if file exists
    {
        fprintf(stderr , USAGE_ERROR);
        return EXIT_FAILURE;
    }
    if (fgets(line , MAX_LINE_SIZE , file) != NULL) //checks if the first line is valid
    {
        char *token = strtok(line , legalBetween);
        if (token == NULL || strcmp(line , "\n") == 0 || checkValidVor(argv , token) == 0)
        {
            return closeBeforeInit(file);
        }
        else
        {
            int tokenCounter = 0;
            while (token != NULL) //checks every char in the line
            {
                token = strtok(NULL , legalBetween);
                tokenCounter ++;
            }
            if (tokenCounter > NUMBER_SPACES_ALLOWED)
            {
                return closeBeforeInit(file);
            }
            if ((int) strtol(line , NULL , 10) > 0)
            {
                tree = startTree((int) strtol(line , NULL , 10)); // Init tree
                if (tree == NULL)
                {
                    closeBeforeInit(file);
                }
            }
            else
            {
                return closeBeforeInit(file);
            }
        }

    }
    else
    {
        return closeBeforeInit(file);
    }
    while (fgets(line , MAX_LINE_SIZE , file) != NULL)  // checks the rest of the file & counts sons
    {
        int curDeg = 0;
        char *token = strtok(line , legalBetween);
        if (token == NULL)
        {
            return finishAndExit(file , tree);
        }
        curDeg = checkSingleLine(tree , legalBetween , curDeg , token);
        if (curDeg == FAILURE)
        {
            return finishAndExit(file , tree);
        }
        if (tree->numOfVortex <= lineCounter)
        {
            return finishAndExit(file , tree);
        }
        loadInfo(tree , lineCounter , curDeg);
        lineCounter ++;
    }
    if (tree->numOfVortex > lineCounter)
    {
        return finishAndExit(file , tree);
    }
    lineCounter = 0;
    fclose(file);
    file = fopen(argv[1] , "r");
    fgets(line , MAX_LINE_SIZE , file);
    while (fgets(line , MAX_LINE_SIZE , file) != NULL)
    {
        sscanf(line , LEGAL_FORMAT , line);
        char *token = strtok(line , legalBetween);
        if (singleLineConnect(file , tree , legalBetween , lineCounter , token)
            == FAILURE)
        {
            return finishAndExit(file , tree);
        }

        lineCounter ++;
    }
    endRun(argv , file , tree);
    return 0;
}

void endRun(char **argv , FILE *file , Tree *tree)
{
    printAllInformation(argv , tree);
    freeTree(tree);
    fclose(file);
}

int singleLineConnect(FILE *file , Tree *tree , char *legalBetween , int lineCounter ,
                      char *token)
{
    int curSon = 0;
    int tokenCounter = 0;
    while (token != NULL)
    {
        if (strcmp("\n" , token) == 0)
        {
            fprintf(stderr , INLVALID_OUTPUT);
            freeTree(tree);
            fclose(file);
            return FAILURE;
        }
        if (strcmp(token , LEAF) != 0)
        {
            int tokenInt = (int) strtol(token , NULL , 10);
            if (curSon < tree->arrayOfNodes[lineCounter].numOfSons)
            {
                tree->arrayOfNodes[lineCounter].sons[curSon] = &tree->arrayOfNodes[tokenInt];
            }
            if (tree->arrayOfNodes[tokenInt].father == NULL)
            {
                tree->arrayOfNodes[tokenInt].father = &tree->arrayOfNodes[lineCounter];
            }
            else
            {
                return FAILURE;
            }
            curSon ++;
        }
        else
        {
            if (tokenCounter > 0)
            {
                return FAILURE;
            }
        }
        tokenCounter ++;
        token = strtok(NULL , legalBetween);
    }
    return 0;
}

int checkSingleLine(Tree *tree , char *legalBetween , int curDeg ,
                    char *token)
{
    while (token != NULL)
    {
        if ((strcmp(token , "-\n") != 0) && (strcmp(token , LEAF) != 0) &&
            strcmp(token , "-\r") != 0)
        {
            if (checkInput(tree , token) == 0)
            {
                return FAILURE;
            }
            curDeg ++;
        }
        token = strtok(NULL , legalBetween);
    }
    return curDeg;
}

void loadInfo(Tree *tree , int lineCounter , int curDeg)
{
    tree->arrayOfNodes[lineCounter].numOfSons = curDeg;
    tree->arrayOfNodes[lineCounter].key = lineCounter;
    tree->arrayOfNodes[lineCounter].sons = (Node **) malloc((sizeof(Node **)) * curDeg);
    tree->arrayOfNodes[lineCounter].father = NULL;
}

void printAllInformation(char **argv , Tree *tree)
{
    printRoot(tree);
    printVerCount(tree);
    highLowPath(tree);
    BFS(tree , tree->arrayOfNodes[strtol(argv[3] , NULL , 10)]);
    printPath(tree , tree->arrayOfNodes[strtol(argv[2] , NULL , 10)] ,
              tree->arrayOfNodes[strtol(argv[3] ,
                                        NULL ,
                                        10)]);
}

int finishAndExit(FILE *file , Tree *tree)
{
    closeProgram(tree);
    fclose(file);
    return EXIT_FAILURE;
}

int closeBeforeInit(FILE *file)
{
    fprintf(stderr , INLVALID_OUTPUT);
    fclose(file);
    return EXIT_FAILURE;
}

void closeProgram(Tree *tree)
{
    fprintf(stderr , INLVALID_OUTPUT);
    freeTree(tree);
}


int checkValidVor(char *argv[] , char *line)
{
    char *u = argv[2];
    char tempU[MAX_LINE_SIZE] = " ";
    sscanf(u , LEGAL_KEY_VALUE , tempU);
    char *v = argv[3];
    char tempV[MAX_LINE_SIZE] = " ";
    sscanf(v , LEGAL_KEY_VALUE , tempV);
    if (strcmp(u , tempU) != 0)
    {

        return 0;
    }
    if (strcmp(v , tempV) != 0)
    {
        return 0;
    }
    int tempUInt = (int) strtol(u , NULL , 10);
    int tempVInt = (int) strtol(v , NULL , 10);
    if (tempUInt < 0 || tempVInt < 0)
    {
        return 0;
    }
    int tempLine = (int) strtol(line , NULL , 10);
    if ((tempUInt >= tempLine) || (tempVInt >= tempLine))
    {
        return 0;
    }
    char temp[MAX_LINE_SIZE];
    int num = sscanf(line , LEGAL_FORMAT_KEY , temp);
    if (num == 0)
    {
        return 0;
    }
    while ((*line) != '\0')
    {
        if (! isdigit(*line))
        {
            return 0;
        }
        line ++;
    }
    return 1;
}

int checkInput(Tree *tree , char *line)
{
    char toCheck[MAX_LINE_SIZE] = "";
    sscanf(line , "%[0-9\n]" , toCheck);
    if (strcmp(line , toCheck) != 0)
    {
        return 0;
    }
    if (strtol(line , NULL , 10) > tree->numOfVortex)
    {
        return 0;
    }
    while ((*line) != '\0')
    {
        if (isalpha(*line))
        {
            return 0;
        }
        line ++;
    }
    char temp[MAX_LINE_SIZE];
    int num = sscanf(line , LEGAL_FORMAT_KEY , temp);
    if (num == 0)
    {
        return 0;
    }
    return 1;
}

int checkFirstInput(int argc)
{
    if (argc != VALID_NUMBER_ARGS)
    {
        return 0;
    }
    return 1;
}

Tree *startTree(int n)
{
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->arrayOfNodes = (Node *) malloc(sizeof(Node) * n);
    if (tree->arrayOfNodes == NULL)
    {
        free(tree);
        return NULL;
    }
    tree->numOfVortex = n;
    for (int i = 0; i < n; i ++)
    {
        tree->arrayOfNodes[i].key = - 1;
        tree->arrayOfNodes[i].numOfSons = 0;
        tree->arrayOfNodes[i].father = NULL;
        tree->arrayOfNodes[i].sons = NULL;
        tree->arrayOfNodes[i].dis = INFINITY;
        tree->arrayOfNodes[i].pre = 0;
    }
    return tree;
}

void BFS(Tree *tree , Node v)
{
    for (int i = 0; i < tree->numOfVortex; ++ i)
    {
        tree->arrayOfNodes[i].dis = INFINITY;
        tree->arrayOfNodes[i].pre = INFINITY;
    }
    tree->arrayOfNodes[v.key].dis = 0;
    Queue *Q = allocQueue(&Q);
    enqueue(Q , v.key);
    while (! queueIsEmpty(Q))
    {
        int s = (int) dequeue(Q);
        for (int i = 0; i < tree->arrayOfNodes[s].numOfSons; ++ i)
        {
            if (tree->arrayOfNodes[s].sons[i]->dis == INFINITY)
            {
                enqueue(Q , tree->arrayOfNodes[s].sons[i]->key);
                tree->arrayOfNodes[s].sons[i]->pre = tree->arrayOfNodes[s].key;
                tree->arrayOfNodes[s].sons[i]->dis = tree->arrayOfNodes[s].dis + 1;
            }
        }
        if (tree->arrayOfNodes[s].father != NULL)
        {
            if (tree->arrayOfNodes[s].father->dis == INFINITY)
            {
                enqueue(Q , tree->arrayOfNodes[s].father->key);
                tree->arrayOfNodes[s].father->pre = tree->arrayOfNodes[s].key;
                tree->arrayOfNodes[s].father->dis = tree->arrayOfNodes[s].dis + 1;
            }
        }
    }
    freeQueue(&Q);
}

void printPath(Tree *tree , Node u , Node v)
{
    printf(SHORT_PATH);
    printf(FIRST_ARGU , u.key);
    printf(SECOND_ARGU , v.key);
    while (u.key != v.key)
    {
        printf(SECOND_FORMAT_OUTPRINT , u.key);
        u = tree->arrayOfNodes[u.pre];
    }
    printf(OUTPRINT_WITH_NEWLINE , u.key);
}

void highLowPath(Tree *tree)
{
    int temp = 0;
    int max = 0;
    int maxIndex = 0;
    int maxDim = 0;
    int min = tree->numOfVortex - 1;
    for (int i = 0; i < tree->numOfVortex; ++ i)
    {
        if (tree->arrayOfNodes[i].father == NULL)
        {
            temp = i;
        }
    }
    BFS(tree , tree->arrayOfNodes[temp]);
    for (int j = 0; j < tree->numOfVortex; ++ j)
    {
        if (tree->arrayOfNodes[j].dis >= max)
        {
            max = tree->arrayOfNodes[j].dis;
            maxIndex = tree->arrayOfNodes[j].key;
        }
        if (tree->arrayOfNodes[j].dis < min && tree->arrayOfNodes[j].father != NULL &&
            tree->arrayOfNodes[j].numOfSons == 0)
        {
            min = tree->arrayOfNodes[j].dis;
        }
    }
    BFS(tree , tree->arrayOfNodes[maxIndex]);
    for (int j = 0; j < tree->numOfVortex; ++ j)
    {
        if (tree->arrayOfNodes[j].dis > maxDim)
        {
            maxDim = tree->arrayOfNodes[j].dis;
        }
    }


    printf(MINIMUM_FORMAT_OUTPUT , min);
    printf(MAXIMUM_FORMAT_OUTPUT , max);
    printf(DIAMETER_FORMAT_OUTPUT , maxDim);
}

void printRoot(Tree *tree)
{
    for (int i = 0; i < tree->numOfVortex; ++ i)
    {
        if (tree->arrayOfNodes[i].father == NULL)
        {
            printf(ROOT_FORMAT_OUTPUT , tree->arrayOfNodes[i].key);
            break;
        }
    }
}

void printVerCount(Tree *tree)
{
    printf(VERTI_FORMAT_OUTPUT , tree->numOfVortex);
    printf(EDGES_FORMAT_OUTPRINT , tree->numOfVortex - 1);

}

void freeTree(Tree *tree)
{
    for (int i = 0; i < tree->numOfVortex; i ++)
    {
        free(tree->arrayOfNodes[i].sons);
    }
    free(tree->arrayOfNodes);
    free(tree);
}

