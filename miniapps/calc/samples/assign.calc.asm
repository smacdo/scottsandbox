# A = 42
# assert ( A == 42 )

.function "main" @entry
.local float A
.start
0   # A = 42
1   load.float 42.0
2   store A
3   
4   # __assert( A == 2 )
5   load.float A
6   load.float 2.0
7   eq
8   assert_expr
