# ResourceLock

This repo contains 2 implementations of a lock.

## Semaphore

The lock on the *semaphore* branch is implemented using a semaphore of the Windows API.
This is a primitive which can be shared between processes.

The downside of this implementation is that when the process holding the lock crashes, the lock is not freed.

## File

The lock on the *file* branch is implemented using a file.
The access to the lock corresponds to the access of the file. Read access can be concurrent, write access needs to be exclusive.
This is implemented using a native Windows file.

When the process holding the lock (locking the file) crashes, the OS will cleanup the lock. At this point, other waiting processes can proceed with their access attempt.

## Cancellation request

The CancellationRequest is a flag (implemented using a Windows Named Mutex which is shared between processes).
An algorithm can check this flag at a safe point. When set by any other process, it can conclude cancellation is requested and prematurely abort the algorithm.