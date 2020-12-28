# Automata Project: C++ Turing Machine simulator
## Directory structure
- structure 
    ```
    |- programs
        |- *.tm                 // description files
    |- turing-project
        |- main.cpp             // main entry
        |- TuringMachine.cpp    // definition of TM class
        |- TuringMachine.h      // declaration  of TM class
    |- README.md
    ```

## How to run
- run Makefile in project root directory and get executable file ```turing```:
    ```
    $ make
    ```
- run ```turing```:
    ```
    $ ./turing [-v|--verbose] [-h|--help] <tm> <input>
    ```
    like:
    ```
    $ ./turing -v case1.tm aabaabb
    ```

## Arguments
- ```-v/--verbose```:
    
    Whether to show step ID and debug detail.

- ```-h/--help```:

    Show help.

- ```<tm>```:

    The relative path of TM description file.

    **e.g.** for file ```./programs/case.tm```, ```<tm>:=case.tm```

- ```<input>```:

    The input string for TM.

    **e.g.** for input "abcabc", ```<input>:=abcabc```.

    Especially for empty string, put ```<input>:=""```.