#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define LINE_SIZE 255

/*
instructions taken:
PSH // push to stack 
POP // pop from stack 
HLT // halt program 
SET // SET reg to val 
ADD // add first two num on stack and put result on top of stack
*/

typedef struct{	
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t f;
	uint32_t pc;
	uint8_t running;
	uint32_t sp;
	uint32_t *ram;
}ProcessingUnit;

int PUSH(uint32_t val, ProcessingUnit *cpu){
	printf("pushing %d\n", val);
	cpu->sp++;
	cpu->ram[cpu->sp] = val;
	return 0;
}

uint32_t REM(ProcessingUnit *cpu){
	uint32_t val = cpu->ram[cpu->sp];
	printf("%d popped\n", val);
	if (cpu->sp != 0)
		cpu->sp --;
	return val;
}

int ADD(ProcessingUnit *cpu){
	uint32_t a, b, result;
	a = REM(cpu);
	b = REM(cpu);
	result = a+b;
	PUSH(result, cpu);
	return 0;
}


int SET(ProcessingUnit *cpu, char *reg, uint32_t val){
	if (strcmp("A", reg) == 0){
		cpu->a = val;
	} else if (strcmp("B", reg) == 0){
		cpu->b = val;
	} else if (strcmp("C", reg) == 0){
		cpu->c = val;
	} else if (strcmp("D", reg) == 0){
		cpu->d = val;
	} else if (strcmp("E", reg) == 0){
		cpu->e = val;
	} else if (strcmp("F", reg) == 0){
		cpu->d = val;
	} else {
		printf("invalid register\n");
	}
	return 0;
}

int main(){	
	uint32_t val, popped;
	char *reg, *check;
	
	ProcessingUnit *cpu = malloc(sizeof(ProcessingUnit));
	if (cpu == NULL)
		// something went very wrong
		exit(1);
	
	cpu->ram = malloc(0x10000); // 16 KB 
	if (cpu->ram == NULL)
		exit(1);
	
	FILE *file = fopen("src.h", "rb");
	if (file == NULL)
		// something went very wrong 
		exit(1);
		
	cpu->running = 1;
	cpu->sp = -1;
	
	char *buffer = malloc(LINE_SIZE);
	if (buffer == NULL)
		exit(1);
	
	while (fgets(buffer, LINE_SIZE, file) && cpu->running){
		char delim[] = " ";
		buffer[strcspn(buffer, "\r\n")] = 0;
		printf("%s\n", buffer);
		
		char *op = strtok(buffer, delim);
		
		if (strcmp(op, "HLT") == 0){
			printf("halt\n");
			cpu->running = 0;
		} else if (strcmp(op, "PSH") == 0){
			if ((check = strtok(NULL, delim)) == NULL){
				printf("bad instruction when using PSH\n");
				exit(1);
			} 
			val = atoi(check);
			PUSH(val, cpu);
			printf("sp: %d\n", cpu->sp);
		} else if (strcmp(op, "POP") == 0){
			popped = REM(cpu);
			printf("sp: %d\n", cpu->sp);
		} else if (strcmp(op, "ADD") == 0){
			printf("adding\n");
			ADD(cpu);
		} else if (strcmp(op, "SET") == 0){
			reg = strtok(NULL, delim);
			val = atoi(strtok(NULL, delim));
			printf("setting reg %s to %d\n", reg, val);
			SET(cpu, reg, val);
		} else {
			printf("NONE\n");
		}
		printf("\n");
		
		cpu->pc ++;
	}
	fclose(file);
	
	free(cpu);
	free(cpu->ram);
	free(buffer);
	
	return 0;
}
	