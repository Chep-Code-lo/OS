import sys
import threading
import math

primes = []
primes_lock = threading.Lock()


def is_prime(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False
    return True


def find_primes_in_range(start, end):
    local_primes = []
    
    for num in range(start, end + 1):
        if is_prime(num):
            local_primes.append(num)
    
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

    range_size = (N - 1) // T
    threads = []

    for i in range(T):
        start = 2 + i * range_size
        
        if i == T - 1:
            end = N
        else:
            end = start + range_size - 1
        
        thread = threading.Thread(target=find_primes_in_range, args=(start, end))
        threads.append(thread)
        thread.start()
        print(f"Thread {i+1}: Checking range [{start}, {end}]")

    for thread in threads:
        thread.join()

    primes.sort()

    print(f"\nFound {len(primes)} prime numbers:")
    print(primes)
    
    print("\nFormatted output (10 per row):")
    for i in range(0, len(primes), 10):
        print(primes[i:i+10])


if __name__ == "__main__":
    main()
