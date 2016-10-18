# Random-access machine

## General

this is my implementation of a random-access machine. It is based on a 8-bit architecture so the biggest usable address is 255 and the biggest possible value is 127 since this implementation can also handle negetive numbers.

In contrast to the model of a RAM, the accumulator is a own register and not located at data[0]

Comments are introduced by `;`

## Instructions

All instructions follow the pattern
```
<operator> <operand>
```

Instruction | Opcode | Description
------------|--------|------------
ADD | 0x01 | `akku += data[<operand>]`
SUB | 0x02 | `akku -= data[<operand>]`
MUL | 0x03 | `akku *= data[<operand>]`
DIV | 0x04 | `akku /= data[<operand>]`
LDA | 0x05 | `akku = data[<operand>]`
LDK | 0x06 | `akku = <operand>`
STA | 0x07 | `data[<operand>] = akku`
INP | 0x08 | `read a number from stdin to data[<operand>]`
OUT | 0x09 | `print data[<operand>] to stdout`
HLT | 0x0A | `halt execution and use <operand> as exitcode`
JMP | 0x0B | `PC = <operand>`
JEZ | 0x0C | `if (akku == 0) PC = <operand>`
JNE | 0x0D | `if (akku != 0) PC = <operand>`
JLZ | 0x0E | `if (akku < 0) PC = <operand>`
JLE | 0x0F | `if (akku <= 0) PC = <operand>`
JGZ | 0x10 | `if (akku > 0) PC = <operand>`
JGE | 0x11 | `if (akku >= 0) PC = <operand>`
