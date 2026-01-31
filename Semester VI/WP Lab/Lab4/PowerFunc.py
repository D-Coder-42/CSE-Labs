class PowerCalculator:
    def __init__(self, x, n):
        self.x = x
        self.n = n

    def pow(self):
        x, n = self.x, self.n
        res = 1

        if n < 0:
            x = 1/x
            n = -n

        while n > 0:
            if n % 2 == 1:
                res *= x
            x *= x
            n //= 2
        
        return res

x = float(input("Enter base (x): "))
n = int(input("Enter exponent (n): "))

calculator = PowerCalculator(x, n)
print(f"{x}^{n} = {round(calculator.pow(),8)}")
