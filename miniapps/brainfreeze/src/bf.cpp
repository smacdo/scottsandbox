/**
 * Brainfreeze Language Interpreter
 * (c) 2009 Scott MacDonald. All rights reserved.
 *
 * Virtual machine source code
 */
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cassert>

#include "bf.h"

const char    CHR_EOF     = '\0';
const char    CHR_PTR_INC = '>';
const char    CHR_PTR_DEC = '<';
const char    CHR_MEM_INC = '+';
const char    CHR_MEM_DEC = '-';
const char    CHR_WRITE   = '.';
const char    CHR_READ    = ',';
const char    CHR_JMP_FWD = '[';
const char    CHR_JMP_BAC = ']';

const uint8_t OP_EOF     = 0;
const uint8_t OP_NOP     = 1;
const uint8_t OP_PTR_INC = 2;
const uint8_t OP_PTR_DEC = 3;
const uint8_t OP_MEM_INC = 4;
const uint8_t OP_MEM_DEC = 5;
const uint8_t OP_READ    = 6;
const uint8_t OP_WRITE   = 7;
const uint8_t OP_JMP_FWD = 9;
const uint8_t OP_JMP_BAC = 10;

namespace BF
{
    bool isInstruction( char c )
    {
        return c == CHR_PTR_INC || 
               c == CHR_PTR_DEC || 
               c == CHR_MEM_INC || 
               c == CHR_MEM_DEC ||
               c == CHR_WRITE   || 
               c == CHR_READ    || 
               c == CHR_JMP_FWD || 
               c == CHR_JMP_BAC;
    }

    Instruction convert( char c )
    {
        switch( c )
        {
            case CHR_PTR_INC:
                return Instruction( OP_PTR_INC, 1 );
            case CHR_PTR_DEC:
                return Instruction( OP_PTR_DEC, 1 );
            case CHR_MEM_INC:
                return Instruction( OP_MEM_INC, 1 );
            case CHR_MEM_DEC:
                return Instruction( OP_MEM_DEC, 1 );
            case CHR_READ:
                return Instruction( OP_READ, 0 );
            case CHR_WRITE:
                return Instruction( OP_WRITE, 0 );
            case CHR_JMP_FWD:
                return Instruction( OP_JMP_FWD, 0 );
            case CHR_JMP_BAC:
                return Instruction( OP_JMP_BAC, 0 );
            default:
                return Instruction( OP_NOP, 0 );
        }
    }

    void write( const Data& d )
    {
        std::cout << static_cast<char>(d);
    }

    Data read()         // fixme
    {
        Data d;

        if( std::cin >> d )
        {
            return d;
        }
        else    // if ( cin.eof() ) ... else ...
        {
            assert("Invalid input failure");
        }

        return d;
    }
}

BFProgram::BFProgram( const std::string& codestr )
    : m_codestr(codestr),
      m_instructions(),
      m_memory(),
      m_ip(),
      m_mp(),
      m_bCompiled(false),
      m_bFinished(false)
{
    m_instructions.clear();
    m_memory.resize(1024 * 32);

    m_mp = m_memory.begin();
}

void BFProgram::run()
{
    // Compile it first
    if(! m_bCompiled )
    {
        compile();
    }

    // Run the application until we hit the terminating point
    //   XXX can we get rid of the null sentinel?
    while( m_ip != m_instructions.end() && !m_ip->isA(OP_EOF) )
    {
        runStep();      // maybe bring m_ip++ here and change runStep name
    }
}

