#__assert( blackbox(40) == 42 )
#
#def blackbox( a )
#	return a + 2
#end

.function void main @entry
.start
0   # blackbox(40)
1   load.float 40.0
2   push_arg
3   load.function blackbox
4   call
5   # __assert( ... )
6   load.float $ret         # this is a special register containing return
7   load.float 42.0
8   eq
9   assert_test "blackbox(40) == 42"


.function float blackbox
.arg float a
.start
0   load.float a
1   load.float 2.0
2   add
3   return

