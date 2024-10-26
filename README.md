# An Easy Compiler for Sysy

(Unfinished) A very first quick try for implementing a compiler. 😊

## 🕶Compiler vs Interpreter

![img](https://github.com/Aununo/An-Easy-Compiler-for-Sysy/blob/master/images/c.png)

**Compiled languages**: Convert source code into machine code through a compilation process, resulting in a standalone executable file that is efficient to run.

![img](https://github.com/Aununo/An-Easy-Compiler-for-Sysy/blob/master/images/python.png)

**Interpreted languages**: Execute source code line by line using an interpreter, generally without pre-compilation.

**Blurred boundary**: Modern languages increasingly use both interpretation and compilation techniques to achieve better performance and flexibility, making the distinction between the two less rigid.

## ⚙️General Structure

![img](https://github.com/Aununo/An-Easy-Compiler-for-Sysy/blob/master/images/structure.png)

We can divide the whole process into **THREE** main parts.

- ***FrontEnd:*** Mainly use **flex & bison**. Through **lexical and syntactic analysis**, the **source code**(SRC) is parsed into an **abstract syntax tree** (AST). Through **semantic** analysis, the AST is scanned to check whether there are semantic errors.
- ***MidEnd:*** **IR**(Intermediate Representation) is a lower-level code format that simplifies the structure of the original SRC, making it easier(M*N to M+N) for the backend to process. It performs **optimizations**, such as DCE, Constant Propagation etc.
- ***BackEnd:*** Takes the optimized IR and translates it into **assembly code**(ASM) for the target architecture (machine-specific).

The assembler takes the assembly code and converts it into machine code, creating an **executable** binary file, that machines can directly run.

## 🐳Docker

*Language:* C++; *IDE:* VS Code; *Platform:* Ubuntu 24.04, WSL; 

*Reference:* https://pku-minic.github.io/online-doc/#/.

This command will help to get the mirror of **pku** practice experiment.

```bash
docker pull maxxing/compiler-dev
```
Tools like `flex`, `bison`, `make`, `qemu-user-static`, `LLVM-toolschain`, `koopapac`, and `autotest`... are already there.

And this helps to 'move' your local project into the test environment.

```bash
docker run -it --rm -v /home/your_path:/root/compiler maxxing/compiler-dev bash
```
If wanna exit, just type `exit` or `Ctrl+D`. Test the Compiler:

```bash
make
build/compiler -koopa hello.c -o hello.koopa
build/compiler -riscv hello.c -o hello.S
autotest -koopa -s lv<1-9> /root/compiler
autotest -riscv -s lv<1-9> /root/compiler
```

I use **git** to push my local project to my github.
```bash
git init
git remote set-url origin git@github.com:Aununo/An-Easy-Compiler-for-Sysy.git
git add . & git commit -m "anything"
git push origin "branch"
```
