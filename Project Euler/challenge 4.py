largest = 0

for factor1 in range(999, 99, -1):
    for factor2 in range(999, 99, -1):
        sum = factor1 * factor2
        n = str(sum)
        if n == n[::-1]:
            largest = max(largest, sum)

print(largest)
