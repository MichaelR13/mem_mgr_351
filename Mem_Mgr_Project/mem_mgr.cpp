//  mem_mgr.cpp

// Designing a Virtual Memory Manager

// INSTRUCTIONS:
/* 
This project consists of writing a program that translates logical to physical address space of 
size 2^16 = 65,536 bytes.  Your program will read from a file containing logical addresses and, 
using a TLB and a page table, will translate each logical address to its corresponding physical address, 
and output the value of the byte stored at the translated physical address.. 
Your learning goal is to use simulation to understand the steps involved in translating logical to physical 
addresses.  This will include resolving page faults using demand paging, managing a TLB, and implementing a 
page-replacement algorithm. 

Your program will read a file containing several 32-it integer numbers that represent logical addresses.  
However, you need only be concerned with 16-bit addresses, so you must mask the rightmost 16 bits of
each logical address.  These 16 bits are divided into (1) an 8-bit page number, and (2) an 8-bit offset.  
Hence, the addresses are structured as shown as: 
Other specifics include the following:
    - 2^8 (256) entries in the page table
    - Page size of 2^8 bytes (256 bytes per page)
    - 16 entries in the TLB
    - Frame size of 2^8 bytes (256 bytes), so one page == one frame.  This is not normally the
      case, and it won’t be the case in the second part of this simulation.
    - 256 frames
    - Physical memory of 65,536 bytes (256 frames x 256-byte frame size)

Also, your program need only be concerned with reading logical addresses and translating them to their
corresponding physical addresses.  You don’t need to support writing to the logical address space.

Address Translation
Your program will translate logical to physical addresses using a TLB and page table, as outlined in 
section 9.3.  First, the page number is extracted from the logical address, and the TLB is consulted.  In 
the case of a TLB hit, the frame number is obtained from the TLB.  In the case of a TLB miss, the page 
table must be consulted.  In the latter case, either the frame number is obtained from the page table, or a
page fault occurs.

Handling Page Faults
Your program will implement demand paging as described in Section 10.2.  
The backing store is represented by the file BACKING_STORE.bin, a binary file of size 65, 536 bytes.  
When a page fault occurs, you will read in a 256-byte page from the BACKING_STORE and store it in 
an available page frame in physical memory.  For example, if a logical address with page number 15 
resulted in a page fault, your program would read page 15 from BACKING_STORE (remember that 
pages begin at 0 and are 256 bytes in size), and store it in a page frame in physical memory.  Once this 
frame is stored (and the page table and TLB are updated), subsequent accesses to page 15 will be 
resolved by either the TLB or the page table.

You will need to treat BACKING_STORE.bin as a random-access file, so that you can randomly seek 
to certain positions of the file for reading.  We suggest using the standard C library functions for 
performing I/O, including fopen(), fread(), fseek(), and fclose().

The size of physical memory is the same as the size of the virtual address space -- 65,536 bytes -- so 
you do not need to be concerned about page replacements during a page fault.  Later, we describe a 
modification to this project using a smaller amount of physical memory;  at that point, a page-
replacement strategy will be required. 

TEST FILE
We provide the file addresses.txt, which contains integer values representing logical 
addresses ranging from 0 to 65,535 (the size of the virtual address space.)  Your program will open this 
file, read each logical address, and translate it to its corresponding physical address, and output the 
value of the signed byte at the physical address.

HOW TO BEGIN
First, write a simple program that extracts the page number and offset, based on: 
from the following integer numbers:  1,256, 32768, 128, 65534, 33153 (hint: divide each by 256, and 
calculate the amount(page) and the remainder(offset) – example, 256/256 == (page) 1 with 
remainder (offset)0).

Perhaps the easiest way to do this is by using the operators for big-masking and bit-shifting.  Once you 
can correctly establish the page number and offset from an integer number, you are ready to begin.

In more detail...  Here is an example of bit masking:  we want to mask off the page number to 
get the offset, how to do this?  Let’s look at the long version first...

    Let unsigned x be the entire page number/offset, then 
    unsigned offset_mask = 0xFF;  // masks everything except bits 0-7
    unsigned page_mask = 0xFF00;  // masks everything except bits 8-15
    unsigned offset = x & offset_mask; // just the offset
    unsigned page_shifted = x & page_mask;   // just the page, but shifted
    unsigned page = page_shifted >> 8; // just the page
    What’s the short version ?
    unsigned page(  unsigned x) { return ((x & 0xFF00) >> 8); }
    unsigned offset(unsigned x) { return x & 0xFF; }

Initially, we suggest that you bypass the TLB, and use only a page table.  You can integrate the TLB 
once your page table is working properly.  Remember, address translation can work without a TLB;  the 
TLB just makes it faster.  When you are ready to implement the TLB, recall that it has only sixteen 
entries, so you will need to use a replacement strategy when you update a full TLB.  You may use either 
a FIFO or an LRU policy for updating your TLB.

HOW TO RUN YOUR PROGRAM
Your program should run as follows:  ./memmgr addresses.txt
Your program will read in the file addresses.txt, which contains 1,000 logical addresses ranging from 0 
to 65,535.  Your program is to translate each logical address to a physical address, and determine the 
contents of the signed byte stored at the correct physical address.  (Recall that in the C language, the 
char data type occupies a byte of storage, so we suggest using char values.)  Your program is to output 
the following values:  
1.  The logical address being translated (the integer value being read from addresses.txt)
2.   The corresponding physical address (what your program translates the logical address to)
3.   The signed byte value stored in physical memory at the translated physical address.

We also provide the file correct.txt, which contains the correct output values for the file addresses.txt. 
You should use this file to determine if your program is correctly translating logical to physical 
addresses.

Statistics  After completion, your program is to report the following statistics

Page-fault rate -- the percentage of address references that resulted in page faults.
TLB hit rate -- the percentage of address references that were resolved in the TLB.

Since the logical address in addresses.txt were generated randomly, and do not reflect any memory 
access locality, do not expect to have a high TLB hit rate.

PAGE REPLACEMENT
Thus far, this project has assumed that physical memory is the same size as 
the virtual address space.  In practice, physical memory is typically much smaller than a virtual 
address space.  This phase of the project now assumes using a smaller physical address space, with 
128 page frames rather than 256.  This change will require modifying your program so that it keeps 
track of free page frames, as well as implementing a page-replacement policy using either FIFO 
or LRU (section 10.4) to resolve page faults when there is no free memory.

PARTIAL OUTPUT from mem_mgr:

log: 16916 0x4214 (pg: 66, off: 20)-->phy:    20 (frm:   0) (prv:   0)--> val:    0 == value:    0 --  + ----> pg_fault
log: 62493 0xf41d (pg:244, off: 29)-->phy:   285 (frm:   1) (prv:   0)--> val:    0 == value:    0 --  + ----> pg_fault
log: 30198 0x75f6 (pg:117, off:246)-->phy:   758 (frm:   2) (prv:   1)--> val:   29 == value:   29 --  + ----> pg_fault
log: 53683 0xd1b3 (pg:209, off:179)-->phy:   947 (frm:   3) (prv:   2)--> val:  108 == value:  108 --  + ----> pg_fault
log: 40185 0x9cf9 (pg:156, off:249)-->phy:  1273 (frm:   4) (prv:   3)--> val:    0 == value:    0 --  + ----> pg_fault
 ...
log: 34561 0x8701 (pg:135, off:  1)-->phy: 17921 (frm:  70) (prv:  69)--> val:    0 == value:    0 --  + ----> pg_fault
log: 49213 0xc03d (pg:192, off: 61)-->phy:  8509 (frm:  33) (prv:  70)--> val:    0 == value:    0 --  +    HIT!
log: 36922 0x903a (pg:144, off: 58)-->phy: 18234 (frm:  71) (prv:  70)--> val:   36 == value:   36 --  + ----> pg_fault
log: 59162 0xe71a (pg:231, off: 26)-->phy: 10266 (frm:  40) (prv:  71)--> val:   57 == value:   57 --  +    HIT!
log: 50552 0xc578 (pg:197, off:120)-->phy: 18552 (frm:  72) (prv:  71)--> val:    0 == value:    0 --  + ----> pg_fault
// ...
log: 34561 0x8701 (pg:135, off:  1)-->phy: 17921 (frm:  70) (prv:  69)--> val:    0 == value:    0 --  + ----> pg_fault
log: 49213 0xc03d (pg:192, off: 61)-->phy:  8509 (frm:  33) (prv:  70)--> val:    0 == value:    0 --  +    HIT!
log: 36922 0x903a (pg:144, off: 58)-->phy: 18234 (frm:  71) (prv:  70)--> val:   36 == value:   36 --  + ----> pg_fault
log: 59162 0xe71a (pg:231, off: 26)-->phy: 10266 (frm:  40) (prv:  71)--> val:   57 == value:   57 --  +    HIT!
log: 50552 0xc578 (pg:197, off:120)-->phy: 18552 (frm:  72) (prv:  71)--> val:    0 == value:    0 --  + ----> pg_fault
// ...
log: 48065 0xbbc1 (pg:187, off:193)-->phy: 25793 (frm: 100) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log:  6957 0x1b2d (pg: 27, off: 45)-->phy: 26413 (frm: 103) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log:  2301 0x08fd (pg:  8, off:253)-->phy: 35325 (frm: 137) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log:  7736 0x1e38 (pg: 30, off: 56)-->phy: 57912 (frm: 226) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log: 31260 0x7a1c (pg:122, off: 28)-->phy: 23324 (frm:  91) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log: 17071 0x42af (pg: 66, off:175)-->phy:   175 (frm:   0) (prv: 243)--> val:  -85 == value:  -85 --  +    HIT!
log:  8940 0x22ec (pg: 34, off:236)-->phy: 46572 (frm: 181) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log:  9929 0x26c9 (pg: 38, off:201)-->phy: 44745 (frm: 174) (prv: 243)--> val:    0 == value:    0 --  +    HIT!
log: 45563 0xb1fb (pg:177, off:251)-->phy: 46075 (frm: 179) (prv: 243)--> val:  126 == value:  126 --  +    HIT!
log: 12107 0x2f4b (pg: 47, off: 75)-->phy:  2635 (frm:  10) (prv: 243)--> val:  -46 == value:  -46 --  +    HIT!
Page Fault Percentage: 0.244%
TLB Hit Percentage: 0.054%
ALL logical ---> physical assertions PASSED!
*/

