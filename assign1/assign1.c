#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

/*********************************************************************
 *
 * These C functions use patterns and functionality often found in
 * operating system code. Your job is to implement them. Of course you
 * should write test cases. However, do not hand in your test cases
 * and (especially) do not hand in a main() function since it will
 * interfere with our tester.
 *
 * Additional requirements on all functions you write:
 *
 * - you may not refer to any global variables
 *
 * - you may not call any functions except those specifically
 *   permitted in the specification
 *
 * - your code must compile successfully on CADE lab Linux
 *   machines when using:
 *
 * /usr/bin/gcc -O2 -fmessage-length=0 -pedantic-errors -std=c99 -Werror -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-include-dirs -Wno-unused-parameter -Wshadow -Wuninitialized -Wold-style-definition -c assign1.c 
 *
 * NOTE 1: Some of the specifications below are specific to 64-bit
 * machines, such as those found in the CADE lab.  If you choose to
 * develop on 32-bit machines, some quantities (the size of an
 * unsigned long and the size of a pointer) will change. Since we will
 * be grading on 64-bit machines, you must make sure your code works
 * there.
 *
 * NOTE 2: You should not need to include any additional header files,
 * but you may do so if you feel that it helps.
 *
 * HANDIN: submit your finished file, still called assign1.c, in Canvas
 *
 *
 *********************************************************************/
static unsigned long swap_bytes(unsigned long x, int size, int hi, int lo);


/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 *
 *********************************************************************/
unsigned long byte_sort(unsigned long arg) {
    unsigned long sort = arg;
    int len = 8;
    for (int i = 0; i < len; ++i) {
        unsigned long b1 = (sort >> (len * i)) & 0xff;
        for (int j = i + 1; j < len; ++j) {
            unsigned long b2 = (sort >> (len * j)) & 0xff;
            if (b1 > b2 && j > i) {
                sort = swap_bytes(sort, len, j, i);
                b1 = (sort >> (len * i)) & 0xff;
            }
        }
    }
    return sort;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/
unsigned long nibble_sort(unsigned long arg) {
    unsigned long sort = arg;
    int len = 4;
    for (int i = 0; i < 16; ++i) {
        unsigned long b1 = (sort >> (len * i)) & 0xf;
        for (int j = i + 1; j < 16; ++j) {
            unsigned long b2 = (sort >> (len * j)) & 0xf;
            if (b1 > b2 && j > i) {
                sort = swap_bytes(sort, len, j, i);
                b1 = (sort >> (len * i)) & 0xf;
            }
        }
    }
    return sort;
}

void test_byte_sort() {
    // * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
    unsigned long x = 0x0403deadbeef0201;

    for (int i = 0; i < 8; ++i) {
        unsigned long b1 = (x >> (8 * i)) & 0xff;
        printf("%lu\n", b1);
    }
    printf("------------------------------\n");
    unsigned long res = byte_sort(x);
    for (int i = 0; i < 8; ++i) {
        unsigned long b1 = (res >> (8 * i)) & 0xff;
        printf("%lu\n", b1);
    }
    printf("------------------------------\n");
//    printf("%lu\n%lu\n", res, 0xefdebead04030201);

}

void test_nib_sort() {
//    * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000

    unsigned long x = 0x0403deadbeef0201;
    for (int i = 0; i < 16; ++i) {
        unsigned char b1 = (x >> (4 * i)) & 0xf;
        printf("%u\n", b1);
    }
    printf("------------------------------\n");
    unsigned long res = nibble_sort(x);
//    unsigned long res = swap_bytes(x, 4, 1, 0);
    for (int i = 0; i < 16; ++i) {
        unsigned char b1 = (res >> (4 * i)) & 0xf;
        printf("%u\n", b1);
    }
    printf("------------------------------\n");
    printf("%lu\n%lu\n", 0xfeeeddba43210000, res);
}

/*
 * Helper function to swap bytes at position hi and lo
 * TODO: generalize function to work with different sizes e.g. bytes, nibbles, etc.
 */
static unsigned long swap_bytes(unsigned long x, int size, int hi, int lo) {
    unsigned long mask = (size == 8) ? 0xff : 0xf;
    unsigned long lside = x & ~(mask << (hi * size));
    unsigned long swp =
            ((x & (mask << (lo * size))) << (size * (hi - lo))) | (x & (mask << (hi * size))) >> (size * (hi - lo));
    unsigned long zeroed = lside & ~(mask << (lo * size));
    return zeroed | swp;
}


/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *  
 *********************************************************************/

struct elt {
    char val;
    struct elt *link;
};

struct elt *name_list(void) {
    char *name = "Gradey";

    struct elt *head = (struct elt *) malloc(sizeof(struct elt));

    if (!head) {
        printf("Malloc for head failed. Exiting. . .");
        free(head);
        return NULL;
    }

    struct elt *curr = (struct elt *) malloc(sizeof(struct elt));

    if (!curr) {
        free(curr);
        free(head);
        return NULL;
    }

    head->val = 'G';
    head->link = curr;

    for (int i = 1; i < 6; ++i) {
        curr->val = name[i];
        curr->link = (struct elt *) malloc(sizeof(struct elt));

        if (!curr->link) {
            struct elt *prev = head;
            curr = prev->link;
            while (prev->link != NULL) {
                free(prev);
                prev = curr;
                curr = curr->link;
            }
            return NULL;
        }

        curr = curr->link;
    }

    curr->link = NULL;

    return head;
}

int main() {

    struct elt *p = malloc(sizeof(struct elt));
    struct elt *head = name_list();
    while (head->link != NULL) {
        printf("%c", head->val);
        head = head->link;
    }

    return 0;
}

/*********************************************************************
 *
 * convert()
 *
 * specification: depending on the value of "mode", print "value" as
 * octal, binary, or hexidecimal to stdout, followed by a newline
 * character
 *
 * extra requirement 1: output must be via putc()
 *
 * extra requirement 2: print nothing if "mode" is not one of OCT,
 * BIN, or HEX
 *
 * extra requirement 3: all leading/trailing zeros should be printed
 *
 * EXAMPLE: convert (HEX, 0xdeadbeef) should print
 * "00000000deadbeef\n" (including the newline character but not
 * including the quotes)
 *
 *********************************************************************/

enum format_t {
    OCT = 66, BIN, HEX
};

void convert(enum format_t mode, unsigned long value) {
}

/*********************************************************************
 *
 * draw_me()
 *
 * this function creates a file called me.txt which contains an ASCII-art
 * picture of you (it does not need to be very big). the file must (pointlessly,
 * since it does not contain executable content) be marked as executable.
 * 
 * extra requirement 1: you may only call the function syscall() (type "man
 * syscall" to see what this does)
 *
 * extra requirement 2: you must ensure that every system call succeeds; if any
 * fails, you must clean up the system state (closing any open files, deleting
 * any files created in the file system, etc.) such that no trash is left
 * sitting around
 *
 * you might be wondering how to learn what system calls to use and what
 * arguments they expect. one way is to look at a web page like this one:
 * http://blog.rchapman.org/post/36801038863/linux-system-call-table-for-x86-64
 * another thing you can do is write some C code that uses standard I/O
 * functions to draw the picture and mark it as executable, compile the program
 * statically (e.g. "gcc foo.c -O -static -o foo"), and then disassemble it
 * ("objdump -d foo") and look at how the system calls are invoked, then do
 * the same thing manually using syscall()
 *
 *********************************************************************/

void draw_me(void) {
}























