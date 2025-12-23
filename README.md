# Database Connection Pooling System (C + PostgreSQL)

A thread-safe database connection pooling system implemented in C using
PostgreSQL (libpq) and POSIX threads.  
The project demonstrates backend performance optimization, concurrency
control, and efficient resource management.

 ## Features
- Fixed-size PostgreSQL connection pool
- Thread-safe connection acquisition and release
- Uses mutex locks and condition variables
- Prevents expensive connection creation per request
- Handles concurrent access efficiently

## Tech Stack
- Language: C
- Database: PostgreSQL
- Libraries: libpq, pthread
- OS: Linux

##  Design Overview
- Connections are created once at startup
- Threads request a free connection from the pool
- If no connection is available, threads wait using condition variables
- Connections are reused instead of closed
- Controlled resource usage under high concurrency

##  Project Structure
src/
├── main.c # Multi-threaded test client
├── db_pool.c # Pool implementation
├── db_pool.h # Pool interface
Makefile
