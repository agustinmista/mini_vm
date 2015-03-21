#define REGS 8
#define MEM_SIZE (1024)

#define ZERO_BIT_FLAGS 1
#define EQUAL_BIT_FLAGS 2
#define LOWER_BIT_FLAGS 3
#define ISSET_BIT(X) (machine.reg[FLAGS] & (1<<(X)))
#define ISSET_ZERO ISSET_BIT(ZERO_BIT_FLAGS)
#define ISSET_EQUAL ISSET_BIT(EQUAL_BIT_FLAGS)
#define ISSET_LOWER ISSET_BIT(LOWER_BIT_FLAGS)

#define SET_BIT(X)  (machine.reg[FLAGS] |= 1<<(X))
#define UNSET_BIT(X) (machine.reg[FLAGS] &= ~(1<<(X)))

typedef enum { ZERO,
            PC,
            SP,
            R0,
            R1,
            R2,
            R3,
            FLAGS
} Registers ;

typedef enum { IMM, REG, MEM, LABELOP } OperandType ;

struct Operand {
  const char *lab;
  OperandType type;
  int val;
} ;

typedef enum {
  NOP,
  MOV,
  LW,
  SW,
  PUSH,
  POP,
  PRINT,
  READ,
  ADD,
  SUB,
  MUL,
  DIV,
  CMP,
  JMP,
  JMPE,
  JMPL,
  HLT,
  LABEL,
  AND,
  OR,
  XOR,
  LSH,
  RSH
} Opcode;

struct Instruction {
  Opcode op;
  struct Operand src;
  struct Operand dst;
} ;

struct Machine {
  int reg[REGS];
  char memory[MEM_SIZE];
} ;

extern int count;
extern struct Instruction code[512];
