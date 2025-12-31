multiples = []
i = 1
total = 0

while i < 1000:
    if i % 3 == 0 or i % 5 == 0:
        multiples.append(i)
    i += 1

print(multiples)
print("Number of multiples:", len(multiples))

total = sum(multiples)
print(total)