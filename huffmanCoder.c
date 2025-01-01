//Avi Felzenstein
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

char *inputFile = "noDupFreq.txt";
char *outputFile = "huffman.out";
int len;
int recursiveIndex=0;

struct Node {
char character;
int freq;
struct Node *left;
struct Node *right;
};

struct huffNode {
  char character;
  char *code;
};
//This function creates a forrest of frequencies
void freqPopulate(unsigned int chars[], char *fileName, int *totalChars, int *totalFreq) {
    FILE *fptr;
     fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    char characters;
    while ((characters=fgetc(fptr)) !=EOF) {
        for(int i = 0; i<256; i++) {
            if(characters == i) {
                if(chars[i]==0) {
                    (*totalChars)++;
                    (*totalFreq)++;
                    chars[i]++;
                }else{
                    chars[i]+=1;
                    (*totalFreq)++;
                }
            }
        }
    }
fclose(fptr);
}
//Prints out the frequency and character of the inital array containing all of the characters in given .txt
void printChars(unsigned int chars[]) {
  for (int i = 0; i < 256; i++) {
    if(chars[i]>0) {
    printf("%c and %d\n", (char)i, chars[i]);
    }
  }
}
//Switching nodes function
void switching(struct Node *x, struct Node *y) {
struct Node temp = *y;
*y = *x;
*x = temp;
}
//Inserts the leaf nodes into forrest.
void insertLeaf(struct Node forrest[], int character, int freq, int size) {
  len=size;
  if (len == 0) {
        forrest[0].character = (char) character;
        forrest[0].freq = freq;
        forrest[0].left =NULL;
        forrest[0].right=NULL;
  } else {
        forrest[len].character = (char) character;
        forrest[len].freq = freq;
        forrest[len].left =NULL;
        forrest[len].right=NULL;
     int i = len;
     //If the child is smaller than the parent, swap it.
     while (i > 0 && forrest[(i - 1) / 2].freq > forrest[i].freq) {
        switching(&forrest[i], &forrest[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
  }
  len++;
}
//Insert function for non leaf nodes, used in huffman()
void insertInternal(struct Node forrest[], struct Node *z) {
    forrest[len] = *z; 
    int i = len;
    while (i > 0 && forrest[(i - 1) / 2].freq > forrest[i].freq) {
        switching(&forrest[i], &forrest[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
  len++;
}
//Function to fill the forrest with the nodes with a freq >0
void fillForrest(unsigned int chars[], struct Node forrest[], int findex) {
  int i = 0;
  while(i<findex) {
    for(int j = 0; j <256; j++) {
        if(chars[j] > 0) {
        insertLeaf(forrest, j, chars[j], i);
        i++;
      }
    } 
  }
}
//Heaps the array after we extract the min. 
void heapify(struct Node forrest[], int sizeOfHeap, int curr) {
  int smallest = curr;
  int l = 2 * curr + 1;
  int r = 2 * curr + 2;
  if (l < sizeOfHeap && forrest[l].freq <= forrest[smallest].freq) {
    smallest = l;
  }
  if (r < sizeOfHeap && forrest[r].freq <= forrest[smallest].freq) {
    smallest = r;
  }
  if (smallest != curr) {
    switching(&forrest[curr], &forrest[smallest]);
    heapify(forrest, sizeOfHeap, smallest);
  }
}
//Size must be a pointer like how we did with the arrays in freqCount so that outside the function, findex will be adjusted
struct Node extractMin(struct Node forrest[]) {
  struct Node min = forrest[0];
  forrest[0] = forrest[len-1];
  len--;
  heapify(forrest, len, 0);
return min;
}
//Function to make a huffman tree
struct Node *huffman(struct Node forrest[], int totalFreq) {
    while (len > 1) {
      struct Node *x = malloc(sizeof(struct Node));
      *x = extractMin(forrest); 
      struct Node *y = malloc(sizeof(struct Node));
      *y = extractMin(forrest);
      struct Node *z = malloc(sizeof(struct Node));
      z->right = y;
      z->left = x;
      z->freq = x->freq + y->freq;
      z->character = '\0';
      //Directly add z node to the forrest
      insertInternal(forrest, z);

    }
    struct Node *root = malloc(sizeof(struct Node));
    *root = forrest[0];
  return root;
}
 
//Function to traverse the tree and find the codes
void findCodes(struct Node *root, struct huffNode codes[], char *bits) {
    if (root == NULL) {
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        codes[recursiveIndex].character = root->character;
        codes[recursiveIndex].code = strdup(bits);
        recursiveIndex++;
        return;
    }

    if (root->left != NULL) {
        size_t size = strlen(bits)+2;
        bits = realloc(bits, size);
        strcat(bits, "1");
        findCodes(root->left, codes, bits);
        //Remove the last character for backtracking
        bits[strlen(bits) - 1] = '\0'; 
    }

    if (root->right != NULL) {
        size_t size = strlen(bits)+2;
        bits = realloc(bits, size);
        strcat(bits, "0");
        findCodes(root->right, codes, bits);
        bits[strlen(bits) - 1] = '\0'; 
    }
}
//Prints out the concatonated codes when reading the file, also resets the array for the next 8
void printBits(char arr[]) {
    for(int i=0; i<8; i++){
        if(arr[i]!='$') {
        //printf("%c",arr[i]);
      }
        arr[i]='$';
    }
     //printf("\n");
}
//Writes the decimal representation of the packed 8 bits to file using fwrite
void writeToFile(int dec_rep){
FILE *file = fopen(outputFile, "a+");
 if (file == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }
    fwrite(&dec_rep, 1, 1, file);
    fclose(file);
}
//Packs the simulated 8 bis from the eight_Bits array
void clownCar(char arr[]) {
//Convert array into a decimal number that translates into the binary rep of it. EX: str "0100" = 4
    int len=0;
    while(arr[len]!='\0') {
        len++;
    }
    double dec_rep=0;
    int expo=1;
    //This is a for loop that's gonna itterate from the array backwards
    for (int i = len - 1; i >= 0; i--) {
        if (i == len - 1 && arr[i] == '1') {
            dec_rep += 1;
        } else if (arr[i] == '1') {
            dec_rep += expo;
        }
        expo *= 2;
    }
    writeToFile((int)dec_rep);
}
// For when we need to pack an array to have 8 bits. We will wanna "pad it with zeroes"
void packWatch(char arr[]){
int length=0;
    while(arr[length]!='\0') {
        length++;
    }
    for(int i=length-1; i>=0; i--) {
        if(arr[i]=='$'){
            arr[i]='0';
        } 
    } 
}
//Function to print out the codes. 
void printCharsAsCodes(struct huffNode codes[], char *fileName, int len) {
    char character;
    char eight_Bits[8];
    int bit_Counter = 0;
    int code_length =0;
    int j=0;
    int length = len;
    FILE *fptr;
     fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    while ((character = fgetc(fptr)) != EOF) { 
        for(int i=0; i<length; i++) {
            //If the character in the string is the same as one of the characters in the array
            //Issue: Goes AA(yet the 2nd A character's code isn't printed correctly) space, then A again? 
            if(character==codes[i].character) {
                // Then we need to insert as many of the 0 and 1 into the eight_Bits as possible
                //Issue I can think of: How do we know the length of code at i? 
                code_length=strlen(codes[i].code);    
                while(j < code_length) {
                eight_Bits[bit_Counter]=codes[i].code[j];
                bit_Counter++;
                j++;
                //Check if we ran out of space in the array, if we did, print out the rest, reset then continue on the while loop.
                if(bit_Counter==8) {
                clownCar(eight_Bits);
                printBits(eight_Bits);
                bit_Counter=0;
                    } 
                }
                //If while we reach the end of eight_bits while we fill out the code
                if(bit_Counter==8) {
                clownCar(eight_Bits);
                printBits(eight_Bits);
                bit_Counter=0;
                }
            }
            //outside the loop, reset j to 0;
                j=0;
        }
}
//Print out whatever is left in the string that's not a factor of 8. 
clownCar(eight_Bits);
printBits(eight_Bits);
packWatch(eight_Bits);
fclose(fptr);
}

//Function to free the Nodes function malloced from huffman()
void freeNodes(struct Node* root) {
    if (root == NULL) {
      return;
    }
    freeNodes(root->left);
    freeNodes(root->right);
    free(root);
}

int main(int argc, char *argv[]) {
    //This stuff is for allowing -d, -i and -o to work, adapted from https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
    int debug = 0;
    int option;
    while ((option = getopt(argc, argv, ":d:i:o:")) != -1) {
        switch (option) {
            case 'd':
                debug = 1;
                break;
            
            case 'i':
                inputFile = optarg;
                break;
            
            case 'o':
                outputFile = optarg;
                break;
            
            case '?':
            default:
                fprintf(stderr, "only -d, -i, and -o allowed!\n");
                return 1;
        }
    }
    if (debug) {
        printf("Debug mode is ON.\n");
    }
    //This section reads the command line argument (section 1)
    unsigned int chars[256]= {0}; 
     int findex = 0;
     int totalFreq= 0;
    freqPopulate(chars,inputFile,&findex,&totalFreq);
    if (debug) {
    printf("Refined forrest array with only non zero frequencies:\n");
    printChars(chars);
    printf("\n");
    }
    //Section 2 part 1: Build a forrest
    struct Node forest [findex];
    fillForrest(chars,forest,findex);
    if (debug) {
    printf("Nodes in forrest queue:\n");
    for(int i=0; i<findex; i++) {
    printf("%c and %d\n",forest[i].character, forest[i].freq);
      }
    }    
//Section 2 part 2: Make a huffman tree
len = findex;
struct Node *root = huffman(forest, len); 
  if (debug) {
  printf("This is the root frequency, this should be the total frequency %d: ", totalFreq);
  printf("%d\n",root->freq);
  }

//Section 3: Find the huffman codes
char *bits = malloc(1); // Allocate memory for an initially empty string
bits[0] = '\0';
struct huffNode codes[findex];
findCodes(root, codes, bits);
if(debug) {
for(int i =0;i<findex;i++) {
  printf("%c %s\n",codes[i].character, codes[i].code);
  }
}
//Section 4: Traverse the initial file again and print out the codes corresponding to the ascii character.  
printCharsAsCodes(codes, inputFile, findex);
    
//Freeing the huffNodes
for (int i = 0; i < findex; i++) {
    free(codes[i].code);
  }
//Freeing the original nodes and bits
  freeNodes(root);
  free(bits);
    return 0;
}
