class TwoSumSolver:
    def __init__(self, arr, target):
        self.arr = arr
        self.target = target

    def findPair(self):
        seen = {}
        for i, num in enumerate(arr):
            comp = target - num;
            if comp in seen:
                return [seen[comp], i]
            seen[num] = i
        return None

arr = list(map(int, input("Enter elements (space separated): ").split()))
target = int(input("Enter target sum: "))

solver = TwoSumSolver(arr, target)
result = solver.findPair()

if result: 
    print(f"Pair found at indexes {result[0]} and {result[1]}")
else: 
    print("No pair found")