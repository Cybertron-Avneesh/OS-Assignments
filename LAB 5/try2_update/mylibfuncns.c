#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include "myheader.h"

typedef struct {
  int frm_no;
  int valid_bit;
  int modify_bit;
  int reference_bit;
}pageItem;

int pagesize = rec_per_page * RECORDSIZE;
char * Frames[no_of_frames];
int fd;
int FrameTable[no_of_frames];
int page_fault_count = 0;
pageItem PageTable[PTABLESIZE];

/*  **********************    FUNCTIONS WRITTEN BY STUDENT    ************************************ */
typedef struct{
  int modify_bit,reference_bit;
}pair_rm;
struct esc_node{
  pair_rm bits;
  int page_no;
  struct esc_node * next;
};
typedef struct{
  struct esc_node *front,*rear;
}queue;
queue *q;
/* REWRITE THIS FUNCTION PROPERLY   */
int getFrameNo(int pno){
  int fno;

  /* Replace the following two lines of codes by proper logic */
  // fno = 0;
  // readPage(pno, fno);

  /*  Scan through the page table to check if the page is present in memory */  //done
  int flag=0;
  if(PageTable[pno].valid_bit==1){
    flag=1;
    PageTable[pno].reference_bit=1;
    update_queue_ref_bit(pno,1);
    return PageTable[pno].frm_no;
  }
  /* If found, simply return the corresponding frame no.  */  //done

  /* If not found, find a free frame-no. by searching the frame table */
  int free_frame_no=-1e9;
  if(!flag){
    for(int i=0;i<no_of_frames;i++){
      if(FrameTable[i]==-1){
        free_frame_no=i;
        break;
      }
    }
  }
  /* If found, read the requested page into the free frame, and then return the corresponding frame no. */
  if(free_frame_no!=-1e9){
    PageTable[pno].frm_no=free_frame_no;
    PageTable[pno].valid_bit=1;
    PageTable[pno].modify_bit=0;
    PageTable[pno].reference_bit=1;
    fno=free_frame_no;
    add_page_to_queue(pno,1,0);
    readPage(pno,fno);
    return fno;
  }
  /* If no free frame found, invoke your Page-Replacement-Algorithm to find victim page-no */
  int victim_page_no = enhanced_second_chance_algo();
  /* Scan through the page table to get the frame-no. corresponding to the victim page-no */
  fno=PageTable[victim_page_no].frm_no;
  PageTable[victim_page_no].valid_bit=0;
  update_victim_page(victim_page_no,pno);
  /* Write the victim page back into the disk-file, IF modify_bit is set */
  if(PageTable[victim_page_no].modify_bit==1){
    writeFrame(fno,victim_page_no);
  }
  /* Read the requested page into the freed frame, and then return the corresponding frame no. */
  readPage(pno,fno);
  return fno;
}
/*for adding the page to the circular queue when a free frame is available*/
void add_page_to_queue(int page_no,int ref_bit,int mod_bit){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames+1){
    if(q1->front->page_no==-1){
      q1->front->page_no=page_no;
      q1->front->bits.reference_bit=ref_bit;
      q1->front->bits.modify_bit=mod_bit;
      break;
    }
    count++;
    q1->front=q1->front->next;
  }
}
void update_victim_page(int victim_page_no,int pno){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames+1){
    if(q1->front->page_no==victim_page_no){
      q1->front->page_no=pno;
      update_queue_mod_bit(pno,0);
      update_queue_ref_bit(pno,1);
      break;
    }
    count++;
    q1->front=q1->front->next;
  }
}
/*for updating the page to the circular queue when page is already available*/
void update_queue_ref_bit(int page_no,int ref_bit){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames+1){
    if(q1->front->page_no==page_no){
      q1->front->bits.reference_bit=ref_bit;
      break;
    }
    count++;
    q1->front=q1->front->next;
  }
}
void update_queue_mod_bit(int page_no,int mod_bit){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames){
    if(q1->front->page_no==page_no){
      q1->front->bits.modify_bit=mod_bit;
      break;
    }
    count++;
    q1->front=q1->front->next;
  }
}
int find00(){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames+1){
    if(q1->front->bits.reference_bit==0 && q1->front->bits.modify_bit==0){
      return q1->front->page_no;
    }
    count++;
    q1->front=q1->front->next;
  }
  return -1;
}
int find01(){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  int count=0;
  while(count!=no_of_frames+1){
    if(q1->front->bits.reference_bit==0 && q1->front->bits.modify_bit==1){
      return q1->front->page_no;
    }
    else{
      q1->front->bits.reference_bit=0;
      PageTable[q1->front->page_no].reference_bit=0;
    }
    count++;
    q1->front=q1->front->next;
  }
  return -1;
}
void move_pointer_to_next(int found_page_no){
  queue *q1=(queue *)malloc(sizeof(queue));
  q1->front=q->front;
  q1->rear=q->rear;
  q1->front=q1->front->next;
  while(q1->front->page_no!=found_page_no){
    q1->front=q1->front->next;
  }
  if(q1->front->page_no==found_page_no){
    q1->front=q1->front->next;
  }
}
int enhanced_second_chance_algo(){
  int found_page_no=-1;
  found_page_no=find00();
  if(found_page_no!=-1){
    move_pointer_to_next(found_page_no);
    return found_page_no;
  }
  else{
    found_page_no=find01();
    if(found_page_no!=-1){
      move_pointer_to_next(found_page_no);
      return found_page_no;
    }
    else{
      found_page_no=find00();
      if(found_page_no!=-1){
        move_pointer_to_next(found_page_no);
        return found_page_no;
      }
      else{
        found_page_no=find01();
        if(found_page_no!=-1){
          move_pointer_to_next(found_page_no);
          return found_page_no;
        }
      }
    }
  }
}
void create_queue(int page_no,int ref_bit,int modi_bit){
  struct esc_node * temp;
  temp=(struct esc_node *)malloc(sizeof(struct esc_node)); 
    temp->page_no = page_no; 
    temp->bits.modify_bit=modi_bit;
    temp->bits.reference_bit=ref_bit;
    if (q->front == NULL) 
        q->front = temp; 
    else
        q->rear->next = temp; 
    q->rear = temp; 
    q->rear->next = q->front;
}
/*  ***************************************  FUNCTION DEFINITIONS GO HERE  *******************************      */
/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/

