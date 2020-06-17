mov r0,#2
mov r1,#5
loop:
sub r0,r0,#1
mul r2,r1,r0
cmp r2,#6
bge loop
