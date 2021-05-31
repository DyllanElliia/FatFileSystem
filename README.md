# Fat-based System

Simple simulator of Fat-based system based on C++

## Development Environment

A PC, Visual Studio Code, C++ 17, make

## How to use

Clone the repository:

```shell
git clone https://github.com/DyllanElliia/FatFileSystem.git
cd FatFileSystem
```

You should create a directory named obj:

```shell
mkdir obj
```

Then make it:

```shell
make
```

## Example

After make down, you can use

```bash
./main
```

to start the Filesystem:

```shell
➜  FatFileSystem git:(main) ✗ ./main
----------------System Data----------------
System Name: FAT-Based System
Producers Name: Dyllan & Taka
Vendor Name: UESTC
Fat Space (64B/64B): 25/32
Memory Space (64B/64B): 931/1024
---------------System Running--------------
Console init over
Fat File System Booting...
To get help about this system, please use "usage".
[/] #
```

After seeing "[/] #", it means you can test the system.

In this system, we've set several commands to control it. You can use the command "usage" to get help:

```shell
[/] # usage
COMMAND and SYNOPSIS:
        ls: list all files and directories;
                usage: ls
        mkdir: create a directory;
                usage: mkdir dirName
        rmdir: remove a directory;
                usage: rmdir dirName
        rm: remove a file;
                usage: rm fileName
        cd: change current path;
                usage: cd pathName
        cat: concatenate and print file;
                usage: cat fileName [another fileName]
        echo: write arguments to the standard output or a file;
                usage: echo string [fileName]
        find: walk a file hierarchy;
                usage: find path fileName
        tree: list contents of directories in a tree-like format;
                usage: tree [level(integer)]
        pwd: return working directory name;
                usage: pwd
        exit: exit Fat File System.
                usage: exit
```

Just like \*nix, you can use the command "mkdir" to create a directory, and use "ls" to list files and directories:

```shell
[/] # ls
name	type	size
a		dir	\
b		dir	\
a.t		file	14byte
dyllan	file	14byte
hw		file	12byte
text		file	233byte
text2		file	233byte
[/] # mkdir dir1
[/] # ls
name	type	size
a		dir	\
b		dir	\
dir1	dir	\
a.t			file	14byte
dyllan	file	14byte
hw			file	12byte
text	file	233byte
text2	file	233byte
```

You can use "echo" to print something to the screen, or use "echo [string] [filename]" to write the string to a file:

```shell
[/] # echo "Hello World!"
Hello World!
[/] # echo "Hello C++!" hc
[/] # ls
name    type    size
a       dir     \
b       dir     \
dir1    dir     \
a.t     file    14byte
dyllan  file    14byte
hc      file    10byte
hw      file    12byte
text    file    233byte
text2   file    233byte
```

If there is a file, the command "cat" can get its content and print it to the screen. Like "echo", you can use "cat [file1] [file2]" to write content from the first file to the second file:

```shell
[/] # cat hc
Hello C++!
[/] # cat hc hc_new
[/] # ls
name    type    size
a       dir     \
b       dir     \
dir1    dir     \
a.t     file    14byte
dyllan  file    14byte
hc      file    10byte
hc_new  file    10byte
hw      file    12byte
text    file    233byte
text2   file    233byte
[/] # cat hc_new
Hello C++!
```

The command "tree [level]" can list all files and directories based on the level:

```shell
[/] # echo FatSystem dir1/file
[/] # mkdir dir1/dir2
[/] # mkdir dir3
[/] # mkdir dir3/dir2
[/] # echo test dir3/file
[/] # tree 2
.
├── a
│   ├── aa
│   ├── b
│   └── c
├── b
│   └── a
├── dir1
│   ├── dir2
│   └── file
├── dir3
│   ├── dir2
│   └── file
├── a.t
├── dyllan
├── hc
├── hc_new
├── hw
├── text
└── text2
```

If you want to find a file or a directory, you can use "find [path] [name]" like:

```shell
[/] # find . file
/dir1/file
/dir3/file
[/] # find . dir2
/dir1/dir2
/dir3/dir2
[/] # find dir1 dir2
/dir1/dir2
```

You can use "cd [path]" to change you path. You can get your current path from the begin of the line or use the command "pwd":

```shell
[/] # cd dir1
[/dir1] # pwd
/dir1
[/dir1] # cd /a
[/a] # cd /
[/] # pwd
/
```

If you want to delete something, please use "rm [path]" for file and "rmdir [path]" for directory:

```shell
[/] # rmdir /dir1/dir2
[/] # rm /dir3/file
[/] # tree 2
.
├── a
│   ├── aa
│   ├── b
│   └── c
├── b
│   └── a
├── dir1
│   └── file
├── dir3
│   └── dir2
├── a.t
├── dyllan
├── hc
├── hc_new
├── hw
├── text
└── text2
```

You can use "exit" to exit the system.

```shell
[/] # exit
----------------System Exit----------------
```

