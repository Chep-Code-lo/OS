def read_input(filename):
    with open(filename, "r") as f:
        lines = [line.strip() for line in f if line.strip()]
    n, m = map(int, lines[0].split())
    index = 1
    allocation = []
    for _ in range(n):
        allocation.append(list(map(int, lines[index].split())))
        index += 1
    request = []
    for _ in range(n):
        request.append(list(map(int, lines[index].split())))
        index += 1
    available = list(map(int, lines[index].split()))
    return n, m, allocation, request, available

def detect_deadlock(n, m, allocation, request, available):
    work = available[:]
    finish = [False] * n
    safe_sequence = []
    while True:
        found = False
        for i in range(n):
            if not finish[i] and all(request[i][j] <= work[j] for j in range(m)):
                for j in range(m):
                    work[j] += allocation[i][j]
                finish[i] = True
                safe_sequence.append(f"P{i}")
                found = True
        if not found:
            break

    if all(finish):
        print("No Deadlock Detected")
        print("Safe Sequence:", " -> ".join(safe_sequence))
    else:
        deadlocked = [f"P{i}" for i in range(n) if not finish[i]]
        print("Deadlock Detected!")
        print("Deadlocked Processes:", ", ".join(deadlocked))

def main():
    filename = "input_deadlock.txt"
    n, m, allocation, request, available = read_input(filename)
    print("Processes:", n)
    print("Resources:", m)
    print("\nAllocation Matrix")
    for row in allocation:
        print(row)
    print("\nRequest Matrix")
    for row in request:
        print(row)
    print("\nAvailable:", available)
    print("\nRunning Deadlock Detection...\n")
    detect_deadlock(n, m, allocation, request, available)

if __name__ == "__main__":
    main()