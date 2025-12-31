count = 0
num = 2

while count < 10001:
    is_prime = True
    for i in range(2, num):
        if num % i == 0:
            is_prime = False
            break

    if is_prime:
        count += 1
    num += 1
print(num)