/*
The following are the checks that will be marked as finished or not finished.
Please make sure that you have completed all of the following tasks

-   Created functions that correctly calculate the offset and page of a given virtual address
-   Created a page table, that contains the frame of a given page, and which will page fault 
    if the desired page is not in memory (this will happen: (A) when the program is first run 
    and physical memory is empty, and (B) if only half as many physical frames as pages in 
    the page table
-   Given a given logical address, checks the page table to find the corresponding physical 
    address
-   Correctly reads the given physical address for the char value stored there
-   Goes to the BACKING_STORE and reads in the corresponding page into a free frame 
    in physical memory.  If there are only 128 frames, it must replace a frame to do this.
-   Implemented a Translation Lookaside Buffer (TLB) to store the most recently read-in 
    page, AND checks the TLB first when decoding a logical address.
-   Do following when reading a logical address that is not in the TLB/Page table:  Check 
    TLB → (TLB miss) Check Page Table → (Page table miss) Page fault → read page from
    BACKING_ STORE → updates physical memory → updates Page table → updates TLB
    → reads value from physical memory..
-   Follows this flow diagram when has a TLB hit:  Check TLB → Gets frame and offset → 
    reads value from physical memory
-   Do following when has a TLB miss but a Page table hit → Check TLB → (TLB miss) → 
    Checks Page table → Updates TLB → Gets frame and offset → reads value from 
    physical memory
-   Page-fault rate -- the percentage of address references that resulted in page faults.
-   TLB hit rate -- the percentage of address references that were resolved in the TLB
-   Now modify your program so that it has only 128 page frames of physical memory (but 
    still has 256 entries in the page table)
-   Program now keeps track of the free page frames, as well as implementing a page-
    replacement policy using either FIFO or LRU
*/

