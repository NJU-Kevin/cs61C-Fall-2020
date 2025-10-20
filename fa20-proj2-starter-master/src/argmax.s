.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:

    # Prologue
    li t0 1
    blt a1, t0, error # if t0 < t1 then target
    
    li t1 0 # current maximal value
    lw t2 0(a0) # t2 is the current maximum
    li t3 1 # t3 is the index used to 

loop_start:
    bge t3, a1, loop_end

    slli t4, t3, 2
    add t4 t4 a0

    lw t5 0(t4)

    ble t5, t2, loop_continue # if t5 <=2t1loop_continuetarget
    
    mv t1 t3
    mv t2 t5

loop_continue:
    addi t3 t3 1
    j loop_start

loop_end:
    
    mv a0 t1
    # Epilogue


    ret 

error:
    li a1 77
    jal exit2