.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:

    # Prologue
    li t0, 1
    blt a2, t0, length_error # if t0 < t1 then target
    blt a3, t0, stride_error # if a3 < 01 stride_errorarget
    blt a4, t0, stride_error
    li t1, 0 # v1
    li t2, 0 # v2
    li t3, 0 # desired result
    li t4, 0 # index for traversal

loop_start:
    bge t4, a2, loop_end
    slli t5, t1, 2
    add t5, t5, a0 # t5 is the current address of v0
    lw t6 0(t5)

    slli t5, t2, 2
    add t5, t5, a1 # now t5 is current address of v1
    lw t0 0(t5) 

    mul t5, t6, t0
    add t3, t3, t5

    addi t4 t4 1
    add t1 t1 a3
    add t2 t2 a4
    j loop_start

loop_end:

    mv a0 t3
    # Epilogue   
    ret

length_error:
    li a1 75
    jal exit2
stride_error:
    li a1 76
    jal exit2