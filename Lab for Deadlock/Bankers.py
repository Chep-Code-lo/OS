def read_input(filename):
    with open(filename) as f:
        lines = [line.strip() for line in f if line.strip()]
    idx = 0
    n = int(lines[idx])
    idx += 1
    Allocation = []
    for i in range(n):
        Allocation.append(list(map(int, lines[idx].split())))
        idx += 1
    Max = []
    for i in range(n):
        Max.append(list(map(int, lines[idx].split())))
        idx += 1
    Total = list(map(int, lines[idx].split()))
    return n, Allocation, Max, Total

def calculate_need(n, Allocation, Max):
    Need = []
    for i in range(n):
        row = []
        for j in range(3):
            row.append(Max[i][j] - Allocation[i][j])
        Need.append(row)
    return Need

def calculate_available(n, Allocation, Total):
    Available = [0,0,0]
    for j in range(3):
        s = 0
        for i in range(n):
            s += Allocation[i][j]
        Available[j] = Total[j] - s
    return Available

def print_matrix(name, matrix):
    print(name)
    for i,row in enumerate(matrix):
        print(f"P{i} : {row}")

def banker(n, Allocation, Need, Available):
    Work = Available[:]
    Finish = [False]*n
    Sequence = []
    print("\nInitial Work =", Work)
    for step in range(n):
        found = False
        for i in range(n):
            if not Finish[i]:
                if (Need[i][0] <= Work[0] and Need[i][1] <= Work[1] and Need[i][2] <= Work[2]):
                    print(f"\nProcess P{i} runs")
                    for j in range(3):
                        Work[j] += Allocation[i][j]
                    Finish[i] = True
                    Sequence.append(i)
                    print("Work becomes =", Work)
                    found = True
        if not found:
            break
    print("\nFinish =", Finish)
    if all(Finish):
        print("SAFE STATE")
        print("Safe Sequence:")
        print(" -> ".join(f"P{i}" for i in Sequence))
    else:
        print("NOT SAFE STATE")

def main():
    filename = "input_bankers.txt"
    n, Allocation, Max, Total = read_input(filename)
    Need = calculate_need(n, Allocation, Max)
    Available = calculate_available(n, Allocation, Total)
    print("Number of processes =", n)
    print_matrix("Allocation", Allocation)
    print_matrix("Max", Max)
    print_matrix("Need", Need)
    print("\nTotal Resources =", Total)
    print("Available =", Available)
    banker(n, Allocation, Need, Available)
if __name__ == "__main__":
    main()