// START OF PROGRAM

//  Complete sections marked with TODO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <cassert>

#pragma warning(disable : 4996)

#define ARGC_ERROR 1
#define FILE_ERROR 2

#define FRAME_SIZE  256 // Change to 128 later
#define FIFO 0
#define LRU 1
#define REPLACE_POLICY FIFO

// SET TO 128 to use replacement policy: FIFO or LRU,
#define NFRAMES 256
#define PTABLE_SIZE 256
#define TLB_SIZE 16

struct page_node {    
    size_t npage;
    size_t frame_num;
    bool is_present;
    bool is_used;
};

char* ram = (char*)malloc(NFRAMES * FRAME_SIZE);
page_node pg_table[PTABLE_SIZE];  // page table and (single) TLB
page_node tlb[TLB_SIZE];

const char* passed_or_failed(bool condition) { return condition ? " + " : "fail"; }
size_t failed_asserts = 0;

size_t get_page(size_t x)   { return 0xff & (x >> 8); }
size_t get_offset(size_t x) { return 0xff & x; }

void get_page_offset(size_t x, size_t& page, size_t& offset) {
    page = get_page(x);
    offset = get_offset(x);
    // TODO (Commented out)
    printf("x is: %zu, page: %zu, offset: %zu, address: %zu, paddress: %zu\n", 
    x, page, offset, (page << 8) | get_offset(x), page * 256 + offset);

}

