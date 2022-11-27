#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define depth 10

int new_size = 0;
int num_Tnode = 0;
int max_len = 4096;

//Tree Node
struct Tnode
{
    int freq;
    char letter;
    struct Tnode *left;
    struct Tnode *right;
};

//Queue Node
struct Qnode
{
    int freq;
    char letter;
    struct Tnode *tree;
    struct Qnode *next;
};

struct Qnode *head = NULL;


struct Qnode *new_Qnode()
{
    struct Qnode *new = (struct Qnode *)malloc(sizeof(struct Qnode));

    if(!new)
    {
        printf("Overflow\n");
        return;
    }
    new -> freq = 0;
    new -> letter = '\0';
    new -> tree = NULL;
    new -> next = NULL;

    return new;
}

void Qinsert(struct Qnode *new)
{
    if(!head)
    {
        head = new;
        return;
    }

    struct Qnode *temp = head;
    struct Qnode *prev = temp;

    while(temp && temp -> freq > new -> freq)
    {
        prev = temp;
        temp = temp -> next;
    }

    if(prev == temp)
    {
        new -> next = temp;
        head = new;
        return;
    }

    new -> next = temp;
    prev -> next = new;
}

void Qinsert_tree(int freq, struct Tnode *tree)
{
    struct Qnode *new = new_Qnode();

    if(!new)
    {
        return;
    }

    new -> freq = freq;
    new -> tree = tree;

    Qinsert(new);
}

void Qinsert_letter(int freq, char letter)
{
    struct Qnode *new = new_Qnode();

    if(!new)
    {
        return;
    }

    new -> freq = freq;
    new -> letter = letter;

    Qinsert(new);
}

void Qdel()
{
    if(!head)
    {
        return;
    }

    struct Qnode *temp = head;
    struct Qnode *prev = temp;

    while(temp -> next)
    {
        prev = temp;
        temp = temp -> next;
    }

    if(temp == prev)
    {
        head = NULL;
        free(temp);
        return;
    }

    prev -> next = NULL;
    free(temp);
}

void Qdisplay()
{
    if(!head)
    {
        printf("Queue Empty");
        return;
    }

    struct Qnode *temp = head;

    printf("\nFreq\tLetter\tTree\n");
    while(temp)
    {
        printf("%d\t%c\t%p\n",temp -> freq, temp -> letter, temp -> tree);
        temp = temp -> next;
    }
}


//Queue Done


struct Tnode *new_Tnode()
{
    struct Tnode *new = (struct Tnode *)malloc(sizeof(struct Tnode));

    if(!new)
    {
        printf("Overflow\n");
        return;
    }

    new -> freq = 0;
    new -> letter = '\0';
    new -> left = NULL;
    new -> right = NULL;

    num_Tnode ++;

    return new;
}

struct Tnode *Tconcat(struct Tnode *right, struct Tnode *left)
{
    struct Tnode *new = new_Tnode();

    if(!new)
    {
        return;
    }

    new -> freq = right -> freq + left -> freq;
    new -> right = right;
    new -> left = left;

    return new;
}

void tree_maker()
{
    if(!head)
    {
        return;
    }

    while(head -> next)
    {
        struct Qnode *temp = head;
        struct Qnode *prev = temp;
        struct Tnode *right, *left;

        while(temp -> next)
        {
            prev = temp;
            temp = temp -> next;
        }

        if(prev -> tree)
        {
            right = prev -> tree;
        }
        else
        {
            right = new_Tnode();
            right -> freq = prev -> freq;
            right -> letter = prev -> letter; 
        }
        
        if(temp -> tree)
        {
            left = temp -> tree;
        }
        else
        {
            left = new_Tnode();
            left -> freq = temp -> freq;
            left -> letter = temp -> letter;
        }

        Qdel();
        Qdel();

        struct Tnode *tree = Tconcat(right, left);
        Qinsert_tree(tree -> freq, tree);
        Qdisplay();
    }
}

void code_maker(struct Tnode* root, int code[], int top)
{
    if (root -> left)
    {
        code[top] = 0;
        code_maker(root -> left, code, top + 1);
    }

    if (root -> right)
    {
        code[top] = 1;
        code_maker(root -> right, code, top + 1);
    }

    if (!root -> left && !root -> right)
    {
        printf("%d\t'%c'\t", root->freq, root->letter);

        for(int i = 0; i < top; ++i)
        {
            printf("%d",code[i]);
        }

        new_size += (root -> freq)*sizeof(char)*top;
        printf("\n");
    }
}

void Tprint(struct Tnode* root, int space)
{
    if(root)
    {
        space += depth;

        Tprint(root -> right, space);

        printf("\n");
        for (int i = depth; i < space; i++)
            printf(" ");
        printf("%d %c\n", root -> freq, root -> letter);
    
        Tprint(root -> left, space);
    }
}

void huffman_maker()
{ 
    char input_arr[max_len];

    printf("Enter String: ");
    fgets(input_arr, max_len - 1, stdin);
    int a, b, index = -1;

    for(a = 0; a < max_len; a++)
    {
        if(input_arr[a]=='\n')
        {
            break;
        }
    }

    char arr[a];
    int freq[a];
    int freq_temp;

    for(b = 0; b <= a; b++)
    {
        if(input_arr[b])
        {
            arr[++index] = input_arr[b];
            freq_temp = 1;
            for(int c = b + 1; c < a; c++)
            {
                if(arr[index] == input_arr[c])
                {
                input_arr[c]=NULL;
                ++freq_temp;
                }
            }
            freq[index] = freq_temp;
        }
    }

    int old_size = 0;
    //ved
    for(int i = 0; i < index; i++)
    {
        Qinsert_letter(freq[i], arr[i]);
        old_size += freq[i]*sizeof(char)*8;
    }

    int code[25];

    Qdisplay();
    tree_maker();

    printf("\nHuffman Tree\n");
    Tprint(head -> tree, 0);

    printf("\n\nFreq\tLetter\tCode\n");
    code_maker(head -> tree, code, 0);

    printf("\n\nNew size: %d\tOld size: %d\nCompression Ratio: %f\n",new_size, old_size, (float)new_size/(float)old_size);

    new_size += num_Tnode*sizeof(struct Tnode);
    printf("\n[INCLUDING SIZE OF THE TREE]");
    printf("\nNew size: %d\tOld size: %d\nCompression Ratio: %f\n",new_size, old_size, (float)new_size/(float)old_size);

    if(new_size >= old_size)
    {
        printf("\nHint: Bigger Text will yeild better result\n");
    }
}


//Tree done


void main()
{
    huffman_maker();
}
