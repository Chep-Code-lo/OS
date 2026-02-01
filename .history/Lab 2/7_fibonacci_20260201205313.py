"""
Problem 2: Multithreaded Fibonacci Sequence Generator
Generate Fibonacci sequence using a separate thread.
Usage: python 7_fibonacci.py <count>
Example: python 7_fibonacci.py 10
"""

import sys
import threading

# Shared data structure to store Fibonacci sequence
fibonacci_sequence = []


def generate_fibonacci(n):
    """Thread function to generate Fibonacci sequence"""
    global fibonacci_sequence
    
    if n <= 0:
        return
    
    # Generate Fibonacci numbers
    if n >= 1:
        fibonacci_sequence.append(0)
    if n >= 2:
        fibonacci_sequence.append(1)
    
    for i in range(2, n):
        next_fib = fibonacci_sequence[i-1] + fibonacci_sequence[i-2]
        fibonacci_sequence.append(next_fib)


def main():
    if len(sys.argv) != 2:
        print("Usage: python 7_fibonacci.py <count>")
        print("Example: python 7_fibonacci.py 10")
        sys.exit(1)

    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Error: Argument must be a valid integer")
        sys.exit(1)

    if n <= 0:
        print("Error: Please provide a positive integer")
        sys.exit(1)

    print(f"Generating first {n} Fibonacci numbers...\n")

    # Create worker thread to generate Fibonacci sequence
    fib_thread = threading.Thread(target=generate_fibonacci, args=(n,))
    
    # Start the thread
    fib_thread.start()
    
    # Wait for the child thread to finish (parent thread waits)
    fib_thread.join()

    # Output the sequence after child thread completes
    print(f"Fibonacci sequence ({n} numbers):")
    print(fibonacci_sequence)
    
    # Pretty print
    print("\nFormatted output:")
    for i, num in enumerate(fibonacci_sequence):
        print(f"fib({i}) = {num}")


if __name__ == "__main__":
    main()