/* Initialization Function: Initializes all aspects */
void Initialize(){
  q= (queue *)malloc(sizeof(queue));
  q->front=q->rear=NULL;
  for(int i=0;i<no_of_frames;i++){
    create_queue(-1,0,0);
  }
  int i;
  /*  Allocating Memory to Frames, and initializing the FrateTable */
  for(i = 0; i < no_of_frames; i++){
    Frames[i] = (char *) calloc(pagesize, sizeof(char));
    FrameTable[i] = -1; // -1 indicates frame is empty
  }
  /* Initializing the PageTable */
  for(i = 0; i < PTABLESIZE; i++){
    PageTable[i].frm_no = -1;
    PageTable[i].valid_bit = 0;
    PageTable[i].modify_bit = 0;
    PageTable[i].reference_bit = 0;
  }
  /*  Opening the Data File  */
  fd = open("./student-data.csv", O_RDWR );
	if(fd<0){
		printf("Error Number % d\n", errno);
        perror("Program");
		exit(1);
	}
}

/* Finalization Function: Finalizes all aspects */
void Finalize(){
  int i;
  /* Write back all non-empty frames to file, and freeing the memory allocated to all frames */
  for(i = 0; i < no_of_frames; i++){
    if(FrameTable[i] != -1){
      writeFrame(i, FrameTable[i]);
    }
    free(Frames[i]);
  }
  printf("Number of Page faults: %d\n",page_fault_count);
  /* Closing the Data File */
  close(fd);
  if(fd<0)printf("Error! Could not close the data file...\n");
}

/* Reads specified page from disk-file to the specified Frame in memory */
void readPage(int pno, int frm_no){
  lseek(fd, (pno*pagesize), SEEK_SET);
  read(fd, Frames[frm_no], pagesize);
  FrameTable[frm_no] = pno;
  page_fault_count++;
}

/* Writes the contents of the specified frame in memory to the specified page of the disk-file */
void writeFrame(int frm_no, int pno){
  int res;
  lseek(fd, (pno*pagesize), SEEK_SET);
  res = write(fd, Frames[frm_no], pagesize);
  if(res<0){
    perror("Write Error.");
    exit(1);
  }
}

/* Displays the contents of the specified memory-frame */
void printFrame(int frm_no){
  int i;
  for(i=0; i < pagesize; i++){
    printf("%c",Frames[frm_no][i]);
  }
  printf("\n");
}

/* Translates the specified slno (record-number) to its corresponding page no. and page offset values */
void getPageDetails(int slno, int *pno, int *offset){
  int q, r;
  q = (slno-1)/rec_per_page;
  r = (slno-1) - (rec_per_page*q);
  *pno = q;
  *offset = r;
}

/* Displays the RECORD contained at the specified offset of the specified memory-frame */
void printRecord(int frm_no, int offset){
  int i, startindx, endindx;
  startindx = offset*RECORDSIZE;
  endindx = startindx + RECORDSIZE;
  for(i = startindx; i < endindx; i++){
    printf("%c",Frames[frm_no][i]);
  }
  printf("\n");
}


/* Updates the Name Field of the RECORD contained at the specified offset of
the specified memory-frame, with the given string */
void updateRecord(int frm_no, int offset, char *string){
  int i, startindx, endindx;
  if(strlen(string) != 4){
    printf("Length of the input string must be equal to 4.\n");
    exit(0);
  }
  else{
    startindx = (offset*RECORDSIZE) + 14;
    PageTable[FrameTable[frm_no]].modify_bit=1;
    update_queue_mod_bit(FrameTable[frm_no],1);
    for(i = 0; i < 4; i++) Frames[frm_no][startindx++] = string[i];
  }
}