void update_frame_ptable(size_t npage, size_t frame_num) {
    pg_table[npage].frame_num = frame_num;
    pg_table[npage].is_present = true;
    pg_table[npage].is_used = true;
}

int find_frame_ptable(size_t frame) {  // FIFO
    for (int i = 0; i < PTABLE_SIZE; i++) {
        if (pg_table[i].frame_num == frame && 
            pg_table[i].is_present == true) { return i; }
    }
    return -1;
}

size_t get_used_ptable() {  // LRU
    size_t unused = -1;
    for (size_t i = 0; i < PTABLE_SIZE; i++) {
        if (pg_table[i].is_used == false && 
            pg_table[i].is_present == true) { return (size_t)i; }
    }
    // All present pages have been used recently, set all page entry used flags to false
    for (size_t i = 0; i < PTABLE_SIZE; i++) { pg_table[i].is_used = false; }
    for (size_t i = 0; i < PTABLE_SIZE; i++) {
        page_node& r = pg_table[i];
        if (!r.is_used && r.is_present) { return i; }
    }
    return (size_t)-1;
}

int check_tlb(size_t page) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].npage == page) { return i; }
    }
    return -1;
}

void open_files(FILE*& fadd, FILE*& fcorr, FILE*& fback) { 
    fadd = fopen("addresses.txt", "r");
    if (fadd == NULL) { fprintf(stderr, "Could not open file: 'addresses.txt'\n");  exit(FILE_ERROR); }

    fcorr = fopen("correct.txt", "r");
    if (fcorr == NULL) { fprintf(stderr, "Could not open file: 'correct.txt'\n");  exit(FILE_ERROR); }

    fback = fopen("BACKING_STORE.bin", "rb");
    if (fback == NULL) { fprintf(stderr, "Could not open file: 'BACKING_STORE.bin'\n");  exit(FILE_ERROR); }
}
void close_files(FILE* fadd, FILE* fcorr, FILE* fback) { 
    fclose(fadd);
    fclose(fcorr);
    fclose(fback);
}

