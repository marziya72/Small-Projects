primes = []
n = 600851475143
factor = 2
largest = 0

while factor * factor <= n:
    if n % factor == 0:
        largest = factor
        n //= factor
    else:
        factor += 1

if n > 1:
    largest = n

print(largest)