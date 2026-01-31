def sort(words):
    for i in range(len(words)):
        key = words[i]
        j = i-1

        while j >= 0 and words[j].lower() > key.lower():
            words[j+1] = words[j]
            j -= 1
        words[j+1] = key
    return words

text = input("Enter words to sort: ")
sorted = sort(text.split())

print("Words in alphabetical order:") 
for word in sorted: 
    print(word)