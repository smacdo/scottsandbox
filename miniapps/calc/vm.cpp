#include <iostream>
#include <vector>
#include <deque>
#include <cassert>
#include <algorithm>
#include <boost/bind.hpp>

enum EOpcode
{
    EOP_NOP = 0,
    EOP_PRINT_FLOAT,
    EOP_PRINT_INT,
    EOP_PRINT_STRING,
    EOP_PRINT_NEWLINE,
    EOP_DEBUG_DUMP,
    EOP_PUSH_FLOAT,
    EOP_POP,
    EOP_DUP,
    EOP_ADD,
    EOP_SUB,
    EOP_CMP_Z,
    EOP_JMP,
    EOP_JMP_T,
};

enum EExecutionStatus
{
    ESTATUS_WAITING,
    ESTATUS_RUNNING,
    ESTATUS_FINISHED
};

enum EValueType
{
    EVALUE_TYPE_NIL,
    EVALUE_TYPE_FLOAT,
    EVALUE_TYPE_INT
};

union DataValue
{
    float fval;
    int ival;
};

struct Instruction
{
    explicit Instruction( EOpcode op )
        : op(op),
          fval( 0.0f )
    {
    }

    Instruction( EOpcode op, float v )
        : op(op),
          fval(v)
    {
    }

    Instruction( EOpcode op, int v )
        : op(op),
          ival(v)
    {
    }

    EOpcode op;

    union
    {
        float fval;
        int   ival;
    };
};

typedef std::vector<Instruction>   InstructionList;
typedef InstructionList::iterator  InstructionListItr;
typedef InstructionList::const_iterator InstructionListConstItr;

struct Program
{
    Program( const std::vector<Instruction>& instructions )
        : instructions(instructions)
    {
    }

    std::vector<std::string> stringtable;
    std::vector<Instruction> instructions;
};

/**
 * ThreadContext contains all information required to execute an instruction
 * in the virtual machine
 */
struct ThreadContext
{
    ThreadContext( const Program& program )
        : program( program ),
          status( ESTATUS_WAITING ),
          ip( 0u ),
          stack()
    {
    }

    // The program that is being executed by this thread context
    const Program& program;

    // Program execution status
    EExecutionStatus status;

    // The current instruction that is scheduled for execution
    std::size_t ip;

    // Contents of the thread's stack
    std::deque<DataValue> stack;
};

void execute( const Instruction& instruction, ThreadContext& context )
{
    DataValue da;
    DataValue db;

    switch ( instruction.op )
    {
        case EOP_NOP:
            break;

        case EOP_PRINT_FLOAT:
            da = context.stack.front();
            context.stack.pop_front();

            std::cout << da.fval;
            break;

        case EOP_PRINT_INT:
            da = context.stack.front();
            context.stack.pop_front();
            break;

        case EOP_PRINT_STRING:
            da = context.stack.front();
            context.stack.pop_front();

            assert( (size_t) da.fval < context.program.stringtable.size() );
            std::cout << context.program.stringtable[da.fval];
            break;

        case EOP_PRINT_NEWLINE:
            std::cout << std::endl;
            break;

        case EOP_DEBUG_DUMP:
            std::cout << " ----- debug dump ----- " << std::endl;
            std::cout << "stacksize: " << context.stack.size() << std::endl;

            for ( auto itr  = context.stack.begin();
                       itr != context.stack.end();
                     ++itr )
            {
                DataValue dv = *itr;

                std::cout << "int: "   << dv.ival << ", "
                          << "float: " << dv.fval
                          << std::endl;
            }

            std::cout << "------" << std::endl;
             break;

        case EOP_DUP:
            context.stack.push_front( context.stack.front() );
            break;

        case EOP_PUSH_FLOAT:
            da.fval = instruction.fval;
            context.stack.push_front( da );
            break;

        case EOP_ADD:
            da = context.stack.front();
            context.stack.pop_front();

            db = context.stack.front();
            context.stack.pop_front();

            da.fval += db.fval;

            context.stack.push_front( da );
            break;

        case EOP_CMP_Z:
            da = context.stack.front();
            context.stack.pop_front();

            da.ival = ( da.ival == 0 ? 1 : 0 );

            context.stack.push_front( da );
            break;

        case EOP_JMP:
            context.ip += instruction.ival - 1;
            break;

        case EOP_JMP_T:
            da = context.stack.front();
            context.stack.pop_front();

            if ( da.ival == 1 )
            {
                context.ip += instruction.ival - 1;
            }

            break;

        default:
            std::cerr << "UNKNOWN OPCODE " << instruction.op << std::endl;
            break;
    }
}

void run( const Program& program )
{
    ThreadContext context( program );
    const InstructionList& instructions = context.program.instructions;

    // Start it
    context.status = ESTATUS_RUNNING;

    // Until it has finished executing, keep running it
    while ( context.status == ESTATUS_RUNNING )
    {
        // See if there are more instructions to execute
        if ( context.ip < instructions.size() )
        {
            // Load the next instruction and execute it
            execute( instructions[ context.ip ], context );

            // Move to the next instruction
            context.ip += 1;
        }
        else
        {
            // Program has exhausted all instructions. Reset the instruction
            // pointer and mark it as finished
            context.ip = 0;
            context.status = ESTATUS_FINISHED;
        }
    }

}

int main( int argc, char* argv[] )
{
    std::vector<Instruction> instructions =
    {
        // i = 3
        Instruction( EOP_PUSH_FLOAT, 5.0f ),        // 0

        // while ( i > 0 ) {
        Instruction( EOP_DUP ),                     // 1
        Instruction( EOP_CMP_Z ),                   // 2
        Instruction( EOP_JMP_T, 11 ),                // 3

        // print "i = "
        Instruction( EOP_NOP ),
        Instruction( EOP_PUSH_FLOAT, 0.0f ),
        Instruction( EOP_PRINT_STRING ),

        // print i
        Instruction( EOP_DUP ),
        Instruction( EOP_PRINT_FLOAT ),

        // print "\n"
        Instruction( EOP_PRINT_NEWLINE ),

        // i--
        Instruction( EOP_PUSH_FLOAT, -1.0f ),
        Instruction( EOP_ADD ),
        Instruction( EOP_NOP ),

        // }
        Instruction( EOP_JMP, -12 ),

        // print "done"
        Instruction( EOP_NOP ),
        Instruction( EOP_PUSH_FLOAT, 1.0f ),
        Instruction( EOP_PRINT_STRING ),
        Instruction( EOP_PRINT_NEWLINE )
    };

    Program p( instructions );

    p.stringtable.push_back( "i = " );
    p.stringtable.push_back( "done" );

    run( p );
};
