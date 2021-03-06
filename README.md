# ABB_efficiency_study
Implementation and comparison of ABBs variants, Design and Analysis of Algorithms assignment.



### To run any C file do:
- `gcc filename.c -o excname -lm` 
- `./excname`

### To run the python file do: 
- `Python file.c`


## About the code
- `test.c` runs the 6 different experiment for 6 BSTs 
- `values_rng.py` creates 10 `.txt` files with 1 million random numbers between 0 to 2^32
- `Abb.c` BST Implementation
- `Avl.c` AVL  Implementation
- `Splay.c` Splay Tree Implementation
- `Btree16.c` BTree Implementation with blocks of 16
- `Btree256.c`  BTree Implementation with blocks of 256
- `Btree4096.c`  BTree Implementation with blocks of 4096

## HOW TO RUN

- You must create a directoy named **Values** for storing the .txt containing the random values before runing the `test.c` C file.
- Run the `values_rng.py` file to create 10 files with 1 million random numbers between 0 to 2^32
- Run the `test.c` file.
- If you got a warning after compiling the file, don't worry, it's normal just excecute with `./excname`. 😁
- You results will be in **Resultados** directory
- If you dont have that directory, please create one by yourself

## Authors
- **Javier Lavados Jillbert** - [JavierLavados](https://github.com/JavierLavados)
- **Lung Wang** - [doragonUni](https://github.com/doragonUni)
- **Nicolás García Ríos** - [Nicolas-Francisco](https://github.com/Nicolas-Francisco)
