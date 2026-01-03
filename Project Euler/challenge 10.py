num = 2
sum = 0
prime = 0

while num < 10:
    is_prime = True
    for i in range(2, num):
        if num % i == 0:
            is_prime = False
            break
    if is_prime:
        sum += num
    num += 1
print(sum)
