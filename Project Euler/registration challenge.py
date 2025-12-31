squares = []
odd = []
i = 1
total = 0

while len(squares) < 375000:
    temp = i**2
    squares.append(temp)
    if temp % 2 == 1:
        odd.append(temp)
    i += 1

print(squares)
print("Number of squares:", len(squares))
print(odd)
print("Number of odd squares:", len(odd))

total = sum(odd)
print(total)
    
