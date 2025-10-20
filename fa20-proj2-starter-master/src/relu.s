.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue
    li t3 1
    blt a1, t3, error # if a1 < 1 then target
    li t0 0 # t0 is the index

loop_start:
    bge t0, a1, loop_end # if t0 >= a1, loop ends

    li t4 4
    mul t1 t0 t4

    add t1 t1 a0

    lw t2, 0(t1) # load the value at t1 to t2
    bge t2, zero, loop_continue # if t2 >= 0 then loop continue
    sw zero, 0(t1)



loop_continue:
    addi t0 t0 1
    j loop_start

loop_end:


    # Epilogue

    
	ret

error:

    li a1 78
    jal exit2