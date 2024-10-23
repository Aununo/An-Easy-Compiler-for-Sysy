# An Easy Compiler for Sysy

A brief report with my ideas for the PKU compiler learning.

## 0. Environment

*Platform:* Ubuntu 24.04; 

*Language:* C++; 

*IDE:* VS Code;

*Reference:* https://pku-minic.github.io/online-doc/#/.

### Docker

First, you should install docker on your platform. This command will help you get the mirror of **pku** practice experiment.

```bash
docker pull maxxing/compiler-dev
```
And this helps 'move' your local project into the test environment.
```bash
docker run -it --rm -v /home/your_path:/root/compiler maxxing/compiler-dev bash
```
If wanna exit, just type `exit` or `Ctrl+D`.

### Make

[sysy-make-template](https://github.com/pku-minic/sysy-make-template)、[sysy-cmake-template](https://github.com/pku-minic/sysy-cmake-template)



```bash
make
build/compiler -koopa hello.c -o hello.koopa
```


