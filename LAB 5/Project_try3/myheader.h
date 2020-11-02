#define RECORDSIZE 20  // Size of a single RECORD - simulated as a Memory-Word
#define rec_per_page 3 // allowing 3 records per page
#define no_of_frames 3  // allocating 3 frames to hold the pages of the data file
#define PTABLESIZE 100

/* Initialization Function: Initializes all aspects */
void Initialize();

/* Finalization Function: Finalizes all aspects */
void Finalize();

/* Reads specified page from disk-file to the specified Frame in memory */
void readPage(int pno, int frm_no);

/* Writes the contents of the specified frame in memory to the specified page of the disk-file */
void writeFrame(int frm_no, int pno);

/* Displays the contents of the specified memory-frame */
void printFrame(int frm_no);

/* Translates the specified slno (record-number) to its corresponding page no. and page offset values */
void getPageDetails(int slno, int *pno, int *offset);

/* Displays the RECORD contained at the specified offset of the specified memory-frame */
void printRecord(int frm_no, int offset);

/* Updates the Name Field of the RECORD contained at the specified offset of
the specified memory-frame, with the given string */
void updateRecord(int frm_no, int offset, char *string);

/*    ****************************** PROTOTYPE DECLARATIONS OF THE FUNCTIONS WRITTEN BY STUDENT   ******************* */

/*  */
int getFrameNo(int pno);
int LRUEnchancedSecondChanceAlgorithm(int pno);
void create_queue();
struct node* create_node();
void SwapOut(int fno,int old_pno);
int check(int reference,int modify);
void SetReferenceBitsZero();