.function void main @entry
.local A
.start
        # A = 0
10      load.float 0.0
11      store A

        # if A > 0
20      load.float A
21      load.float 0.0
22      ge
23      jmpf 28
        # __failTest( "if broken" )
25      test_fail "if broken"
28      nop

# ---------------------------------------------------------------------
#       if ( A == 0 and 1 != 0 ) 
30      load.float A
31      load.float 0.0
32      eq
33      load.float 1.0
34      load.float 0.0
35      ne
36      and
37      jf 40

#       ...
38      no
39      jmp 41 

#       else
#       __failTest()
40      test_fail

# end
41      nop

if A == 0
	# ...
elseif A > 0
	__failTest("A should not be greater than zero")
else
	__failTest("A should not be less than zero")
end
