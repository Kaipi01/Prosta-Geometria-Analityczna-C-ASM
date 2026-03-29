.section .rodata
.VALUE_2: 
    .double 2.0
.VALUE_0_5: 
    .double 0.5

.section .text

.globl calc_distance_of_two_points
.globl calc_midpoint
.globl calc_line_equation
.globl calc_triangle_perimeter
.globl calc_triangle_area

.type calc_distance_of_two_points, @function
#; Function calculate length of the segment |AB|
calc_distance_of_two_points:
    pushq %rbp
    movq %rsp, %rbp
    #;Arg 1: xmm0  = xa 
    #;Arg 2: xmm1  = ya  
    #;Arg 3: xmm2 = xb
    #;Arg 4: xmm3 = yb

    #; return %xmm0 - segment length of |AB|

    #; The length of the segment with ends at points A=(xa,ya) and B=(xb,yb) is expressed by the formula:
    #; |AB| = √((xb − xa)^2 + (yb − ya)^2)

    movsd %xmm2, %xmm12
    subsd %xmm0, %xmm12 #; xmm12 = xb − xa
    mulsd %xmm12, %xmm12 #; xmm12 = (xb − xa)^2

    movsd %xmm3, %xmm13
    subsd %xmm1, %xmm13 #; xmm13 = yb − ya
    mulsd %xmm13, %xmm13 #; xmm13 = (yb − ya)^2

    movsd %xmm12, %xmm14
    addsd %xmm13, %xmm14 #; %xmm14 = (xb − xa)^2 + (yb − ya)^2

    movsd %xmm14, %xmm0
    call sqrt
    #; %xmm0 = √((xb − xa)^2 + (yb − ya)^2)

    popq %rbp
    ret

.type calc_midpoint, @function
#; Function calculate coordinates of point S, which lies in the middle of the segment |AB|
calc_midpoint:
    pushq %rbp
    movq %rsp, %rbp
    #; Arg 1: xmm0  = x1 
    #; Arg 2: xmm1  = y1  
    #; Arg 3: xmm2 = x2
    #; Arg 4: xmm3 = y2
    #; return %xmm0 - x value of S
    #; return %xmm1 - y value of S

    #; The midpoint of the segment AB, where A=(x1,y1) and B=(x2,y2), is the point:
    #; S = ((x1 + x2) / 2; (y1 + y2) / 2)

    addsd %xmm2, %xmm0 #; %xmm0 = x1 + x2
    addsd %xmm3, %xmm1 #; %xmm1 = y1 + y2

    movsd .VALUE_2(%rip), %xmm15  

    divsd %xmm15, %xmm0 #; xmm0 = xmm0 / 2
    divsd %xmm15, %xmm1 #; xmm1 = xmm1 / 2
    popq %rbp
    ret

.type calc_line_equation, @function
#; Function calculate equation of the line passing through points A and B
calc_line_equation:
    pushq %rbp
    movq %rsp, %rbp

    #; Arg 1: xmm0 = x1 
    #; Arg 2: xmm1 = y1  
    #; Arg 3: xmm2 = x2
    #; Arg 4: xmm3 = y2
    #; return %xmm0 - value of a
    #; return %xmm1 - value of b

    #; If we know the coordinates of two points on a line, we can determine its equation.
    #; The equation of the line: y = ax + b
    #; Must calculate the slope of the line: a and the constant term b

    #; The formula for calculating a and b:
    #; y1 = ax1 + b
    #; y2 = ax2 + b

    #; y2 − y1 = ax1 - ax2
    #; y2 − y1 = a(x2 − x1)
    #; a = (y2 − y1) / (x2 − x1)
    #; b = y1 - ax1

    movsd %xmm3, %xmm12
    subsd %xmm1, %xmm12 #; xmm12 = y2 − y1

    movsd %xmm2, %xmm13
    subsd %xmm0, %xmm13 #; xmm13 = x2 − x1

    divsd %xmm13, %xmm12 #; xmm12 = (y2 − y1) / (x2 − x1)
    movq %xmm12, %xmm13 #; xmm13 = a

    movsd %xmm13, %xmm14 #; save a value to xmm14

    mulsd %xmm0, %xmm13 #; xmm13 = a * x1

    movsd %xmm1, %xmm15
    subsd %xmm13, %xmm15 #; xmm15 = b = y1 - ax1

    movsd %xmm14, %xmm0
    movsd %xmm15, %xmm1
    popq %rbp
    ret


.type calc_triangle_perimeter, @function
calc_triangle_perimeter:
    pushq %rbp
    movq %rsp, %rbp
    #; Arg 1: xmm0 = a 
    #; Arg 2: xmm1 = b  
    #; Arg 3: xmm2 = c
    #; return %xmm0 => a+b+c

    addsd %xmm1, %xmm0
    addsd %xmm2, %xmm0

    popq %rbp
    ret

.type calc_triangle_area, @function
calc_triangle_area:
    pushq %rbp
    movq %rsp, %rbp
    #; Arg 1: xmm0 = a 
    #; Arg 2: xmm1 = h  
    #; return %xmm0 => 0.5 * a * h

    movsd .VALUE_0_5(%rip), %xmm15  

    mulsd %xmm15, %xmm0 #; xmm0 = 0.5 * a
    mulsd %xmm1, %xmm0 #; xmm0 = h * xmm0
    
    popq %rbp
    ret 
