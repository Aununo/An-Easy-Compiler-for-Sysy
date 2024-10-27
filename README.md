# An Easy Compiler for Sysy

(Unfinished) A very first quick try for implementing a compiler. 😊

## 🐳Docker

*Language:* C++; *IDE:* VS Code; *Platform:* Ubuntu 24.04, WSL; 

*Reference:* https://pku-minic.github.io/online-doc/#/.

This command will help to get the mirror of **pku** practice experiment. Tools are already there.

```bash
docker pull maxxing/compiler-dev
```

And this helps to 'move' your local project into the test environment.

```bash
docker run -it --rm -v /home/your_path:/root/compiler maxxing/compiler-dev bash
```
If wanna exit, just type `exit` or `Ctrl+D`. 
```bash
# How to test?
make
build/compiler -koopa test/hello.c -o test/hello.koopa
autotest -koopa -s lv1 /root/compiler
```