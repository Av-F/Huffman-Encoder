Avi Felzenstein
2/22/24

"I know that you can do all things; no purpose of yours can be thwarted." -Job 42:2

The program huffmanCoder.c is an implementation of a huffman encoder. 

Before I discuss the the functions, very quick I would like to highlight my structures. My first structure is my node structure. This structure holds the nodes for the
huffman tree which includes the character, the frequency and the pointers to left and right children. The next structure is a huffNode node which is for holding the character
and the corresponding prefix code. Also of note are "global" variables that I needed outside of the functions. These include the input and output file names, the index for my findCodes() function
and a len variable for the length of the forest of nodes used in my huffman functions. 

Within the program, my first function to highlight is my freqPopulate function.
In it, it reads the inputFile variable which was determined in the main function. From there, it opens it (or gives an error if the file isn't openable) in read mode and while it hasn't reached the 
end of the file, take the character and try to match it with it's decimal equeivelent in a 256 integer array. How this works is that each index represents 
a decimal value which corresponds with an ascii character if casted to char. Thus by that logic, I can update that index by 1 if that index's character 
has 1 or more appearences in the file. I also have a total frequency variable for later useage that I update using a pointer for every character frequency 
updated. This strategy was originally adapted from https://www.learnc.net/c-tutorial/c-read-text-file/ where I took the while loop and modified what's 
inside the while loop to service getting the frequencies of the array.
 
My third task was to build a min heap. While looking at the internet for a template to adapt, I came across a max heap https://www.programiz.com/dsa/priority-queue.
That website gave me a template to work with and modify as originally it's for a max heap but I needed a min heap. Furthermore, it gave me a plethora of tips
on how to understand how it works. Starting with switching() function, barring a few variable name changes, the process is practically the same as the template. Both the template and I decided 
to work with pointers and by following normal swaping convention, I made a temp node that allows me to temporary store the address of y, swap the addresses of y and x and replace x with y. 

Next came insertLeaf which stayed relatively similar to how it's done on the site. The difference however comes in with how I am inserting a node rather than an integer. This meant that I had to change what lines of code to insert into an array of a custom node rather than an int.
I also kept track of a local size varaible which worked with len to keep track of the current length of the heap. Since you insert at the bottom of a heap I created a way for it 
to automatically heapify rather than calling a heap function. This was while we aren't at the root and the frequency of the parent was smaller than the frequency of the child, to
swap the two nodes. I did it this way instead of calling heapify because at the time, I didn't have a heapify function built yet nor did the heapify function from the template
would work for a min heap since it was built for a max heap. Furthermore, after some testing with my huffman() function, I figured out that I would need two insert functions, thus I made this insert function
insert the leafNodes only which will be described later in the file. This meant that I also set the left and right children of those nodes to NULL as leaf nodes shouldn't have any children incase my 
code decided to give them children when I would iterate through the huffman tree. 

The heapify function also needed to be modified from the template. One change that had to be done was determining the left and right children to be based off of the node frequencies. The next change 
was heapifying a min heap rather than a max heap. This was accomplished by creating a smallest variable to be the current node's frequency, then check the left and right children of that node. If they were smaller 
than the parent, set the smallest variable to either the left or right child. If one of them were smaller, call our switching function and swap parent and child then call heapify again until we have a min heap. 

Interestingly enough I don't pass my initial 256 array, rather an Node array called forrest. This is because I decided to create a Node array that holds only the characters with
a frequency greater than 0. I was able to populate this array by calling fillForrest. How this function worked is that while itterating through the 256 character array. If the index (which corresponds with a character)
had a frequency greater than 0, to call the insert function with forrest. Also included is a variable called findex which is taken from totalChars from freqPopulate, findex determines the size of the forrest array to when
I put in a character into forrest, to update a counter that as long it's less than findex, I have an open spot for the character in the node array i.

Now I can explain extractMin. that function works by creating a min node and having it have the value of whatever the current forrest[0] is since that index should have the character with 
the smallest frequency. Then I swap the smallest frequency Node with the biggest frequency Node which should be index 0 and the last index respectively, subtract the size of the forrest aray down by 1 so then when I call heapify again, it doesn't include  
the old smallest character frequency. This is different than the template's deleteMin since I output the minimum character and then call on my heapify function rather than doing what I did in
insert since I had a heapify function built. I also use my global variable len to keep track of the length of the forrest array and what is the "end of the array".  

Now that I have described my min heap funtions, I can discuss my huffman function. Originally, I had it return a root but now I had it return a pointer to forrest[0] as I would need a pointer to 
traverse the tree I would be creating. What I did was create two functions that with the min heap functions would work togeather to insert the internal nodes and build a huffman tree. First created was the huffman function.
This function followed the algorithm found in "Introduction to Algorithms 4th edition" aka the textbook for cs 310. Essentially it was to extract the two nodes with the shortest frequencies. Create an intenral node called z,
have z's frequency be the combined frequency of the two smallest nodes' frequencies, set it's character to '\0' and have it be reinserted back into the forrest array until we are left with only one Node left in the array. 
However, one problem I encoutnered was that while this would happen, the left and right children for the z node wouldn't actually point correctly. So while I would have the huffman function successfully make my array into a size of one,
the fact was that node (called root) wouldn't have it's children! To fix this issue of child custody and ending up with an alimony to pay (my grade). I made a new function that would directly insert the z node into the array.

This function was called insertInternal and it works similary to how swtiching works in the sense that I work with pointers and have the z node point to where the array at the index of the current size directly, and to modify the array's index value directly. 
follow the same heapify function in insertLeaf and update the size. This function fixes the issue I had in huffman() as instead of now calling on insertLeaf, I call insertNode for each z node and modify it directly there. 
 
After I had created my huffman tree, I went out to work on finding the codes. Since at the time, my huffman tree was incorrect, I hard coded a tree to test out the function I was creating.
How my function works is by recursively calling on the root and it's children until it finds a node with no children, that meant I found a leaf node and that the node will have a character and frequency.
I then kept a "global" recursiveIndex outside of the function that I would call whenever I need to insert the leaf node into a new array of a new structure called codes. This is an array that has the structure
called huffCodes which has space for the character and the code associated with the character then return. Learning by how I did with my x y and z nodes in huffman(), I made sure to dynamically allocate a 
string I called bits. This would come in handy as when I recur on the left and right child, I could dynamically allocate space for either a 1 or a 0 and a null terminator after I call strcat (which needs a fixed size). 
This allowed me to have a dynamically changing size depending on how far I was in the tree. Speaking of null terminator, I have to add it in for backtracking needs in case there was any other paths I could take while navigating
through the tree. Once I found a leaf node, using recursiveIndex to keep track of the current index of codes[], i'd duplicate bits into codes[recursiveIndex].code and also the character in codes[R...I...].character.

The final challenge was to present the codes and store them into a single byte. This meant that I had to create new functions. The first one was printCharsAsCodes. Since I saved the codes as characters, I would need them to be lined up in
groups of eight to represent the eight bits in a byte. This was accomplished by reading over the same input file similar to freqPopulate. Then while reading, I made an array of 8 characters called eight_Bits to similate the eight bits needed to make 
a byte to encode. Then with multiple loops, I keep a variable j to act as a counter for each code in codes[] so while j < the length of the code corresponding with the character being read from the file, put it into the "8 bit" array and if there is no more space,
go to clownCar() and then writeToFile(), afterwards, clearing out the stored 8 bits in printBits(). After that, resume putting in the codes (either if j hadn't completed taking it from codes[current character] or resetting j to zero and getting the next code to implement).
This process repeats until all that is left is the eight_Bits array that may have some codes left inside. I call another function called packWatch() to pack the array such that the remaining bits not filled are packed with zeroes, then I call my clownCar and print functions.

So what about those functions? First is clownCar() and clown car works by compressing the simulated eight bits in the array into a integer which is a byte in size. I call it clown car as it's like packing a bunch of clowns (bits) into a car (byte). This program works by 
looping through the length of the "8 bit" array, and treating it as a binary number. when going through the array backwards, if I find a '1', to add a local variable called expo by 2 to another local variable called dec_rep for finding the decimal representation of the binary code, then multiplying expo by 2.
This process repeats until all of the characters in the array are looked over. Then outside of the loop, I call writeToFile, passing dec_Rep.

writeToFile operates by opening up the output file and appending to it. I then call fwrite usng the dec_rep found in clownCar() to write to the designated output file given in the commandline.

printBits was going to originally print out the binary represnetation found in eight_Bits array, however I repurposed the function to run through the array and turn every one and zero into a '$' to designate to myself
that the array was "cleared" for the next 8 bits. This works by itterating through the arrray using a forloop. Finally, I did comment out a print statement to ensure all 8 bits were being ittereated correctly.

packWatch works by itterating through the final version of eight_Bits when the end of the file has been reached to turn any $ into a zero. This works by finding the length of the array, itterating though the array
with a for loop and if there is a '$' to replace it with a '0'. This pads the array such that we have all 8 slots for bits filled up to then be clown carred. 

Final function before the main is my freeNodes function which itterates through the tree made in huffman() and frees all of the roots since I dynamically allocated to make them using malloc.
This function uses post order traversal so I can visit the left and right nodes first before freeing the root node as trying this with preorder traversal did not work with me. 

Finally my main function is where I call upon all of my functions. First starting with adding in additions from getopt.h which since I had no idea how it worked, I adapted a way of calling it from
https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/. This allows things such as -d to work. I had also made sure to read the commandline arguments to ensure if someone posted a txt file to encode.
Otherwise it would default to a complete work of shakespeare provided in the instructions. The main function is also where I created most of the arrays discussed, findex and other variables such as totalFreq. 
I also made sure to free up the dynamically allocated spots calling on freeNodes and other forloops within main to free the codes[] and bits once the code finished operating lest I want Moloch to curse my code because I used malloc. 

Also of note is my makefile, that exists too which I just followed the instructions from the makefile handout on blackboard and the links found from there. You run it by either just saying make.
Then afterwards, you do ./huffmanCoder [text.txt] [-d]. Otherwise you can make clean using make clean. 

Other things of note is that when testing my code, make sure that if your using the same output file to remove the contents of whatever may be stored in there before testing or the data will be messed up. I 
had this problem with my huffman.out wherin with every run of my program, huffman.out would grow since I forgot to clear it out. 

Overall this program took a bunch of turns, deviated from how the professor wanted only to find workarounds to deliver what I assume to be the expected final product.
Only other thing of note is that compiling with shakespeare may take a while but shorter programs seem to work well, when running diff, no errors occured for me. 

Thank you for reading all of this,
-Avi