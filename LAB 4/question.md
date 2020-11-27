Question Statement:

Using the concepts of shared-memory based programming, design and implement a client-server based
project. At first, write three service programs, viz., service1.c, service2.c and service3.c which can
provide three different types of services. Implement a shared request-queue Q which can be accessed by
both the server and any client process. A client process for requesting a service just places (ENQUE) a
request-item on the shared request-queue Q. Since multiple clients can place their requests, at any instant in time, there can be more than one item on the request-queue Q. Before placing a request, a client process creates a shared memory segment called result-buf er. After placing the request, the client blocks itself (‘pause’ system call). A request-item in the queue Q contains the details of the request, including, the type of service requested (1,2 or 3), requesting processes’s PID, a reference to the result-buf er created by the requesting process, and the required inputs for the requested service. The server process monitors the request-queue Q. Whenever there is an item in the queue, it forks a new child process and schedules the corresponding service program for execution by the new child process. It passes all the input data, including the reference to the result-buf er, and the PID of the requesting client process as command-line arguments to the service program (in an ‘exec’ system call). The server continues to create new children processes in this way as long as there are items in the request-queue Q (remember that the server is an infinite loop - so it never terminates; when the queue is empty, it waits for the next request to come).
When the service program finishes its computations, places the result in the result-buf er, and sends a
signal (‘kill’ system call) to the corresponding client process. The requesting client process gets
unblocked at this point, retrieves the result from its result-buffer, and prints the result on the screen. The client process frees the shared memory segment result-buf er which it created and then terminates. The server process has an interrupt-signal handler, which is invoked when ‘Ctrl+C’ is pressed from the terminal. The handler first frees the shared memory segment request-queue Q, and then terminates the server process.

    NOTE: since the shared memory segment request-queue Q can be updated by multiple processes (server
    and clients) you must ensure some locking mechanism (preferably using semaphore) to ensure mutually
    exclusive access to this queue.

Service 1: find LCM of 4 given numbers
Service 2: find GCD of 2 numbers
Service 3: find LCM and GCD of 2 numbers