void BFProgram::runStep()
{
    switch( m_ip->opcode() )
    {
        case OP_PTR_INC:
            assert( m_mp != m_memory.end() );
            m_mp += m_ip->param();
            break;

        case OP_PTR_DEC:
            assert( m_mp != m_memory.begin() );
            m_mp -= m_ip->param();
            break;

        case OP_MEM_INC:
            assert( m_mp != m_memory.end() );
            *m_mp += m_ip->param();
            break;

        case OP_MEM_DEC:
            assert( m_mp != m_memory.end() );
            *m_mp -= m_ip->param();
            break;

        case OP_WRITE:
            BF::write( *m_mp );
            break;

        case OP_READ:
            *m_mp = BF::read();
            break;

        case OP_JMP_FWD:
            // Only execute if byte at data pointer is zero
            if( *m_mp != 0 ) 
            {
                break;
            }
#ifdef ENABLE_FAST_JUMPS
            else
            {
                m_ip += m_ip->param();
            }
#else
            while( true )
            {
                m_ip++;

                // don't go past the end
                assert( m_ip != m_instructions.end() );
                assert( m_ip->isA(OP_EOF) == false );
                
                // scope open/close?
                if( m_ip->isA(OP_JMP_FWD) )
                {
                    // open...
                    depth++;
                }
                else if ( m_ip->isA(OP_JMP_BAC) )
                {
                    // close
                    if( depth == 0 )
                    {
                        break;
                    }
                    else
                    {
                        depth--;
                    }
                }
            }
#endif

            break;

        case OP_JMP_BAC:
            // Only execute if byte at data pointer is non-zero
            if( *m_mp == 0 ) 
            {
                break;
            }
#ifdef ENABLE_FAST_JUMPS
            else
            {
                m_ip -= m_ip->param();
            }
#else
            while( true )
            {
                m_ip--;

                // don't go past the end
                assert( m_ip != m_instructions.begin() );
                
                // scope open/close?
                if( m_ip->isA(OP_JMP_BAC) )
                {
                    // open...
                    depth++;
                }
                else if ( m_ip->isA(OP_JMP_FWD) )
                {
                    // close
                    if( depth == 0 )
                    {
                        break;
                    }
                    else
                    {
                        depth--;
                    }
                }
            }
#endif
            break;

        default:
            assert("Unknown opcode");
    }

    m_ip++;            // maybe integrate into switch
}

/**
 * Compiles a brainfreeze program. It converts the program's
 * textual representation into a program containing only the
 * brainfreeze instructions.
 *
 * Additionally, it performs jump optimizations so jumps no
 * longer need to be calculated on execution.
 */
void BFProgram::compile()
{
    Instructions temp;
    Instruction  last = Instruction( OP_EOF, 0 );
    std::stack<int> jumps;  // record positions for [

    //
    // Scan the code string. Replace each BF instruction 
    // with its symbolic equivilant.
    //
    int icount = 0;
    for( std::string::iterator itr  = m_codestr.begin();
                               itr != m_codestr.end();
                             ++itr )
    {
        if( BF::isInstruction( *itr ) )
        {
            // Convert the character into an instruction
            Instruction instr = BF::convert( *itr );

            //
            // Is this a jump instruction?
            //
            if ( *itr == CHR_JMP_FWD )
            {
                // This is a forward jump. Record its position
                jumps.push(icount);
            }
            else if ( *itr == CHR_JMP_BAC )
            {
                // This is a backward jump. Pop the corresponding
                // forward jump marker off the stack, and update both
                // of the instructions with the location of their 
                // corresponding targets.
                int backpos = jumps.top(); jumps.pop();
                int dist    = icount - backpos;

                assert( dist > 0 );

                // Update the [ character
                temp[backpos].setParam( dist );

                // Update the ] (current) character
                instr.setParam( dist );
            }

            //
            // Was the last character a repeat of +, -, >, <
            //
            if ( false /* ( instr == last ) &&
                 ( instr == OP_PTR_INC || instr == OP_PTR_DEC ||
                   instr == OP_MEM_INC || instr == OP_MEM_DEC ) */ )
            {
                // Get the last character, and update its occurrence
                temp[icount-1].setParam( temp[icount-1].param() + 1 );
            }
            else
            {
                // Add the instruction to the instruction stream
                temp.push_back( instr );

                // Remember last instruction
                last = instr;

                // Increment instruction counter
                icount += 1;
            }

            // Remember the last instruction
            last = instr;
        }
    }

    // Verify the jump stack is empty. If not, then there is a
    // mismatched jump somewhere!
    assert( jumps.size() == 0 && "Mismatched jump detected" );

    // Insert end of program instruction
    temp.push_back( Instruction( OP_EOF, 0 ) );
    
    // Save it to m_instructions
    m_instructions.swap( temp );
    m_ip = m_instructions.begin();
}

Data BFProgram::valueAt( int offset ) const
{
    assert( offset >= 0 );
    assert( offset < static_cast<int>(m_memory.size()) );

    return m_memory[ offset ];
}

std::size_t BFProgram::instructionOffset() const
{
    return m_ip - m_instructions.begin();
}

std::size_t BFProgram::memoryPointerOffset() const
{
    return m_mp - m_memory.begin();
}
