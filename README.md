# FatFileSystem

Simple simulator of Fat Filesystem based on C++

## Development Environment

A PC, Visual Studio Code, C++ 17, make

## How to use

Clone the repository:

```bash
git clone https://github.com/DyllanElliia/FatFileSystem.git
cd FatFileSystem
```

You should create a directory named obj:

```bash
mkdir obj
```

Then make it:

```bash
make
```

## Example

After make down, you can use

```bash
./main
```

to start the Filesystem:

![image-20210528161040973](./images/image-20210528161040973.png)

After seeing "[/] #", it means you can test the system.

In this system, we've set several commands to control it. You can use the command "usage" to get help:

![image-20210528161547286](./images/image-20210528161547286.png)

Just like \*nix, you can use the command "mkdir" to create a directory, and use "ls" to list files and directories:

![image-20210528162118385](./images/image-20210528162118385.png)

You can use "echo" to print something to the screen, or use "echo [string] [filename]" to write the string to a file:

![image-20210528162339330](./images/image-20210528162339330.png)

If there is a file, the command "cat" can get its content and print it to the screen. Like "echo", you can use "cat [file1] [file2]" to write content from the first file to the second file:

![image-20210528162728767](./images/image-20210528162728767.png)

The command "tree [level]" can list all files and directories based on the level:

![image-20210528163012088](./images/image-20210528163012088.png)

If you want to find a file or a directory, you can use "find [path] [name]" like:

![image-20210528163131820](./images/image-20210528163131820.png)

You can use "cd [path]" to change you path. You can get your current path from the begin of the line or use the command "pwd":

![image-20210528163454865](./images/image-20210528163454865.png)

If you want to delete something, please use "rm [path]" for file and "rmdir [path]" for directory:

![image-20210528163728368](./images/image-20210528163728368.png)

You can use "exit" to exit the system.

![image-20210528163815073](./images/image-20210528163815073.png)
