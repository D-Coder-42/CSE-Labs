class SubsetGenerator:
    def __init__(self, nums):
        self.nums = nums

    def getSubsets(self):
        result = [[]]

        for num in self.nums:
            new = [subset + [num] for subset in result]
            result.extend(new)
        
        return result

nums = list(map(int, input("Enter distinct integers (space separated): ").split())) 
generator = SubsetGenerator(nums)
subsets = generator.getSubsets()

print("All unique subsets:") 
print(subsets)