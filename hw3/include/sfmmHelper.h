/* This is a helper function that splits a free block into smaller blocks */
void split( );



/* This is a helper function that reads the allocation bit and determines if the block is free*/
bool isBlockFree();



/* This is a helper function that gets the previous block */
void* getPrevBlock();



/* This is a helper function that gets the next block */
void* getNextBlock();



/* Helper Function: Gets size of the next block */
void getNextBlockSize();



/*HelperFunction: Get size of the previous block */
void getPrevBlockSize();


/* This is a helper function to request extra heap space: mem_sbrk()? */
void heapIncrementRequest();


/* Helper Function: Pads block to aligned boundaries. */
void padToAlignment();

 
/* Helper Function : Flip allocation Bit */
void flipAllocationBit();


/* This is a helper function that searches the free list for the next available block 
 * according to the next-fit policy
 */
void* findNextFitPolicy();

/* This is a helper function that searches the free list for the next available block 
 * according to the first-fit policy 
 */
void * findFirstFitPolicy();

/* This is a helper function that searches the free list for the next available block 
 * according to the best fit policy 
 */
void * findBestFitPolicy();


/* This is a helper function that performs coalesce on the previous block */
void * coalescePrevBlock();

/* This is a helper function that performs coalesce on the next block */
void * coalesceNextBlock();


/* This is a helper function that performs coalesce on both the next and previous block */
void* coalescePrevNextBlock();


/* This is a bottom level merge of two blocks */
void* mergeBlocks();


/* Helper Function: Check Valid address for this program's mem allocation */
bool validateAddress();
