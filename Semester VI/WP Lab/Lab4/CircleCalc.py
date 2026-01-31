import math
class Circle:
    def __init__(self,radius):
        self.radius=radius
    def area(self):
        return math.pi*self.radius**2
    def perimeter(self):
        return 2*math.pi*self.radius

r = float(input("Enter radius: "))
c = Circle(r)
print(c.area())
print(c.perimeter())