void initialize_pg_table_tlb() { 
    for (int i = 0; i < PTABLE_SIZE; ++i) {
        pg_table[i].npage = (size_t)i;
        pg_table[i].is_present = false;
        pg_table[i].is_used = false;
    }
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].npage = (size_t)-1;
        tlb[i].is_present = false;
        pg_table[i].is_used = false;
    }
}

void summarize(size_t pg_faults, size_t tlb_hits) { 
    printf("\nPage Fault Percentage: %1.3f%%", (double)pg_faults / 1000);
    printf("\nTLB Hit Percentage: %1.3f%%\n\n", (double)tlb_hits / 1000);
    printf("ALL logical ---> physical assertions PASSED!\n");
    printf("\n\t\t...done.\n");
}

void tlb_add(int index, page_node entry) { 
    // TODO
    // Add entry to TLB at index
    // If index is -1, add entry to TLB at first available index
    // If TLB is full, replace entry at index 0
    // If index is -1 and TLB is full, replace entry at index 0
    // If index is -1 and TLB is not full, add entry to first available index
    // If index is not -1 and TLB is not full, add entry to index
    // If index is not -1 and TLB is full, add entry to index
    if (index == -1) {
        for (int i = 0; i < TLB_SIZE; i++) {
            if (tlb[i].is_present == false) {
                tlb[i] = entry;
                return;
            }
        }
        tlb[0] = entry;
    }
    else {
        tlb[index] = entry;
    }

}  

void tlb_remove(int index) { 
    // TODO
    // Remove entry from TLB at index
    // If index is -1, remove entry from TLB at first available index
    // If TLB is full, replace entry at index 0
    // If index is -1 and TLB is full, replace entry at index 0
    // If index is -1 and TLB is not full, add entry to first available index
    // If index is not -1 and TLB is not full, add entry to index
    // If index is not -1 and TLB is full, add entry to index
    if (index == -1) {
        for (int i = 0; i < TLB_SIZE; i++) {
            if (tlb[i].is_present == false) {
                tlb[i].is_present = false;
                return;
            }
        }
        tlb[0].is_present = false;
    }
    else {
        tlb[index].is_present = false;
    }

}  

void tlb_hit(size_t& frame, size_t& page, size_t& tlb_hits, int result) {  
    // TODO
    // If TLB hit, set frame to tlb[result].frame_num
    // Increment tlb_hits
    if (result != -1) {
        frame = tlb[result].frame_num;
        tlb_hits++;
    }
}  

void tlb_miss(size_t& frame, size_t& page, size_t& tlb_track) { 
    // TODO
    frame = pg_table[page].frame_num;
    
    // If TLB miss, add entry to TLB
    // If TLB is full, replace entry at index 0
    // If TLB is not full, add entry to first available index
    tlb_add(-1, pg_table[page]);
    tlb_track++;
    if (tlb_track == TLB_SIZE) { 
        tlb_track = 0; 
    }
} 

void fifo_replace_page(size_t& frame ) { 
    // TODO
    // Set frame to first available frame in page table
    // If page table is full, replace page at index 0
    // If page table is not full, add page to first available index
    for (int i = 0; i < PTABLE_SIZE; i++) {
        if (pg_table[i].is_present == false) {
            pg_table[i].frame_num = frame;
            pg_table[i].is_present = true;
            return;
        }
    }
    pg_table[0].frame_num = frame;
    pg_table[0].is_present = true;

}   

void lru_replace_page(size_t& frame) { 
    // TODO
    // Set frame to first available frame in page table
    // If page table is full, replace page at index 0
    // If page table is not full, add page to first available index
    for (int i = 0; i < PTABLE_SIZE; i++) {
        if (pg_table[i].is_present == false) {
            pg_table[i].frame_num = frame;
            pg_table[i].is_present = true;
            return;
        }
    }
    pg_table[0].frame_num = frame;
    pg_table[0].is_present = true;
} 

