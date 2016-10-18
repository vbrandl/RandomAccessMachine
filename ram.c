/*
 * This is free software, released under the AGPL-3.0
 * https://github.com/vbrandl/RandomAccessMachine
 *
 * This is a implementation of a random-access machine with a slightly modified instruction set
 * The RAM runs on a 8-bit architecture so the program and data memory are 256 fields long and the biggest possible value is 127 since this implementation can handle negative numbers
 * In contrast to the model, the accumulator is located in a own register instead of data[0].
 * Comments in the code are introduced by ";"
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define BUFFER_SIZE 256
#define LINE_SIZE   256

/* enumeration of all operations, the RAM can execute */
enum operation {
    ADD = 0x01,
    SUB = 0x02,
    MUL = 0x03,
    DIV = 0x04,
    LDA = 0x05,
    LDK = 0x06,
    STA = 0x07,
    INP = 0x08,
    OUT = 0x09,
    HLT = 0x0a,
    JMP = 0x0b,
    JEZ = 0x0c,
    JNE = 0x0d,
    JLZ = 0x0e,
    JLE = 0x0f,
    JGZ = 0x10,
    JGE = 0x11
};

struct instruction {
    enum operation operator;
    char operand;
};

/* the buffer for the compiled program */
static struct instruction PROG[BUFFER_SIZE];

/* trim a string */
size_t trim(char*, size_t, const char*);
/* compiles a file containing RAM code */
void compile(FILE*);
/* compile a single line of code */
unsigned int compile_line(char*, unsigned int);
/* execute the compiled program */
int execute(void);

int
main(int argc, char **argv)
{
    FILE *fp;
    if (argc < 2) {
        fprintf(stderr, "usage: %s filename", argv[0]);
        exit(1);
    }

    if (NULL == (fp = fopen(argv[1], "r"))) {
        fprintf(stderr, "cannot open file %s", argv[1]);
        exit(1);
    }

    compile(fp);
    execute();

    return 0;
}

/* trims a string str of length len and writes the result to out.
 * Returns the length of the trimed string */
size_t
trim(char *out, size_t len, const char *str)
{
    if (0 == len) {
        return 0;
    }

    const char *end;
    size_t out_size;

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if ('\0' == *str) {
        *out = '\0';
        return 0;
    }

    end = str + strlen(str);
    while (end > str && (isspace((unsigned char)*end) || '\n' == (unsigned char)*end)) {
        end--;
    }

    out_size = (size_t)(end - str) < len-1 ? (size_t)(end-str) : len;

    memcpy(out, str, out_size);
    out[out_size] = '\0';

    return out_size;
}

/* compiles a file fp line by line, skipping comments and empty lines */
void
compile(FILE *fp)
{
    unsigned int cnt = 0;
    char line[LINE_SIZE];
    while (NULL != fgets(line, LINE_SIZE, fp) && cnt < BUFFER_SIZE) {
        if (compile_line(line, cnt)) {
            cnt++;
        }
    }
}

/* compiles a single line, returns 0 if the line is empty or a comment, 1 if the line was compiled or terminates in case of a invalid instruction.
 * The compiled instruction is written to cnt in PROG */
unsigned int
compile_line(char *line, unsigned int cnt)
{
    char *tmp, *clean_line, *endptr;
    struct instruction instr;

    if (';' == line[0] || NULL == (tmp = strtok(line, ";"))) {
        return 0;
    }

    clean_line = malloc(strlen(tmp) + 1);
    trim(clean_line, strlen(tmp), tmp);

    tmp = strtok(clean_line, " ");
    if (NULL == tmp) {
        return 0;
    } else if (strcasecmp(tmp, "add") == 0) {
        instr.operator = ADD;
    } else if (strcasecmp(tmp, "sub") == 0) {
        instr.operator = SUB;
    } else if (strcasecmp(tmp, "mul") == 0) {
        instr.operator = MUL;
    } else if (strcasecmp(tmp, "div") == 0) {
        instr.operator = DIV;
    } else if (strcasecmp(tmp, "lda") == 0) {
        instr.operator = LDA;
    } else if (strcasecmp(tmp, "ldk") == 0) {
        instr.operator = LDK;
    } else if (strcasecmp(tmp, "sta") == 0) {
        instr.operator = STA;
    } else if (strcasecmp(tmp, "inp") == 0) {
        instr.operator = INP;
    } else if (strcasecmp(tmp, "out") == 0) {
        instr.operator = OUT;
    } else if (strcasecmp(tmp, "hlt") == 0) {
        instr.operator = HLT;
    } else if (strcasecmp(tmp, "jmp") == 0) {
        instr.operator = JMP;
    } else if (strcasecmp(tmp, "jez") == 0) {
        instr.operator = JEZ;
    } else if (strcasecmp(tmp, "jne") == 0) {
        instr.operator = JNE;
    } else if (strcasecmp(tmp, "jlz") == 0) {
        instr.operator = JLZ;
    } else if (strcasecmp(tmp, "jle") == 0) {
        instr.operator = JLE;
    } else if (strcasecmp(tmp, "jgz") == 0) {
        instr.operator = JGZ;
    } else if (strcasecmp(tmp, "jge") == 0) {
        instr.operator = JGE;
    } else {
        fprintf(stderr, "invalid operator \"%s\" in line\n\"%s\"\n", tmp, line);
        exit(1);
    }

    tmp = strtok(NULL, " ");
    long int operand = strtol(tmp, &endptr, 0);
    if (0 == operand && strcmp(tmp, endptr) == 0) {
        fprintf(stderr, "invalid operand \"%s\" ind line\n\"%s\"\n", tmp, line);
        exit(1);
    }

    instr.operand = operand;

    PROG[cnt] = instr;

    return 1;
}

/* executes a compiled RAM program */
int
execute(void)
{
    char data[BUFFER_SIZE];
    char akku = 0;
    unsigned char pc = 0;
    unsigned int ptr = BUFFER_SIZE;
    while (--ptr) { data[ptr] = 0; }
    while (1) {
        switch (PROG[pc].operator) {
            case ADD: akku += data[(unsigned char)PROG[pc].operand]; break;
            case SUB: akku -= data[(unsigned char)PROG[pc].operand]; break;
            case MUL: akku *= data[(unsigned char)PROG[pc].operand]; break;
            case DIV: akku /= data[(unsigned char)PROG[pc].operand]; break;
            case LDA: akku = data[(unsigned char)PROG[pc].operand]; break;
            case LDK: akku = PROG[pc].operand; break;
            case STA: data[(unsigned char)PROG[pc].operand] = akku; break;
            case INP:
                scanf("%hhd", &data[(unsigned char)PROG[pc].operand]);
                break;
            case OUT: printf("%d\n", data[(unsigned char)PROG[pc].operand]); break;
            case HLT: exit(PROG[pc].operand); break;
            case JMP: pc = (unsigned char)PROG[pc].operand; continue;
            case JEZ:
                if (0 == akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            case JNE:
                if (0 != akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            case JLZ:
                if (0 > akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            case JLE:
                if (0 >= akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            case JGZ:
                if (0 < akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            case JGE:
                if (0 <= akku) {
                    pc = (unsigned char)PROG[pc].operand;
                    continue;
                }
                break;
            default:
                fprintf(stderr, "unknown error. illegal code has been compiled!\n");
                exit(1);
        }
        pc++;
    }
}
