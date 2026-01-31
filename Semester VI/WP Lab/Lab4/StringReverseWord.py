class StringReverser:
    def reverseWords(self,s):
        return ' '.join(s.split()[::-1])

s = input("Enter a sentence: ")
print(StringReverser().reverseWords(s))