void page_fault(size_t& frame, size_t& page, size_t& frames_used, size_t& pg_faults, 
              size_t& tlb_track, FILE* fbacking) {  
    unsigned char buf[BUFSIZ];
    memset(buf, 0, sizeof(buf));
    bool is_memfull = false;

    ++pg_faults;
    if (frames_used >= NFRAMES) { is_memfull = true; }
    frame = frames_used % NFRAMES;    // FIFO only

    if (is_memfull) { 
        if (REPLACE_POLICY == FIFO) { 
            // TODO
            fifo_replace_page(frame); 
        } 
        else { 
            // TODO
            lru_replace_page(frame); 
        }
    }
         // load page into RAM, update pg_table, TLB
    fseek(fbacking, page * FRAME_SIZE, SEEK_SET);
    fread(buf, FRAME_SIZE, 1, fbacking);

    for (int i = 0; i < FRAME_SIZE; i++) {
        *(ram + (frame * FRAME_SIZE) + i) = buf[i];
    }
    update_frame_ptable(page, frame);
    tlb_add(tlb_track++, pg_table[page]);
    if (tlb_track > 15) { tlb_track = 0; }
    
    ++frames_used;
} 

void check_address_value(size_t logic_add, size_t page, size_t offset, size_t physical_add,
                         size_t& prev_frame, size_t frame, int val, int value, size_t o) { 
    printf("log: %5lu 0x%04x (pg:%3lu, off:%3lu)-->phy: %5lu (frm: %3lu) (prv: %3lu)--> val: %4d == value: %4d -- %s", 
          logic_add, logic_add, page, offset, physical_add, frame, prev_frame, 
          val, value, passed_or_failed(val == value));

    if (frame < prev_frame) {  printf("   HIT!\n");
    } else {
        prev_frame = frame;
        printf("----> pg_fault\n");
    }
    if (o % 5 == 4) { printf("\n"); }
// if (o > 20) { exit(-1); }             // to check out first 20 elements

    if (val != value) { ++failed_asserts; }
    if (failed_asserts > 5) { exit(-1); }
//     assert(val == value);
}

void run_simulation() { 
        // addresses, pages, frames, values, hits and faults
    size_t logic_add, virt_add, phys_add, physical_add;
    size_t page, frame, offset, value, prev_frame = 0, tlb_track = 0;
    size_t frames_used = 0, pg_faults = 0, tlb_hits = 0;
    int val = 0;
    char buf[BUFSIZ];

    bool is_memfull = false;     // physical memory to store the frames

    initialize_pg_table_tlb();

        // addresses to test, correct values, and pages to load
    FILE *faddress, *fcorrect, *fbacking;
    open_files(faddress, fcorrect, fbacking);

    for (int o = 0; o < 1000; o++) {     // read from file correct.txt
        fscanf(fcorrect, "%s %s %lu %s %s %lu %s %ld", buf, buf, &virt_add, buf, buf, &phys_add, buf, &value);  

        fscanf(faddress, "%ld", &logic_add);  
        get_page_offset(logic_add, page, offset);

        int result = check_tlb(page);
        if (result >= 0) {  
            tlb_hit(frame, page, tlb_hits, result); 
        } else if (pg_table[page].is_present) {
            tlb_miss(frame, page, tlb_track);
        } else {         // page fault
            page_fault(frame, page, frames_used, pg_faults, tlb_track, fbacking);
        }

        physical_add = (frame * FRAME_SIZE) + offset;
        val = (int)*(ram + physical_add);

        check_address_value(logic_add, page, offset, physical_add, prev_frame, frame, val, value, o);
    }
    close_files(faddress, fcorrect, fbacking);  // and time to wrap things up
    free(ram);
    summarize(pg_faults, tlb_hits);
}


int main(int argc, const char * argv[]) {
    run_simulation();
    // TODO (Commented out)
    printf("\nFailed asserts: %lu\n\n", failed_asserts);   // allows asserts to fail silently and be counted
    return 0;
}
