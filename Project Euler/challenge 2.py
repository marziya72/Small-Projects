terms = [1,2]
even = []
total = 0
i = 2

while True:
    next = terms[i-1] + terms[i-2]

    if next >= 4000000:
        break

    terms.append(next)
    if next % 2 == 0:
        even.append(next)
    i += 1

print(terms)
print("Number of terms:", len(terms))

print("Even terms:", even)
total = sum(even)
print("Sum of even terms:", total)
