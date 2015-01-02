/* Typedefs */
/* Memory storage */
typedef int32_t mem;
/* Stores counter variables such as instructionCounter */
typedef uint16_t counter;

typedef uint8_t code;
typedef int16_t address;

/* Defaults */
#define MEM_SIZE ( 1000 )

#define ACCUMULATOR_DEFAULT ( 0x0000 )
#define INSTRUCTION_COUNTER_DEFAULT ( 0x0000 )
#define INSTRUCTION_REGISTER_DEFAULT ( 0x0000 )
#define OPERATION_CODE_DEFAULT ( 0x0000 )
#define OPERAND_DEFAULT ( 0x0000 )
#define SENTINEL_VALUE ( 0xFFFFF )

#define MAX_INPUT ( 0xFFFFF )
#define MIN_INPUT ( 0x00000 )

#define SUCCESS ( 0 )

#define PROGRAM_END ( 0 )
#define GENERAL_ERROR ( 1 )
#define DIVIDE_BY_ZERO ( 2 )
#define INVALID_OP_CODE ( 3 )
#define ACCUMULATOR_OVERFLOW ( 4 )

#define FATAL_ERROR_STRING ( "*** Simpletrong execution abnormally terminated ***" )

/* Operation codes */

/* Input/output operations */
/* Inputs/outputs values from the console */
#define READ ( 0x10 )
#define WRITE ( 0x11 )
#define STRING ( 0x12 )
#define NEWLINE ( 0x13 )

/* Load/store operations */
/* Load/store values to specific memory locations */
#define LOAD ( 0x20 )
#define STORE ( 0x21 )

/* Arithmetic operations */
/* Operates on current accumulator value */
#define ADD ( 0x30 )
#define SUBTRACT ( 0x31 )
#define DIVIDE ( 0x32 )
#define MULTIPLY ( 0x33 )
#define REMAINDER ( 0x34 )
#define EXPONENT ( 0x35 )

/* Transfer-of-control operations */
/* Controls program execution */
#define BRANCH ( 0x40 )
#define BRANCHNEG ( 0x41 )
#define BRANCHZERO ( 0x42 )
#define HALT ( 0x43 )

/* State altering operations */
/* Controls simulation */
#define TERMINATE ( 0xFF )


/* Function Prototypes */
void dump(void);
