.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
	addi sp sp -20
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw ra 16(sp)

    mv s0 a0 # char filename
    mv s1 a1 # row num
    mv s2 a2 # column num

    # jal fopen
    # a1 is a pointer to a string containing the filename of the file to open
    # a2 is an integer denoting the permissions we open the file with
    mv a1 s0
    li a2 0
    jal fopen
    li t0 -1
    beq a0 t0 fopen_error
    mv s3 a0

    # jal fread
    # a1 is the file descriptor of the file we want to read from, previously returned by fopen
    # a2 is a pointer to the buffer that we’re going to read the bytes from the file into
    # a3 is the number of bytes to read from the file

    # read first 8 bytes to allocate memory
    addi sp sp -8
    mv a1 s3
    mv a2 sp
    li a3 8
    jal fread
    li t0 8
    bne a0 t0 fread_error
    lw t0 0(sp) # rows num
    lw t1 4(sp) # columns num
    addi sp sp 8

    sw t0 0(s1)
    sw t1 0(s2)

    mul a0 t0 t1
    slli a0 a0 2
    jal malloc
    beqz a0 malloc_error
    mv s0 a0 # matrix pointer

    # now read the full matrix
    mv a1 s3
    mv a2 s0
    lw t0 0(s1)
    lw t1 0(s2)
    mul t2 t0 t1
    slli a3 t2 2
    jal fread # t2 may be cleared  
    lw t0 0(s1)
    lw t1 0(s2)
    mul t2 t0 t1
    slli t2 t2 2
    bne a0 t2 fread_error

    # now close file
    # a1 is the file descriptor of the file we want to close to, previously returned by fopen
    mv a1 s3
    jal fclose
    li t0 -1
    beq a0 t0 fclose_error

    mv a0 s0
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw ra 16(sp)
    addi sp sp 20
    ret

fopen_error:
    li a1 90
    jal exit2
fread_error:
    li a1 91
    jal exit2
malloc_error:
    li a1 88
    jal exit2
fclose_error:
    li a1 92
    jal exit2