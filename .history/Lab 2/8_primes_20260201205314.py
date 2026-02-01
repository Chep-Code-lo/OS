"""
Problem 3: Multithreaded Prime Numbers Finder
Find all prime numbers up to N using T threads.
Usage: python 8_primes.py <N> [T]
Example: python 8_primes.py 100 4
"""

import sys
import threading
import math

# Shared list to store prime numbers (thread-safe with lock)
primes = []
primes_lock = threading.Lock()


def is_prime(n):
    """Check if a number is prime"""
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    # Check odd divisors up to sqrt(n)
    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False
    return True


def find_primes_in_range(start, end):
    """Thread function to find primes in a given range [start, end]"""
    local_primes = []
    
    for num in range(start, end + 1):
        if is_prime(num):
            local_primes.append(num)
    
    # Add local primes to global list (thread-safe)
    with primes_lock:
        primes.extend(local_primes)


def main():
    if len(sys.argv) < 2:
        print("Usage: python 8_primes.py <N> [T]")
        print("  N: Find all primes <= N")
        print("  T: Number of threads (default: 4)")
        print("Example: python 8_primes.py 100 4")
        sys.exit(1)

    try:
        N = int(sys.argv[1])
        T = int(sys.argv[2]) if len(sys.argv) > 2 else 4
    except ValueError:
        print("Error: Arguments must be valid integers")
        sys.exit(1)

    if N < 2:
        print("No prime numbers less than 2")
        return

    if T < 1:
        print("Error: Number of threads must be at least 1")
        sys.exit(1)

    print(f"Finding all prime numbers <= {N} using {T} threads...\n")

    # Divide the range [2, N] among T threads
    range_size = (N - 1) // T  # Numbers from 2 to N
    threads = []

    for i in range(T):
        start = 2 + i * range_size
        
        # Last thread takes the remaining range
        if i == T - 1:
            end = N
        else:
            end = start + range_size - 1
        
        # Create and start thread
        thread = threading.Thread(target=find_primes_in_range, args=(start, end))
        threads.append(thread)
        thread.start()
        print(f"Thread {i+1}: Checking range [{start}, {end}]")

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

    # Sort the primes (since threads may finish in any order)
    primes.sort()

    # Output results
    print(f"\nFound {len(primes)} prime numbers:")
    print(primes)
    
    # Display in formatted rows
    print("\nFormatted output (10 per row):")
    for i in range(0, len(primes), 10):
        print(primes[i:i+10])


if __name__ == "__main__":
    main()
