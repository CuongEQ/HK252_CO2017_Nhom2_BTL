# Project Overview
This is a simulated Operating System written in standard C for the Operating Systems (CO2017) course at Ho Chi Minh City University of Technology. The system manages CPU Scheduling, Virtual Memory, Paging, and Synchronization for concurrent processes.

# Source Code Structure
- `include/`: Contains core header files (`pcb_t`, `krnl_t`, `mm_struct`, `memphy_struct`, `sc_regs`).
- `src/`: Contains C source files implementing the OS.
- `input/` & `output/`: Contains simulated processes, configuration files, and expected outputs.

# Build, Test & CI/CD Guidelines
- Compile all: `make all`
- Clean build files: `make clean`
- Run OS: `./os [config_file_name_in_input_dir]`
- CI pipeline on the `development` branch checks:
  1. Coding Standard: `clang-format --style=GNU` (mandatory apply only for syscalls).
  2. Static Analysis: `cppcheck`.
  3. Memory Safety: `valgrind --leak-check=full --error-exitcode=1`.

# Critical Architectural Constraints
- Dual-mode Operation: Strict separation between User Space and Kernel Space.
- User Space is strictly prohibited from directly manipulating `pcb_t` pointers or kernel memory space. All interactions must go through the `krnl_t` structure in Kernel Mode.
- Language: Strictly standard C. No C++ syntax or external libraries not defined in the `Makefile`.
