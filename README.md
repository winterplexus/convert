Convert Applications
====================
Convert applications are command-line utility programs to convert file formats from one operating system to another.

Applications include:

* dtol - convert DOS formatted text files into Linux formatted text files

* ltod - converts Linux formatted text files into DOS formatted text files

DOS refers to Microsoft's MS-DOS (Disk Operating System). 

Formatted text file refers to end-of-line character sequence(s) in text files:

| Operating System | Character(s) | ASCII Code(s) |
| --- | --- | --- |
| DOS | carriage-return line-feed | 0x0d 0x0a |
| Linux | line-feed | 0x0a |

Each application is written in the C language for both Windows and Linux operating systems.

### dtol
```
usage: dtol (options)

options: -i <input file name>
         -o <output file name>
         -w enable write over input file as output mode
         -v display version
         -? print this usage
```

### ltod
```
usage: ltod (options)

options: -i <input file name>
         -o <output file name>
         -w enable write over input file as output mode
         -v display version
         -? print this usage
```