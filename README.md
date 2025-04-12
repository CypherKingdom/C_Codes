# C Inter-Process Communication (IPC) Examples

This repository contains various C code examples demonstrating different Inter-Process Communication (IPC) mechanisms in Unix/Linux systems. The examples are organized into different directories based on the IPC mechanism they use.

## Repository Structure

- **Sockets**: Examples of socket-based communication
  - **Lab**: Contains practical socket programming examples
    - TCP client/server implementation
    - UDP client/server implementation
    - Local socket communication
  - Local socket client/server implementation
  - Internet client implementation

- **Pipe**: Examples of unnamed pipes for communication between related processes
  - **Lab**: Contains exercises demonstrating pipe usage
    - Exercise 1: Producer-consumer pattern with character conversion
    - Exercise 2: File transfer between processes
    - Exercise 3: Command pipeline simulation (ps | grep | wc)
    - Exercise 4: Ring of processes for string manipulation
  - Unidirectional pipe examples
  - Bidirectional pipe communication
  - Building pipeline using pipes

- **FIFO**: Examples of named pipes (FIFOs) for communication between unrelated processes
  - **Lab**: Client/server application using FIFOs
  - Various FIFO examples demonstrating different communication patterns

- **SharedMemory**: Examples of shared memory IPC
  - Basic shared memory allocation and usage
  - Segment creation and control

- **MappedMemory**: Examples of memory-mapped files
  - Memory-mapped file writing
  - Memory-mapped file reading

- **Practice**: Various example programs demonstrating IPC concepts
  - Simulation exercises
  - Text manipulation examples

## Compilation

To compile these examples, use GCC:

```bash
gcc -o program_name source_file.c
```

## Running Examples

Most examples can be run directly after compilation. Some require command-line arguments - check the source code comments for details on usage.

## Socket Examples

Socket examples demonstrate different types of socket communication:
- TCP (connected) sockets
- UDP (connectionless) sockets
- Local (Unix domain) sockets

## Pipe Examples

Pipe examples show how to use unnamed pipes for communication between parent and child processes. They demonstrate:
- Unidirectional communication
- Bidirectional communication
- Command pipelines similar to shell pipes

## FIFO Examples

FIFO examples demonstrate named pipes for communication between unrelated processes. These include:
- Client/server models
- Request-response patterns
- Data transfer mechanisms

## Shared Memory Examples

Shared memory examples show how to:
- Create shared memory segments
- Attach/detach from shared memory
- Control shared memory permissions and properties

## Memory-Mapped Files

These examples demonstrate how to use memory mapping for file I/O operations.

## Note

These examples are primarily intended for Unix/Linux systems and may require specific headers and libraries available on these platforms.
