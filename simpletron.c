#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simpletron.h"

#define HEADER_FORMAT ( "        0x%03X   0x%03X   0x%03X   0x%03X   0x%03X   0x%03X   0x%03X   0x%03X   0x%03X   0x%03X" )

/* Globals */
static mem memory[MEM_SIZE];
static mem accumulator;
static mem instructionRegister;

static code operationCode;
static address operand;

static counter instructionCounter;

/* Cleanup before exit */
int cleanup(int status);

int main(void) {
	/*
	 * Initializes runtime variables to specified default values
	 * and initializes ncurses window
	 */
	int init(void);
	/* Loads program into memory */
	int load(void);
	/* Execute program */
	int execute(void);

	init();

	puts("*** Welcome to Simpletron! ***");
	puts("*** Please enter your program one instruction ***");
	puts("*** (or data word) at a time. I will type the ***");
	puts("*** location number and a question mark (?).  ***");
	puts("*** You then type the word for that location. ***");
	printf("*** Type the sentinel %05X to stop entering ***", SENTINEL_VALUE);
	puts("*** your program. ***");

	load();
	execute();

	return cleanup(PROGRAM_END);

}

/*
 * Initializes `accumulator`, `instructionCounter`, `instructionRegister`,
 * `operationCode`, and `operand` to defaults specified in simpletron.h
 *
 * Returns SUCCESS for success, otherwise returns an error code
 */
int init(void) {
	counter i;
	for (i=0; i < MEM_SIZE; i++) {
		memory[i] = 0;
	}

	accumulator = ACCUMULATOR_DEFAULT;
	instructionCounter = INSTRUCTION_COUNTER_DEFAULT;
	instructionRegister = INSTRUCTION_REGISTER_DEFAULT;
	operationCode = OPERATION_CODE_DEFAULT;
	operand = OPERAND_DEFAULT;

	return SUCCESS;
}

int execute(void) {
	int input = 0;
	while (instructionCounter < MEM_SIZE) {
		instructionRegister = memory[instructionCounter];

		operationCode = instructionRegister / 0x1000;
		operand = instructionRegister % 0x1000;

		/* Used for exponential expression */
		switch (operationCode) {
			/* Input/output */
			case READ:
				printf("0x%03X ? ", operand);
				scanf("%X", &input);
				memory[operand] = input;
				++instructionCounter;
				break;
			case WRITE:
				printf("0x%03X : %05X\n", operand, memory[operand]);
				++instructionCounter;
				break;
			case NEWLINE:
				putchar('\n');
				++instructionCounter;
				break;

			/* Load/store */
			case LOAD:
				accumulator = memory[operand];
				++instructionCounter;
				break;
			case STORE:
				memory[operand] = accumulator;
				++instructionCounter;
				break;

			/* Arithmetic operations */
			case ADD:
				if (accumulator + memory[operand] > MAX_INPUT ||
						accumulator + memory[operand] < MIN_INPUT) {
					cleanup(ACCUMULATOR_OVERFLOW);
				}
				accumulator += memory[operand];
				++instructionCounter;
				break;
			case SUBTRACT:
				if (accumulator - memory[operand] > MAX_INPUT ||
						accumulator - memory[operand] < MIN_INPUT) {
					cleanup(ACCUMULATOR_OVERFLOW);
				}
				accumulator -= memory[operand];
				++instructionCounter;
				break;
			case DIVIDE:
				if (accumulator / memory[operand] > MAX_INPUT ||
						accumulator / memory[operand] < MIN_INPUT) {
					cleanup(ACCUMULATOR_OVERFLOW);
				}
				if (memory[operand] == 0) {
					cleanup(DIVIDE_BY_ZERO);
				}
				accumulator /= memory[operand];
				++instructionCounter;
				break;
			case MULTIPLY:
				if (accumulator * memory[operand] > MAX_INPUT ||
						accumulator * memory[operand] < MIN_INPUT) {
					cleanup(ACCUMULATOR_OVERFLOW);
				}
				accumulator *= memory[operand];
				++instructionCounter;
				break;

			/* Transfer-of-control operations */
			case BRANCH:
				instructionCounter = operand;
				break;
			case BRANCHNEG:
				if (accumulator < 0) {
					instructionCounter = operand;
				}
				break;
			case BRANCHZERO:
				if (accumulator == 0) {
					instructionCounter = operand;
				}
				break;
			case HALT:
				puts("\n*** Simpletron execution terminated ***");
				dump();
				++instructionCounter;
				break;

			/* State altering operations */
			case TERMINATE:
				cleanup(PROGRAM_END);
				break;

			default:
				cleanup(INVALID_OP_CODE);
				break;
		}

	}
	return SUCCESS;
}

/*
 * Loads program into memory as the user enters it
 */
int load(void) {
	counter inputCounter = 0;
	int input;
	while (inputCounter < MEM_SIZE) {
		do {
			input = 0;
			printf("0x%03X ? ", inputCounter);
			scanf("%X", &input);
		} while (input > MAX_INPUT | input < MIN_INPUT);

		memory[inputCounter] = (mem)input;

		if (input == SENTINEL_VALUE)
			break;

		inputCounter++;
	}
	putchar('\n');
	return SUCCESS;
}


/*
 * Dumps contents of registers and memory to console
 */
void dump(void) {
	puts("REGISTER:");
	printf("accumulator\t\t0x%05X\n", accumulator);
	printf("instructionCounter\t0x%02X\n", instructionCounter);
	printf("instructionRegister\t0x%05X\n", instructionRegister);
	printf("operationCode\t\t0x%02X\n", operationCode);
	printf("operand\t\t\t0x%03X\n", operand);

	puts("\nMEMORY:");
	counter address;
	counter line = 0;

	printf(HEADER_FORMAT, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	for (address = 0; address < MEM_SIZE; address++) {
		if (address % 10 == 0) {
			printf("\n0x%03X", line);
			line += 0x10;
		}
		printf(" 0x%05X", memory[address]);
	}
	puts("\n");
}

/* Cleans up and returns an exit code */
int cleanup(int status) {
	if (status != PROGRAM_END) {
		dump();
	}

	switch (status) {
		case PROGRAM_END:
			/* Only terminates successfully form main */
			exit(EXIT_SUCCESS);
			break;
		case GENERAL_ERROR:
			puts(FATAL_ERROR_STRING);
			exit(GENERAL_ERROR);
			break;
		case DIVIDE_BY_ZERO:
			puts("*** Attempt to divide by zero ***");
			puts(FATAL_ERROR_STRING);
			exit(DIVIDE_BY_ZERO);
			break;
		case INVALID_OP_CODE:
			puts("*** Invalid operation code ***");
			puts(FATAL_ERROR_STRING);
			exit(INVALID_OP_CODE);
			break;
		case ACCUMULATOR_OVERFLOW:
			puts("*** Accumulator overflow ***");
			puts(FATAL_ERROR_STRING);
			exit(ACCUMULATOR_OVERFLOW);
			break;
		default:
			return EXIT_FAILURE;
			break;
	}
}
