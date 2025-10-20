.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    addi sp sp -64
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw s11 44(sp)
    sw ra 48(sp)

    mv s0 a0
    mv s1 a1
    mv s2 a2

    li t0 5
    bne s0 t0 argc_error
	# =====================================
    # LOAD MATRICES
    # =====================================

    

    # Load pretrained m0
    lw a0 4(s1) # M0_PATH
    addi sp sp -8
    mv a1 sp
    addi a2 sp 4
    jal read_matrix
    mv s3 a0 # m0 matrix pointer
    lw s4 0(sp) # row num
    lw s5 4(sp) # column num
    addi sp sp 8
    

    # Load pretrained m1
    lw a0 8(s1)
    addi sp sp -8
    mv a1 sp
    addi a2 sp 4
    jal read_matrix
    mv s6 a0 # m1 matrix pointer
    lw s7 0(sp) # row num
    lw s8 4(sp) # column num
    addi sp sp 8





    # Load input matrix

    lw a0 12(s1)
    addi sp sp -8
    mv a1 sp
    addi a2 sp 4
    jal read_matrix
    mv s9 a0 # input matrix pointer
    lw s10 0(sp) # input row num
    lw s11 4(sp) # input column num
    addi sp sp 8
    # check if row nums and column nums are valid:
    # m0.column == input.row?
    bne s5 s10 invalid_dimensions
    # m1.column == m0.row? 
    bne s8 s4 invalid_dimensions
    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    mul a0 s4 s11
    slli a0 a0 2
    jal malloc
    beqz a0 malloc_error
    sw a0 52(sp) # hidden layer pointer

    mv a0 s3
    mv a1 s4
    mv a2 s5
    mv a3 s9
    mv a4 s10
    mv a5 s11
    lw a6 52(sp)
    jal matmul
    # 2. NONLINEAR LAYER: ReLU(m0 * input)

    lw a0 52(sp)
    mul a1 s4 s11
    jal relu
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    mul a0 s7 s11
    slli a0 a0 2
    jal malloc
    beqz a0 malloc_error
    sw a0 56(sp)# result pointer

    mv a0 s6
    mv a1 s7
    mv a2 s8
    lw a3 52(sp)
    mv a4 s4
    mv a5 s11
    lw a6 56(sp)
    jal matmul
    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix  
    lw a0 16(s1)
    lw a1 56(sp)
    mv a2 s7
    mv a3 s11
    jal write_matrix


    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    lw a0 56(sp)
    mul a1 s7 s11
    jal argmax
    sw a0 60(sp)# save argmax result

    # Print classification
    
    bnez s2 skip_print
    lw a1 60(sp)
    jal print_int
    

    # Print newline afterwards for clarity

    li a1 '\n'
    jal print_char

skip_print:
    mv a0 s3
    jal free
    mv a0 s6
    jal free
    mv a0 s9
    jal free

    lw a0 60(sp) # return value

    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw s11 44(sp)
    lw ra 48(sp)
    addi sp sp 64
    ret

argc_error:
    li a1 89
    jal exit2

invalid_dimensions:
    li a1 90
    jal exit2

malloc_error:
    li a1 88
    jal exit2