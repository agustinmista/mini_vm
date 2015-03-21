#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"

#define DEBUG_MODE 0

int count;
struct Instruction code[512];

const char *regname[REGS] = { "\%zero", "\%pc", "\%sp", "\%r0","\%r1","\%r2","\%r3", "\%flags"};

struct Machine machine;

int reg(const char* r) {
  r++; // Skip %
  if (!strcmp(r,"zero")) return 0;
  if (!strcmp(r,"pc")) return 1;
  if (!strcmp(r,"sp")) return 2;
  if (!strcmp(r,"r0")) return 3;
  if (!strcmp(r,"r1")) return 4;
  if (!strcmp(r,"r2")) return 5;
  if (!strcmp(r,"r3")) return 6;
  if (!strcmp(r,"flags")) return 7;
  printf("Unkown Register %s\n",r);
  abort();
}

void printOperand(struct Operand s) {
  switch (s.type) {
    case IMM:
      printf("$%d", s.val);
      break;
    case REG:
      printf("%s", regname[s.val]);
      break;
    case MEM:
      printf("%d", s.val);
      break;
  }
}

void printInstr(struct Instruction i) {
  switch (i.op) {
    case NOP:
      printf("NOP\n");
      break;

    case HLT:
      printf("HLT\n");
      break;

    case MOV:
      printf("MOV ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case ADD:
      printf("ADD ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case MUL:
      printf("MUL ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case SUB:
      printf("SUB ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case DIV:
      printf("DIV ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case JMPL:
      printf("JMPL ");
      printOperand(i.src);
      if (i.src.lab)
        printf("%s",i.src.lab);
      printf("\n");
      break;

    case JMPE:
      printf("JMPE ");
      printOperand(i.src);
      if (i.src.lab)
        printf("%s",i.src.lab);
      printf("\n");
      break;

    case JMP:
      printf("JMP ");
      printOperand(i.src);
      if (i.src.lab)
        printf("%s",i.src.lab);
      printf("\n");
      break;

    case LABEL:
      printf("LABEL %s",i.src.lab);
      printf("\n");
      break;

    case CMP:
      printf("CMP ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case PRINT:
      printf("PRINT ");
      printOperand(i.src);
      printf("\n");
      break;

    case READ:
      printf("READ ");
      printOperand(i.src);
      printf("\n");
      break;

    case AND:
      printf("AND ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case OR:
      printf("OR ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case XOR:
      printf("XOR ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case LSH:
      printf("LSH ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    case RSH:
      printf("RSH ");
      printOperand(i.src);
      printf(",");
      printOperand(i.dst);
      printf("\n");
      break;

    default:
      printf("Instrucction not printed");
      printf("\n");
  }
}

void dumpMachine() {
  int i;
  printf("******************************************************\n");
  for (i=0;i<REGS;i++)
    if (strlen(regname[i])==0) continue;
    else printf("%s\t\t= \t\t%d \t\t%x\n",regname[i],machine.reg[i],machine.reg[i]);
  printf("******************************************************\n");
}

void runIns(struct Instruction i) {
  switch (i.op) {

    case NOP:
    /*
    * no realiza ninguna operacion.
    */
    {
      break;
    }

    case MOV:
    /*
    * mueve un valor de/hacia registro/memoria/constante.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }
        case REG:
        {
          if(i.src.val == PC) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }
        case MEM:
        {
          if(i.src.val < 0 || i.src.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for source.\n", machine.reg[PC]);
            abort();
          } else
            src_value = machine.memory[i.src.val * 4];
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            machine.reg[i.dst.val] = src_value;

          break;
        }
        case MEM:
        {
          if(i.dst.val < 0 || i.dst.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for destination.\n", machine.reg[PC]);
            abort();
          } else
            machine.memory[i.dst.val * 4] = src_value;
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case LW:
    /*
    * carga en dst el valor de la memoria apuntada por src.
    */
    {
      int src_value;
      switch(i.src.type){
        case MEM:
        {
          if(i.src.val < 0 || i.src.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for source.\n", machine.reg[PC]);
            abort();
          } else
            src_value = machine.memory[i.src.val * 4];
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] = src_value;

          break;
        }
        case MEM:
        {
          if(i.dst.val < 0 || i.dst.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for destination.\n", machine.reg[PC]);
            abort();
          } else
            machine.memory[i.dst.val * 4] = src_value;
          break;
        }
        default:
        {
          printf("%d Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case SW:
    /*
    * guarda el valor de src en la direccion de memoria dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }
        case REG:
        {
          if(i.src.val == PC) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }
        case MEM:
        {
          if(i.src.val < 0 || i.src.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for source.\n", machine.reg[PC]);
            abort();
          } else
            src_value = machine.memory[i.src.val * 4];
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case MEM:
        {
          if(i.dst.val < 0 || i.dst.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for destination.\n", machine.reg[PC]);
            abort();
          } else
            machine.memory[i.dst.val * 4] = src_value;
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case PUSH:
    /*
    * escribe el argumento src en el tope de la pila,
    * decrementando el SP por 4.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }
        case REG:
        {
          if(i.dst.val == PC) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }
        case MEM:
        {
          if(i.src.val < 0 || i.src.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for source.\n", machine.reg[PC]);
            abort();
          } else
            src_value = machine.memory[i.src.val * 4];
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      if(machine.reg[SP] <= 0) {
        printf("%d: No free space in the stack.\n", machine.reg[PC]);
        abort();
      } else {
        machine.reg[SP] -= 4;
        machine.memory[machine.reg[SP]] = src_value;
      }

      break;
    }

    case POP:
    /*
    * escribe en el argumento dst el tope de la pila,
    * incrementando el SP por 4.
    */
    {
      int stack_value;
      if(machine.reg[SP] >= MEM_SIZE) {
        printf("%d: The stack is empty.\n", machine.reg[PC]);
        abort();
      } else {
        stack_value = machine.memory[machine.reg[SP]];
        machine.reg[SP] += 4;
      }

      switch(i.src.type) {
        case REG:
        {
          if(i.src.val == ZERO || i.src.val == FLAGS || i.src.val == SP || i.src.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            machine.reg[i.src.val] = stack_value;

          break;
        }
        case MEM:
        {
          if(i.src.val < 0 || i.src.val > (MEM_SIZE - 1) / 4) {
            printf("%d: Not valid memory index for destination.\n", machine.reg[PC]);
            abort();
          } else
            machine.memory[i.src.val * 4] = stack_value;
          break;
        }
        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case PRINT:
    /*
    * imprime por pantalla el entero descripto en el argumento src.
    */
    {
      switch(i.src.type){
        case IMM:
        printf("%d\n", i.src.val);
        break;

        case REG:
        printf("%d\n", machine.reg[i.src.val]);
        break;

        case MEM:
        printf("%d\n", machine.memory[i.src.val]);
        break;

        default:
        printf("%d: Operand type not valid.\n", machine.reg[PC]);
        abort();
      }

      break;
    }

    case READ:
    /*
    * lee un entero y lo guarda en el argumento destino.
    */
    {
      switch(i.src.type){

        case REG:
        {
          if(i.src.val == ZERO || i.src.val == FLAGS || i.src.val == SP || i.src.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else{
            int value;
            scanf("%d", &value);
            machine.reg[i.src.val] = value;
          }
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case ADD:
    /*
    * suma los argumentos src y dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){

        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){

        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] += src_value;

          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case SUB:
    /*
    * resta los argumentos src y dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] -= src_value;

          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case MUL:
    /*
    * multiplica los argumentos src y dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] *= src_value;

          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case DIV:
    /*
    * divide los argumentos src y dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] /= src_value;

          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case CMP:
    /*
    * compara src y dst y setea los bits correspondiente en el registro FLAGS.
    */
    {
      int src_value, dst_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }
        case REG:
        {
          if(i.src.val == PC || i.dst.val == SP || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];

          break;
        }
        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case IMM:
        {
          dst_value = i.dst.val;
        }
        case REG:
        {
          if(i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            dst_value = machine.reg[i.dst.val];

          break;
        }
        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      if(dst_value == src_value){
        machine.reg[FLAGS] |= 1<<EQUAL_BIT_FLAGS;
        machine.reg[FLAGS] &= ~(1<<LOWER_BIT_FLAGS);
      } else if(dst_value > src_value) {
        machine.reg[FLAGS] |= 1<<LOWER_BIT_FLAGS;
        machine.reg[FLAGS] &= ~(1<<EQUAL_BIT_FLAGS);
      } else {
        machine.reg[FLAGS] &= ~(1<<EQUAL_BIT_FLAGS);
        machine.reg[FLAGS] &= ~(1<<LOWER_BIT_FLAGS);
      }

      break;
    }

    case JMP:
    /*
    * saltan a la instruccion numero dst de la lista de instrucciones.
    */
    {
      machine.reg[PC] = i.src.val-1;
      break;
    }

    case JMPE:
    /*
    * saltan a la instruccion numero dst si la bandera de igualdad esta seteada.
    */
    {
      if(ISSET_EQUAL)
      machine.reg[PC] = i.src.val-1;
      break;
    }

    case JMPL:
    /*
    * saltan a la instruccion numero dst si la bandera de menor esta seteada.
    */
    {
      if(ISSET_LOWER)
      machine.reg[PC] = i.src.val-1;
      break;
    }

    case AND:
    /*
    * realiza un bitwise and entre los dos operandos y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] &= src_value;
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case OR:
    /*
    * realiza un bitwise or entre los dos operandos y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] |= src_value;
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case XOR:
    /*
    * realiza un bitwise xor entre los dos operandos y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] ^= src_value;
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case LSH:
    /*
    * realiza un corrimiento de src bits a izquierda de dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] <<= src_value;
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case RSH:
    /*
    * realiza un corrimiento de src bits a derecha de dst y lo guarda en dst.
    */
    {
      int src_value;
      switch(i.src.type){
        case IMM:
        {
          src_value = i.src.val;
          break;
        }

        case REG:
        {
          if(i.src.val == PC || i.src.val == FLAGS) {
            printf("%d: Not valid register for source: %s\n", machine.reg[PC], regname[i.src.val]);
            abort();
          } else
            src_value = machine.reg[i.src.val];
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for source.\n", machine.reg[PC]);
          abort();
        }
      }

      switch(i.dst.type){
        case REG:
        {
          if(i.dst.val == ZERO || i.dst.val == FLAGS || i.dst.val == SP || i.dst.val == PC) {
            printf("%d: Not valid register for destination: %s\n", machine.reg[PC], regname[i.dst.val]);
            abort();
          } else
            machine.reg[i.dst.val] >>= src_value;
          break;
        }

        default:
        {
          printf("%d: Not valid operand type for destination.\n", machine.reg[PC]);
          abort();
        }
      }

      break;
    }

    case HLT:
    /*
    * termina el programa.
    */
    {
      exit(1);
      break;
    }

    default:
    printf("Instruction not implemented\n");
    abort();
  }
}

void run(struct Instruction *code) {
  machine.reg[PC] = 0; // Start at first instruction
  machine.reg[SP] = MEM_SIZE;
  while (code[machine.reg[PC]].op!=HLT) {
    if(DEBUG_MODE){
      printf("\n******************************************************\n");
      printf("Instruction:\t");
      printInstr(code[machine.reg[PC]]);
      printf("PC:\t\t%d\n", machine.reg[PC]);
      dumpMachine();
    }
    runIns(code[machine.reg[PC]]);
    // If not a jump, continue with next instruction
    machine.reg[PC]++;
  }
}

void processLabels() {
  int i,j,k;
  for (i=0;i<count;i++) {
    if (code[i].op==LABEL) {
      for (j=0;j<count;j++) {
        if (code[j].op == JMP || code[j].op== JMPE || code[j].op==JMPL) {
          if (code[j].src.lab && strcmp(code[j].src.lab,code[i].src.lab)==0) {
            code[j].src.type=IMM;
            code[j].src.val=i;
            code[j].src.lab=NULL;
          }
        }
      }
      for (j=i;j<count-1;j++) {
        code[j]=code[j+1];
      }
      count--;
    }
  }
  for (j=0;j<count;j++) {
    if (code[j].op == JMP || code[j].op== JMPE || code[j].op==JMPL) {
      if (code[j].src.lab) {
        printf("Jump to unkown label %s\n",code[j].src.lab);
        exit(-1);
      }
    }
  }
}

int main() {

  int i;
  yyparse();
  processLabels();

  printf("Running the following code:\n");
  printf("***************\n");
  for (i=0;i<count; i++)  {
    printf("%d: ",i);
    printInstr(code[i]);
  }
  printf("***************\n\n");

  run(code);

  return 0;
}
