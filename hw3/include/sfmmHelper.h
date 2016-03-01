/* This is a helper function that splits a free block into smaller blocks */
void split( );



/* This is a helper function that reads the allocation bit and determines if the block is free*/
unsigned long readAllocatedBit(void* ptr);

/* Helper function that reads the block size bits of a header then returns it */
unsigned long readBlockSize(void* ptr);

/* This is a helper function that reads the requested size of a headerblock */
unsigned long readRequestedSize(void* ptr);

/* This is a helper function that writes to the requested_size bits */
void writeRequestedSize(unsigned long requested_size, void* ptr);

/* This is a helper function that gets the previous block */
void* getPrevBlock();


/* This is a helper function that tests if an address is aligned */
bool isAligned(void* address);



/* This is a helper function that gets the next block */
void* getNextBlock();



/* Helper Function: Gets size of the next block */
void getNextBlockSize();



/*HelperFunction: Get size of the previous block */
void getPrevBlockSize();


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
