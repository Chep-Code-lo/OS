"""
Problem 1: Multithreaded Statistical Calculator
Calculate average, minimum, and maximum of numbers using separate threads.
Usage: python 6_statistics.py 90 81 78 95 79 72 85
"""

import sys
import threading

# Global variables to store results
average = 0
minimum = 0
maximum = 0


def calculate_average(numbers):
    """Thread function to calculate average"""
    global average
    average = sum(numbers) / len(numbers)


def calculate_minimum(numbers):
    """Thread function to find minimum value"""
    global minimum
    minimum = min(numbers)


def calculate_maximum(numbers):
    """Thread function to find maximum value"""
    global maximum
    maximum = max(numbers)


def main():
    if len(sys.argv) < 2:
        print("Usage: python 6_statistics.py <number1> <number2> ... <numberN>")
        print("Example: python 6_statistics.py 90 81 78 95 79 72 85")
        sys.exit(1)

    # Parse command line arguments to get numbers
    try:
        numbers = [int(arg) for arg in sys.argv[1:]]
    except ValueError:
        print("Error: All arguments must be valid integers")
        sys.exit(1)

    if len(numbers) == 0:
        print("Error: Please provide at least one number")
        sys.exit(1)

    print(f"Input numbers: {numbers}")
    print("Calculating statistics using multiple threads...\n")

    # Create three worker threads
    thread_avg = threading.Thread(target=calculate_average, args=(numbers,))
    thread_min = threading.Thread(target=calculate_minimum, args=(numbers,))
    thread_max = threading.Thread(target=calculate_maximum, args=(numbers,))

    # Start all threads
    thread_avg.start()
    thread_min.start()
    thread_max.start()

    # Wait for all threads to complete
    thread_avg.join()
    thread_min.join()
    thread_max.join()

    # Output results (after all worker threads have finished)
    print(f"The average value is {average:.0f}")
    print(f"The minimum value is {minimum}")
    print(f"The maximum value is {maximum}")


if __name__ == "__main__":
    main()
