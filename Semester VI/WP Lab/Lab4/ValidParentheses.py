class ParenthesesValidator:
    def isValid(self,s):
        stack=[]
        mapping={')':'(',']':'[','}':'{'}
        for ch in s:
            if ch in mapping.values():
                stack.append(ch)
            elif ch in mapping:
                if not stack or stack[-1]!=mapping[ch]:
                    return False
                stack.pop()
        return not stack

s = input("Enter string of parentheses: ")
print(ParenthesesValidator().isValid(s))
