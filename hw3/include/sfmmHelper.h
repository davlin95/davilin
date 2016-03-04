/* This is a helper function that splits a free block into smaller blocks */
void split( );



/* This is a helper function that reads the allocation bit and determines if the block is free*/
unsigned long readAllocatedBit(void* ptr);

/* A helper function that returns the mutiple of 4096 that fits encapsulates size*/
int calculateRequestSize(size_t size);

/* Helper function that reads the block size bits of a header then returns it */
unsigned long readBlockSize(void* ptr);

/*Helper function that aligns the heap */
void* alignHeap();

/* Helper function that updates the free block to being allocated*/
void updateBlockToAllocated(sf_free_header* block,size_t size);

/* Helper function to modify the pointer at the heap*/
void putLongAtPointer(void* pointer, unsigned long value);

/*Helper function to read the value of the header at the heap */
unsigned long  getLongAtPointer(void* pointer);

/*Helper function that returns the payload*/
void* getPayloadPtr(void* node);

/*Helper function that returns a created block */
void* createAlignedBlock(int multipleOfPageSize);

/* A function that inserts a footer struct into the memory heap */
void* insertFooter(void* startingAddress, int alloc, unsigned long block_size);

/* A function that inserts a header struct into the memory heap */
void* insertHeader(void* startingAddress, int alloc, int block_size, int requested_size);

/* This is a helper function that reads the requested size of a headerblock */
unsigned long readRequestedSize(void* ptr);

/* This is a helper function that aligns the pointer to the nearest 16 divisible address */
void* alignPtr(void* address);

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

/* HelperFunction: Returns an initialized block */
void initializeFreelistHeader(size_t size);

/* Function that helps find the first fit policy node */
void* findNextFitPolicy(size_t requested_size, sf_free_header* curr_head, sf_free_header* freelist_head);

/* Function that helps find the firstfit policy*/
sf_free_header* findFirstFitPolicy(size_t requested_size, sf_free_header* head);

/* Helper Function: Pads block to aligned boundaries. */
void padToAlignment();

 
/* Helper Function : Flip allocation Bit */
void flipAllocationBit();



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
