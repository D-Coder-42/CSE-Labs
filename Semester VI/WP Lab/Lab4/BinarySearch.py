def BinarySearch(arr, target, low, high):
    if low > high:
        return -1
    mid = (low + high) // 2

    if (arr[mid] == target):
        return mid
    elif target < arr[mid]:
        return BinarySearch(arr, target, low, mid-1)
    else:
        return BinarySearch(arr, target, mid+1, high)
    
arr = list(map(int, input("Enter sorted elements (space separated): ").split()))
target = int(input("Enter target element: "))

result = BinarySearch(arr, target, 0, len(arr)-1)

if (result != -1):
    print(f"Element {target} found at index {result}")
else:
    print(f"Element {target} not found in array")