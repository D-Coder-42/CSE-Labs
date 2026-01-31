def findSmallest(arr):
    smallest = arr[0]
    for x in arr[1:]:
        if x < smallest:
            smallest = x
    return smallest

arr = list(map(int, input("Enter numbers (space separated): ").split()))
print(findSmallest(arr))
