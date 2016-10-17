# Random-access machine

## General

This is my implementation of a ramdom-access machine. It is based on a 8-bit architecture so the biggest usable address is 256 and the biggest possible value is 127 since this implementation can also handle negetive numbers.

In contrast to the model of a RAM, th accumulator is a own register and not located at data[0]

Comments are intruduced by `;`

## Instructions

All instructions follow the pattern
```
<operator> <operand>
```

Instruction | Description
------------|------------
ADD | akku += data[<operand>]
SUB | akku -= data[<operand>]
MUL | akku \*= data[<operand>]
DIV | akku /= data[<operand>]
LDA | akku = data[<operand>]
LDK | akku = <operand>
STA | data[<operand>] = akku
INP | read a number from stdin to data[<operand>]
OUT | print data[<operand>] to stdout
HLT | halt execution and use <operand> as exitcode
JMP | PC = <operand>
JEZ | if (akku == 0) PC = <operand>
JNE | if (akku != 0) PC = <operand>
JLZ | if (akku < 0) PC = <operand>
JLE | if (akku <= 0) PC = <operand>
JGZ | if (akku > 0) PC = <operand>
JGE | if (akku >= 0) PC = <operand>
