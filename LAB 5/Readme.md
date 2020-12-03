# Topic: Memory Management and Virtual Memory
## Question Statement:
 - Implement(Mimic) LRU Enhanced Second Chance Algorithm

### How to test the implementation?
Execute the iteration (the ‘while’ loop) in the main program for at least 20~25 times. In these successive
iterations, input a random sequence of record numbers, all in the range 01 and 94 (creating our simulated
‘reference string’), with a random choice of ‘read-only’ (only displaying the record) or ‘read & write’
(update record) request for each record number. Note that, in order to reduce your effort to enter input
values manually, you may also modify the main program to take this sequence of inputs from a file, in
some specific format. For example, the following content in the input file:-
27 xxxx, 38 ----, 10 abcd

indicates that the length of the reference string is 3 which is <27, 38, 10>. You want to update the record
for your first and third requests but don’t want to update for the second request. The string by which you
want to update record no. 27 is “xxxx” and that for record no. 10 is “abcd”.
Now, calculate (manually, using paper and pencil) the number of page faults that would occur if
you use ‘optimal page replacement algorithm’ for the same reference string and compare it with the result
obtained from your program. Repeat the experiment for different values of ‘no_of_frames’ (defined inside

‘myheader.h’) and plot the results (no.-of-frames vs. no.-of-page-faults) for both the page replacement
algorithms (your assigned algo. & optimal algo) in a graph.

### Folder `codes` contains all the coed related to this assignment.
### Folder `Graph` contains graph and raw data for LRU-Enhanced Second chance and Optimal Algo
