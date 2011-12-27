#!/usr/bin/ruby
###########################################################################


###########################################################################
### Main application
###########################################################################
# Reads a sokoban level file
inputfilename = ARGV[0]
puts "FILE IS : #{inputfilename}"

# Open the input file up
currentRow    = 0
currentCol    = 0
currentLevel  = -1

puzzles       = []

File.open( inputfilename ).each_line do |line|
    line.chomp!

    # skip empty lines
    if line.empty? or line.match /^\s+$/
        next
    end

    # is this the start of a new puzzle?
    if line.match /^;\s*(.+)\s*$/
        puzzles <<
        {
            :title => $1,
            :rows  => 0,
            :cols  => 0,
            :tiles => [],
            :player => nil,
            :jewels => [],
            :goals => []
        }

        currentRow = 0
        currentCol = 0
        currentLevel += 1

        puts $1
    else
        # Its (probably) a row of characters
        currentCol = 0
        rowtiles   = []

        line.each_byte do |c|
            if c == 0x23        # '#' wall
                rowtiles << :wall
            elsif c == 0x40     # '@' player
                rowtiles << :floor
                puzzles[currentLevel][:player] = [ currentRow, currentCol ]
            elsif c == 0x2b     # '+' player on goal
                rowtiles << :floor
                puzzles[currentLevel][:player] = [ currentRow, currentCol ]
                puzzles[currentLevel][:goals] << [ currentRow, currentCol ]
            elsif c == 0x24     # '$' box
                rowtiles << :floor
                puzzles[currentLevel][:jewels] << [ currentRow, currentCol ]
            elsif c == 0x2a     # '*' box on goal
                rowtiles << :floor
                puzzles[currentLevel][:jewels] << [ currentRow, currentCol ]
                puzzles[currentLevel][:goals]  << [ currentRow, currentCol ]
            elsif c == 0x2e     # '.' goal square
                rowtiles << :floor
                puzzles[currentLevel][:goals]  << [ currentRow, currentCol ]
            elsif c == 0x20     # ' ' floor
                rowtiles << :floor
            else
                puts "Unknown character: #{c}"
                puts "Line: '#{line}'"
                exit
            end

            # Move to next column
            currentCol += 1
        end

        # Store the column
        puzzles[ currentLevel ][:tiles] << rowtiles

        # Move to the next row in the level
        currentRow += 1
        puzzles[ currentLevel ][:rows] = currentRow

        # Is this the largest column so far for the level? if so store it
        if currentCol > puzzles[currentLevel][:cols]
            puzzles[currentLevel][:cols] = currentCol
        end
    end
end

puts puzzles[0].inspect

